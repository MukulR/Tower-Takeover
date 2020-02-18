//#include "movement.hpp"
#include "motordefs.hpp"
#include "auton_commons.hpp"

const int DRIVE_BRAKE_DELAY_MS = 125;
const double SLOW_DOWN_DISTANCE = 0.75;
const int TURN_SPEED = 50;
const int TURN_BRAKE_DELAY_MS = 125;

AutonCommons::AutonCommons(MotorDefs *md, pros::Imu *imu){
    mtrDefs = md;
	this->imu = imu;
 }
AutonCommons::~AutonCommons(){}

pros::ADIDigitalIn limswitch ('H');
pros::ADIAnalogIn poten ('A');
pros::ADILineSensor line_l ('F');
pros::ADILineSensor line_r ('C');


void AutonCommons::waitForCompletion(){
    while((abs(mtrDefs->right_mtr_b->get_position() - mtrDefs->right_mtr_b->get_target_position()) + 
        abs(mtrDefs->left_mtr_b->get_position() - mtrDefs->left_mtr_b->get_target_position()) +
        abs(mtrDefs->left_mtr_f->get_position() - mtrDefs->left_mtr_f->get_target_position()) +
        abs(mtrDefs->right_mtr_f->get_position() - mtrDefs->right_mtr_f->get_target_position())) > 6 * 4 /* Number of drive mtrs */ ) {
        pros::Task::delay(5);
    }
}

void AutonCommons::driveRobot(double degrees, int speed){
	int delay_ms = speed; // delay units eq. to speed
	mtrDefs->left_mtr_f->tare_position();
	mtrDefs->right_mtr_f->tare_position();
	int setSpeed = speed;
	mtrDefs->left_mtr_f->move(setSpeed);
	mtrDefs->left_mtr_b->move(setSpeed);
	mtrDefs->right_mtr_f->move(setSpeed);
	mtrDefs->right_mtr_b->move(setSpeed);
	while(true){
		int degreesTraveled = (mtrDefs->left_mtr_f->get_position() + mtrDefs->right_mtr_f->get_position()) / 2;
		if (degreesTraveled >= degrees){
			mtrDefs->left_mtr_f->move(-setSpeed);
			mtrDefs->left_mtr_b->move(-setSpeed);
			mtrDefs->right_mtr_f->move(-setSpeed);
			mtrDefs->right_mtr_b->move(-setSpeed);
			pros::Task::delay(delay_ms);
			mtrDefs->left_mtr_f->move(0);
			mtrDefs->left_mtr_b->move(0);
			mtrDefs->right_mtr_f->move(0);
			mtrDefs->right_mtr_b->move(0);
			break;
		} else if (degreesTraveled >= degrees * SLOW_DOWN_DISTANCE){ // slow down after traveling 3/4 of the distance
			setSpeed = speed / 2;
			mtrDefs->left_mtr_f->move(setSpeed);
			mtrDefs->left_mtr_b->move(setSpeed);
			mtrDefs->right_mtr_f->move(setSpeed);
			mtrDefs->right_mtr_b->move(setSpeed);
		}
	}
}

void AutonCommons::driveRobotBack(double degrees, int speed){
	int delay_ms = speed; // delay units eq. to speed
	mtrDefs->left_mtr_f->tare_position();
	mtrDefs->right_mtr_f->tare_position();
	int setSpeed = speed;
	mtrDefs->left_mtr_f->move(setSpeed);
	mtrDefs->left_mtr_b->move(setSpeed);
	mtrDefs->right_mtr_f->move(setSpeed);
	mtrDefs->right_mtr_b->move(setSpeed);
	while(true){
		int degreesTraveled = (mtrDefs->left_mtr_f->get_position() + mtrDefs->right_mtr_f->get_position()) / 2;
		if (abs(degreesTraveled) >= abs(degrees)){
			mtrDefs->left_mtr_f->move(-setSpeed);
			mtrDefs->left_mtr_b->move(-setSpeed);
			mtrDefs->right_mtr_f->move(-setSpeed);
			mtrDefs->right_mtr_b->move(-setSpeed);
			pros::Task::delay(delay_ms * -1);
			mtrDefs->left_mtr_f->move(0);
			mtrDefs->left_mtr_b->move(0);
			mtrDefs->right_mtr_f->move(0);
			mtrDefs->right_mtr_b->move(0);
			break;
		} else if (abs(degreesTraveled) >= abs(degrees) * SLOW_DOWN_DISTANCE){ // slow down after traveling 3/4 of the distance
			setSpeed = speed / 2;
			mtrDefs->left_mtr_f->move(speed);
			mtrDefs->left_mtr_b->move(speed);
			mtrDefs->right_mtr_f->move(speed);
			mtrDefs->right_mtr_b->move(speed);
		}
	}
}

void AutonCommons::turnRobotRight(double degrees){
	degrees = degrees - 6; // Correction for Inertial sensor's constant overshoot of 6 degs
	mtrDefs->left_mtr_f->move(TURN_SPEED); 
	mtrDefs->left_mtr_b->move(TURN_SPEED);
	mtrDefs->right_mtr_f->move(-TURN_SPEED);
	mtrDefs->right_mtr_b->move(-TURN_SPEED);
	while (degrees - imu->get_heading() < 0); // bring the robot to 0 heading
	while (imu->get_heading() < degrees); // turn the robot the accurate amount of degs
	mtrDefs->left_mtr_f->move(-TURN_SPEED);
	mtrDefs->left_mtr_b->move(-TURN_SPEED);
	mtrDefs->right_mtr_f->move(TURN_SPEED);
	mtrDefs->right_mtr_b->move(TURN_SPEED);
	pros::Task::delay(TURN_BRAKE_DELAY_MS);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
}

void AutonCommons::turnRobotRightSlowly(double degrees){
	degrees = degrees - 6; // Correction for Inertial sensor's constant overshoot of 6 degs
	mtrDefs->left_mtr_f->move(40); 
	mtrDefs->left_mtr_b->move(40);
	mtrDefs->right_mtr_f->move(-40);
	mtrDefs->right_mtr_b->move(-40);
	while (degrees - imu->get_heading() < 0); // bring the robot to 0 heading
	while (imu->get_heading() < degrees); // turn the robot the accurate amount of degs
	mtrDefs->left_mtr_f->move(-40);
	mtrDefs->left_mtr_b->move(-40);
	mtrDefs->right_mtr_f->move(40);
	mtrDefs->right_mtr_b->move(40);
	pros::Task::delay(TURN_BRAKE_DELAY_MS);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
}

void AutonCommons::turnRobotLeftSlowly(double degrees){
	degrees = degrees - 6; // Correction for Inertial sensor's constant overshoot of 5 degs
	mtrDefs->left_mtr_f->move(-40); 
	mtrDefs->left_mtr_b->move(-40);
	mtrDefs->right_mtr_f->move(40);
	mtrDefs->right_mtr_b->move(40);
	while (imu->get_heading() - degrees < 0); // correction loop
	while (imu->get_heading() > 360 - degrees); // turn the robot the accurate amount of degs
	mtrDefs->left_mtr_f->move(40);
	mtrDefs->left_mtr_b->move(40);
	mtrDefs->right_mtr_f->move(-40);
	mtrDefs->right_mtr_b->move(-40);
	pros::Task::delay(TURN_BRAKE_DELAY_MS);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
}

void AutonCommons::turnRobotLeft(double degrees){
	degrees = degrees - 6; // Correction for Inertial sensor's constant overshoot of 5 degs
	mtrDefs->left_mtr_f->move(-TURN_SPEED); 
	mtrDefs->left_mtr_b->move(-TURN_SPEED);
	mtrDefs->right_mtr_f->move(TURN_SPEED);
	mtrDefs->right_mtr_b->move(TURN_SPEED);
	while (imu->get_heading() - degrees < 0); // correction loop
	while (imu->get_heading() > 360 - degrees); // turn the robot the accurate amount of degs
	mtrDefs->left_mtr_f->move(TURN_SPEED);
	mtrDefs->left_mtr_b->move(TURN_SPEED);
	mtrDefs->right_mtr_f->move(-TURN_SPEED);
	mtrDefs->right_mtr_b->move(-TURN_SPEED);
	pros::Task::delay(TURN_BRAKE_DELAY_MS);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
}

void AutonCommons::turnRightToZeroHeading() {
	mtrDefs->left_mtr_f->move(TURN_SPEED); 
	mtrDefs->left_mtr_b->move(TURN_SPEED);
	mtrDefs->right_mtr_f->move(-TURN_SPEED);
	mtrDefs->right_mtr_b->move(-TURN_SPEED);
	while (true){
		if (imu->get_heading() >= 0 && imu->get_heading() < 180){ // brake the robot when heading reaches the right hemisphere
			mtrDefs->left_mtr_f->move(-TURN_SPEED);
			mtrDefs->left_mtr_b->move(-TURN_SPEED);
			mtrDefs->right_mtr_f->move(TURN_SPEED);
			mtrDefs->right_mtr_b->move(TURN_SPEED);
			pros::Task::delay(TURN_BRAKE_DELAY_MS);
			mtrDefs->left_mtr_f->move(0);
			mtrDefs->left_mtr_b->move(0);
			mtrDefs->right_mtr_f->move(0);
			mtrDefs->right_mtr_b->move(0);
			break;
		}
	}
}

void AutonCommons::turnLeftToZeroHeading() {
	mtrDefs->left_mtr_f->move(-TURN_SPEED); 
	mtrDefs->left_mtr_b->move(-TURN_SPEED);
	mtrDefs->right_mtr_f->move(TURN_SPEED);
	mtrDefs->right_mtr_b->move(TURN_SPEED);
	while (true){
		if (imu->get_heading() <= 360 && imu->get_heading() > 180){ // brake the robot when heading reaches the right hemisphere
			mtrDefs->left_mtr_f->move(TURN_SPEED);
			mtrDefs->left_mtr_b->move(TURN_SPEED);
			mtrDefs->right_mtr_f->move(-TURN_SPEED);
			mtrDefs->right_mtr_b->move(-TURN_SPEED);
			pros::Task::delay(TURN_BRAKE_DELAY_MS);
			mtrDefs->left_mtr_f->move(0);
			mtrDefs->left_mtr_b->move(0);
			mtrDefs->right_mtr_f->move(0);
			mtrDefs->right_mtr_b->move(0);
			break;
		}
	}
}



void AutonCommons::driveWithCoast(int time, int power){
	mtrDefs->left_mtr_f->move(power);
	mtrDefs->left_mtr_b->move(power);
	mtrDefs->right_mtr_f->move(power);
	mtrDefs->right_mtr_b->move(power);
	pros::Task::delay(time);
	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
}

void AutonCommons::tilt(void *param){
	MotorDefs *mtrDefs = (MotorDefs*)param;
	mtrDefs->tilt_mtr->move(-60);
}

void AutonCommons::deposit() {
    alignCubeToIntake();
    depositStack();
    pros::Task tiltTask(AutonCommons::tilt, mtrDefs);
    backup();
}
void AutonCommons::flipoutAndStartIntake() {
    // Outake hard a little to release tray. Also applies downward hold-power on lift to
    // prevent bouncing up.
    mtrDefs->lift_mtr->move(20);
    mtrDefs->intake_mtr_l->move(127);
    mtrDefs->intake_mtr_r->move(127);
    pros::Task::delay(400);
    mtrDefs->intake_mtr_l->move(-127);
    mtrDefs->intake_mtr_r->move(-127);
}

void AutonCommons::backup(){
	mtrDefs->intake_mtr_l->move(90);
	mtrDefs->intake_mtr_r->move(90);

	mtrDefs->left_mtr_f->move(-60);
	mtrDefs->left_mtr_b->move(-60);
	mtrDefs->right_mtr_f->move(-60);
	mtrDefs->right_mtr_b->move(-60);
	pros::Task::delay(1000);
	mtrDefs->intake_mtr_l->move(0);
	mtrDefs->intake_mtr_r->move(0);

	mtrDefs->left_mtr_f->move(0);
	mtrDefs->left_mtr_b->move(0);
	mtrDefs->right_mtr_f->move(0);
	mtrDefs->right_mtr_b->move(0);
}

void AutonCommons::alignCubeToIntake(){
	mtrDefs->intake_mtr_l->move(35);
	mtrDefs->intake_mtr_r->move(35);
	while ((line_l.get_value() + line_r.get_value())/2 > 1110){
		pros::Task::delay(10);
	}
	pros::Task::delay(140);
	mtrDefs->intake_mtr_l->move(-10);
	mtrDefs->intake_mtr_r->move(-10);
	pros::Task::delay(30);
	mtrDefs->intake_mtr_l->move(0);
	mtrDefs->intake_mtr_r->move(0);
}

void AutonCommons::depositStack(){
	mtrDefs->lift_mtr->move(20);
	mtrDefs->tilt_mtr->move(127);
	mtrDefs->intake_mtr_l->move(-30);
	mtrDefs->intake_mtr_r->move(-30);
	while(poten.get_value() > 1790){
		pros::Task::delay(10);
	}
	mtrDefs->tilt_mtr->move(45);
	while(poten.get_value() > 1040){
		pros::Task::delay(10);
	}
	mtrDefs->tilt_mtr->move(-10);
	pros::Task::delay(50);
	mtrDefs->tilt_mtr->move(0);
	mtrDefs->intake_mtr_l->move(0);
	mtrDefs->intake_mtr_r->move(0);
}