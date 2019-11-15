#include "SGZ.hpp"
#include "motordefs.hpp"
#include "okapi/api.hpp"

const double TURN_SCALE_FACTOR = 4.44444;

using namespace okapi;


SGZ::SGZ(MotorDefs *md, bool ra){
    mtrDefs = md;
    redAlliance = ra;

}

SGZ::~SGZ() {}

void turn(double angle, int multiplier, MotorDefs *mtrDefs){
  double scaled_angle = TURN_SCALE_FACTOR * angle;
  mtrDefs->left_mtr_f->move_relative(scaled_angle * multiplier, 100);
  mtrDefs->left_mtr_b->move_relative(scaled_angle * multiplier, 100);
  mtrDefs->right_mtr_f->move_relative(-scaled_angle * multiplier, 100);
  mtrDefs->right_mtr_b->move_relative(-scaled_angle * multiplier, 100);
}

void tilt(int power, int time, MotorDefs *mtrDefs){
  mtrDefs->tilt_mtr->move(power);
  pros::Task::delay(time);

  mtrDefs->tilt_mtr->move(10);
  pros::Task::delay(100);
  mtrDefs->tilt_mtr->move(0);
}

void lift(int power, int time, int multiplier, MotorDefs *mtrDefs){
  mtrDefs->lift_mtr->move(-power * multiplier);
  pros::Task::delay(time);
  mtrDefs->lift_mtr->move(0);
}

void waitForCompletion(MotorDefs *mtrDefs){
  while((abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) + 
			abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) +
			abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
			abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position())) > 6 * 4 /* Number of drive mtrs */ ) {
		pros::Task::delay(5);
	}
}

  auto myChassis = ChassisControllerFactory::create(
      {15, 13}, // Left motors
      {-16, -20},   // Right motors
      AbstractMotor::gearset::red, // Torque gearset
      {3.25_in, 13.5_in} // 4 inch wheels, 13.5 inch wheelbase width
  );

  auto profileController = AsyncControllerFactory::motionProfile(
      1.0,  // Maximum linear velocity of the Chassis in m/s
      2.0,  // Maximum linear acceleration of the Chassis in m/s/s
      10.0, // Maximum linear jerk of the Chassis in m/s/s/s
      myChassis // Chassis Controller
  );

void SGZ::runAuton(){
    lift(127, 900, 1, mtrDefs);
    pros::Task::delay(200);
    lift(127, 900, -1, mtrDefs);

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
      turn(135, 1, mtrDefs); //turn right 135 degrees
    } else {
      turn(135, -1, mtrDefs); //turn left 135 degrees
    }
    waitForCompletion(mtrDefs);

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
