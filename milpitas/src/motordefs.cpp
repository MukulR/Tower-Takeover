#include "motordefs.hpp"

MotorDefs::MotorDefs() {
    left_mtr_f = new pros::Motor(15, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_f = new pros::Motor(16, pros::E_MOTOR_GEARSET_18, true);
    left_mtr_b = new pros::Motor(13, pros::E_MOTOR_GEARSET_18, false);
    right_mtr_b = new pros::Motor(20, pros::E_MOTOR_GEARSET_18, true);
    tilt_mtr = new pros::Motor(4, pros::E_MOTOR_GEARSET_36, true);
    intake_mtr_l = new pros::Motor(2, pros::E_MOTOR_GEARSET_18, true);
    intake_mtr_r = new pros::Motor(7, pros::E_MOTOR_GEARSET_18, false);
    lift_mtr = new pros::Motor(8, pros::E_MOTOR_GEARSET_36, true);
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

