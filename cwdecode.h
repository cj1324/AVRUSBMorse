/*
 * =====================================================================================
 *
 *       Filename:  cwdecode.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2020 11:35:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _CWDECODE_H_
#define _CWDECODE_H_
void decode_timer_init(void);
uint8_t decode_timer_exec(uint8_t code);

void decode_clean(void);
void decode_update(uint8_t code);
uint8_t decode_result(void);

#endif
