#include "auton_commons.hpp"
#include "DGZ.hpp"
#include "motordefs.hpp"

pros::ADIAnalogIn pote ('A');


auto chassis = ChassisControllerFactory::create(
        {Ports::LEFT_MTR_F_PORT, Ports::LEFT_MTR_B_PORT}, // Left motors
        {-Ports::RIGHT_MTR_F_PORT, -Ports::RIGHT_MTR_B_PORT},   // Right motors
        AbstractMotor::gearset::red, // Torque gearset
        {3.25_in, 13.5_in} // 3.25 inch wheels, 13.5 inch wheelbase width
);

auto pcontroller = AsyncControllerFactory::motionProfile(
    1.08,  // Maximum linear velocity of the Chassis in m/s
    4.0,  // Maximum linear acceleration of the Chassis in m/s/s
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
    mtrDefs->lift_mtr->move(20);
    mtrDefs->intake_mtr_l->move(127);
    mtrDefs->intake_mtr_r->move(127);
    pros::Task::delay(200);
    mtrDefs->intake_mtr_l->move(-127);
    mtrDefs->intake_mtr_r->move(-127);


    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{5.5_ft, 0_ft, 0_deg}}, "forward");
    pcontroller.setTarget("forward", false);
    pcontroller.waitUntilSettled();

    if (redAlliance){
        ac.turn(92, -1);
    } else {
        ac.turn(92, -1);
    }
    ac.waitForCompletion();

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{5.5_ft, 0_ft, 0_deg}}, "fr");
    pcontroller.setTarget("fr", false);
    pcontroller.waitUntilSettled();

    mtrDefs->intake_mtr_l->move(60);
    mtrDefs->intake_mtr_r->move(60);
    pros::Task::delay(625);
    mtrDefs->intake_mtr_l->move(0);
    mtrDefs->intake_mtr_r->move(0);

    mtrDefs->tilt_mtr->move(100);
  mtrDefs->intake_mtr_l->move(-30);
  mtrDefs->intake_mtr_r->move(-30);
  while(pote.get_value() < 3090){
    pros::Task::delay(10);
  }
  mtrDefs->tilt_mtr->move(40);
  while(pote.get_value() < 3900){
    pros::Task::delay(10);
  }
  mtrDefs->intake_mtr_l->move(0);
  mtrDefs->intake_mtr_r->move(0);
  mtrDefs->tilt_mtr->move(0);

  pros::Task::delay(100);

  mtrDefs->intake_mtr_l->move(60);
  mtrDefs->intake_mtr_r->move(60);

  mtrDefs->left_mtr_f->move(-30);
  mtrDefs->left_mtr_b->move(-30);
  mtrDefs->right_mtr_f->move(-30);
  mtrDefs->right_mtr_b->move(-30);

  pros::Task::delay(2000);

  mtrDefs->intake_mtr_l->move(0);
  mtrDefs->intake_mtr_r->move(0);

  mtrDefs->left_mtr_f->move(0);
  mtrDefs->left_mtr_b->move(0);
  mtrDefs->right_mtr_f->move(0);
  mtrDefs->right_mtr_b->move(0);


    /*

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{2_ft, 0_ft, 0_deg}}, "move back");
    pcontroller.setTarget("move back", true);
    pcontroller.waitUntilSettled(); // move back

    if(redAlliance){
        ac.turn(90, -1);
    } else {
        ac.turn(90, 1);
    }
    ac.waitForCompletion();

    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{4_ft, 0_ft, 0_deg}}, "goal");
    pcontroller.setTarget("goal", false);
    pcontroller.waitUntilSettled();

    if(redAlliance){
        ac.turn(51, -1);
    } else {
        ac.turn(51, 1);
    }
    ac.waitForCompletion();

    mtrDefs->intake_mtr_l->move(0);
    mtrDefs->intake_mtr_r->move(0);
    
    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{0.8_ft, 0_ft, 0_deg}}, "to goal");
    pcontroller.setTarget("to goal", false);
    pcontroller.waitUntilSettled();

    mtrDefs->tilt_mtr->move(-70);
    pros::Task::delay(2500);
    mtrDefs->tilt_mtr->move(-10);


    pcontroller.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
    pcontroller.setTarget("backup", true);
    pcontroller.waitUntilSettled();
    */
}


