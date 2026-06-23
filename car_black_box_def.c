#include "main.h"

unsigned char clock_reg[3];
char time[7];
char log[11];
char pos = 0;
unsigned char sec, return_time;
char *menus[] = {"View log", "Clear log", "Download log", "Set time", "Change password"};
char log[11];
char menu_pos;

void clear_screen(void) {
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
}

void store_log() {
    char addr = 0x05;
    if (pos++ == 10) {
        pos = 0;
    }
    addr = addr + pos * 10;
    ext_eeprom_24C02_str_write(addr, log);
}

void log_event(unsigned char event[], unsigned char speed) {
    //time event speed
    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    log[8] = speed / 10 + '0';
    log[9] = speed % 10 + '0';
    log[10] = '\0';
    store_log();


}

void get_time(void) {
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    time[0] = ((clock_reg[0] >> 4)&0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';

    time[2] = ((clock_reg[1] >> 4)&0x03) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';

    time[4] = ((clock_reg[2] >> 4)&0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';

    time[6] = '\0';
}

void display_time() {
    get_time();

    clcd_putch(time[0], LINE2(2));
    clcd_putch(time[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(time[2], LINE2(5));
    clcd_putch(time[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(time[4], LINE2(8));
    clcd_putch(time[5], LINE2(9));

}

void display_dashboard_screen(unsigned char event[], unsigned char speed) {
    clcd_print("    TIME   EV SP", LINE1(0));


    clcd_print(event, LINE2(11));


    clcd_putch(speed / 10 + '0', LINE2(14));
    clcd_putch(speed % 10 + '0', LINE2(15));


    display_time();

}

unsigned char login_screen(unsigned char key, unsigned char reset_flag) {

    static char usr_passwd[4];
    static unsigned char i;
    static unsigned char attempt_left;
    char stored_passwd[4];

    if (reset_flag == RESET_LOGIN_SCREEN) {
        i = 0;
        attempt_left = 3;
        TMR2ON = 1;
        key = ALL_RELEASED;
        return_time = 10;
    }
    //Taking input
    if (return_time == 0)
        return RETURN_BACK;

    if (key == SW4 && i < 4) { //1
        usr_passwd[i] = '1';
        clcd_putch('*', LINE2(4 + i));
        i++;
        return_time = 5;

    } else if (key == SW5 && i < 4) { //0
        usr_passwd[i] = '0';
        clcd_putch('*', LINE2(4 + i));
        i++;
        return_time = 5;
    }
    __delay_ms(250);

    //Compare
    if (i == 4) {

        for (int j = 0; j < 4; j++) {
            stored_passwd[j] = ext_eeprom_24C02_read(j);
        }


        if (strncmp(usr_passwd, stored_passwd, 4) == 0) {
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, LINE2(4));
            clcd_print("Login Success", LINE1(2));
            __delay_ms(1000);
            return LOGIN_SUCCESS;
        } else {
            attempt_left--;
            clear_screen();
            if (attempt_left == 0) {
                clcd_write(DISP_ON_AND_CURSOR_OFF, LINE2(4));
                clcd_print("You are blocked", LINE1(0));
                clcd_print("Wait for", LINE2(0));
                clcd_print("secs", LINE2(12));
                sec = 60;
                while (sec) {
                    clcd_putch(sec / 10 + '0', LINE2(9));
                    clcd_putch(sec % 10 + '0', LINE2(10));
                }
                attempt_left = 3;
            } else {

                clcd_write(DISP_ON_AND_CURSOR_OFF, LINE2(4));
                clcd_print("Wrong Password", LINE1(0));
                clcd_putch(attempt_left + '0', LINE2(0));
                clcd_print("Attempts left", LINE2(2));
                __delay_ms(2000);
            }
            clear_screen();
            i = 0;
            return_time = 5;
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
        }
    }

}

unsigned char menu_screen(unsigned char key, unsigned char reset_flag) {
    static unsigned int press_count = 0;
    static unsigned char pressed_key = ALL_RELEASED;

    if (reset_flag == RESET_MENU) {
        menu_pos = 0;
        return_time = 7;
        TMR2ON = 1; 
        press_count = 0;
        pressed_key = ALL_RELEASED;
        clear_screen();
    }

    if (return_time == 0) {
        clear_screen();
        return RETURN_BACK;
    }


    /* Detect key press using LEVEL */
    key = read_digital_keypad(LEVEL);

    if (key != ALL_RELEASED) {
        return_time = 10;
        if (pressed_key == ALL_RELEASED) {
            pressed_key = key;
            press_count = 0;
        }

        press_count++;
    } else {
        if (pressed_key != ALL_RELEASED) {
            /* LONG PRESS */

            if (press_count >= LONG_PRESS) {
                if (pressed_key == SW4) {
                    pressed_key = ALL_RELEASED;
                    press_count = 0;

                    switch (menu_pos) {
                        case 0: return VIEW_LOG;
                        case 1: return CLEAR_LOG;
                        case 2: return DOWNLOAD_LOG;
                        case 3: return SET_TIME;
                        case 4: return CHANGE_PASSWORD;
                    }
                } else if (pressed_key == SW5) {
                    pressed_key = ALL_RELEASED;
                    press_count = 0;
                    clear_screen();
                    return RETURN_BACK;
                }
            }/* SHORT PRESS */
            else {
                if (pressed_key == SW4 && menu_pos > 0) {
                    menu_pos--;
                    clear_screen();
                } else if (pressed_key == SW5 && menu_pos < 4) {
                    menu_pos++;
                    clear_screen();
                }
            }

            pressed_key = ALL_RELEASED;
            press_count = 0;
        }
    }

    /* Display menu */
    if (menu_pos == 4) {
        clcd_print(menus[menu_pos - 1], LINE1(2));
        clcd_print(menus[menu_pos], LINE2(2));
        clcd_putch('*', LINE2(0));
    } else {
        clcd_print(menus[menu_pos], LINE1(2));
        clcd_print(menus[menu_pos + 1], LINE2(2));
        clcd_putch('*', LINE1(0));
    }

    return 0;
}

unsigned char view_log(unsigned char key, unsigned char reset_flag) {
    static signed char log_pos;
    static unsigned int lp_count = 0;
    unsigned char addr;
    unsigned char empty = 1;

    if (reset_flag == RESET_VIEW_LOG) {
        log_pos = 0;
        return_time = 7;
        TMR2ON = 1; //Auto log out setup
        clear_screen();
    }

    if (return_time == 0)
        return RETURN_MAIN_MENU;

    /* Long Press SW5 -> Back to Main Menu */
    if (read_digital_keypad(LEVEL) == SW5) {
        lp_count++;

        if (lp_count >= LONG_PRESS) {
            while (read_digital_keypad(LEVEL) == SW5);

            lp_count = 0;
            clear_screen();
            return RETURN_MAIN_MENU;
        }
    } else {
        lp_count = 0;
    }

    /* Read current log record */
    addr = 0x05 + (log_pos * 10);

    for (unsigned char i = 0; i < 10; i++) {
        log[i] = ext_eeprom_24C02_read(addr + i);
    }

    log[10] = '\0';

    /* Check if log entry is empty */
    for (unsigned char i = 0; i < 10; i++) {
        if (log[i] != 0xFF) {
            empty = 0;
            break;
        }
    }

    if (empty) {
        clcd_print("No Logs Found", LINE1(1));
        return 0;
    }

    /* Previous Log */
    if (key == SW4) {
        if (log_pos == 0)
            log_pos = 9;
        else
            log_pos--;

        return_time = 5;
        clear_screen();

        addr = 0x05 + (log_pos * 10);

        for (unsigned char i = 0; i < 10; i++) {
            log[i] = ext_eeprom_24C02_read(addr + i);
        }
    }/* Next Log */
    else if (key == SW5) {
        if (log_pos == 9)
            log_pos = 0;
        else
            log_pos++;

        return_time = 5;
        clear_screen();

        addr = 0x05 + (log_pos * 10);

        for (unsigned char i = 0; i < 10; i++) {
            log[i] = ext_eeprom_24C02_read(addr + i);
        }
    }

    /* Display Header */
    clcd_print("#   TIME   EV SP", LINE1(0));

    /* Record Number */
    clcd_putch(log_pos + '0', LINE2(0));
    clcd_putch(' ', LINE2(1));

    /* Time */
    clcd_putch(log[0], LINE2(2));
    clcd_putch(log[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(log[2], LINE2(5));
    clcd_putch(log[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(log[4], LINE2(8));
    clcd_putch(log[5], LINE2(9));

    /* Event */
    clcd_putch(log[6], LINE2(11));
    clcd_putch(log[7], LINE2(12));

    /* Speed */
    clcd_putch(log[8], LINE2(14));
    clcd_putch(log[9], LINE2(15));

    return 0;
}

unsigned char clear_log(unsigned char key, unsigned char reset_flag) {

    if (reset_flag == RESET_CLEAR_LOG) {
        return_time = 5;
        return_time = 7;
        TMR2ON = 1; //Auto log out setup
        clear_screen();
        clcd_print("Clear Logs ?", LINE1(2));
        clcd_print(" SW4:YES SW5:NO", LINE2(0));

    }



    if (return_time == 0)
        return RETURN_MAIN_MENU;

    /* SW4 = YES */
    if (key == SW4) {
        for (unsigned char i = 0; i < 10; i++) {
            unsigned char addr = 0x05 + (i * 10);

            for (unsigned char j = 0; j < 10; j++) {
                ext_eeprom_24C02_byte_write(addr + j, 0xFF);
            }
        }

        pos = 0;

        clear_screen();
        clcd_print("Logs Cleared", LINE1(2));
        __delay_ms(1500);

        return RETURN_MAIN_MENU;
    }

    /* SW5 = NO */
    if (key == SW5) {
        return RETURN_MAIN_MENU;
    }

    return 0;
}

unsigned char change_password(unsigned char key, unsigned char reset_flag) {
    static unsigned char new_pass[4];
    static unsigned char confirm_pass[4];
    static unsigned char i;
    static unsigned char stage;

    if (reset_flag == RESET_CHANGE_PASSWORD) {
        i = 0;
        stage = 0;
        return_time = 7;
        TMR2ON = 1; //Auto log out setup

        clear_screen();
        clcd_print("Enter New Pass", LINE1(0));

        clcd_write(LINE2(4), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE); // Cursor ON
    }

    if (return_time == 0) {
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Cursor ON
        clear_screen();
        return RETURN_MAIN_MENU;
    }


    /* Password Entry */
    if (key == SW4 && i < 4) {
        if (stage == 0)
            new_pass[i] = '1';
        else
            confirm_pass[i] = '1';

        clcd_putch('*', LINE2(4 + i));
        i++;
        return_time = 5;

        clcd_write(LINE2(4 + i), INST_MODE); // Move cursor
    } else if (key == SW5 && i < 4) {
        if (stage == 0)
            new_pass[i] = '0';
        else
            confirm_pass[i] = '0';

        clcd_putch('*', LINE2(4 + i));
        i++;
        return_time = 5;

        clcd_write(LINE2(4 + i), INST_MODE); // Move cursor
    }

    /* First Password Complete */
    if (i == 4 && stage == 0) {
        stage = 1;
        i = 0;

        clear_screen();
        clcd_print("Re-enter Pass", LINE1(0));

        clcd_write(LINE2(4), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE); // Cursor ON
    }

    /* Confirmation Complete */
    if (i == 4 && stage == 1) {
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Cursor OFF

        if (strncmp(new_pass, confirm_pass, 4) == 0) {
            char temp[5];

            temp[0] = new_pass[0];
            temp[1] = new_pass[1];
            temp[2] = new_pass[2];
            temp[3] = new_pass[3];
            temp[4] = '\0';

            ext_eeprom_24C02_str_write(0x00, temp);

            clear_screen();
            clcd_print("Pass Changed", LINE1(1));
            __delay_ms(1500);

            return PASSWORD_CHANGED;
        } else {
            clear_screen();
            clcd_print("Mismatch", LINE1(4));
            __delay_ms(1500);

            stage = 0;
            i = 0;

            clear_screen();
            clcd_print("Enter New Pass", LINE1(0));

            clcd_write(LINE2(4), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE); // Cursor ON again
        }
    }

    return 0;
}

unsigned char download_log(unsigned char key, unsigned char reset_flag) {
    unsigned char addr;
    unsigned char empty;
    char log_data[11];

    if (reset_flag == RESET_DOWNLOAD_LOG) {
        clear_screen();
        clcd_print("Downloading...", LINE1(0));
        clcd_print("Check UART", LINE2(2));

        puts("\r\n--------------------------------");
        puts("\r\nS.No TIME     EV SP");
        puts("\r\n--------------------------------\n");

        for (unsigned char rec = 0; rec < 10; rec++) {
            addr = 0x05 + (rec * 10);
            empty = 1;

            for (unsigned char i = 0; i < 10; i++) {
                log_data[i] = ext_eeprom_24C02_read(addr + i);

                if (log_data[i] != 0xFF)
                    empty = 0;
            }

            if (empty)
                continue;

            putchar(rec + '0');
            puts("   ");

            putchar(log_data[0]);
            putchar(log_data[1]);
            putchar(':');
            putchar(log_data[2]);
            putchar(log_data[3]);
            putchar(':');
            putchar(log_data[4]);
            putchar(log_data[5]);

            puts("   ");

            putchar(log_data[6]);
            putchar(log_data[7]);

            puts("   ");

            putchar(log_data[8]);
            putchar(log_data[9]);

            puts("\r\n");
        }

        puts("--------------------------------\r\n");
    }

    if (key == SW5) {
        clear_screen();
        return RETURN_MAIN_MENU;
    }

    return 0;
}
unsigned char set_time(unsigned char key, unsigned char reset_flag)
{
    static unsigned char hr;
    static unsigned char min;
    static unsigned char sec;
    static unsigned char field;
    static unsigned int lp_count;

    unsigned char blink_flag;

    if(reset_flag == RESET_SET_TIME)
    {
        return_time = 10;
        TMR2ON = 1;

        hr = (((read_ds1307(HOUR_ADDR) >> 4) & 0x03) * 10)
                + (read_ds1307(HOUR_ADDR) & 0x0F);

        min = (((read_ds1307(MIN_ADDR) >> 4) & 0x07) * 10)
                + (read_ds1307(MIN_ADDR) & 0x0F);

        sec = (((read_ds1307(SEC_ADDR) >> 4) & 0x07) * 10)
                + (read_ds1307(SEC_ADDR) & 0x0F);

        field = 0;
        lp_count = 0;

        clear_screen();

        clcd_print("SET TIME", LINE1(3));

        clcd_putch(':', LINE2(5));
        clcd_putch(':', LINE2(8));

        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    }

    if(return_time == 0)
    {
        clear_screen();
        return RETURN_MAIN_MENU;
    }

    /* SW4 -> Increment */
    if(key == SW4)
    {
        switch(field)
        {
            case 0:
                hr = (hr + 1) % 24;
                break;

            case 1:
                min = (min + 1) % 60;
                break;

            case 2:
                sec = (sec + 1) % 60;
                break;
        }

        return_time = 10;
    }

    /* SW5 -> Next Field */
    if(key == SW5)
    {
        field++;

        if(field > 2)
            field = 0;

        return_time = 10;
    }

    /* Long Press SW4 -> Save */
    if(read_digital_keypad(LEVEL) == SW4)
    {
        lp_count++;

        if(lp_count >= LONG_PRESS)
        {
            write_ds1307(HOUR_ADDR,
                    ((hr / 10) << 4) | (hr % 10));

            write_ds1307(MIN_ADDR,
                    ((min / 10) << 4) | (min % 10));

            write_ds1307(SEC_ADDR,
                    ((sec / 10) << 4) | (sec % 10));

            clear_screen();
            clcd_print("TIME UPDATED", LINE1(2));

            __delay_ms(1500);

            return RETURN_MAIN_MENU;
        }
    }
    else
    {
        lp_count = 0;
    }

    /* Blink source */
    blink_flag = (return_time & 0x01);

    /* HH */
    if(field == 0 && blink_flag)
    {
        clcd_putch(' ', LINE2(3));
        clcd_putch(' ', LINE2(4));
    }
    else
    {
        clcd_putch(hr / 10 + '0', LINE2(3));
        clcd_putch(hr % 10 + '0', LINE2(4));
    }

    /* MM */
    if(field == 1 && blink_flag)
    {
        clcd_putch(' ', LINE2(6));
        clcd_putch(' ', LINE2(7));
    }
    else
    {
        clcd_putch(min / 10 + '0', LINE2(6));
        clcd_putch(min % 10 + '0', LINE2(7));
    }

    /* SS */
    if(field == 2 && blink_flag)
    {
        clcd_putch(' ', LINE2(9));
        clcd_putch(' ', LINE2(10));
    }
    else
    {
        clcd_putch(sec / 10 + '0', LINE2(9));
        clcd_putch(sec % 10 + '0', LINE2(10));
    }

    return 0;
}