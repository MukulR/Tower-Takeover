#include "main.h"

#include "okapi/api.hpp"

pros::Motor intake_l(2, pros::E_MOTOR_GEARSET_18, true);
pros::Motor intake_r(7, pros::E_MOTOR_GEARSET_18, false);
pros::Motor tilt_mtr(4, pros::E_MOTOR_GEARSET_36, true);
pros::Motor lift_mtr(10, pros::E_MOTOR_GEARSET_36, true);
pros::Motor right_f(16, pros::E_MOTOR_GEARSET_18, true);
pros::Motor right_b(19, pros::E_MOTOR_GEARSET_18, true);
pros::Motor left_f(15, pros::E_MOTOR_GEARSET_18, false);
pros::Motor left_b(13, pros::E_MOTOR_GEARSET_18, false);



pros::Controller master(pros::E_CONTROLLER_MASTER);


void drive(void* param){
	while(true) {
		int forward = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
		int turn = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
		float scaledTurn = ((turn * 100) * 0.75) / 100;
		float leftMtrVals = (forward + scaledTurn);
		float rightMtrVals = -(scaledTurn - forward);
		if(leftMtrVals > 127){
			leftMtrVals = 127;
		}
		if(leftMtrVals < -127){
			leftMtrVals = -127;
		}
		if(rightMtrVals > 127){
			rightMtrVals = 127;
		}
		if(rightMtrVals < -127){
			rightMtrVals = -127;
		}
		left_f.move(leftMtrVals);
		left_b.move(leftMtrVals);
		right_f.move(rightMtrVals);
		right_b.move(rightMtrVals);
		// The below delay is required for tasks to work in PROS.
		pros::Task::delay(10);
	}
}

void intake(void* param){
	static bool intakeStarted = false;
    while(true){
        if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
			if(!intakeStarted){
				intake_l.move(127);	
				intake_r.move(127);
				intakeStarted = true;
			} else if(intakeStarted){
				intake_l.move(0);
				intake_r.move(0);
				intakeStarted = false;
			}
			pros::Task::delay(300);
		};
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){	
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
				intake_l.move(-127);
				intake_r.move(-127);
			}
			intake_l.move(0);
			intake_r.move(0);
		}
        pros::Task::delay(10);
    }
}



void tilt(void *param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			tilt_mtr.move(75);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				pros::Task::delay(10);
			}
			tilt_mtr.move(0);
		}
        //depositing
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			tilt_mtr.move_velocity(-50);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
				pros::Task::delay(10);
			}
			tilt_mtr.move(-3);
		}
		pros::Task::delay(0);
	}
}

void lift(void *param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			lift_mtr.move(125);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				pros::Task::delay(10);
			}
			lift_mtr.move(-10);
		}
		//going up
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			lift_mtr.move(-127);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
				pros::Task::delay(10);
			}
			lift_mtr.move(-10);
		}
		pros::Task::delay(0);
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
//task starter function (called by competition)


void opcontrol() {
	pros::Task intaking(intake);
	pros::Task tilting(tilt);
	pros::Task driving(drive);
	pros::Task lifting(lift);
}