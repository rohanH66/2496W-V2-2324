#ifndef __DISPLAYINFO__
#define __DISPLAYINFO__

#include "main.h"
#include "global.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "display/lv_objx/lv_gauge.h"

//#include "images/reg_img.h"
#include "images/field_img.h"

#define num_motors 8 


namespace disp{
    // setting up shapes for auton selector graphics
    static const lv_point_t close_awp_points[] = {{180, 40}, {200, 30}, {60, 60}, {20, 30}, {0, 0}};
    static const lv_point_t close_rush_points[] = {{0, 0}, {50, 30}, {0, 60}, {20, 30}, {0, 0}};
    static const lv_point_t far_6b_points[] = {{0, 0}, {50, 30}, {0, 60}, {20, 30}, {0, 0}};
    static const lv_point_t far_6bsafe_points[] = {{0, 0}, {50, 30}, {0, 60}, {20, 30}, {0, 0}};
    static const lv_point_t skills_points[] = {{0, 0}, {50, 30}, {0, 60}, {20, 30}, {0, 0}};
    static const lv_point_t skills_driver_points[] = {{0, 0}, {50, 30}, {0, 60}, {20, 30}, {0, 0}}; 

    lv_obj_t* circles[6];
    lv_obj_t* square;
    lv_obj_t* robot_square;
    lv_obj_t* alliTri;
    lv_obj_t* line_arrow;




    const lv_img_dsc_t field_img = {
    {
        LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
        0,
        0,
        240,
        240,
    },
    57600 * LV_COLOR_SIZE / 8,
    field_img_map,
    };
    //const lv_img_dsc_t reg_img = {
    // {
    //     LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
    //     0,
    //     0,
    //     240,
    //     240,
    // },
    // 57600 * LV_COLOR_SIZE / 8,
    // reg_pic_map,
    // };
   
    //LV_IMG_DECLARE(field);
    // Arrays to store widget references
    lv_obj_t* motor_lmeters[num_motors];
    lv_obj_t* motor_temp_labels[num_motors];

    static const char * auton_arr[] = {
        "Close half-awp",
        "Close rush",
        "Far 6-ball",
        "Far 6-b safe",
        "Skills",
        "Driver Skills",
        "Regression",
        "Testing",
        "None",

    };
    static int auton_index = 0; // Index of the currently displayed auton option
    static lv_obj_t *auton_label; // Label for displaying the current auton option

    // Global pointers for view containers
    lv_obj_t *container_temps, *container_auton, *container_ts, *container_def;


    // Function prototypes (definitions to be added based on your content)
    void createDisplayTempsView();
    void createDispAutonView();
    void createDispTSView();
    void createDispDefView();

    void update_shapes();

    static lv_obj_t *img_field;

    static lv_res_t btn_left_action(lv_obj_t *btn) {
        auton_index--;
        if (auton_index<0) auton_index = 8;
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        update_shapes();
        return LV_RES_OK;
    }
    static lv_res_t btn_right_action(lv_obj_t *btn) {
        auton_index++;
        if (auton_index>8) auton_index = 0;
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        update_shapes();
        return LV_RES_OK;
    }

    void init_shapes() {

        // Define points for the arrow shape

        // Create a style for the line
        static lv_style_t style_line;
        lv_style_copy(&style_line, &lv_style_plain);
        style_line.line.color = LV_COLOR_RED; // Set the color of the arrow
        style_line.line.width = 3; // Set the width of the lines

        // Style for circles and square with transparent fill and yellow outline
        static lv_style_t style_circle;
        lv_style_copy(&style_circle, &lv_style_plain); // Start with a base style
        style_circle.body.empty = 1; // Make it transparent inside
        style_circle.body.border.color = LV_COLOR_YELLOW; // Set the border color to bright yellow
        style_circle.body.border.width = 4; // Set the border width
        style_circle.body.radius = LV_RADIUS_CIRCLE; // This should make the shape fully rounded

        static lv_style_t style_square;
        lv_style_copy(&style_square, &style_circle); // Start with a base style
        style_square.body.radius = 0;

        // Apply the style to each circle
        for (int i = 0; i < 6; i++) {
            circles[i] = lv_btn_create(container_auton, NULL); // Create the button
            lv_obj_set_size(circles[i], 30, 30); // Set the size to ensure it's square
            lv_btn_set_style(circles[i], LV_BTN_STYLE_REL, &style_circle); // Apply the circular style
            lv_obj_set_hidden(circles[i], true); // Initially hide it
        }// Max radius for circles

        // Style for robot_square with light purple fill and dark purple outline
        static lv_style_t style_robot_square;
        lv_style_copy(&style_robot_square, &lv_style_plain); // Copy a base style
        style_robot_square.body.main_color = LV_COLOR_MAKE(204, 153, 255); // Light purple
        style_robot_square.body.grad_color = LV_COLOR_MAKE(204, 153, 255); // Light purple
        style_robot_square.body.border.color = LV_COLOR_MAKE(128, 0, 128); // Dark purple
        style_robot_square.body.border.width = 5; // Border width
        style_robot_square.body.radius = 0; // No rounding for squares


        // Square with transparent fill and yellow outline
        //style_circle.body.radius = 0; 
        square = lv_btn_create(container_auton, NULL);
        lv_obj_set_hidden(square, true);
        lv_btn_set_style(square, LV_BTN_STYLE_REL, &style_square); // Apply the style
        lv_obj_set_size(square, 40, 40); // Example size, adjust as needed
       // Override for square

        // Robot square with light purple fill and dark purple outline
        robot_square = lv_btn_create(container_auton, NULL);
        lv_obj_set_hidden(robot_square, true);
        lv_btn_set_style(robot_square, LV_BTN_STYLE_REL, &style_robot_square); // Apply the style
        lv_obj_set_size(robot_square, 40, 40); // Example size, adjust as needed

        line_arrow = lv_line_create(container_auton, NULL); // Create the line object on the active screen
        lv_line_set_points(line_arrow, close_awp_points, 5); // Set the points (the second parameter is the number of points)
        lv_line_set_style(line_arrow, &style_line); // Apply the style
        //lv_obj_align(line_arrow, NULL, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_hidden(line_arrow, true);

    }

    // Adjust shapes based on auton_index

    void update_shapes() {
        // Hide all shapes first
        for (int i = 0; i < 6; i++) {
            lv_obj_set_hidden(circles[i], true);
        }
        lv_obj_set_hidden(square, true);
        lv_obj_set_hidden(robot_square, true);
        lv_obj_set_hidden(line_arrow, true);


        // Logic to adjust positions, sizes, visibility based on auton_index
        // This example shows a simple way to manage it. You need to adapt it based on your specific requirements.
        switch (auton_index) {
            case 0:
                // close_awp positioning!
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 210, 0); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 200, 30); 

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 20, 45);
                lv_obj_set_pos(square, 110, 0);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 160, 1); 

                lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, close_awp_points, 5);

                break;
            
            case 1:

                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 210, 0); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 200, 30); 

                lv_obj_set_hidden(circles[2], false);
                lv_obj_set_pos(circles[2], 144, 105); 

                lv_obj_set_hidden(square, false);
                lv_obj_set_size(square, 20, 45);
                lv_obj_set_pos(square, 110, 0);

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 160, 1); 

                lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, close_rush_points, 5);

                break;
            case 2:
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 0, 0); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 97, 105); 

                lv_obj_set_hidden(circles[2], false);
                lv_obj_set_pos(circles[2], 63, 105); 
                
                lv_obj_set_hidden(circles[3], false);
                lv_obj_set_pos(circles[3], 10, 30); 

                lv_obj_set_hidden(circles[4], false);
                lv_obj_set_pos(circles[4], 97, 65); 

                lv_obj_set_hidden(circles[5], false);
                lv_obj_set_pos(circles[5], 105, 5); 

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 40, 1); 

                lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, far_6b_points, 5);
                break;

            case 3:
                lv_obj_set_hidden(circles[0], false);
                lv_obj_set_pos(circles[0], 0, 0); 

                lv_obj_set_hidden(circles[1], false);
                lv_obj_set_pos(circles[1], 97, 105); 

                lv_obj_set_hidden(circles[2], false);
                lv_obj_set_pos(circles[2], 63, 105); 
                
                lv_obj_set_hidden(circles[3], false);
                lv_obj_set_pos(circles[3], 10, 30); 

                lv_obj_set_hidden(circles[4], false);
                lv_obj_set_pos(circles[4], 97, 65); 

                lv_obj_set_hidden(circles[5], false);
                lv_obj_set_pos(circles[5], 105, 5); 

                lv_obj_set_hidden(robot_square, false);
                lv_obj_set_pos(robot_square, 40, 1); 

                lv_obj_set_hidden(line_arrow, false);
                lv_line_set_points(line_arrow, far_6bsafe_points, 5);
                break;
            

            
        }
    }

    void updateMotorTemps() {
        char temp_str[10];
        int temp;

        for (int i = 0; i < num_motors; i++) {
            temp = glb::temps_a[i];

            // Update the line meter's value
            lv_lmeter_set_value(motor_lmeters[i], temp);

            // Update the label text
            sprintf(temp_str, "%d°C", temp);
            lv_label_set_text(motor_temp_labels[i], temp_str);
        }
    }


    void switchView(lv_obj_t *view_container) {
        // Hide all containers
        lv_obj_set_hidden(container_temps, true);
        lv_obj_set_hidden(container_auton, true);
        lv_obj_set_hidden(container_ts, true);
        lv_obj_set_hidden(container_def, true);

        // Show the selected view container
        lv_obj_set_hidden(view_container, false);
    }

    // Example button event callback for switching to the displayTemps view
    static lv_res_t btn_temps_action(lv_obj_t *btn) {
        switchView(container_temps);
        return LV_RES_OK; // Indicate the button is not deleted
    }
    // Example button event callback for switching to the auton view
    static lv_res_t btn_auton_action(lv_obj_t *btn) {
        switchView(container_auton);
        return LV_RES_OK; // Indicate the button is not deleted
    }

    static lv_res_t btn_ts_action(lv_obj_t *btn) {
        switchView(container_ts);
        return LV_RES_OK; // Indicate the button is not deleted
    }
    // Example button event callback for switching to the auton view
    static lv_res_t btn_def_action(lv_obj_t *btn) {
        switchView(container_def);
        return LV_RES_OK; // Indicate the button is not deleted
    }




    void setupUI() {


        static lv_style_t style_btn;
        lv_style_copy(&style_btn, &lv_style_plain); // Start with a plain style for more control

        // Taskbar button style (used to copypaste onto all buttons below)
        style_btn.body.border.color = LV_COLOR_MAKE(0x00, 0x00, 0x80); // colored navy
        style_btn.body.border.width = 3; // change for thickness
        style_btn.body.border.part = LV_BORDER_FULL; 
        style_btn.body.main_color = LV_COLOR_MAKE(0x00, 0x00, 0x00); 
        style_btn.body.grad_color = LV_COLOR_MAKE(0x00, 0x00, 0x00); 
        style_btn.body.opa = LV_OPA_TRANSP; 
        style_btn.body.radius = LV_DPI / 8; // rounded radius calculator
        style_btn.text.color = LV_COLOR_MAKE(0xFF, 0xFF, 0xFF);

        // taskbar on  right
        lv_obj_t *taskbar = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(taskbar, 60, 240); 
        lv_obj_set_pos(taskbar, 420, 0); 

        // taskbar buttons  (set individually to assign actions later)
        lv_obj_t *btn_temps = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_temps, LV_BTN_STYLE_REL, &style_btn);
        lv_obj_set_size(btn_temps, 56, 56); 
        lv_obj_set_pos(btn_temps, 2, 2);

        lv_obj_t *btn_auton = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_auton, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_auton, 56, 56); 
        lv_obj_set_pos(btn_auton, 2, 62);

        lv_obj_t *btn_ts = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_ts, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_ts, 56, 56); 
        lv_obj_set_pos(btn_ts, 2, 122);

        lv_obj_t *btn_def = lv_btn_create(taskbar, NULL);
        lv_btn_set_style(btn_def, LV_BTN_STYLE_REL, &style_btn); 
        lv_obj_set_size(btn_def, 56, 56); 
        lv_obj_set_pos(btn_def, 2, 182);

        // button actions
        lv_btn_set_action(btn_temps, LV_BTN_ACTION_CLICK, btn_temps_action);
        lv_btn_set_action(btn_auton, LV_BTN_ACTION_CLICK, btn_auton_action);
        lv_btn_set_action(btn_ts, LV_BTN_ACTION_CLICK, btn_ts_action);
        lv_btn_set_action(btn_def, LV_BTN_ACTION_CLICK, btn_def_action);



        // Initializing containers
        container_temps = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_temps, 420, 240); 
        lv_obj_set_pos(container_temps, 0, 0); 
        lv_obj_set_hidden(container_temps, false);
      
        container_auton = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_auton, 420, 240); 
        lv_obj_set_pos(container_auton, 0, 0); 
        lv_obj_set_hidden(container_auton, true); //set as true for default

        container_ts = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_ts, 420, 240); 
        lv_obj_set_pos(container_ts, 0, 0); 
        lv_obj_set_hidden(container_ts, true);

        container_def = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_def, 420, 240); 
        lv_obj_set_pos(container_def, 0, 0); 
        lv_obj_set_hidden(container_def, true);

        // "turn on" each view to start displaying thigs
        createDisplayTempsView();
        createDispAutonView();
    }



    // temperature monitoring view for mid-match
    void createDisplayTempsView() {

        // calculate dimensions for each view
        int temp;
        const lv_coord_t container_width = 420; 
        const lv_coord_t container_height = 240;
        const lv_coord_t motor_block_width = container_width / 4; 
        const lv_coord_t motor_block_height = container_height / 2; 

        // create style to be applied to each view
        static lv_style_t lmeter_style;
        lv_style_copy(&lmeter_style, &lv_style_pretty);
        lmeter_style.body.main_color = LV_COLOR_MAKE(0xE0, 0xE0, 0xE0); // Light grey for the inactive part
        lmeter_style.body.grad_color = LV_COLOR_MAKE(0xB0, 0x00, 0x00); // Darker red for the active part
        lmeter_style.line.color = LV_COLOR_MAKE(0x5E, 0x5E, 0x5E); // Dark grey for the scale lines
        lmeter_style.line.width = 3; // Thickness of the scale lines
        

        for (int i = 0; i < num_motors; i++) {
            // creating a container for each motor temp display
            lv_coord_t x_pos = (i % 4) * motor_block_width;
            lv_coord_t y_pos = (i / 4) * motor_block_height;
            lv_obj_t *motor_container = lv_cont_create(container_temps, NULL);
            lv_obj_set_size(motor_container, motor_block_width, motor_block_height);
            lv_obj_set_pos(motor_container, x_pos, y_pos);

            // Creating stuff for container
            //label
            lv_obj_t *label = lv_label_create(motor_container, NULL);


            lv_label_set_text(label, glb::motor_labels[i].c_str());
            lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

            // meter
            lv_obj_t *lmeter = lv_lmeter_create(motor_container, NULL);
            lv_lmeter_set_range(lmeter, 15, 60); // Set the new temperature range
           
            
            // adjusting temp
            //if (pros::millis()%500==0) updateTemps();
            temp = glb::temps_a[i];

            lv_lmeter_set_value(lmeter, temp); 
            lv_obj_set_size(lmeter, motor_block_width * 3 / 4, motor_block_height * 3 / 4);
            lv_obj_align(lmeter, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

            // apply style
            lv_lmeter_set_style(lmeter, &lmeter_style);
            motor_lmeters[i] = lmeter;

            // Label for meter temp
            lv_obj_t *temp_label = lv_label_create(motor_container, NULL);
            char temp_str[10];
            sprintf(temp_str, "%d°C", temp); 
            lv_label_set_text(temp_label, temp_str);
            lv_obj_align(temp_label, lmeter, LV_ALIGN_CENTER, 0, 0);
            motor_temp_labels[i] = temp_label;
        }
    }


    // view for auton selector
    void createDispAutonView(){
        // LV_IMG_DECLARE(close_awp_img);

        lv_obj_t *img_field = lv_img_create(container_auton, NULL);
        lv_img_set_src(img_field, &field_img);
        lv_obj_set_size(img_field, 240, 240);
        lv_obj_set_pos(img_field, 0, 0);

        // L. arrow button
        lv_obj_t *btn_left = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_left, 20, 150); 
        lv_obj_set_size(btn_left, 70, 70);
        lv_obj_t *label_left = lv_label_create(btn_left, NULL);

        // R. arrow button
        lv_obj_t *btn_right = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_right, 240-70-20, 150); 
        lv_obj_set_size(btn_right, 70, 70);
        lv_obj_t *label_right = lv_label_create(btn_right, NULL);

        // Define a custom style for the buttons
        static lv_style_t style_btn_select;
        lv_style_copy(&style_btn_select, &lv_style_pretty); // Start with a base style

        // Set the background (fill) color to light orange
        style_btn_select.body.main_color = LV_COLOR_MAKE(255, 165, 0); // Example light orange color
        style_btn_select.body.grad_color = LV_COLOR_MAKE(255, 165, 0); // Same as main_color for a solid fill

        // Make the filling semi-transparent (opaque)
        style_btn_select.body.opa = LV_OPA_70; // Adjust opacity as needed, LV_OPA_70 is ~70% opacity
        style_btn_select.body.border.color = LV_COLOR_MAKE(255, 140, 0); // Example dark orange color
        style_btn_select.body.border.width = 2;

        // Set the corner roundness
        style_btn_select.body.radius = 10; // For fully rounded corners, or use a specific pixel value for less roundness


        // Apply the custom style to the left button
        lv_btn_set_style(btn_left, LV_BTN_STYLE_REL, &style_btn_select); // Apply the style for the released state
        lv_btn_set_style(btn_right, LV_BTN_STYLE_REL, &style_btn_select);

        static lv_style_t style_btn_released;
        lv_style_copy(&style_btn_released, &style_btn_select);

        style_btn_select.body.opa = LV_OPA_50;
        lv_btn_set_style(btn_left, LV_BTN_STYLE_PR, &style_btn_released); // Apply the style for the released state
        lv_btn_set_style(btn_right, LV_BTN_STYLE_PR, &style_btn_released); // Apply the same style for the pressed state

        // Set the text for the button labels
        lv_label_set_text(label_left, "<"); // Set text for the left button
        lv_label_set_text(label_right, ">"); // Set text for the right button


        // Auton options
        auton_label = lv_label_create(container_auton, NULL);
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        lv_obj_set_pos(auton_label, 240+50, 15); // centered in between buttons

        // confirm button
        lv_obj_t *btn_confirm = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_confirm, 240 + 40, 195); // Positioned at the bottom of the right half
        lv_obj_set_size(btn_confirm, 100, 40);
        lv_obj_t *label_confirm = lv_label_create(btn_confirm, NULL);
        lv_label_set_text(label_confirm, "Confirm");
        lv_btn_set_style(btn_confirm, LV_BTN_STYLE_REL, &lv_style_btn_rel); 
        // Set the button to green or any other color/style adjustments as needed

        // assigning actions
        lv_btn_set_action(btn_left, LV_BTN_ACTION_CLICK, btn_left_action);
        lv_btn_set_action(btn_right, LV_BTN_ACTION_CLICK, btn_right_action);
        // lv_btn_set_action(btn_confirm, LV_BTN_ACTION_CLICK, btn_confirm_action); // TODO

        init_shapes();
        update_shapes();
    }

}

#endif