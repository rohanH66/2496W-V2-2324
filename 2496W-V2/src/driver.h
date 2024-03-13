#ifndef __DRIVER__
#define __DRIVER__

#include "main.h"
#include "global.h"
#include "lib/auton_obj.h"
#include <iostream>
#include <cmath>
#include "../include/pros/misc.h"


using namespace pros;
using namespace glb;
using namespace std;

bool intakeCooldown = false;
bool facing_side = false;
void drive()
{
    double left = abs(con.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) > 10 ? con.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) : 0;
    double right = abs(con.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)) > 10 ? con.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) : 0;
    
    
    right /= 1.2275;

    if (pto.get_status() == true)
        chas.spin(left);
    else if(left || right)
    {
        chas.spin_left(left + right);
        chas.spin_right(left - right);
    }
    else
        chas.stop();
}

void slapperCon()
{

    static bool matchload = false;
    if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
        matchload = !matchload;
    } 

    if (matchload) {
        cata.move(100);
    }
    else {
        cata.move(0);
    }
}

void distCon(int time){
    int pos = ((int) cata.get_position() % 900);
    static int deadzone = 270;
    const int realDead = 270;
    const int cutoff = 320;
    const int delay = 160; //delay for after distance detects --> start firing, for human error
    static bool isTri = false;
    static bool shoot = false;
    static int timeCount = 0;
    static bool matchload = false;

    if (isTri){
        timeCount++;
        if (timeCount>delay){
            timeCount = 0;
            isTri = false;
            shoot = true;
        }

    }
    else if (shoot){
        cata.move(127);
        timeCount++;
        if (timeCount>100) shoot = false;
    }
    else{
        // if (dist.get() < 8 && shoot == false && pos>deadzone && pos<cutoff) isTri = true;
        // Turn on when distance sensor is put back on!
    }

    if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
        matchload = !matchload;
        //cata.move(100);
    }
    if (matchload){
        cata.move(100);
    }
    else {
        if (pos>deadzone && pos<cutoff){
            deadzone = realDead - 50;
            cata.move(0);
        }
        else {
            deadzone = realDead;
            cata.move(100);
        }
    }

}

void intakeCon()
{
    if(con.get_digital(E_CONTROLLER_DIGITAL_R1)) 
		intake.move(127);
    else if(con.get_digital(E_CONTROLLER_DIGITAL_R2))
        intake.move(-127);
    else 
        intake.move(0);
}


void updateTemps(){
    static int count = 0;
    glb::temps_a[count] = motorV[count].get_temperature();
    //printf("%.2f\n",temps_a[count]);
    count++;
    if (count>7) count=0;
}

void piston_cont(bool skills)
{
    static bool pressed_once = false;

    static bool dWings = false;

    if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_B)){
        if (!pressed_once){
            hangP.set(true);
            pto.set(false);
            pressed_once = true;
        }
        else{
            hangP.set(false);
            pto.set(true);
            pressed_once = false;
        }
    }


    if (dWings){
        // if (skills){
        //     LbackP.set(true);
        //     RbackP.set(true);
        // }
        // else{
        //     LfrontP.set(true);
        //     RfrontP.set(true);
        // }
        LfrontP.set(true);
        RfrontP.set(true);
    }
    else if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_Y)){
        if (skills) RbackP.toggle();
        else RfrontP.toggle();
    }    
    
    else if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_RIGHT)){
        if (skills) LbackP.toggle();
        else LfrontP.toggle();
    }

    if (con.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){
        dWings = !dWings;

        if (!dWings){
            // if (skills){
            //     LbackP.set(false);
            //     RbackP.set(false);
            // }
            // else{
            //     LfrontP.set(false);
            //     RfrontP.set(false);
            // }
            LfrontP.set(false);
            RfrontP.set(false);
        }

    }
    // else{
    //     if (con.get_digital(E_CONTROLLER_DIGITAL_L1)){
    //         LfrontP.set(true);
    //         RfrontP.set(true);
    //     }
    //     else{
    //         LfrontP.set(false);
    //         RfrontP.set(false);
    //     }



    // }
    if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){
        // if (skills){
        //     RfrontP.toggle();
        //     LfrontP.toggle();
        // }
        // else{
        //     LbackP.toggle();
        //     RbackP.toggle();
        // }
        LbackP.toggle();
        RbackP.toggle();
    }
    
}

void print_info(int time, bool chassis_on)
{

    if(time % 50 == 0 && time % 100 != 0 && time % 150 != 0)
        con.print(0, 0, !chassis_on ? "CHASSIS OFF (left)            " : "%.1lf | %.1lf | %.1lf", chas.temp(), intake.get_temperature(), cata.get_temperature());
    if(time % 100 == 0 && time % 150 != 0)
        con.print(1, 0, "%.2f  | %.2f", chas.pos(), imu.get_heading());
    // if(time % 150 == 0)
    //     con.print(2, 0, "dis: %d     mm  ", abs((int) cata.get_position()) % 900);
}

void print_info_auton(int time, double error, double speed)
{
    if(time % 50 == 0 && time%2000 != 0) 
        con.print(0, 0, "Error: %.2f : %.2f    ", error, speed);
    if(time % 100 == 0 && time % 150 != 0 && time%2000 != 0) 
        con.print(1, 0, "%.2f : %.2f          ", imu.get_heading());
    if(time % 150 == 0 && time % 100 != 0 && time % 150 != 0 && time%2000 != 0) 
        con.print(2, 0, "%.2f | %.0f       ", error, time);
}

void print_info_R(int time, double error, double guess)
{
    if (time%50 == 0 && time%100 != 0 && time%150 != 0)
        con.print(0,0,"Reg K=%.4f" , guess);
    if(time % 100 == 0 && time%150 != 0) 
        con.print(1, 0, "Error: %.2f        ", error);
    if(time % 150 == 0) 
        con.print(2, 0, "%.2f : %.2f          ", imu.get_heading(), chas.pos());
}



void print_name(int time, string name){
    if (time % 50 == 0 and time % 2000 != 0){
        con.print(0, 0, "%s", name);
    }
}

void print_skills(int time, int count){
    if (time % 2 == 0){
        con.print(2, 0, "%d", count);
    }
}

Auton auton_selector(std::vector<Auton> autons)
{
    short int selected = 0;
    int timer = 0;

    while(true)
    {
        if(!glb::con.get_digital(pros::E_CONTROLLER_DIGITAL_A))
        {
            if(timer % 50 == 0 && timer % 100 != 0) 
                glb::con.print(0, 0, "Select: %s         ", autons.at(selected).get_name());
            if(timer % 100 == 0) 
                glb::con.print(1, 0, "%s            ", autons.at(selected).get_d());
            if(glb::con.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT) && selected > 0)
                selected--;
            if(glb::con.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT) && selected < autons.size()-1)
                selected++;
        }
        else
        {
            pros::delay(50);
            glb::con.clear();
            pros::delay(50);
            glb::con.print(0, 0, "selected:         "); 
            pros::delay(50);
            //glb::con.print(0, 0, "Selected           ");   
            glb::con.print(1, 0, "auton: %s         ", autons.at(selected).get_name());   
            pros::delay(50);
            //glb::con.print(0, 0, "Selected           ");   
            glb::con.print(2, 0, "%s             ", autons.at(selected).get_d()); 
            pros::delay(800);
            glb::con.clear();
            return autons.at(selected);
        }

        pros::delay(1);
        timer++;
    }
}

#endif