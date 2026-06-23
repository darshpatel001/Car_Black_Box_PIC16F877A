/* 
 * File:   car_black_box_def.h
 * Author: darsh
 *
 * Created on May 27, 2026, 9:48 PM
 */

#ifndef CAR_BLACK_BOX_DEF_H
#define	CAR_BLACK_BOX_DEF_H



void display_dashboard_screen(unsigned char event[], unsigned char speed);
unsigned char login_screen(unsigned char key, unsigned char reset_flag); //
void log_event(unsigned char event[], unsigned char speed);
unsigned char menu_screen(unsigned char key, unsigned char reset_flag); //
void clear_screen(void);


unsigned char view_log(unsigned char key, unsigned char reset_flag); // 
unsigned char clear_log(unsigned char key, unsigned char reset_flag); //
unsigned char change_password(unsigned char key, unsigned char reset_flag); //
unsigned char download_log(unsigned char key, unsigned char reset_flag);
unsigned char set_time(unsigned char key, unsigned char reset_flag);


#endif	/* CAR_BLACK_BOX_DEF_H */

