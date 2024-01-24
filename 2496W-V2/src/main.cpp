#include "main.h"
#include "global.h"
#include "driver.h"
#include "auton.h"
#include "auton_func.h"
#include "pid.h"
#include "lib/auton_obj.h"
#include "lib/chassis.h"
#include "lib/piston.h"


#include <iostream>

using namespace pros;
using namespace std;
using namespace glb;
 
Auton *auton;


void initialize() {
	lcd::initialize();
	con.clear();
	static Auton temp = auton_selector(autons);
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
	// cata.tare_position();
	hang.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	chas.set_brake();
	// if ((*auton).get_name() == "V1 skills" || (*auton).get_name() == "V2 skills") pid::global_heading = 337;

	while(true)
	{
		chas.set_brake();
		drive();
		intakeCon();
		piston_cont();
		hangCon();
		slapperCon();
		// if ((*auton).get_name() != "V1 skills" && (*auton).get_name() != "V2 skills"){
		print_info(time, chassis_on);
		// }
		if(time % 100 == 0 && time % 150 != 0) {
			if(hangP.get_status() == true){
        		con.print(2, 0, "HANG");
			}
		}
        // con.print(1, 0, "%.2f", imu.get_heading());
		if(con.get_digital_new_press(E_CONTROLLER_DIGITAL_LEFT)) chassis_on = !chassis_on;
		if(con.get_digital(E_CONTROLLER_DIGITAL_UP) && chassis_on) autonomous();

		delay(2);
		time += 2;
	}
}
