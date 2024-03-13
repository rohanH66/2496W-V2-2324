#ifndef __AUTFUNC__
#define __AUTFUNC__

#include "main.h"
#include "global.h"
#include "driver.h"


#include "pros/misc.h"
#include "pros/rtos.h"
#include <iostream>
#include <fstream>
#include <list>
#include <iostream>

using namespace pros;
using namespace std;
using namespace glb;

void matchload(int triballs){
    int time = 0;
    int timeout = 28000;
    while (cata.get_position()/900 < triballs){ 
        cata.move(100);
        time += 2;
        pros::delay(2);
    }
    cata.move(0);
}

void cata_rest(){
    int pos = ((int) cata.get_position() % 900);
    const int deadzone = 300;
    int time = 0;
    int timeout = 500;

    while (time < timeout){
        pos = ((int) cata.get_position() % 900);
        cata.move(50);
        pros::delay(2);
        time += 2;
    }
    cata.move(0);
}


#endif