#ifndef __AUTON__
#define __AUTON__

#include "main.h"
#include "global.h"
#include "pid.h"
#include "auton_func.h"
#include "lib/auton_obj.h"

#include "pros/misc.h"
#include "pros/rtos.h"
#include <iostream>
#include <fstream>
#include <list>
#include <iostream>

using namespace pros;
using namespace std;
using namespace pid;

void left_safe(){
    chas.spin(127);
    delay(100);
    chas.stop();
    delay(600);
    LwingsP.set(true);
    delay(600);
    chas.spin(-127);
    delay(90);
    chas.stop();
    delay(600);
    chas.spin_left(-127);
    chas.spin_right(127);
    delay(100);
    chas.stop();
    delay(600);
    chas.spin_left(127);
    chas.spin_right(-127);
    delay(100);
    chas.spin(127);
    delay(150);
    chas.stop();
    chas.spin_left(127);
    chas.spin_right(0);
    delay(180);
    chas.stop();
    intake.move(-127);
    delay(500);
    chas.spin_left(-127);
    chas.spin_right(127);
    delay(250);
    chas.stop();
    delay(500);
    chas.spin(-127);
    delay(120);
    chas.stop();
    delay(1000);
    chas.spin(127);
    delay(200);
    chas.stop();
    delay(10);
    chas.spin_left(0);
    chas.spin_right(127);
    delay(575);
    chas.stop();
    delay(100);
    chas.spin(127);
    delay(25);
    chas.stop();

}

void left_straight(){}

void left_diagonal(){}

void elims_left(){}

void rush_right_block(){}

void rush_right_straight(){}

void right_safe_bar(){
    chas.spin(-127);
    delay(2000);
    chas.stop();
}

void skills(){}


void blank(){}

void six_ball(){}

void test_mode()
{
    long long timer = 0;
    double temp_kp = DRIVE_KP_H;
    double temp_ki = DRIVE_KI_H;
    double temp_kd = DRIVE_KD_H;
    double temp_imuk = IMU_K_H;
    int selected = 0;
    string items[4] ={"KP","KI","KD","IMU_K"};

    while(true)
    {
        if(timer % 50 == 0) 
            glb::con.print(0, 0, "Constant tuning: %s         ", items[selected]);

        if(glb::con.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT) && selected > 0)
            selected--;

        if(glb::con.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT) && selected < 3)
            selected++;

        if(glb::con.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP))
            selected++;
        if(glb::con.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN))
            selected++;

        if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_A)) {}
        pros::delay(1);
        timer++;
    }
}

void test(){
    turn(180);
    delay(200);

    turn(-90);
    delay(200);

    turn(45);
    delay(200);

    turn(-22.5);
    delay(200);

    turn(12.25);
    delay(200);

    turn(-6);
    delay(200);

    turn(3);
    delay(200);

    turn(-1);
}

std::vector<Auton> autons
{
    Auton("safe left", left_safe, ""),
    Auton("block rush right", rush_right_block, ""),
    Auton("straight rush right", rush_right_straight, ""),
    Auton("bar safe right", right_safe_bar, ""),
    Auton("elims rush", elims_left, ""),
    Auton("elims straight", left_straight, ""),
    Auton("elims diagonal", left_diagonal, ""),
    Auton("V1 skills", skills, ""),
    Auton("no auton", blank, "blank"),
    Auton("test", test, "test_function")
};

#endif