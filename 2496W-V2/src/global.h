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
    #define P_IMU 21
    #define P_RF 10
    #define P_RM 9
    #define P_RB 8
    #define P_LF 7
    #define P_LM 6
    #define P_LB 5
    #define P_INTAKE 4
    #define P_CATA 1
    #define P_DISTANCE 2


    #define P_HANGP 'C'
    #define P_PTO 'E'
    #define P_RWFRONTP 'B'
    #define P_LWFRONTP 'A'
    #define P_RWBACKP 'B'
    #define P_LWBACKP 'A'

    // #define P_BLOCKERP 'D'
   
   
    //objects
    Chassis chas({P_LF, P_LM, P_LB}, {P_RF, P_RM, P_RB}, pros::E_MOTOR_GEARSET_06, false);
    // Motor RF (P_RF, E_MOTOR_GEARSET_06);
    // Motor RM (P_RM, E_MOTOR_GEARSET_06);
    // Motor RB (P_RB, E_MOTOR_GEARSET_06, 1);
    // Motor LF (P_LF, E_MOTOR_GEARSET_06,1);
    // Motor LM (P_LM, E_MOTOR_GEARSET_06,1);
    // Motor LB (P_LB, E_MOTOR_GEARSET_06);
    Motor intake(P_INTAKE, E_MOTOR_GEARSET_06);
    Motor cata (P_CATA, E_MOTOR_GEARSET_18,1);

    Piston LfrontP(P_LWFRONTP);
    Piston RfrontP(P_RWFRONTP);
    Piston LbackP(P_LWBACKP);
    Piston RbackP(P_RWBACKP);

    Piston hangP(P_HANGP);
    Piston pto(P_PTO);
    // Piston blockerP(P_BLOCKERP);
    // Distance dist(P_DISTANCE);
    Imu imu(P_IMU);
    Distance dist(P_DISTANCE);
    Controller con(E_CONTROLLER_MASTER);

}
#endif