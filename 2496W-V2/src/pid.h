#ifndef __PID__
#define __PID__

#include "main.h"
#include "global.h"
#include "auton_func.h"
#include "lib/chassis.h"

#include "pros/misc.h"
#include "pros/rtos.h"
#include <iostream>
#include <fstream>
#include <list>
#include <cmath>

using namespace pros;
using namespace std;
using namespace glb;

// #define DRIVE_KP 0.2

// #define DRIVE_KI 0
// #define DRIVE_KD 0
// #define IMU_K 0

namespace pid
{
    double start_head = 0; 
    double end_head = 0;
    double global_heading;

    
    double rCount = 0;
    double driveR(double target_dist, double kP, double kI = 0, double kD = 0, int timeout=1000, double mult=1.0, double max_speed=127, int exit_time=100)
    {   
        pid::rCount++;

        //Set Variables
        double target = target_dist + chas.pos();
        double error = target - chas.pos();
        double prev_error;
        double integral = 0;
        double derivative = 0;

        bool exit = false;
        bool same_error = false;

        int scaler = 100;
        int time = 0;

        while (time < timeout)
        {
            prev_error = error;
            
            //P
            error = target - chas.pos();
            //I
            if(fabs(error)<30) {
                integral += error;
            }
            //D
            derivative = (error - prev_error) * 1000;
            //PID
            double speed = mult * (error * kP + integral * kI + derivative * kD);


            //Cap speed and correction sum to max
            if (fabs(speed) > max_speed) 
            {
                double multiplier = max_speed/(fabs(speed));
                speed *= multiplier;
            }

            chas.spin(speed);

            //Logging
            print_info_R(time, error * (target_dist<0 ? -1 : 1), kP);
            
            //Prevent infinite loops
            delay(2);
            time++;
        }
        chas.stop();
        return error * (target_dist<0 ? -1 : 1);

    }

    void drive(double target_dist, int timeout=2000, double mult=1.0, double max_speed=127, Piston pis = NULL, int piston_time = 0, int piston_open_time = 0, int exit_time=100)
    {
        double drive_kp = 4.62646 * std::pow(fabs(target_dist), -0.689989) + 0.107432; 
        //NEW:
        // 4.62646 * std::pow(fabs(target_dist), -0.689989) + 0.107432 for 0
        // 2.04035 * std::pow(fabs(target_dist), -0.534162) + 0.0949831 for 2.5
        
        
        
        //((17.6647 * (pow(fabs(target_dist), -0.975028))) + 0.139685) //0.14, 0.16929
        //500: 0.1777
        //1000: 0.1685
        //2000: 0.1429
        double drive_ki = 0.002;
        double drive_kd = 0; //0 for good


        double imu_k = 0.001;

        if (fabs(end_head) - fabs(imu.get_heading()) > 1) {
            start_head += end_head-imu.get_heading();
        }

        int starting = 180;
        start_head -= starting;
        imu.set_heading(starting);

        //Set Variables
        double target = target_dist + chas.pos();
        double error = target - chas.pos();
        double prev_error;
        double integral = 0;
        double kintegral = 0;
        double derivative = 0;
        double init_heading = imu.get_heading();
        double heading_error = 0;
        double error_range_time = 0;
        bool start_positive = target_dist >= 0 ? true : false;
        double early_exit_time = 0;

        bool exit = false;
        bool same_error = false;

        int scaler = 100;
        int time = 0;
        pis.set(false);

        int piston_on_time = piston_time + piston_open_time;

        while (time < timeout)
        {
            if (time>piston_on_time) pis.set(false);
            else if (time>piston_time) pis.set(true);

            prev_error = error;
            
            //P
            error = target - chas.pos();
            //I
            if(fabs(error)<30) {
                integral += error;
            }
            //D
            derivative = (error - prev_error) * 1000;

            //Correct sides, ensure heading stays same as beginning
            heading_error = init_heading - imu.get_heading();

            //PID
            double speed = mult * (error * drive_kp + integral * drive_ki + derivative * drive_kd);

            //Heading correction
            kintegral += heading_error;

            double correction = (kintegral * imu_k);

            //Cap speed and correction sum to max
            if (fabs(speed) + fabs(correction) > max_speed) 
            {
                double multiplier = max_speed/(fabs(speed) + fabs(correction));
                speed *= multiplier;
                correction *= multiplier;
            }

            //Exit Loop
            if (fabs(error) < 6) 
            {
                if(!exit)
                    exit = true;
                else
                    error_range_time++;
                if (exit_time <= error_range_time)
                    break;
            }
            
            // if (fabs(error) < 20 && (std::round(prev_error * scaler) / scaler - std::round(error * scaler) / scaler < 0.2)) // 0.15
            // {
            //     if(!same_error)
            //         same_error = true;
            //     else
            //         early_exit_time += 10;
            //     if (exit_time <= early_exit_time)
            //         break;
            // }

            //Keep sides moving the same distances
            // chas.spin_left(speed + correction * speed / 127.0);
            // chas.spin_right(speed - correction * speed / 127.0);
            chas.spin(speed);

            //Logging
            print_info_auton(time, error, speed);
            
            //Prevent infinite loops
            pros::delay(1);
            time++;
        }
        chas.stop();
        double diff = imu.get_heading() - starting;
        if (fabs(diff)>2) {
            start_head+=diff;
        }
        
        end_head = imu.get_heading();

        global_heading += imu.get_heading() - starting;

        //pis.set(false);
    }


    void drive_const(double distance, int speed=127, Piston pis = NULL, int piston_activate_time = 0, int piston_on_dur = 0, int timeout=3000)
    {
        int time = 0;
        double start_pos = glb::chas.pos();
        double target = start_pos + distance;
        double s = distance / fabs(distance) * abs(speed);
        
        double straight_kP = 1.8;
        double straight_kI = 0.7;
        double straight_i = 0;
        double init_heading = global_heading;
        double cur_heading = glb::imu.get_heading();
        double last_heading = glb::imu.get_heading();

        int piston_on_time = piston_activate_time + piston_on_dur;

        while((distance < 0 ? glb::chas.pos() > target : glb::chas.pos() < target) && time < timeout)
        {
            if (time>piston_on_time) pis.set(false);
            else if (time>piston_activate_time) pis.set(true);
            // inertial wrapping
            if(cur_heading - last_heading > 100)
            {
                global_heading += (cur_heading - 360) - last_heading;
            }
            else if(cur_heading - last_heading < -100)
            {
                global_heading += cur_heading + (360 - last_heading);
            }
            else
            {
                global_heading += cur_heading - last_heading;
            }

            last_heading = cur_heading;
            cur_heading = glb::imu.get_heading();

            straight_i += (global_heading - init_heading) / 100;
            double correction = straight_i * straight_kI + (global_heading - init_heading) * straight_kP;
            
            glb::chas.spin_left(s - correction);
            glb::chas.spin_right(s + correction);
            pros::delay(10);
            time += 10;
        }

        glb::chas.stop();
    }

    double turn_f(double error)
    {
        return error / fabs(error) * (25 * log(0.25 * (fabs(error) + 4)) + 5);
    }

    void turn(double target_deg, int timeout=1000, bool early_exit = true, double multi=1.0, double max_speed=127, int exit_time=75)
    {  
        
        // fix turns more/less than 180
        if (target_deg > 180) target_deg -= 360;
        else if (target_deg <= -180) target_deg += 360;

        
        double TURN_KP;
        double TURN_KI;
        double TURN_KD;

        
        if (fabs(target_deg) <= 185) 
        {
            TURN_KP = 4.6;
            TURN_KI = 1.05; 
            TURN_KD = 0.37;  
        }
        if (fabs(target_deg) < 150) { 
            TURN_KP = 4.75; //last=4.75//4.3, 4.5
            TURN_KI = 0.47; //last=.4//0.08, .1, .42
            TURN_KD = 0.33; //last=.26//.25, .26(earlier), .27(90)
        }
        if (fabs(target_deg) < 110){
            TURN_KP = 4.75; 
            TURN_KI = 0.47; 
            TURN_KD = 0.33; 
        }
        if (fabs(target_deg) < 65){
            TURN_KP = 4.5; 
            TURN_KI = 0.36; 
            TURN_KD = 0.26; 
        }


        //3/3 180deg good ig
        // TURN_KP = 4.6; /
        // TURN_KI = 0.55; 
        // TURN_KD = 0.37;  

        //3/3 135deg good enough?????????????????
        // TURN_KP = 4.5; 
        // TURN_KI = 0.36; //.36
        // TURN_KD = 0.3;  //.26

        //3/3 45deg tuned below
        // TURN_KP = 4.5; 
        // TURN_KI = 0.36; 
        // TURN_KD = 0.26; 


        // 3/3 90deg Tuned below
        // TURN_KP = 4.75; 
        // TURN_KI = 0.42; 
        // TURN_KD = 0.33; 

        int starting;

        if (fabs(end_head) - fabs(imu.get_heading()) > 1) {
            start_head += end_head-imu.get_heading();
        }
        if (target_deg > 150)
            starting = 30;
        else if (target_deg < -150)
            starting = 330;
        else
            starting = 180;
        
        imu.set_heading(starting);

        double target = target_deg + imu.get_heading();
        double error = target_deg;
        double prev_error;
        double integral = 0;
        double derivative = 0;
        double error_range_time;
        double early_exit_time = 0;

        bool exit = false;
        bool same_error = false;


        int time = 0;

        //int scaler = (target_deg<20 ? 1000 : 100);
        int scaler = 100;


        while (time<timeout)
        {
            double speed;
            prev_error = error;
            error = target - imu.get_heading();
            
            if (target_deg < 120){
                if(fabs(error) < 2){
                    integral += error / 100;
                }
            }
            else{
                if(fabs(error) < 2){
                    integral += error / 100;
                }
            }
            derivative = (error - prev_error) * 100;

            if (derivative){
                speed = error * TURN_KP + integral * TURN_KI + derivative * TURN_KD;
            }

            if (fabs(speed) > max_speed) 
            {
                double multiplier = max_speed/fabs(speed);
                speed *= multiplier;
            }

            if (fabs(error) < .8) // 0.15
            {
                if(!exit)
                    exit = true;
                else
                    error_range_time += 10;
                if (exit_time <= error_range_time)
                    break;
            }

            // if (fabs(error) < 2 && (std::round(prev_error * scaler) / scaler - std::round(error * scaler) / scaler < 0.02)) // 0.15
            // {
            //     if(!same_error)
            //         same_error = true;
            //     else
            //         early_exit_time += 10;
            //     if (exit_time <= early_exit_time)
            //         break;
            // }
            // if (target_deg > 0 && speed < 0 &&(-6<error && error<0.3)) speed *= K_BOOST;
            // else if (target_deg < 0 && speed > 0 && (0.3<error && error<6)) speed *= K_BOOST;

            chas.spin_left(speed);
            chas.spin_right(-speed);

            print_info_auton(time, error, speed);

            pros::delay(10);
            time+= 10;
        }
        chas.stop();

        double diff = imu.get_heading() - starting;
        
        start_head+=diff;
        
        end_head = imu.get_heading();

        global_heading += imu.get_heading() - starting;
    }

    void turn_to(double degree_to, int timeout=1000, double multi=1.0, double max_speed=127, int exit_time=100)
    {
        double degree = degree_to - global_heading;
        degree = (degree > 180) ? -(360 - degree) : ((degree < -180) ? (360 + degree) : (degree)); // optimize the turn direction
        turn(degree, timeout, multi, max_speed, exit_time);
    }

    void drive_var(double degree_to, int l_s=127, int r_s = 127, int timeout=3000)
    {

        int time = 0;

        int starting;

        if (fabs(end_head) - fabs(imu.get_heading()) > 1) {
            start_head += end_head-imu.get_heading();
        }
        if (degree_to > 150)
            starting = 30;
        else if (degree_to < -150)
            starting = 330;
        else
            starting = 180;
        
        imu.set_heading(starting);

        double target = degree_to + imu.get_heading();

        
        // double straight_kP = 1.8;
        // double straight_kI = 0.7;
        // double straight_i = 0;
        //double init_heading = global_heading;
        double cur_heading = glb::imu.get_heading();
        double last_heading = glb::imu.get_heading();

        while((degree_to < 0 ? cur_heading > target+4 : cur_heading < target-4) && time < timeout)
        {
            // inertial wrapping
            // if(cur_heading - last_heading > 100)
            // {
            //     global_heading += (cur_heading - 360) - last_heading;
            // }
            // else if(cur_heading - last_heading < -100)
            // {
            //     global_heading += cur_heading + (360 - last_heading);
            // }
            // else
            // {
            //     global_heading += cur_heading - last_heading;
            // }

            last_heading = cur_heading;
            cur_heading = glb::imu.get_heading();

            // straight_i += (global_heading - init_heading) / 100;
            // double correction = straight_i * straight_kI + (global_heading - init_heading) * straight_kP;
            
            glb::chas.spin_left(l_s);
            glb::chas.spin_right(r_s);
            pros::delay(5);
            time += 5;
        }
        
        double diff = imu.get_heading() - starting;
        
        start_head+=diff;
        
        end_head = imu.get_heading();

        global_heading += imu.get_heading() - starting;

        glb::chas.stop();
    }

    void drive_var_to(double degree_to, int l_s=127, int r_s = 127, int timeout=3000)
    {
        double degree = degree_to - global_heading;
        degree = (degree > 180) ? -(360 - degree) : ((degree < -180) ? (360 + degree) : (degree)); // optimize the turn direction
        drive_var(degree, l_s, r_s, timeout);
    }



}
//180

// good work out there rohan ur like that ur that guy ur the rizzler keep it up
// thanks aayush, you too :pray:

//#define TURN_KP 0.9//((32.7676 * (pow(fabs(fabs(target_deg) > 1 ? target_deg : 1), -1.07131))) + 0.719255) //.7
//define TURN_KI 0 //10
//#define TURN_KD 0.2 //0.3 //.45

//180
/*
TURN_KP = 3.299;
TURN_KI = 0.09;
TURN_KD = 0.32;
*/


//older
/*double TURN_KP = 3.275;
double TURN_KI = 0;
double TURN_KD = 0.224;*/

//better
/*double TURN_KP = 3.275;
double TURN_KI = 0.008;
double TURN_KD = 0.224;*/
/*

//more better
double TURN_KP = 3.275;
double TURN_KI = 0.01;
double TURN_KD = 0.224;*/

/*if (target_deg < 135)
{        
    #define TURN_KP 2.2
    #define TURN_KI 0
    #define TURN_KD 1
}
else{
    //180 vals here -- need to tune
    #define TURN_KP 3.228
    #define TURN_KI 0.25
    #define TURN_KD 0.2
}*/

// 3.215, 0.2555, 0.214, integral < 2.65

// old: 180: 0.9, 0, 0.2
//0.15,-0.1, -1.72, 1.53, -0.02, -0.14, -0.54, -0.39, 0.08, 0.04, -1.69

//90: 3.15, 0, .218

//3.2, 0, .215

/*
errors:
//0.4, -1.06, 0.45, -0.51, 1.2, -0.93
-0.41, 1.53, -0.09, 1.07, -0.9, 0.81, -0.09, 0.83
*/

// 3.21, 0.1, .215

#endif


