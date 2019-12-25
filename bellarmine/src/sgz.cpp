#include "auton_commons.hpp"
#include "motordefs.hpp"
#include "SGZ.hpp"

auto myChassis = ChassisControllerFactory::create(
        {Ports::LEFT_MTR_F_PORT, Ports::LEFT_MTR_B_PORT}, // Left motors
        {-Ports::RIGHT_MTR_F_PORT, -Ports::RIGHT_MTR_B_PORT},   // Right motors
        AbstractMotor::gearset::green, // Torque gearset (SHOULD BE GREEN)
        {3.25_in, 13.5_in} // 3.25 inch wheels, 13.5 inch wheelbase width
);

auto profileController = AsyncControllerFactory::motionProfile(
    1,  // Maximum linear velocity of the Chassis in m/s /3
    2,  // Maximum linear acceleration of the Chassis in m/s/s /15
    10, // Maximum linear jerk of the Chassis in m/s/s/s /15
    myChassis // Chassis Controller
);


SGZ::SGZ(MotorDefs *md, bool ra){
    mtrDefs = md;
    redAlliance = ra;
}

SGZ::~SGZ() {}

void tilter(void *param){
  MotorDefs *mtrs;
  MotorDefs mtrDefs;
  mtrs = &mtrDefs;
  mtrs->tilt_mtr->move(127);
  pros::Task::delay(1300);
  mtrs->tilt_mtr->move(0); 
}

void SGZ::runAuton(){
  AutonCommons ac(mtrDefs);
  mtrDefs->intake_mtr_l->move(127);
  mtrDefs->intake_mtr_r->move(127);

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{4_ft, 0_ft, 0_deg}}, "f");
  profileController.setTarget("f", false);
  profileController.waitUntilSettled();

  ac.turn(30, -1);

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{4.3_ft, 0_ft, 0_deg}}, "back");
  profileController.setTarget("back", true);
  profileController.waitUntilSettled();

  ac.turn(42, 1);

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{3_ft, 0_ft, 0_deg}}, "fwd");
  profileController.setTarget("fwd", false);
  profileController.waitUntilSettled();

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
  profileController.setTarget("backup", true);
  profileController.waitUntilSettled();

  ac.turn(120, 1);

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{1.7_ft, 0_ft, 0_deg}}, "to goal");
  profileController.setTarget("to goal", false);
  profileController.waitUntilSettled();

  ac.tilt(127, 2000);

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{1.7_ft, 0_ft, 0_deg}}, "ba");
  profileController.setTarget("a", true);
  profileController.waitUntilSettled();
  




  /*

  profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{3.8_ft, -3.8_ft, 0_deg}}, "a");
  profileController.setTarget("a", true);

  //profileController.generatePath({Point{0_ft,0_ft,0_deg}, Point{1_ft, 1_ft, 90_deg}}, "b");
  profileController.waitUntilSettled();
  */
 // profileController.setTarget("b", true);
  //profileController.waitUntilSettled();

}
  /*
  AutonCommons ac(mtrDefs);
  mtrDefs->tilt_mtr->move(-10);
  ac.lift(127, 900, 1);
  pros::Task::delay(200);
  ac.lift(127, 900, -1);

  mtrDefs->tilt_mtr->move(60);
  pros::Task::delay(2300);
  mtrDefs->tilt_mtr->move(0);


  mtrDefs->intake_mtr_l->move(127);
  mtrDefs->intake_mtr_r->move(127);

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{6_ft, 0_ft, 0_deg}}, "forward");
  profileController.setTarget("forward", false);
  profileController.waitUntilSettled();

  //profileController.removePath("forward");
  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1.5_ft, 0_ft, 0_deg}}, "backward");
  profileController.setTarget("backward", true);
  profileController.waitUntilSettled();

  //profileController.removePath("backward");
  if(redAlliance){
    ac.turn(135, 1); //turn right 135 degrees
  } else {
    ac.turn(135, -1); //turn left 135 degrees
  }
  ac.waitForCompletion();

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "go to goal");
  profileController.setTarget("go to goal", false);
  profileController.waitUntilSettled();

  mtrDefs->intake_mtr_l->move(0);
  mtrDefs->intake_mtr_r->move(0);
  
  mtrDefs->tilt_mtr->move(-70);
  pros::Task::delay(2500);
  mtrDefs->tilt_mtr->move(-10);


  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
  profileController.setTarget("backup", true);
  profileController.waitUntilSettled();
}
*/
/*
void SGZ::runAuton(){
  AutonCommons ac(mtrDefs);
  ac.lift(127, 900, 1);
  pros::Task::delay(200);
  ac.lift(127, 900, -1);
  pros::Task tiltTask(tilter);

  mtrDefs->intake_mtr_l->move(127);
  mtrDefs->intake_mtr_r->move(127);

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3.57_ft, 0_ft, 0_deg}}, "forward");
  profileController.setTarget("forward", false);
  profileController.waitUntilSettled();

  if(redAlliance){
    ac.turn(40, -1);
  } else {
    ac.turn(40, 1);
  }
  ac.waitForCompletion();

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{0.5_ft, 0_ft, 0_deg}}, "tower");
  profileController.setTarget("tower", false);
  profileController.waitUntilSettled();

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{0.5_ft, 0_ft, 0_deg}}, "tower");
  profileController.setTarget("tower", true);
  profileController.waitUntilSettled();

  if(redAlliance){
    ac.turn(40, 1);
  } else {
    ac.turn(40, -1);
  }
  ac.waitForCompletion();

  //profileController.removePath("forward");
  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1.8_ft, 0_ft, 0_deg}}, "backward");
  profileController.setTarget("backward", true);
  profileController.waitUntilSettled();

  //profileController.removePath("backward");
  if(redAlliance){
    ac.turn(135, 1); //turn right 135 degrees
  } else {
    ac.turn(135, -1); //turn left 135 degrees
  }
  ac.waitForCompletion();

  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{1_ft, 0_ft, 0_deg}}, "go to goal");
  profileController.setTarget("go to goal", false);
  profileController.waitUntilSettled();

  mtrDefs->intake_mtr_l->move(0);
  mtrDefs->intake_mtr_r->move(0);
  
  mtrDefs->tilt_mtr->move(-70);
  pros::Task::delay(2500);
  mtrDefs->tilt_mtr->move(-10);


  profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{3_ft, 0_ft, 0_deg}}, "backup");
  profileController.setTarget("backup", true);
  profileController.waitUntilSettled();
}
*/