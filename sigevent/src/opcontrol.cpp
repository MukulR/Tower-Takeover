#include "main.h"
#include "motordefs.hpp"
#include "okapi/api.hpp"


pros::Controller master(pros::E_CONTROLLER_MASTER);

MotorDefs mtrDefs;
pros::ADIDigitalIn limitswitch ('H');
pros::ADIAnalogIn pot ('A');
pros::ADILineSensor lineleft ('F');
pros::ADILineSensor lineright ('C');

bool suspended = false;
bool hold = false;

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
		if (suspended == false){
			mtrDefs.left_mtr_f->move(leftMtrVals);
			mtrDefs.left_mtr_b->move(leftMtrVals);
			mtrDefs.right_mtr_f->move(rightMtrVals);
			mtrDefs.right_mtr_b->move(rightMtrVals);
		}
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
				mtrDefs.intake_mtr_l->move(-200);	
				mtrDefs.intake_mtr_r->move(-200);
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
				mtrDefs.intake_mtr_l->move(65);
				mtrDefs.intake_mtr_r->move(65);
			}
			mtrDefs.intake_mtr_l->move(0);
			mtrDefs.intake_mtr_r->move(0);
		}

		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
			suspended = true;	
			mtrDefs.intake_mtr_l->move(60);
			mtrDefs.intake_mtr_r->move(60);

			mtrDefs.left_mtr_f->move(-30);
			mtrDefs.left_mtr_b->move(-30);
			mtrDefs.right_mtr_f->move(-30);
			mtrDefs.right_mtr_b->move(-30);

			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_B)){
				pros::Task::delay(10);
			}

			mtrDefs.intake_mtr_l->move(0);
			mtrDefs.intake_mtr_r->move(0);

			mtrDefs.left_mtr_f->move(0);
			mtrDefs.left_mtr_b->move(0);
			mtrDefs.right_mtr_f->move(0);
			mtrDefs.right_mtr_b->move(0);
			suspended = false;
		}

		if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
			mtrDefs.intake_mtr_l->move(127);
			mtrDefs.intake_mtr_r->move(127);
			while (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)) {
				pros::Task::delay(5);
			}
			mtrDefs.intake_mtr_l->move(0);
			mtrDefs.intake_mtr_r->move(0);
		}
        pros::Task::delay(10);
    }
}

void backup(){
	suspended = true;	
	mtrDefs.intake_mtr_l->move(60);
	mtrDefs.intake_mtr_r->move(60);

	mtrDefs.left_mtr_f->move(-30);
	mtrDefs.left_mtr_b->move(-30);
	mtrDefs.right_mtr_f->move(-30);
	mtrDefs.right_mtr_b->move(-30);
	pros::Task::delay(1000);
	mtrDefs.intake_mtr_l->move(0);
	mtrDefs.intake_mtr_r->move(0);

	mtrDefs.left_mtr_f->move(0);
	mtrDefs.left_mtr_b->move(0);
	mtrDefs.right_mtr_f->move(0);
	mtrDefs.right_mtr_b->move(0);
	suspended = false;
}

void alignCubeToIntake(){
	mtrDefs.intake_mtr_l->move(35);
	mtrDefs.intake_mtr_r->move(35);
	while ((lineleft.get_value() + lineright.get_value())/2 > 1110){
		pros::Task::delay(10);
	}
	mtrDefs.intake_mtr_l->move(-10);
	mtrDefs.intake_mtr_r->move(-10);
	pros::Task::delay(30);
	mtrDefs.intake_mtr_l->move(0);
	mtrDefs.intake_mtr_r->move(0);
}

void depositStack(){
	mtrDefs.lift_mtr->move(20);
	mtrDefs.tilt_mtr->move(127);
	mtrDefs.intake_mtr_l->move(-30);
	mtrDefs.intake_mtr_r->move(-30);
	while(pot.get_value() > 1634){
		pros::Task::delay(10);
	}
	mtrDefs.tilt_mtr->move(30);
	while(pot.get_value() > 880){
		pros::Task::delay(10);
	}
	mtrDefs.tilt_mtr->move(-10);
	pros::Task::delay(50);
	mtrDefs.tilt_mtr->move(0);
	mtrDefs.intake_mtr_l->move(0);
	mtrDefs.intake_mtr_r->move(0);
}

void tilt(void *param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
			mtrDefs.tilt_mtr->move(-75);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				pros::Task::delay(10);
			}
			mtrDefs.tilt_mtr->move(-15);
		}
        //depositing
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
			mtrDefs.tilt_mtr->move_velocity(30);
			mtrDefs.intake_mtr_l->move(-25);
			mtrDefs.intake_mtr_r->move(-25);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
				pros::Task::delay(10);
			}
			mtrDefs.tilt_mtr->move(0);
			mtrDefs.intake_mtr_l->move(0);
			mtrDefs.intake_mtr_r->move(0);
		}
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
			depositStack();
		}
		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
			alignCubeToIntake();
		}

		if(master.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)){
			alignCubeToIntake();
			pros::Task::delay(50);
			depositStack();
			pros::Task::delay(200);
			mtrDefs.tilt_mtr->move(-60);
			pros::Task::delay(600);
			mtrDefs.tilt_mtr->move(0);
			backup();
		}
		pros::Task::delay(10);
	}
}

void lift(void *param){
	while(true){
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
			mtrDefs.lift_mtr->move(125);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				pros::Task::delay(10);
			}
			if(!hold){
				mtrDefs.lift_mtr->move(-10);
			}
		}
		//going up
		 
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
			mtrDefs.lift_mtr->move(-127);
			while(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
				pros::Task::delay(10);
			}
			mtrDefs.lift_mtr->move(-10);
		}
		if (limitswitch.get_new_press()) {
			mtrDefs.lift_mtr->move(30);
			hold = true;
			pros::Task::delay(300);
			hold = false;
   		}
	
		pros::Task::delay(10);


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