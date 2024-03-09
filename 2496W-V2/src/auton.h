#ifndef __AUTON__
#define __AUTON__

#include "main.h"
#include "global.h"
#include "pid.h"
#include "auton_func.h"
#include "lib/auton_obj.h"
#include "regression.h"

#include "pros/misc.h"
#include "pros/rtos.h"
#include <iostream>
#include <fstream>
#include <list>
#include <iostream>

using namespace pros;
using namespace std;
using namespace pid;

void close_awp(){

    drive(400);
    LbackP.set(true);
    delay(400);
    turn(-30);
    LbackP.set(false);
    turn_to(34);
    drive_const(200);
    intake.move(-127);
    drive(1000, 1000);
    intake.move(0);
    drive(-600, 800);

    intake.move(-127);
    drive(1000, 900);
    intake.move(0);
    drive(-900);

    turn_to(180);
    drive(950, 800);
    turn_to(137);
    intake.move(-127);
    drive(1400);
    delay(1000);
    intake.move(0);


}

void close_rush(){
    intake.move(127);
    drive(2250, 2000, 1.0, 127, LfrontP, 0, 20000);
    LfrontP.set(false);
    delay(300);
    turn_to(-95);
    intake.move(0);
    RbackP.set(true);
    drive_const(-1200, 127, RbackP, 0, 460, 1000);
    turn_to(-145, 800);
    RbackP.set(false);
    drive(2400);
    turn_to(-10);
    LbackP.set(true);
    drive(-550);
    turn_to(-55);
    delay(400);
    LbackP.set(false);
    turn_to(88);

    intake.move(-127);
    drive(1650);
    delay(500);
    turn_to(91);
    drive(-1450);
    turn_to(133);
    drive(-850);

    //RbackP.set(true);

}

void far_6b_a(){
    // rush and come back
    intake.move(127);
    drive(2310, 1200, 1, 127, RfrontP, 0, 450);
    drive(-2600, 1550);

    // position near goal 
    intake.move(0);
    turn_to(80, 500);
    intake.move(-127);
    delay(300);
    turn_to(-78, 800);
    intake.move(127);
    drive(1260, 800);
    //turn_to(-81);
    drive(-1640, 925);
    intake.move(0);
    turn_to(-120, 400);
    drive(-600, 500, 1, 127, LbackP, 200, 20000);
    turn_to(-155, 350);
    LbackP.set(false);
    turn_to(-115, 350);
    
    RbackP.set(true);
    drive_var(-40, -70, -33);

    drive_const(-1500, 127, NULL, 0, 0, 450);
    RbackP.set(false);
    drive(400, 500);

    // turn around to score next push
    turn(178, 1200);
    intake.move(-127);
    delay(200);
    drive_const(1500, 127, NULL, 0, 0, 600);
    drive(-900, 600);

    turn_to(-71, 600);
    intake.move(127);
    drive(2275, 1500);

    drive_var(137, 100, 0);
    intake.move(-127);
    drive_const(2500, 127, NULL, 0, 0, 1200);
    drive(-800);

    turn_to(-78);


    

}

void far_6bsafe_a(){
    // drive_var(-20, -40, 0);
    // turn_to(0);

}


void skills_lineup(){
    drive(-675);
    turn_to(45, 1000);
    drive_const(-1100, 127, NULL, 0, 0, 1000);
    drive(600, 800);
    turn_to(-57); /// we gotta find a way to get this good
    drive(-238, 450);
    RbackP.set(true);
    LbackP.set(true);
}

void skills_post_matchload(){
    //cata_rest();

    
    LbackP.set(false);
    RbackP.set(false);
    delay(500);
    drive(238);
    turn_to(178, 2000, 1.0, 127.0, 100);
    drive(-1780, 1400);
    turn_to(146);

    //drive down alley
    drive(-3500, 2000, 1.0, 127.0, RbackP, 1000, 7000);
    turn_to(100);
    //RbackP.set(true);
    drive(-530, 1000);

    drive_var(-45, -105, -42);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    drive(600, 800);
    turn_to(45, 500);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    RbackP.set(false);
    //turn_to(45);
    //drive(-1700, 1000, 5);
    drive_var(45, 80, 30);
    turn_to(-20);


    drive(-1600, 2000, 1.0, 127.0, LbackP, 550, 3000);
    RbackP.set(true);


    // turn_to(65);
    LbackP.set(true);
    // delay(300);
    // turn_to(-20);

    //turn to scoop
    drive_var(142, -18, -80, 1500, RbackP, 0, 1000);
    //RbackP.toggle();
    //LbackP.toggle();
    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    drive(800);
    turn_to(135, 500);
    LbackP.set(true);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    drive(1600);

    LbackP.set(true);

    // left side
    turn_to(45);
    //RbackP.set(true);
    drive(-200);
    RbackP.set(true);

    drive_var(83, -25, -90, 2000);
    LbackP.set(true);

    drive_const(-2000, 127, NULL, 0, 0, 800);
    // LbackP.set(false);
    // RbackP.set(false);
    drive(800);
    turn_to(135, 500);
    RbackP.set(true);
    LbackP.set(true);
    drive_const(-2000, 127, NULL, 0, 0, 800);

    drive(1200);

    LbackP.set(false);
    RbackP.set(false);
    turn_to(58);

    //left push among us
    drive(-2500);
    LbackP.set(true);

    drive_var(120, -20, -90);

    //drive(-530);

    drive_var(120, -100, -127, 1400);
    //drive_const(-3000, 127, NULL, 0, 0, 1500);
    LbackP.set(false);
    drive(500);
    turn_to(-146, 1000);
    LbackP.set(true);
    intake.move(-127);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    drive(800);
    turn_to(-135, 1000);

    LbackP.set(true);
    RbackP.set(true);
    intake.move(-127);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    RbackP.set(false);
    drive(1600);


    //turn_to(180);

   //

    intake.move(0);
    chas.stop();


//here here here
    
    // turn_to(160);
    // RbackP.set(false);
    // LbackP.set(true);

    // drive_const(-2000, 127, NULL, 0, 0, 800);
    // LbackP.set(false);
    // drive(800);
    // //LbackP.set(true);
    // drive_const(-2000, 127, NULL, 0, 0, 800);

    // drive_var(90, 100, 40);



    // ALL BELOW IS OLD CODE 
    
    // RbackP.set(true);
    // LbackP.set(true);
    // drive(-400, 1000, 5);
    // RbackP.set(false);
    // LbackP.set(false);

    // //After pistons are off
    // //turn_to(50);
    // drive(450);
    // turn_to(-45);
    // drive(-900);



    // turn_to(60);
    // LbackP.set(true);
    // RbackP.set(true);
    // drive(-800, 1000, 0.5);
    // LbackP.set(false);
    // RbackP.set(false);
    // drive(600);
    // turn_to(0);
    // drive(-600);
    // turn_to(135);



}

void skills_no_load(){
    skills_lineup();
    // int temp_heading = imu.get_heading();
    // imu.set_heading(temp_heading);
    delay(500);
    skills_post_matchload();
}
void skills_load(){
    skills_lineup();
    int temp_heading = imu.get_heading();
    matchload(46);
    imu.set_heading(temp_heading);
    delay(500);
    skills_post_matchload();  
}



void test_all_drive(){
    drive(1750);
    delay(1000);
    drive(-1750);
    delay(1000);

    drive(1500);
    delay(1000);
    drive(-1500);
    delay(1000);

    drive(1250);
    delay(1000);
    drive(-1250);
    delay(1000);

    drive(1000);
    delay(1000);
    drive(-1000);
    delay(1000);

    drive(750);
    delay(1000);
    drive(-750);
    delay(1000);

    drive(500);
    delay(1000);
    drive(-500);
    delay(1000);

    drive(250);
    delay(1000);
    drive(-250);
    delay(1000);

}

void test_all_turn(){
    turn(180, 2000);
    delay(1000);
    turn(-179.9, 2000);
    delay(1000);

    turn(150, 2000);
    delay(1000);
    turn(-150, 2000);
    delay(1000);

    turn(135, 2000);
    delay(1000);
    turn(-135, 2000);
    delay(1000);

    turn(110, 2000);
    delay(1000);
    turn(-110, 2000);
    delay(1000);

    turn(90, 2000);
    delay(1000);
    turn(-90, 2000);
    delay(1000);

    turn(60, 2000);
    delay(1000);
    turn(-60, 2000);
    delay(1000);


    turn(45, 2000);
    delay(1000);
    turn(-45, 2000);
    delay(1000);

    turn(20, 2000);
    delay(1000);
    turn(-20, 2000);
    delay(1000);

}
void test(){
    // test_all_drive();
    test_all_turn();
    // drive_var(30, 70, 40);
}


void regr(){
    delay(100);
    con.clear();
    delay(100);
    con.print(0,0,"not good");
    delay(3000);
}

void blank(){}


std::vector<Auton> autons
{   
    Auton("skills no load", skills_no_load, ""),
    Auton("skills load", skills_load, ""),
    Auton("skills driver", skills_lineup, "For lineup"),
    Auton("close half-awp", close_awp, ""),
    Auton("close rush", close_rush, ""),
    Auton("far 6-ball", far_6b_a, ""),
    Auton("far 6-b safe", far_6bsafe_a, ""),
    
    Auton("reg tuning", regr, "Linear model"),
    Auton("test", test, "For testing"),
    Auton("no auton", blank, "blank"),
};

#endif