#ifndef _SGZ_HPP_
#define _SGZ_HPP_

class RobotDriver;
class MotorDefs;

/**
 * Autonomous that stacks cubes in the single stack zone.
 */
class SGZ {
    private:
        MotorDefs *mtrDefs;
        bool redAlliance;
    public:
        SGZ(MotorDefs *mtrDefs, bool ra);
        ~SGZ();
        void runAuton();
};

#endif //_SGZ_HPP_