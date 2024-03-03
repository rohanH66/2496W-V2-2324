#include "main.h"
#include "global.h"
#include "display/lv_core/lv_obj.h"
#include "display/lv_objx/lv_label.h"
#include "display/lv_objx/lv_gauge.h"
#include "images/field_new.c"
#include "images/thermo.c"


    //const lv_img_dsc_t field;

namespace disp{
    //LV_IMG_DECLARE(field);

    std::string motor_labels[] = {
        "R. Top",
        "R. Middle",
        "R. Bottom",
        "Intake",
        "L. Top",
        "L. Middle",
        "L. Bottom",
        "Shooter",
    };

    const lv_img_dsc_t field_new = {
    {
        LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED,
        0,
        0,
        240,
        240,
    },
    57600 * LV_COLOR_SIZE / 8,
    field_new_map,
    };

    const lv_img_dsc_t thermo = {
        { // Initialize the header substructure
            LV_IMG_CF_TRUE_COLOR, // cf
            0,                     // always_zero
            0,                     // reserved
            64,                   // w
            64                    // h
        },
        4096 * LV_COLOR_SIZE / 8, // data_size
        thermo_map,                  // data
    };
    static const char * auton_arr[] = {
        "Close half-awp",
        "Close safe",
        "Close rush",
        "Far half-awp",
        "Far safe",
        "Far rush",
        "Far 6-ball",
        "Skills",
        "None",
        "Testing",
        "Regression",
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

    static lv_res_t btn_left_action(lv_obj_t *btn) {
        auton_index = (auton_index + (sizeof(auton_arr) / sizeof(auton_arr[0])) - 1) % (sizeof(auton_arr) / sizeof(auton_arr[0]));
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        return LV_RES_OK;
    }

    static lv_res_t btn_right_action(lv_obj_t *btn) {
        auton_index = (auton_index + 1) % (sizeof(auton_arr) / sizeof(auton_arr[0]));
        lv_label_set_text(auton_label, auton_arr[auton_index]);
        return LV_RES_OK;
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
        lv_obj_set_hidden(container_temps, true);
      
        container_auton = lv_cont_create(lv_scr_act(), NULL);
        lv_obj_set_size(container_auton, 420, 240); 
        lv_obj_set_pos(container_auton, 0, 0); 
        lv_obj_set_hidden(container_auton, false); //set as true for default

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
        const int num_motors = 8;
        const lv_coord_t container_width = 420; 
        const lv_coord_t container_height = 240;
        const lv_coord_t motor_block_width = container_width / 4; 
        const lv_coord_t motor_block_height = container_height / 2; 

        // create style to be applied to each view
        static lv_style_t lmeter_style;
        lv_style_copy(&lmeter_style, &lv_style_pretty);
        lmeter_style.body.main_color = LV_COLOR_MAKE(0x00, 0xB0, 0x00); // Green for the inactive part
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


            lv_label_set_text(label, motor_labels[i].c_str());
            lv_obj_align(label, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

            // meter
            lv_obj_t *lmeter = lv_lmeter_create(motor_container, NULL);
            lv_lmeter_set_range(lmeter, 15, 60); // Set the new temperature range

            // adjusting temp
            int temp = /* motor_x.get_temperature() */ 50; 

            lv_lmeter_set_value(lmeter, temp); 
            lv_obj_set_size(lmeter, motor_block_width * 3 / 4, motor_block_height * 3 / 4);
            lv_obj_align(lmeter, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);

            // apply style
            lv_lmeter_set_style(lmeter, &lmeter_style);

            // Label for meter temp
            lv_obj_t *temp_label = lv_label_create(motor_container, NULL);
            char temp_str[10];
            sprintf(temp_str, "%d°C", temp); 
            lv_label_set_text(temp_label, temp_str);
            lv_obj_align(temp_label, lmeter, LV_ALIGN_CENTER, 0, 0);
        }
    }


    // view for auton selector
    void createDispAutonView(){
        LV_IMG_DECLARE(field_new);

        lv_obj_t *img_field = lv_img_create(container_auton, NULL);
        lv_img_set_src(img_field, &field_new);
        lv_obj_set_size(img_field, 240, 240);
        lv_obj_set_pos(img_field, 0, 0);

        // L. arrow button
        lv_obj_t *btn_left = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_left, 240 + 5, 5); 
        lv_obj_set_size(btn_left, 40, 40);
        lv_obj_t *label_left = lv_label_create(btn_left, NULL);

        // R. arrow button
        lv_obj_t *btn_right = lv_btn_create(container_auton, NULL);
        lv_obj_set_pos(btn_right, 420 - 45, 5); 
        lv_obj_set_size(btn_right, 40, 40);
        lv_obj_t *label_right = lv_label_create(btn_right, NULL);

        // setting text
        lv_label_set_text(label_left, "<-"); 
        lv_label_set_text(label_right, "->"); 


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
    }

}

