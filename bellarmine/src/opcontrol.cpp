#include "main.h"
#include "motordefs.hpp"
#include "okapi/api.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);
MotorDefs mtrDefs;

void drive(void* param){
	mtrDefs.lift_mtr->set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);
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
		mtrDefs.left_mtr_f->move(leftMtrVals);
		mtrDefs.left_mtr_b->move(leftMtrVals);
		mtrDefs.right_mtr_f->move(rightMtrVals);
		mtrDefs.right_mtr_b->move(rightMtrVals);
		// The below delay is required for tasks to work in PROS.
		pros::Task::delay(10);
	}
}

void intake(void* param){
	static bool intakeStarted = false;
    while(true){
		//start
        if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
			if(!intakeStarted){
				mtrDefs.intake_mtr_l->move(127);	
				mtrDefs.intake_mtr_r->move(127);
				intakeStarted = true;
			} else if(intakeStarted){
				mtrDefs.intake_mtr_l->move(0);
				mtrDefs.intake_mtr_r->move(0);
				intakeStarted = false;
			}
			pros::Task::delay(300);
		};
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){	
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_A)){
				mtrDefs.intake_mtr_l->move(-127);
				mtrDefs.intake_mtr_r->move(-127);
			}
			mtrDefs.intake_mtr_l->move(0);
			mtrDefs.intake_mtr_r->move(0);
		}
        pros::Task::delay(10);
    }
}

void tilt(void *param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			mtrDefs.tilt_mtr->move(75);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				pros::Task::delay(10);
			}
			mtrDefs.tilt_mtr->move(0);
		}
        //depositing
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			mtrDefs.tilt_mtr->move_velocity(-35);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
				pros::Task::delay(10);
			}
			mtrDefs.tilt_mtr->move(-3);
		}
		pros::Task::delay(0);
	}
}

void lift(void *param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			mtrDefs.lift_mtr->move(125);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				pros::Task::delay(10);
			}
			mtrDefs.lift_mtr->move(-10);
		}
		//going up
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			mtrDefs.lift_mtr->move(-127);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
				pros::Task::delay(10);
			}
			mtrDefs.lift_mtr->move(-10);
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