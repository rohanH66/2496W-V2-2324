#include "main.h"
#include "global.h"
#include "driver.h"
#include "auton.h"
#include "auton_func.h"
#include "pid.h"
#include "lib/auton_obj.h"
#include "lib/chassis.h"
#include "lib/piston.h"
#include "regression.h"
#include "displayC.h"

#include <vector> 
#include <numeric>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <cstring>


using namespace pros;
using namespace std;
using namespace glb;
 
Auton *auton;
string names;


void initialize() {
	// disp::setupUI();
	//lcd::initialize();

	sylib::initialize();
	con.clear();
	static Auton temp = auton_selector(autons);
	names = temp.get_name();
	auton = &temp;

}

void autonomous() {
	
	(*auton).run();
}


void opcontrol() 
{
	glb::con.clear();
	long long time = 0;
	bool chassis_on = true;
	const bool skillsb = (names.find("skills")!= std::string::npos);
	chas.set_brake();
	pto.set(false);
	// if ((*auton).get_name() == "V1 skills" || (*auton).get_name() == "V2 skills") pid::global_heading = 337;

	//eg::regMain(750, 15, 0.17);
    //std::uint32_t clock = sylib::millis();

	while(true)
	{
		//sylib::delay_until(&clock, 10);

		//display stuff
		// if (time%2000==0) disp::updateMotorTemps();
		// if (time%100==0) disp::update_motor_stats();
		// if (time%200==0) updateTemps();

		if (chassis_on) drive();
		intakeCon();
		piston_cont(skillsb);

		print_info(time, chassis_on);

		if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) chassis_on = !chassis_on;
		if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_UP) && chassis_on){
			if (names == "reg tuning") reg::regMain(750, 15, 0.17);
			else autonomous();
		}

		delay(2);
		time += 2;
	}
}
