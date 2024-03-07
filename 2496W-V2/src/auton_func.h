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
    cata.tare_position();
    while (cata.get_position()/900 < triballs) cata.move(100);
    delay(200);
    cata.move(0);
}


#endif