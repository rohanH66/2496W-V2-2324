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

}

void close_rush(){

}

void far_6b_a(){

}

void far_6bsafe_a(){

}

void skills(){

}

void skills_lineup(){

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
    test_all_drive();
    test_all_turn();
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
    Auton("close half-awp", close_awp, ""),
    Auton("close rush", close_rush, ""),
    Auton("far 6-ball", far_6b_a, ""),
    Auton("far 6-b safe", far_6bsafe_a, ""),
    Auton("skills", skills, ""),
    Auton("skills driver", skills_lineup, "For lineup"),
    Auton("reg tuning", regr, "Linear model"),
    Auton("test", test, "For testing"),
    Auton("no auton", blank, "blank"),
};

#endif