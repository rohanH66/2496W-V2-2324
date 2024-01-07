#ifndef __DRIVER__
#define __DRIVER__

#include "main.h"
#include "global.h"
#include "lib/auton_obj.h"
#include <iostream>
#include <cmath>

#define TURN_K 2.2

//using namespace pros;
using namespace glb;
//using namespace std;
bool intakeCooldown = false;
void drive()
{
    double left = abs(con.get_analog(E_CONTROLLER_ANALOG_LEFT_Y)) > 10 ? con.get_analog(E_CONTROLLER_ANALOG_LEFT_Y) : 0;
    double right = abs(con.get_analog(E_CONTROLLER_ANALOG_RIGHT_X)) > 10 ? con.get_analog(E_CONTROLLER_ANALOG_RIGHT_X) : 0;
    
    // int power = con.get_analog(ANALOG_LEFT_Y); // left joystick y axis is power
    // int turn = con.get_analog(ANALOG_RIGHT_X); // right joystick x axis controls turn


    // double turn = (abs(valForTurn) * valForTurn / 75);
    // double turn = valForTurn; 
    if(left || right)
    {
        RF.move(left - right);
        RM.move(left - right); //hii
        RB.move(left - right); // hi
        LF.move(left + right);
        LM.move(left + right);
        LB.move(left + right);
    }
    else{
        RF.move(0);
        RM.move(0); //hii
        RB.move(0); // hi
        LF.move(0);
        LM.move(0);
        LB.move(0);
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

void piston_cont()
{
    if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){
        LwingsP.toggle();
        RwingsP.toggle();
    }
}

void print_info(int time, bool chassis_on)
{

    if(time % 50 == 0 && time % 100 != 0 && time % 150 != 0)
        con.print(0, 0, !chassis_on ? "CHASSIS OFF (right)            " : "%.1lf | %.1lf", intake.get_temperature());
    if(time % 100 == 0 && time % 150 != 0) 
        con.print(1, 0, "%.2f", imu.get_heading());
  //if(time % 150 == 0)
        //con.print(2, 0, "auton: %s         ", (*auton).get_name());
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
            pros::delay(1500);
            glb::con.clear();
            return autons.at(selected);
        }

        pros::delay(1);
        timer++;
    }
}

#endif