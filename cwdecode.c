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

#define Table_Count  51


const uint8_t Morse_Code_Table[Table_Count][3] PROGMEM ={
    {0, 0b0, '*'},
    {1, 0b0, 'E'},
    {1 ,0b1, 'T'},
    {2, 0b00, 'I'},
    {2, 0b01, 'A'},
    {2, 0b10, 'N'},
    {2, 0b11, 'M'},
    {3, 0b000, 'S'},
    {3, 0b001, 'U'},
    {3, 0b010, 'R'},
    {3, 0b011, 'W'},
    {3, 0b100, 'D'},
    {3, 0b101, 'K'},
    {3, 0b110, 'G'},
    {3, 0b111, 'O'},
    {4, 0b0000, 'H'},
    {4, 0b0001, 'V'},
    {4, 0b0010, 'F'},
    {4, 0b0011, '-'}, // unknown u?
    {4, 0b0100, 'L'},
    {4, 0b0101, '-'}, // unknown a?
    {4, 0b0110, 'P'},
    {4, 0b0111, 'J'},
    {4, 0b1000, 'B'},
    {4, 0b1001, 'X'},
    {4, 0b1010, 'C'},
    {4, 0b1011, 'Y'},
    {4, 0b1100, 'Z'},
    {4, 0b1101, 'Q'},
    {4, 0b1110, '-'}, // unknown o？
    {4, 0b1111, '-'}, // unknown

    {5, 0b00000, '5'},
    {5, 0b00001, '4'},
    {5, 0b00011, '3'},
    {5, 0b00111, '2'},
    {5, 0b01111, '1'},
    {5, 0b10000, '6'},
    {5, 0b11000, '7'},
    {5, 0b11100, '8'},
    {5, 0b11110, '9'},
    {5, 0b11111, '0'},

    {5, 0b10010, '/'},
    {5, 0b10110, '('},

    {5, 0b101101, ')'},
    {6, 0b110011, ','},
    {6, 0b010101, '.'},
    {6, 0b001100, '?'},
    {6, 0b010010, '"'},
    {6, 0b111000, ':'},
    {6, 0b011110, '\''},
    {6, 0b100001, '_'},
};

uint8_t index = 0;
uint8_t max_index = 6;
uint8_t precode = 0;
uint8_t rawvalue = 0;

void decode_timer_init(void){
    TCCR0A = 0;
    TCCR0B = 0;
    TCNT0  = 0;
    OCR0A = 249;
    TCCR0A |= _BV(WGM01);
    TCCR0B |= _BV(CS01) | _BV(CS00);
    TIMSK0 |= (1 << OCIE0A);
}

uint8_t decode_timer_exec(uint8_t code){
    uint8_t ret = 0;
    if (code == 1 ||code ==  2 || code == 3 || code == 4){
        if (precode != code){
            precode = code;
            if (code == 1){
                ret =  decode_result();
                decode_clean();
                return ret;
            }
            else if (index > max_index){
                decode_clean();
                ret = 0;
            }
            else if (code == 2 || code == 3){
                decode_update(code);
                index ++;
            }
        }
    }
    else if ( code == 5){
        // TODO blank count .
    }
    else{
        decode_clean();
        ret = 1;
    }
    return ret;
}

void decode_clean(void){
    index = 0;
    rawvalue = 0;
}

void decode_update(uint8_t code){
    if (code == 3){
        rawvalue |= _BV(index);
    }
}

uint8_t decode_result(void){
    uint8_t pindex = 0;
    uint8_t pcode = 0;
    uint8_t pvalue = pgm_read_byte(&(Morse_Code_Table[0][2]));

    for (uint8_t i =0; i< Table_Count; i++){
        pindex = pgm_read_byte(&(Morse_Code_Table[i][0]));
        if (pindex  == index){
            pcode = pgm_read_byte(&(Morse_Code_Table[i][1]));
            if (pcode == rawvalue){
                pvalue = pgm_read_byte(&(Morse_Code_Table[i][2]));
            }
        }
    }
    return pvalue;
}
