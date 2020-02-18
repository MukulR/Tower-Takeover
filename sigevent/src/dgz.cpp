#include "motordefs.hpp"
#include "DGZ.hpp"
#include "auton_commons.hpp"

DGZ::DGZ(MotorDefs *md, bool ra, pros::Imu *imu){
    mtrDefs = md;
    redAlliance = ra;
    ac = new AutonCommons(mtrDefs, imu);
}

DGZ::~DGZ() {
    delete ac;
}

void DGZ::raiseLift(double degrees) {
    //mtrDefs->lift_mtr->tare_position();

    mtrDefs->lift_mtr->move(-127); // assign upward power to lift
    while(abs(mtrDefs->lift_mtr->get_position()) < degrees){
        printf("%f\n", mtrDefs->lift_mtr->get_position());
        pros::Task::delay(5);
    }
     mtrDefs->lift_mtr->move(-5); // upwards hold power on lift
}

void DGZ::runAuton(){
    ac->flipoutAndStartIntake();
    goToStack(700);
    raiseLift(1000);
    getStack();
    turnToGoal(135);
    getCubeAndGoToGoal(1800);
    deposit();
}

void DGZ::goToStack(int degrees){
    ac->driveRobot(degrees, 80);
}

void DGZ::getStack(){
    ac->driveRobot(100, 50);
    mtrDefs->lift_mtr->move(50);
    pros::Task::delay(3000);
    mtrDefs->lift_mtr->move(0);
}

void DGZ::turnToGoal(int degrees){
    ac->driveRobot(300, 80);
    if(redAlliance){
        ac->turnRobotLeft(degrees);
    } else {
        ac->turnRobotRight(degrees);
    }
}

void DGZ::getCubeAndGoToGoal(int seconds){
    ac->driveWithCoast(seconds, 80);
}

void DGZ::deposit(){
    ac->deposit();
}
