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

void close_safe(){
    drive(400, 600);
    LbackP.set(true);
    drive(-275, 400);
    turn_to(-25, 800);
    delay(200);
    LbackP.set(false);
    turn_to(0, 600);
    drive(-200, 400);
    turn_to(135, 600);
    //RfrontP.set(true);
    spinIntake(-127);
    delay(100);
    drive(1525, 600);
    //RfrontP.set(false);
    drive(320, 1000, 1, 70);
    chas.stop();
    

}

void close_rush(){
    spinIntake(127);
    drive(2240, 1200, 1.0, 127);
    LfrontP.set(false);
    spinIntake(0);
    drive(-1000, 600);
    turn_to(30, 500);
    drive(-1050, 1200);
    
    // drive_var(-110, -100, -15, 1000, LbackP, 0, 650);
    drive_var(-110, -90, -13.5, 1000, LbackP, 0, 650);

    delay(150);
    turn_to(85, 600);
    spinIntake(-127);
    RfrontP.set(true);
    delay(10);
    drive(1293, 2000, 1, 100);
    RfrontP.set(false);   
   
    chas.stop();

}

void close_rush_elims(){
    spinIntake(127);
    drive(2240, 1200, 1.0, 127);
    LfrontP.set(false);
    spinIntake(0);
    drive(-1000, 600);
    turn_to(30, 500);
    drive(-1050, 1200);
    
    // drive_var(-110, -100, -15, 1000, LbackP, 0, 650);
    drive_var(-110, -90, -13.5, 1000, LbackP, 0, 650);

    delay(150);
    turn_to(85, 600);
    spinIntake(-127);
    RfrontP.set(true);
    delay(10);
    drive(1293, 2000, 1, 100);
    RfrontP.set(false);   
    drive(-1400);
    turn_to(135, 600);
    drive(-700, 600);
    turn(-2);
    delay(200);
    LbackP.set(true);
    RbackP.set(true);
}

void far_safe(){

}


void far_rush(){
    //rush
    spinIntake(127);
    drive(2175, 1250, 1, 127);
    drive(-2400, 1300);
    spinIntake(0);
    //RfrontP.set(true);
    //RfrontP.set(false);
    delay(10);
    
    //turn to outtake
    turn(55, 400);
    spinIntake(-127);
    RfrontP.set(true);
    delay(200);
    
    //turn to align with ball under elevation bar
    turn_to(-74, 500);
    RfrontP.set(false);
    spinIntake(127);
    drive(1350, 1000);
    turn_to(-74, 300);
    drive(-1560, 1000);
    spinIntake(0);
    
    //turn to angle with the matchload bar
    turn_to(-124, 300);
    RbackP.set(true);
    drive(-770, 2000, 1, 127, LbackP, 0, 2000);
    
    // //scoop
    turn_to(-155, 300);
    LbackP.set(false);
    
    //align for arc turn
    turn_to(-117, 300);
    RbackP.set(true);
    drive_var_to(-147, -90, -35, 1000);
    turn_to(-151, 300);
    drive_const(-775, 127, RbackP, 0, 900, 800);
    drive(700, 600);
    RbackP.set(false);
    turn_to(29, 800);
    spinIntake(-127);
    delay(250);
    drive_const(500, 127, NULL, 0, 0, 900);
    turn_to(29, 400);
    drive(-400, 400);
    spinIntake(127);
    turn_to(-56, 600);
    drive(2150, 1000);
    drive(-210, 400);
    turn_to(65, 500);
    chas.stop();
    spinIntake(-90);
    drive_const(975, 127, NULL, 0, 0, 1000);
    drive(-500, 500);
    // delay(500);
    // turn_to(-12, 400);
    // chas.stop();
    // spinIntake(127);
    // drive(900, 500);
    // turn_to(100, 600);
    // LfrontP.set(true);
    // RfrontP.set(true);
    // drive_const(800, 127, NULL, 0, 0, 900);
    // spinIntake(-127);
    // drive_const(700, 127, NULL, 0, 0, 600);
    // 

    chas.stop();
    


}





void skills_lineup(){
    drive(-675, 1000);
    turn_to(45, 700);
    drive_const(-1100, 127, NULL, 0, 0, 1000);
    drive(600, 800);
    turn_to(-60, 1000); /// we gotta find a way to get this good
    drive(-175, 600);
    turn_to(-60, 600);
    RbackP.set(true);
    LbackP.set(true);
}

void skills_post_matchload(){
    
    LbackP.set(false);
    RbackP.set(false);
    //delay(300);
    drive(175); 
    turn_to(178, 1000);
    drive(-1780, 1150);
    turn_to(146, 650);

    //drive down alley
    drive(-3500, 1800, 1.0, 127.0, RbackP, 1000, 7000);
    turn_to(100, 450);
    //RbackP.set(true);
    drive(-530, 550);

    //right scoop
    drive_var(-45, -105, -42, 1200);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    drive(600, 700);
    turn_to(45, 500);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    RbackP.set(false);
    //turn_to(45);
    //drive(-1700, 1000, 5);
    drive_var(45, 80, 30);
    turn_to(-20);


    drive(-1400, 1000, 1.0, 127.0, LbackP, 125, 3000);
    RbackP.set(true);


    // turn_to(65);
    LbackP.set(true);
    // delay(300);
    // turn_to(-20);

    //turn to scoop
    drive_var(139, -21, -80, 1500, RbackP, 0, 1450);
    RbackP.set(false);
    //LbackP.toggle();
    drive_const(-2000, 127, NULL, 0, 0, 600);
    LbackP.set(false);

    //removed second push below
    // drive(800);
    // turn_to(135, 500);
    // LbackP.set(true);
    // drive_const(-2000, 127, NULL, 0, 0, 800);
    // LbackP.set(false);
    drive(1600, 1400);

    LbackP.set(true);

    // left side
    turn_to(45, 850);
    //RbackP.set(true);
    drive(-200, 500);
    RbackP.set(true);

    //second scoop on left side
    drive_var(82, -34, -90, 2000);
    LbackP.set(true);

    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    // RbackP.set(false);
    drive(1000, 2000, 1.0, 127.0, RbackP, 300, 200);
    turn_to(150, 500);
    RbackP.set(true);
    LbackP.set(true);
    drive_const(-2000, 127, NULL, 0, 0, 800);

    drive(700);

    LbackP.set(false);
    RbackP.set(false);
    turn_to(58);

    //left push among us
    drive(-2500);
    LbackP.set(true);
    //RbackP.set(true);

    //scoop for right
    drive_var(120, -15, -90);
    RbackP.set(false);

    //drive(-530);

    drive_var(120, -100, -127, 1400);
    //drive_const(-3000, 127, NULL, 0, 0, 1500);
    LbackP.set(false);
    drive(500);
    turn_to(-151, 1000);
    LbackP.set(true);
    spinIntake(-127);

    // edit this after if it's out of time
    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    drive(800);
    turn_to(-145, 1000);

    LbackP.set(true);
    RbackP.set(true);
    spinIntake(-127);
    drive_const(-2000, 127, NULL, 0, 0, 800);
    LbackP.set(false);
    RbackP.set(false);

    drive_var(-140, 0, 127);
    //drive(600);


    //turn_to(180);

   //

    //intake.move(0);
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

    
    delay(200);
    imu.tare();
    delay(200);
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
    //matchload(46);
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
    Auton("close safe", close_safe, "make if have time, after hang done"),
    Auton("close rush", close_rush, "done"),
    Auton("close elims", close_rush_elims, "done"),
    
    Auton("far safe", far_safe, "need to make fs"),
    Auton("far rush", far_rush, "need to make"),
    
    Auton("reg tuning", regr, "Linear model"),
    Auton("test", test, "For testing"),
    Auton("no auton", blank, "blank"),

    Auton("skills driver", skills_lineup, "For lineup"),
    Auton("skills no load", skills_no_load, ""),
    Auton("skills load", skills_load, "")
};

void old_close_safe(){
    drive(400);
    LbackP.set(true);
    delay(400);
    turn(-30);
    LbackP.set(false);
    turn_to(34);
    drive_const(200);
    spinIntake(-127);
    drive(1000, 1000);
    spinIntake(0);
    drive(-600, 800);

    spinIntake(-127);
    drive(1000, 900);
    spinIntake(0);
    drive(-900);

    turn_to(180);
    drive(950, 800);
    turn_to(137);
    spinIntake(-127);
    drive(1400);
    delay(1000);
    spinIntake(0);
}

void old_far_rush_elims(){
    // rush and come back
    spinIntake(127);
    drive(2310, 1200, 1, 127, RfrontP, 0, 450);
    drive(-2600, 1550);

    // position near goal 
    spinIntake(0);
    turn_to(80, 400);
    spinIntake(-127);
    delay(300);
    turn_to(-75);
    spinIntake(127);
    drive(1360);
    //turn_to(-81);
    drive(-1550);
    spinIntake(0);
    turn_to(-120, 400);
    drive(-600, 500, 1, 127, LbackP, 200, 20000);
    turn_to(-155, 350);
    LbackP.set(false);
    turn_to(-115, 350);
    
    RbackP.set(true);
    drive_var(-40, -70, -37);

    drive_const(-1500, 127, NULL, 0, 0, 450);
    RbackP.set(false);
    drive(400, 500);

    // turn around to score next push
    turn(180, 2000);
    spinIntake(-127);
    delay(200);
    drive_const(1500, 127, NULL, 0, 0, 600);
    drive(-900, 600);

    turn_to(-60, 1500);
    spinIntake(127);
    drive(2222, 1500);
    drive(-200, 300);

    //LfrontP.set(true);
    drive_var(137, 100, 0, 3000, LfrontP, 500,5000);
    spinIntake(-127);
    //LfrontP.set(true);
    drive_const(2500, 80, NULL, 0, 0, 800);
    LfrontP.set(false);
    drive(-800);

    turn_to(-78);

}

void old_rush_elims_far_2(){
    
    spinIntake(127);
    drive(2650, 1500);
    RfrontP.set(true);
    turn_to(-4, 100);
    drive(-2715, 1550);
    RfrontP.set(false);
    turn_to(75, 600);
    spinIntake(-127);
    delay(250);
    turn_to(-63,700);
    spinIntake(127);
    drive(1570, 900);
    // delay(10);
    // drive(-1450, 900);
    
    // turn_to(-92, 600);
    // drive(-850, 600, 1, 127, LbackP, 200, 20000);
    // turn_to(-155, 350);
    // LbackP.set(false);
    // turn_to(-110, 350);
    // //drive(-200, 200);
    
    // RbackP.set(true);
    // drive_var(-125, -110, -50, 400);

    // drive_const(-1000, 127, NULL, 0, 0, 450);
    // drive(400, 400);
    // delay(10);
    // RbackP.set(false);
    // turn_to(45, 600);
    // spinIntake(-127);
    // delay(00);
    // drive_const(800, 127, NULL, 0, 0, 600);
    // delay(50);
    // drive(-500, 400);
    // delay(100);
    // turn_to(-39, 600);
    // spinIntake(127);
    // drive(2110, 1300);
    // drive(-300, 300);
    // drive_var(140, 127, 20, 3000);
    // LfrontP.set(true);
    // RfrontP.set(true);
    // spinIntake(-127);
    // drive_const(900, 127, NULL, 0, 0, 500);
    
    // drive(-300, 300);
    // LfrontP.set(false);
    // RfrontP.set(false);
    // turn(180);
    // delay(10);
   

    



}

void old_old_rush_far(){
    spinIntake(127);
    drive(800, 700, 1, 127, RfrontP, 0, 2000);
    drive_var(-56, 70, 127, 1500, RfrontP, 0, 4000);
    delay(100);
    RfrontP.set(false);
    turn_to(-42, 200);
   
    drive(-3000, 2000);
    turn_to(45, 500);
    spinIntake(-127);
    delay(300);
    turn_to(-90, 600);
    spinIntake(127);
    

    drive_var_to(-140, 80, 127, 400);
    drive_var_to(-90, 127, 80, 600);
    // turn_to(-90, 200);
    drive(-1500, 1000);
    // // chas.stop();
}


#endif