#include "motordefs.hpp"
#include "SGZ.hpp"
#include "auton_commons.hpp"

bool eightCubeAuton = false;

SGZ::SGZ(MotorDefs *md, bool ra, pros::Imu *imu){
    mtrDefs = md;
    redAlliance = ra;
    ac = new AutonCommons(mtrDefs, imu);
}

SGZ::~SGZ() {
    delete ac;
}

void SGZ::runAuton(){
    flipoutAndStartIntake();
    if (eightCubeAuton){
        pickupFirstCubes(1400);
    } else {
        pickupFirstCubes(700);
    }
    pros::Task::delay(10);

    turn(58);
    driveBackFast(-770); //MEU
    //turnToFaceCubes(6); // essential 0
    if(redAlliance){
        ac->turnRightToZeroHeading();
    } else {
        ac->turnLeftToZeroHeading();
    }
    pickupSecondCubes(1000);
    //driveForwardFast(230);
    driveBack(-500);
    if(redAlliance){
         ac->turnRobotRightSlowly(131);
    } else {
         ac->turnRobotLeftSlowly(138);
    }
    ac->driveWithCoast(1000, 60);
    deposit();
}


void SGZ::flipoutAndStartIntake() {
    ac->flipoutAndStartIntake();
}

void SGZ::turn(double degrees) {
    if(redAlliance){
        ac->turnRobotLeft(degrees);
    } else {
        ac->turnRobotRight(degrees);
    }  
}
void SGZ::turnToFaceCubes(double degrees) {
    ac->turnRightToZeroHeading();
}
void SGZ::pickupFirstCubes(int degrees) {
    ac->driveRobot(degrees, 80);
}

void SGZ::driveForwardFast(int degrees){
    ac->driveRobot(degrees, 100);
}

void SGZ::pickupSecondCubes(int degrees) {
    ac->driveRobot(degrees, 70);
}
void SGZ::driveBack(int degrees) {
    ac->driveRobotBack(degrees, -90);
}
void SGZ::driveBackFast(int degrees) {
    ac->driveRobotBack(degrees, -90);
}
void SGZ::driveToGoal(int degrees) {
    ac->driveRobot(degrees, 45);
}
void SGZ::deposit() {
    ac->deposit(); // full deposit using all 3 helper funcs
}

