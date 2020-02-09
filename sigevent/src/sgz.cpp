#include "motordefs.hpp"
#include "SGZ.hpp"
#include "auton_commons.hpp"

bool eightCubeAuton = true;

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
        pickupCubes(1400);
    } else {
        pickupCubes(900);
    }
    pros::Task::delay(10);
    turn(30);
    driveBack(-1350); //MEU
    turnToFaceCubes(7); // essential 0
    pickupCubes(1000);
    driveBack(-475);
    ac->turnRobotRight(131);
    ac->driveWithCoast(700, 60);
    deposit();
}


void SGZ::flipoutAndStartIntake() {
    ac->flipoutAndStartIntake();
}

void SGZ::turn(double degrees) {
    ac->turnRobotLeft(degrees);
}
void SGZ::turnToFaceCubes(double degrees) {
    ac->turnRightToZeroHeading();
}
void SGZ::pickupCubes(int degrees) {
    ac->driveRobot(degrees, 80);
}
void SGZ::driveBack(int degrees) {
    ac->driveRobotBack(degrees, -90);
}
void SGZ::driveToGoal(int degrees) {
    ac->driveRobot(degrees, 45);
}
void SGZ::deposit() {
    ac->deposit(); // full deposit using all 3 helper funcs
}

