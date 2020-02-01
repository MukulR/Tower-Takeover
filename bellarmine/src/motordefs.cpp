#include "motordefs.hpp"

int Ports::LEFT_MTR_B_PORT = 13;
int Ports::LEFT_MTR_F_PORT = 4;
int Ports::RIGHT_MTR_B_PORT = 19;
int Ports::RIGHT_MTR_F_PORT = 15;
int Ports::TILT_MTR_PORT = 6;
int Ports::LEFT_INTAKE_MTR_PORT = 8;
int Ports::RIGHT_INTAKE_MTR_PORT = 10;
int Ports::LIFT_MTR_PORT = 17;

MotorDefs::MotorDefs() {
    left_mtr_f = new pros::Motor(Ports::LEFT_MTR_F_PORT, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_f = new pros::Motor(Ports::RIGHT_MTR_F_PORT, pros::E_MOTOR_GEARSET_18, true);
    left_mtr_b = new pros::Motor(Ports::LEFT_MTR_B_PORT, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_b = new pros::Motor(Ports::RIGHT_MTR_B_PORT, pros::E_MOTOR_GEARSET_18, true);
    tilt_mtr = new pros::Motor(Ports::TILT_MTR_PORT, pros::E_MOTOR_GEARSET_36, false);
    intake_mtr_l = new pros::Motor(Ports::LEFT_INTAKE_MTR_PORT, pros::E_MOTOR_GEARSET_18, true);
    intake_mtr_r = new pros::Motor(Ports::RIGHT_INTAKE_MTR_PORT, pros::E_MOTOR_GEARSET_18, false);
    lift_mtr = new pros::Motor(Ports::LIFT_MTR_PORT, pros::E_MOTOR_GEARSET_18, true);
}

MotorDefs::~MotorDefs() { 
    delete left_mtr_b;
    delete left_mtr_f;
    delete right_mtr_f;
    delete right_mtr_b;
    delete tilt_mtr;
    delete intake_mtr_l;
    delete intake_mtr_r;
    delete lift_mtr;
} 

