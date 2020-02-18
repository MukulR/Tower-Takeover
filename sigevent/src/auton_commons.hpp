#ifndef _AUTON_COMMONS_HPP_
#define _AUTON_COMMONS_HPP_

class MotorDefs;
namespace pros {
    class Imu;
}

class AutonCommons {
    public:
        AutonCommons(MotorDefs *md, pros::Imu *imu);
        ~AutonCommons();
        MotorDefs *mtrDefs;
        void waitForCompletion();
        void driveRobot(double degrees, int velocity);
        void driveRobotBack(double degrees, int velocity);
        void driveWithCoast(int time, int power);
        void turnRobotRight(double degrees);
        void turnRobotLeft(double degrees);
        void turnRobotLeftSlowly(double degrees);
        void turnRobotRightSlowly(double degrees);
        void turnRightToZeroHeading();
        void turnLeftToZeroHeading();
        void deposit();
        void flipoutAndStartIntake();
        void backup();
        static void tilt(void *param);
        void alignCubeToIntake();
        void depositStack();
    private:
        pros::Imu *imu;
};


#endif // _AUTON_COMMONS_HPP_