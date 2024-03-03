#include "main.h"
#include "global.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "display/lv_objx/lv_gauge.h"

lv_obj_t * myButton;
lv_obj_t * myButtonLabel;
lv_obj_t * myLabel;

lv_style_t myButtonStyleREL; //relesed style
lv_style_t myButtonStylePR; //pressed style




static lv_res_t btn_click_action(lv_obj_t * btn)
{
    uint8_t id = lv_obj_get_free_num(btn); //id usefull when there are multiple buttons

    if(id == 0)
    {
        char buffer[100];
		sprintf(buffer, "button was clicked %i milliseconds from start", pros::millis());
		lv_label_set_text(myLabel, buffer);
    }

    return LV_RES_OK;
}

namespace disp{

    void displayTemps() {
    const int num_motors = 8;
    const lv_coord_t screen_width = 420;
    const lv_coord_t screen_height = 240;
    const lv_coord_t motor_block_width = screen_width / 4; // 4 motors in a row
    const lv_coord_t motor_block_height = screen_height / 2; // 2 rows

    // In LVGL 5.3, styles are often applied directly when creating the widget or by setting them afterwards
    static lv_style_t lmeter_style;
    lv_style_copy(&lmeter_style, &lv_style_pretty);
    lmeter_style.body.main_color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0); // Light grey for the inactive part
    lmeter_style.body.grad_color = LV_COLOR_MAKE(0xD0, 0x00, 0x00); // Red for the active part
    lmeter_style.line.color = LV_COLOR_MAKE(0x5E, 0x5E, 0x5E); // Dark grey for the scale lines
    lmeter_style.line.width = 2; // Thickness of the scale lines

    for (int i = 0; i < num_motors; i++) {
        // Calculate position
        lv_coord_t x_pos = (i % 4) * motor_block_width;
        lv_coord_t y_pos = (i / 4) * motor_block_height;

        // Create a container for each motor's display elements
        lv_obj_t *container = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container, motor_block_width, motor_block_height);
        lv_obj_set_pos(container, x_pos, y_pos);

        // Create a label for the motor name
        lv_obj_t *label = lv_label_create(container, NULL);
        char motor_name[10];
        sprintf(motor_name, "Motor_%d", i + 1);
        lv_label_set_text(label, motor_name);
        lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

        // Create a line meter for the temperature
        lv_obj_t *lmeter = lv_lmeter_create(container, NULL);
        lv_lmeter_set_range(lmeter, 15, 65); // Set the new temperature range

        // Assuming a function to get the actual temperature for each motor
        int temp = /* motor_x.get_temperature() */ 50; // Placeholder for demonstration

        lv_lmeter_set_value(lmeter, temp); // Set the adjusted temperature value
        lv_obj_set_size(lmeter, motor_block_width * 3 / 4, motor_block_height * 3 / 4);
        lv_obj_align(lmeter, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

        // Directly applying the custom style to the line meter
        lv_lmeter_set_style(lmeter, &lmeter_style);

        // Create a label for the temperature value inside the line meter
        lv_obj_t *temp_label = lv_label_create(container, NULL);
        char temp_str[10];
        sprintf(temp_str, "%d°C", temp); // Display the actual temperature value
        lv_label_set_text(temp_label, temp_str);
        lv_obj_align(temp_label, lmeter, LV_ALIGN_CENTER, 0, 0);
    }
}

// void displayTemps() {
//     const int num_motors = 8;
//     const lv_coord_t screen_width = 480;
//     const lv_coord_t screen_height = 240;
//     const lv_coord_t motor_block_width = screen_width / 4; // 4 motors in a row
//     const lv_coord_t motor_block_height = screen_height / 2; // 2 rows

//     // In LVGL 5.3, styles are often applied directly when creating the widget or by setting them afterwards
//     static lv_style_t gauge_style;
//     lv_style_copy(&gauge_style, &lv_style_pretty);
//     gauge_style.body.main_color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0); // Light grey
//     gauge_style.body.grad_color = LV_COLOR_MAKE(0x5E, 0x5E, 0x5E); // Dark grey
//     gauge_style.line.color = LV_COLOR_MAKE(0xD0, 0x00, 0x00); // Needle color

//     for (int i = 0; i < num_motors; i++) {
//         // Calculate position
//         lv_coord_t x_pos = (i % 4) * motor_block_width;
//         lv_coord_t y_pos = (i / 4) * motor_block_height;

//         // Create a container for each motor's display elements
//         lv_obj_t *container = lv_cont_create(lv_scr_act(), NULL);
//         lv_obj_set_size(container, motor_block_width, motor_block_height);
//         lv_obj_set_pos(container, x_pos, y_pos);

//         // Create a label for the motor name
//         lv_obj_t *label = lv_label_create(container, NULL);
//         char motor_name[10];
//         sprintf(motor_name, "Motor_%d", i + 1);
//         lv_label_set_text(label, motor_name);
//         lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

//         // Create a gauge for the temperature
//         lv_obj_t *gauge = lv_gauge_create(container, NULL);
//         lv_gauge_set_range(gauge, 0, 100); // Assuming temperature range 0-100
//         lv_gauge_set_critical_value(gauge, 80); // Set the critical value, for example, 80
//         lv_gauge_set_scale(gauge, 270, 10, 1); // 270 degrees, 10 lines
//         lv_obj_set_size(gauge, motor_block_width *3 / 4 , motor_block_height *3 / 4);
//         lv_obj_align(gauge, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

//         // Directly applying the custom style to the gauge
//         lv_gauge_set_style(gauge, &gauge_style);

//         // Assuming a function to get temperature for each motor
//         int temp = /* motor_x.get_temperature() */ 50; // Placeholder
//         lv_gauge_set_value(gauge, 0, temp);

//         // Create a label for the temperature value inside the gauge
//         lv_obj_t *temp_label = lv_label_create(container, NULL);
//         char temp_str[10];
//         sprintf(temp_str, "%d°C", temp); // Use actual temperature value
//         lv_label_set_text(temp_label, temp_str);
//         lv_obj_align(temp_label, gauge, LV_ALIGN_CENTER, 0, 0);
//     }
// }

void init()
    {
        lv_style_copy(&myButtonStyleREL, &lv_style_plain);
        myButtonStyleREL.body.main_color = LV_COLOR_MAKE(150, 0, 0);
        myButtonStyleREL.body.grad_color = LV_COLOR_MAKE(0, 0, 150);
        myButtonStyleREL.body.radius = 0;
        myButtonStyleREL.text.color = LV_COLOR_MAKE(255, 255, 255);

        lv_style_copy(&myButtonStylePR, &lv_style_plain);
        myButtonStylePR.body.main_color = LV_COLOR_MAKE(255, 0, 0);
        myButtonStylePR.body.grad_color = LV_COLOR_MAKE(0, 0, 255);
        myButtonStylePR.body.radius = 0;
        myButtonStylePR.text.color = LV_COLOR_MAKE(255, 255, 255);

        myButton = lv_btn_create(lv_scr_act(), NULL); //create button, lv_scr_act() is deafult screen object
        lv_obj_set_free_num(myButton, 0); //set button is to 0
        lv_btn_set_action(myButton, LV_BTN_ACTION_CLICK, btn_click_action); //set function to be called on button click
        lv_btn_set_style(myButton, LV_BTN_STYLE_REL, &myButtonStyleREL); //set the relesed style
        lv_btn_set_style(myButton, LV_BTN_STYLE_PR, &myButtonStylePR); //set the pressed style
        lv_obj_set_size(myButton, 200, 50); //set the button size
        lv_obj_align(myButton, NULL, LV_ALIGN_IN_TOP_LEFT, 10, 10); //set the position to top mid

        myButtonLabel = lv_label_create(myButton, NULL); //create label and puts it inside of the button
        lv_label_set_text(myButtonLabel, "Click the Button"); //sets label text

        myLabel = lv_label_create(lv_scr_act(), NULL); //create label and puts it on the screen
        lv_label_set_text(myLabel, "Button has not been clicked yet"); //sets label text
        lv_obj_align(myLabel, NULL, LV_ALIGN_IN_LEFT_MID, 10, 0); //set the position to center
    }
}
