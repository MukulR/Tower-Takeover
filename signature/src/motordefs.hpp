//motor definitions for full robot
#include "main.h"
#ifndef _MOTORDEFS_H_
#define _MOTORDEFS_H_

//pros::Imu gyro('D');

class Ports {
    public:
        static int LEFT_MTR_F_PORT;
        static int RIGHT_MTR_F_PORT;
        static int LEFT_MTR_B_PORT;
        static int RIGHT_MTR_B_PORT;
        static int TILT_MTR_PORT;
        static int LEFT_INTAKE_MTR_PORT;
        static int RIGHT_INTAKE_MTR_PORT;
        static int LIFT_MTR_PORT;
};

class MotorDefs {
    public:
        pros::Motor *left_mtr_f;
        pros::Motor *left_mtr_b;
        pros::Motor *right_mtr_f;
        pros::Motor *right_mtr_b;
        pros::Motor *tilt_mtr;
        pros::Motor *intake_mtr_l;
        pros::Motor *intake_mtr_r;
        pros::Motor *lift_mtr;

        MotorDefs();
        ~MotorDefs();
};


#endif //_MOTORDEFS_H_