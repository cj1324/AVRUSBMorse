/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/27/2020 09:14:28 PM
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
#include <avr/interrupt.h>

#include <util/delay.h>

#include "main.h"

enum KeyStatus{
    KS_INIT=0,
    KS_READY,
    KS_DOT,
    KS_DASH,
    KS_TIMER
};

volatile uint8_t key_status = KS_INIT;
volatile uint8_t loop_out_inc = 0;


int main (void)
{


    // ALL LED Init, Ouput Hi
    DDRD |= _BV(PD0);
    PORTD |= _BV(PD0);

    // Key
    DDRD &= ~(_BV(PD5) | _BV(PD4));
    PORTD |= (_BV(PD5) | _BV(PD4));

    DDRB |= _BV(PB6);
    PORTB |= _BV(PB6);

    DDRB |= _BV(PB7);
    PORTB |= _BV(PB7);

    // Buzz Init, Ouput Low
    DDRC |= _BV(PC6);
    PORTC &= ~_BV(PC6);

    // PCINT0 Init
    DDRB &= ~_BV(PB0);
    PORTB |= _BV(PB0);


    // PCINT0 Enable
    PCICR |= (_BV(PCIE0) | _BV(PCIE1));
    PCMSK0 |= _BV(PCINT0);
    PCMSK1 |= _BV(PCINT12);

    EICRB |= _BV(ISC51);
    EICRB &= ~_BV(ISC50);
    EIMSK |= _BV(INT5);

    sei();

    for (;;){
        _delay_ms(1);
        if (key_status == KS_TIMER){
            key_timer_exec();
        }
        else if (key_status == KS_INIT){
            key_enable();
        }
    }
}

uint8_t key_timer_exec(void){
    loop_out_inc +=1;
    if (loop_out_inc >= LOOP_TIMEOUT){
        loop_out_inc =0;
        key_enable();
        return 0;
    }
    uint8_t in_dot = (PIND & _BV(PD5)) == 0 ? 1 : 0;
    uint8_t in_dash  = (PIND & _BV(PD4)) == 0 ? 1 : 0;
    if (in_dot + in_dash  == 2){
        return 0;
    }

    loop_out_inc += 1;
    _delay_ms(1);

    uint8_t in_dot_2 = (PIND & _BV(PD5)) == 0 ? 1 : 0;
    uint8_t in_dash_2  = (PIND & _BV(PD4)) == 0 ? 1 : 0;
    if (in_dot == 1 && in_dot_2 == 1){
        loop_out_inc =0;
        trigger_dot();
        return 1;
    }

    if (in_dash == 1 && in_dash_2 == 1){
        loop_out_inc =0;
        trigger_dash();
        return 1;
    }
    return 0;
}


void key_enable(void){
    PCMSK1 |= _BV(PCINT12);
    EIMSK |= _BV(INT5);
    key_status = KS_READY;
}


void key_disable(void){
    PCMSK1 &= ~_BV(PCINT12);
    EIMSK &= ~_BV(INT5);
    key_status = KS_INIT;
}


void trigger_dot(void){
    key_status = KS_DOT;
    PORTC |= _BV(PC6);
    _delay_ms(UnitTime);
    PORTC &= ~_BV(PC6);
    _delay_ms(UnitTime);
    key_status = KS_TIMER;
}


void trigger_dash(void){
    key_status = KS_DASH;
    PORTC |= _BV(PC6);
    _delay_ms(UnitTime * 3);
    PORTC &= ~_BV(PC6);
    _delay_ms(UnitTime);
    key_status = KS_TIMER;
}


ISR(PCINT0_vect)
{
        PORTC |= _BV(PC6);
        _delay_ms(10);
        PORTC &= ~_BV(PC6);
        key_enable();
}

ISR(PCINT1_vect)
{

    key_disable();
    _delay_ms(1);
    if ((PIND & _BV(PD5)) == 0)
    {
        trigger_dot();
    }
    else
    {
        key_enable();
    }
}

ISR(INT5_vect)
{
    key_disable();
    _delay_ms(1);
    if ((PIND & _BV(PD4)) == 0)
    {
        trigger_dash();
    }
    else
    {
        key_enable();
    }
}