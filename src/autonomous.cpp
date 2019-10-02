#include "main.h"
#include "okapi/api.hpp"

const double TURN_SCALE_FACTOR = 4.44444;

pros::Motor intake_l1(2, pros::E_MOTOR_GEARSET_18, true);
pros::Motor intake_r1(7, pros::E_MOTOR_GEARSET_18, false);
pros::Motor tilt_mtr1(4, pros::E_MOTOR_GEARSET_36, true);
pros::Motor lift_mtr1(10, pros::E_MOTOR_GEARSET_36, true);
pros::Motor right_f1(16, pros::E_MOTOR_GEARSET_18, true);
pros::Motor right_b1(19, pros::E_MOTOR_GEARSET_18, true);
pros::Motor left_f1(15, pros::E_MOTOR_GEARSET_18, false);
pros::Motor left_b1(13, pros::E_MOTOR_GEARSET_18, false);


void waitForCompletion(){
  while((abs(right_b1.get_position() - right_b1.get_target_position()) + 
			abs(left_b1.get_position() - left_b1.get_target_position()) +
			abs(left_f1.get_position() - left_f1.get_target_position()) +
			abs(right_f1.get_position() - right_f1.get_target_position())) > 6 * 4 /* Number of drive mtrs */ ) {
		pros::Task::delay(5);
	}
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

using namespace okapi;

auto myChassis = ChassisControllerFactory::create(
  {15, 13}, // Left motors
  {-16, -19},   // Right motors
  AbstractMotor::gearset::red, // Torque gearset
  {3.25_in, 13.5_in} // 4 inch wheels, 12.5 inch wheelbase width
);

auto profileController = AsyncControllerFactory::motionProfile(
  1.0,  // Maximum linear velocity of the Chassis in m/s
  2.0,  // Maximum linear acceleration of the Chassis in m/s/s
  10.0, // Maximum linear jerk of the Chassis in m/s/s/s
  myChassis // Chassis Controller
);

void turn(double angle, int multiplier){
  double scaled_angle = TURN_SCALE_FACTOR * angle;
  left_f1.move_relative(scaled_angle * multiplier, 50);
  left_b1.move_relative(scaled_angle * multiplier, 50);
  right_f1.move_relative(-scaled_angle * multiplier, 50);
  right_b1.move_relative(-scaled_angle * multiplier, 50);
}

void tilt(int power, int time){
  tilt_mtr1.move(power);
  pros::Task::delay(time);

  tilt_mtr1.move(10);
  pros::Task::delay(100);
  tilt_mtr1.move(0);
}

void lift(int power, int time, int multiplier){
  lift_mtr1.move(-power * multiplier);
  pros::Task::delay(time);
  lift_mtr1.move(0);
}


void autonomous() {
  lift(100, 700, 1);
  pros::Task::delay(200);
  lift(100, 700, -1);

  pros::Task::delay(100);

  tilt_mtr1.move(60);
  pros::Task::delay(2500);
  tilt_mtr1.move(0);


  intake_l1.move(127);
  intake_r1.move(127);

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{6_ft, 0_ft, 0_deg}}, "forward");
  profileController.setTarget("forward", false);
  profileController.waitUntilSettled();

  intake_l1.move(0);
  intake_r1.move(0);

  //profileController.removePath("forward");
  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1.5_ft, 0_ft, 0_deg}}, "backward");
  profileController.setTarget("backward", true);
  profileController.waitUntilSettled();

  //profileController.removePath("backward");
  turn(135, 1); //turn right 135 degrees
  waitForCompletion();

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{2.7_ft, 0_ft, 0_deg}}, "go to goal");
  profileController.setTarget("go to goal", false);
  profileController.waitUntilSettled();
  
  tilt_mtr1.move(-60);
  pros::Task::delay(2500);
  tilt_mtr1.move(-10);


  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
  profileController.setTarget("backup", true);
  profileController.waitUntilSettled();
} 
