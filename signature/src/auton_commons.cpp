#include "auton_commons.hpp"
#include "motordefs.hpp"
#include "okapi/api.hpp"

const double TURN_SCALE_FACTOR = 4.44444;

ChassisControllerIntegrated AutonCommons::createChasis() {
    return ChassisControllerFactory::create(
        {Ports::LEFT_MTR_F_PORT, Ports::LEFT_MTR_B_PORT}, // Left motors
        {-Ports::RIGHT_MTR_F_PORT, -Ports::RIGHT_MTR_B_PORT},   // Right motors
        AbstractMotor::gearset::red, // Torque gearset
        {3.25_in, 13.5_in} // 3.25 inch wheels, 13.5 inch wheelbase width
    );
}

AsyncMotionProfileController AutonCommons::createAsyncMotionProfileController(ChassisControllerIntegrated *cci) {
    return AsyncControllerFactory::motionProfile(
        1,  // Maximum linear velocity of the Chassis in m/s
        2,  // Maximum linear acceleration of the Chassis in m/s/s
        10, // Maximum linear jerk of the Chassis in m/s/s/s
        *cci // Chassis Controller
    );
}


AutonCommons::AutonCommons(MotorDefs *mtrs){
    mtrDefs = mtrs;
}

AutonCommons::~AutonCommons(){}

void AutonCommons::turn(double angle, int multiplier){
    double scaled_angle = TURN_SCALE_FACTOR * angle;
    mtrDefs->left_mtr_f->move_relative(scaled_angle * multiplier, 100);
    mtrDefs->left_mtr_b->move_relative(scaled_angle * multiplier, 100);
    mtrDefs->right_mtr_f->move_relative(-scaled_angle * multiplier, 100);
    mtrDefs->right_mtr_b->move_relative(-scaled_angle * multiplier, 100);
}

void AutonCommons::tilt(int power, int time){
    mtrDefs->tilt_mtr->move(power);
    pros::Task::delay(time);

    mtrDefs->tilt_mtr->move(10);
    pros::Task::delay(100);
    mtrDefs->tilt_mtr->move(0);
}

void AutonCommons::lift(int power, int time, int multiplier){
    mtrDefs->lift_mtr->move(-power * multiplier);
    pros::Task::delay(time);
    mtrDefs->lift_mtr->move(0);
}

void AutonCommons::waitForCompletion(){
    while((abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) + 
        abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) +
        abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
        abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position())) > 6 * 4 /* Number of drive mtrs */ ) {
        pros::Task::delay(5);
    }
}
