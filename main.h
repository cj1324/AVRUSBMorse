/*
 * =====================================================================================
 *
 *       Filename:  main.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/07/2020 04:26:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef _MAIN_H_
#define _MAIN_H_

#define WPM       10
#define UnitTime 1200/WPM
#define LOOP_TIMEOUT 2 * 1200/WPM

void init_buzz_pwm(void);
void key_enable(void);
void key_disable(void);
void trigger_dot(void);
void trigger_dash(void);
uint8_t key_timer_exec(void);
#endif
