#ifndef _DGZ_HPP_
#define _DGZ_HPP_

class AutonCommons;
class RobotDriver;
class MotorDefs;

/**
 * Autonomous that stacks cubes in the single stack zone.
 */
class DGZ {
    private:
        MotorDefs *mtrDefs;
        bool redAlliance;
        AutonCommons *ac;
    public:
        DGZ(MotorDefs *mtrDefs, bool ra);
        ~DGZ();
        void runAuton();
};

#endif //_DGZ_HPP_