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

namespace cook
{
    double start_head = 0; 
    //For Yousef: finish abs turn
    //For Brandon: improve
    double end_head = 0;
    double global_heading;

    void cookage(double target_dist, double target_deg, int timeout=1500, double max_speed=127, double exit_time=100)
    {
        #define DRIVE_KP ((17.6647 * (pow(fabs(target_dist), -0.975028))) + 0.139685) //0.14
        #define DRIVE_KI 0.002
        #define DRIVE_KD 0 //0 for good
        target_deg = fabs(target_deg)<=180 ? target_deg : (target_deg<0 ? target_deg + 180 : target_deg - 180);
        
        double TURN_KP = 4.75;
        double TURN_KI = 0.42;
        double TURN_KD = 0.33;

        #define START_TURN 0
        #define MULT 1

        double init_heading = imu.get_heading();


        double target = target_dist + chas.pos();
        double error = target - chas.pos();
        double prev_error;
        double integral = 0;
        double kintegral = 0;
        double derivative = 0;
        double heading_error = 0;
        double error_range_time = 0;
        bool start_positive = target_dist >= 0 ? true : false;
        double early_exit_time = 0;

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
            double speed = (error * DRIVE_KP + integral * DRIVE_KI + derivative * DRIVE_KD);

            if (fabs(error) < (fabs(target_dist) * 0.1)){
                    // add turn stuff in here? only run loop if ur close to target
            }


            //Exit Loop
            if (fabs(error) < 0) /// CHANGED FOR TESTING
            {
                if(!exit)
                    exit = true;
                else
                    error_range_time++;
                if (exit_time <= error_range_time)
                    break;
            }
            
            if (fabs(error) < 20 && (std::round(prev_error * scaler) / scaler - std::round(error * scaler) / scaler < 0.2)) // 0.15
            {
                if(!same_error)
                    same_error = true;
                else
                    early_exit_time += 10;
                if (exit_time <= early_exit_time)
                    break;
            }

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

        
        



    }
    void drive(double target_dist, int timeout=1500, double mult=1.0, double max_speed=127, int exit_time=100)
    {
        #define DRIVE_KP ((17.6647 * (pow(fabs(target_dist), -0.975028))) + 0.139685) //0.14
        //500: 0.1777
        //1000: 0.1685
        //2000: 0.1429
        #define DRIVE_KI 0.002
        #define DRIVE_KD 0 //0 for good


        #define IMU_K 0

        if (fabs(end_head) - fabs(imu.get_heading()) > 1) {
            start_head += end_head-imu.get_heading();
        }

        int starting = 180;
        // start_head -= starting;
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

            //Correct sides, ensure heading stays same as beginning
            heading_error = init_heading - imu.get_heading();

            //PID
            double speed = mult * (error * DRIVE_KP + integral * DRIVE_KI + derivative * DRIVE_KD);

            //Heading correction
            kintegral += heading_error;

            double correction = (kintegral * IMU_K);

            //Cap speed and correction sum to max
            if (fabs(speed) + fabs(correction) > max_speed) 
            {
                double multiplier = max_speed/(fabs(speed) + fabs(correction));
                speed *= multiplier;
                correction *= multiplier;
            }

            //Exit Loop
            if (fabs(error) < 0) /// CHANGED FOR TESTING
            {
                if(!exit)
                    exit = true;
                else
                    error_range_time++;
                if (exit_time <= error_range_time)
                    break;
            }
            
            if (fabs(error) < 20 && (std::round(prev_error * scaler) / scaler - std::round(error * scaler) / scaler < 0.2)) // 0.15
            {
                if(!same_error)
                    same_error = true;
                else
                    early_exit_time += 10;
                if (exit_time <= early_exit_time)
                    break;
            }

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
    }

    double turn_f(double error)
    {
        return error / fabs(error) * (25 * log(0.25 * (fabs(error) + 4)) + 5);
    }

    void turn(double target_deg, int timeout=1000, bool early_exit = true, double multi=1.0, double max_speed=127, int exit_time=75)
    {  

        target_deg = fabs(target_deg)<=180 ? target_deg : (target_deg<0 ? target_deg + 180 : target_deg - 180);
        
        double TURN_KP;
        double TURN_KI;
        double TURN_KD;
        
        if (target_deg < 70) 
        {
            TURN_KP = 4.15;
            TURN_KI = 0.04;
            TURN_KD = 0.26;
        }
        else{ //not tuned yet :(
            TURN_KP = 4.75; //last=4.75//4.3, 4.5
            TURN_KI = 0.42; //last=.4//0.08, .1
            TURN_KD = 0.33; //last=.26//.25, .26(earlier), .27(90)
        }

        //comment this out later. If need to revert, comment out the lines below.
        TURN_KP = 4.75; //last=4.75//4.3, 4.5
        TURN_KI = 0.42; //last=.4//0.08, .1
        TURN_KD = 0.33; //last=.26//.25, .26(earlier), .27(90)

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

            if (fabs(error) < 0.5) // 0.15
            {
                if(!exit)
                    exit = true;
                else
                    error_range_time += 10;
                if (exit_time <= error_range_time)
                    break;
            }

            if (fabs(error) < 2 && (std::round(prev_error * scaler) / scaler - std::round(error * scaler) / scaler < 0.02)) // 0.15
            {
                if(!same_error)
                    same_error = true;
                else
                    early_exit_time += 10;
                if (exit_time <= early_exit_time)
                    break;
            }
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




}


#endif