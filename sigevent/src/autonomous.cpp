#include "main.h"
#include "okapi/api.hpp"
#include "autonselection.hpp"
#include "motordefs.hpp"
#include "sgz.hpp"


const double TURN_SCALE_FACTOR = 4.44444;

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

void noAuton(){}

void autonomous() {
	MotorDefs mtrDefs;
	switch (autonSelected) {
		case 0:
			{
				SGZ sgz(&mtrDefs, redAlliance, imu);
				sgz.runAuton();
			}
			break;
		case 1:
			{
				// dgzAuton(&mtrDefs, redAlliance);
				//DGZ dgz(&mtrDefs, redAlliance);
				//dgz.runAuton();
			}
			break;
		default:
			noAuton();
			break;
	}
} 
