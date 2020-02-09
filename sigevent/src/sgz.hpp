#ifndef _SGZ_HPP_
#define _SGZ_HPP_

class AutonCommons;
class MotorDefs;
namespace pros{
    class Imu;
}

/**
 * Autonomous that stacks cubes in the single stack zone.
 */
class SGZ {
    private:
        MotorDefs *mtrDefs;
        AutonCommons *ac;
        bool redAlliance;
        void flipoutAndStartIntake();
        void turn(double degrees);
        void driveBack(int degrees); //MEU
        void turnToFaceCubes(double degrees);
        void pickupCubes(int degrees);
        void driveToGoal(int degrees);
        void deposit();
    public:
        SGZ(MotorDefs *mtrDefs, bool ra, pros::Imu *imu);
        ~SGZ();
        void runAuton();
        
};

#endif //_SGZ_HPP_