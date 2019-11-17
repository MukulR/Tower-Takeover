#include "auton_commons.hpp"
#include "DGZ.hpp"
#include "motordefs.hpp"

auto chassis = ChassisControllerFactory::create(
        {Ports::LEFT_MTR_F_PORT, Ports::LEFT_MTR_B_PORT}, // Left motors
        {-Ports::RIGHT_MTR_F_PORT, -Ports::RIGHT_MTR_B_PORT},   // Right motors
        AbstractMotor::gearset::red, // Torque gearset
        {3.25_in, 13.5_in} // 3.25 inch wheels, 13.5 inch wheelbase width
);

auto pcontroller = AsyncControllerFactory::motionProfile(
    1.0,  // Maximum linear velocity of the Chassis in m/s
    2.0,  // Maximum linear acceleration of the Chassis in m/s/s
    10.0, // Maximum linear jerk of the Chassis in m/s/s/s
    chassis // Chassis Controller
);

DGZ::DGZ(MotorDefs *md, bool ra){
    mtrDefs = md;
    redAlliance = ra;

}

DGZ::~DGZ() {}

void DGZ::runAuton(){  
    AutonCommons ac(mtrDefs);
    ac.lift(127, 900, 1);
    pros::Task::delay(200);
    ac.lift(127, 900, -1);

    mtrDefs->tilt_mtr->move(60);
    pros::Task::delay(2300);
    mtrDefs->tilt_mtr->move(0);

    mtrDefs->intake_mtr_l->move(127);
    mtrDefs->intake_mtr_r->move(127);

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{7.3_ft, 0_ft, 0_deg}}, "forward");
    pcontroller.setTarget("forward", false);
    pcontroller.waitUntilSettled();

    if(redAlliance){
        ac.turn(92, 1);
    } else {
        ac.turn(92, -1);
    }
    ac.waitForCompletion();

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{0.5_ft, 0_ft, 0_deg}}, "to tower");
    pcontroller.setTarget("to tower", false);
    pcontroller.waitUntilSettled();


    ac.lift(127, 2500, 1); // lift lift
    mtrDefs->lift_mtr->move(-10);

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{0.35_ft, 0_ft, 0_deg}}, "tower");
    pcontroller.setTarget("tower", false);
    pcontroller.waitUntilSettled(); // move a bit forward

    mtrDefs->intake_mtr_l->move(127);
    mtrDefs->intake_mtr_r->move(127);

    ac.lift(60, 3500, -1); // bring down lift to intake stack of 4
    pros::Task::delay(200);
    if(redAlliance){
        ac.turn(120, 1);
    } else {
        ac.turn(120, -1);
    }
    ac.waitForCompletion();

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{5_ft, 0_ft, 0_deg}}, "goal");
    pcontroller.setTarget("goal", false);
    pcontroller.waitUntilSettled();

    mtrDefs->intake_mtr_l->move(0);
    mtrDefs->intake_mtr_r->move(0);
    
    mtrDefs->tilt_mtr->move(-70);
    pros::Task::delay(2500);
    mtrDefs->tilt_mtr->move(-10);


    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
    pcontroller.setTarget("backup", true);
    pcontroller.waitUntilSettled();
}


