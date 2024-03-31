#ifndef __GLOBAL__
#define __GLOBAL__

#include "main.h"
#include "lib/chassis.h"
#include "lib/piston.h"

using namespace pros;
using namespace std;

namespace glb
{
    // ports
    #define P_IMU 19
    #define P_RF 10
    #define P_RM 20
    #define P_RB 8
    #define P_LF 7
    #define P_LM 6
    #define P_LB 5
    #define P_INTAKE1 4
    #define P_INTAKE2 9
    #define P_DISTANCE 14


    #define P_HANGP 'F'
    #define P_PTO 'B'
    #define P_RWFRONTP 'D'
    #define P_LWFRONTP 'C'
    #define P_RWBACKP 'E'
    #define P_LWBACKP 'A'

   
   
    //objects
    Chassis chas({P_LF, P_LM, P_LB}, {P_RF, P_RM, P_RB}, pros::E_MOTOR_GEARSET_06, false);

    Motor intake1(P_INTAKE1, E_MOTOR_GEARSET_06);
    Motor intake2(P_INTAKE2, E_MOTOR_GEARSET_06);

    Piston LfrontP(P_LWFRONTP);
    Piston RfrontP(P_RWFRONTP);
    Piston LbackP(P_LWBACKP);
    Piston RbackP(P_RWBACKP);

    Piston hangP(P_HANGP);
    Piston pto(P_PTO);

    Imu imu(P_IMU);
    Distance dist(P_DISTANCE);
    Controller con(E_CONTROLLER_MASTER);



    // This is all jank stuff for display code, pls ignore it lol
    std::vector<pros::Motor> right_m = chas.get_right();
    std::vector<pros::Motor> left_m = chas.get_left();
    std::vector<pros::Motor> motorV = {
        right_m[0],
        right_m[1],
        right_m[2],
        intake_1,
        intake_2,
        left_m[0],
        left_m[1],
        left_m[2],
    };
    std::string motor_labels[] = {
        "R. Top",
        "R. Middle",
        "R. Bottom",
        "Intake 1",
        "Intake 2"
        "L. Top",
        "L. Middle",
        "L. Bottom",
        "Shooter",
    };

    std::string efficiency_a[] = {
        "100%",
        "R. Middle",
        "R. Bottom",
        "Intake 1",
        "Intake 2"
        "L. Top",
        "L. Middle",
        "L. Bottom"
    };

    int temps_a[] = {
        50,
        50,
        50,
        50,
        50,
        50,
        50,
        50
    };

}
#endif