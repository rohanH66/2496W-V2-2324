#ifndef __CHAS__
#define __CHAS__

#include "main.h"
#include <vector>
#include <array>

class Chassis
{
private:
    std::vector<pros::Motor> left_motors;
    std::vector<pros::Motor> right_motors;


public:
    Chassis(std::vector<int> left_motor_ports, std::vector<int> right_motor_ports, pros::motor_gearset_e gearset=pros::E_MOTOR_GEARSET_06, bool reverse_spin=false)
    {
        for(int port : left_motor_ports)
            left_motors.push_back(pros::Motor(port, gearset, reverse_spin + abs(port) > 0));
        for(int port : right_motor_ports)
            right_motors.push_back(pros::Motor(port, gearset, !(reverse_spin + abs(port) > 0)));
    }

    std::vector<pros::Motor> get_left(){
        return left_motors;
    }
    std::vector<pros::Motor> get_right(){
        return right_motors;
    }

    void spin_left(double speed)
    {
        for(pros::Motor motor : left_motors)
            motor = speed;
    }

    void spin_right(double speed)
    {
        for(pros::Motor motor : right_motors)
            motor = speed;
    }

    void spin(double speed)
    {
        spin_left(speed);
        spin_right(speed);
    }

    void stop()
    {
        spin(0);
    }

    void reset()
    {
        for(pros::Motor motor : left_motors)
            motor.tare_position();
        for(pros::Motor motor : right_motors)
            motor.tare_position();
    }

    double left_pos()
    {
        double sum = 0;
        for(pros::Motor motor : left_motors)
        {
            sum += motor.get_position();
        }
        return sum / left_motors.size();
    }

    double right_pos()
    {
        double sum = 0;
        for(pros::Motor motor : right_motors)
        {
            sum += motor.get_position();
        }
        return sum / left_motors.size();
    }
    
    double pos()
    {
        return (left_pos() + right_pos()) / 2;
    }

    double temp()
    {
        double sum = 0;
        for(pros::Motor motor : left_motors)
            sum += motor.get_temperature();
        for(pros::Motor motor : right_motors)
            sum += motor.get_temperature();

        return sum / (left_motors.size() + right_motors.size());
    }

    void spin_dist(double distance, double speed=127, int timeout=5000)
    {
        int time = 0;
        double start_pos = pos();
        double target = start_pos + distance;

        while((distance < 0 ? pos() > target : pos() < target) && time < timeout)
        {
            spin(distance / fabs(distance) * speed);
            pros::delay(1);
            time += 1;
        }
        stop();
    }

    void set_brake(){
        for(pros::Motor motor : left_motors)
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        for(pros::Motor motor : right_motors)
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    }

    void set_brake_brake(){
        for(pros::Motor motor : left_motors)
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
        for(pros::Motor motor : right_motors)
            motor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    }
};

#endif