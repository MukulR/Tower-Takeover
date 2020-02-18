#ifndef _DGZ_HPP_
#define _DGZ_HPP_

class AutonCommons;
class MotorDefs;
namespace pros{
    class Imu;
}

/**
 * Autonomous that stacks cubes in the single stack zone.
 */
class DGZ {
    private:
        MotorDefs *mtrDefs;
        AutonCommons *ac;
        bool redAlliance;
        /*
        void flipoutAndStartIntake();
        void turn(double degrees);
        void driveBack(int degrees); //MEU
        void driveBackFast(int degrees);
        void turnToFaceCubes(double degrees);
        void pickupFirstCubes(int degrees);
        void pickupSecondCubes(int degrees);
        void driveToGoal(int degrees);
        void deposit();
        */
        void raiseLift(double degrees);
        void goToStack(int degrees);
        void getStack();
        void turnToGoal(int degrees);
        void getCubeAndGoToGoal(int seconds);
        void deposit();

    public:
        DGZ(MotorDefs *mtrDefs, bool ra, pros::Imu *imu);
        ~DGZ();
        void runAuton();
        
};

#endif // _DGZ_HPP_