#include "auton_commons.hpp"
#include "motordefs.hpp"
#include "SGZ.hpp"

auto myChassis = ChassisControllerFactory::create(
        {Ports::LEFT_MTR_F_PORT, Ports::LEFT_MTR_B_PORT}, // Left motors
        {-Ports::RIGHT_MTR_F_PORT, -Ports::RIGHT_MTR_B_PORT},   // Right motors
        AbstractMotor::gearset::red, // Torque gearset
        {3.25_in, 13.5_in} // 3.25 inch wheels, 13.5 inch wheelbase width
);

auto profileController = AsyncControllerFactory::motionProfile(
    2,  // Maximum linear velocity of the Chassis in m/s
    2,  // Maximum linear acceleration of the Chassis in m/s/s
    3.45, // Maximum linear jerk of the Chassis in m/s/s/s
    myChassis // Chassis Controller
);

SGZ::SGZ(MotorDefs *md, bool ra){
    mtrDefs = md;
    redAlliance = ra;
}

SGZ::~SGZ() {}


void SGZ::runAuton(){
    AutonCommons ac(mtrDefs);
    ac.lift(127, 900, 1);
    pros::Task::delay(200);
    ac.lift(127, 900, -1);

    mtrDefs->tilt_mtr->move(60);
    pros::Task::delay(2300);
    mtrDefs->tilt_mtr->move(0);


    mtrDefs->intake_mtr_l->move(127);
    mtrDefs->intake_mtr_r->move(127);

    profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{6_ft, 0_ft, 0_deg}}, "forward");
    profileController.setTarget("forward", false);
    profileController.waitUntilSettled();

    //profileController.removePath("forward");
    profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1.5_ft, 0_ft, 0_deg}}, "backward");
    profileController.setTarget("backward", true);
    profileController.waitUntilSettled();

    //profileController.removePath("backward");
    if(redAlliance){
      ac.turn(135, 1); //turn right 135 degrees
    } else {
      ac.turn(135, -1); //turn left 135 degrees
    }
    ac.waitForCompletion();

    profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{2.9_ft, 0_ft, 0_deg}}, "go to goal");
    profileController.setTarget("go to goal", false);
    profileController.waitUntilSettled();

    mtrDefs->intake_mtr_l->move(0);
    mtrDefs->intake_mtr_r->move(0);
    
    mtrDefs->tilt_mtr->move(-70);
    pros::Task::delay(2500);
    mtrDefs->tilt_mtr->move(-10);


    profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
    profileController.setTarget("backup", true);
    profileController.waitUntilSettled();
}
