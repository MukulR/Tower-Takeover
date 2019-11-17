#include "okapi/api.hpp"

#ifndef _AUTON_COMMONS_HPP_
#define _AUTON_COMMONS_HPP_

class MotorDefs;

using namespace okapi;

class AutonCommons {
    private:
        MotorDefs *mtrDefs;
    public:
        static ChassisControllerIntegrated createChasis();
        static AsyncMotionProfileController createAsyncMotionProfileController(ChassisControllerIntegrated *cci);

    public:
        AutonCommons(MotorDefs *mtrDefs);
        ~AutonCommons();

        void turn(double angle, int multiplier);
        void tilt(int power, int time);
        void lift(int power, int time, int multiplier);
        void waitForCompletion();


};

#endif //_AUTON_COMMONS_HPP_