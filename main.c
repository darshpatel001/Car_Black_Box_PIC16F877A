
#include "main.h"
#include "car_black_box_def.h"

#pragma config WDTE = OFF

void init_config() {
    init_i2c(100000);
    init_ds1307();
    init_digital_keypad();
    init_adc();
    init_clcd();
    init_timer2();
    init_uart(9600);
    GIE = 1;
    PEIE = 1;
}

void main(void) {
    init_config();
    unsigned char event[] = "ON"; //ON C GN GR G1 G2 G3 G4
    unsigned char speed = 10;
    static unsigned char operational_flag = DASHBOARD_SCREEN;
    static unsigned char reset_flag = RESET_LOGIN_SCREEN;
    char *gear[] = {
        "GN", "GR", "G1", "G2", "G3", "G4"
    };
    unsigned char gr = 0;
    unsigned char key;
    unsigned char passW[] = "0000";
    static unsigned char menu_op;
    ext_eeprom_24C02_str_write(0x00, passW); 

    log_event(event, speed);

    while (1) {
        speed = ((unsigned long) read_adc(CHANNEL0)) * 99 / 1023;

        key = read_digital_keypad(STATE);

        for (unsigned int delay = 300; delay--;);

        if (key == SW1) {
            strcpy(event, "C ");
            log_event(event, speed);
        } else if (key == SW2 && gr < 6) {
            strcpy(event, gear[gr]);
            gr++;
            log_event(event, speed);
        } else if (key == SW3 && gr > 0) {
            gr--;
            strcpy(event, gear[gr]);
            log_event(event, speed);
        } else if ((key == SW4 || key == SW5) && operational_flag == DASHBOARD_SCREEN) {
            operational_flag = LOGIN_SCREEN;
            reset_flag = RESET_LOGIN_SCREEN;
            clear_screen();
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, LINE2(4));
        }



        switch (operational_flag) {
                //Dashboard
            case DASHBOARD_SCREEN:
                display_dashboard_screen(event, speed);
                break;
                //login
            case LOGIN_SCREEN:
                switch (login_screen(key, reset_flag)) {
                    case RETURN_BACK:
                        operational_flag = DASHBOARD_SCREEN;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        break;

                    case LOGIN_SUCCESS:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        continue;

                }
                break;

            case MAIN_MENU_SCREEN:
                menu_op = menu_screen(key, reset_flag); //Solved
                switch (menu_op) {
                    case RETURN_BACK:
                        operational_flag = DASHBOARD_SCREEN;
                        break;

                    case VIEW_LOG:
                        operational_flag = VIEW_LOG_SCREEN;
                        reset_flag = RESET_VIEW_LOG;
                        continue;

                    case CLEAR_LOG:
                        operational_flag = CLEAR_LOG_SCREEN;
                        reset_flag = RESET_CLEAR_LOG;
                        continue;

                    case DOWNLOAD_LOG:
                        operational_flag = DOWNLOAD_LOG_SCREEN;
                        reset_flag = RESET_DOWNLOAD_LOG;
                        continue;

                    case SET_TIME:
                        operational_flag = SET_TIME_SCREEN;
                        reset_flag = RESET_SET_TIME;
                        continue;

                    case CHANGE_PASSWORD:
                        operational_flag = CHANGE_PASSWORD_SCREEN;
                        reset_flag = RESET_CHANGE_PASSWORD;
                        continue;

                }
                break;


            case VIEW_LOG_SCREEN:

                switch (view_log(key, reset_flag)) {
                    case RETURN_MAIN_MENU:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        break;

                    case RETURN_BACK:
                        operational_flag = DASHBOARD_SCREEN;
                        clear_screen();
                        break;
                }
                break;

            case CLEAR_LOG_SCREEN:

                switch (clear_log(key, reset_flag)) {
                    case RETURN_MAIN_MENU:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        break;
                }
                break;

            case DOWNLOAD_LOG_SCREEN:

                switch (download_log(key, reset_flag)) {
                    case RETURN_MAIN_MENU:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        break;
                }
                break;

            case SET_TIME_SCREEN:

                switch (set_time(key, reset_flag)) {
                    case RETURN_MAIN_MENU:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        break;
                }
                break;

            case CHANGE_PASSWORD_SCREEN:

                switch (change_password(key, reset_flag)) {
                    case RETURN_MAIN_MENU:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        break;

                    case PASSWORD_CHANGED:
                        operational_flag = MAIN_MENU_SCREEN;
                        reset_flag = RESET_MENU;
                        clear_screen();
                        break;
                }
                break;
        }

        reset_flag = RESET_NOTHING;


    }
    return;
}
