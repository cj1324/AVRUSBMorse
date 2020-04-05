/*
 * =====================================================================================
 *
 *       Filename:  cwdecode.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2020 11:35:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "cwdecode.h"

const uint8_t Number[] PROGMEM = {1, 2, 3, 4};

void decode_timer_init(void){
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0  = 0;
    OCR0A = 249;
    TCCR0A |= _BV(WGM01);
    TCCR0B |= _BV(CS01) | _BV(CS00);
    TIMSK0 |= (1 << OCIE0A);
}

void decode_timer_exec(void){

}

