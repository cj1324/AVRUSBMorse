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

#include "GenericHID.h"

#include "main.h"
#include "cwdecode.h"

enum KeyStatus{
    KS_INIT=0,
    KS_READY,
    KS_DOT,
    KS_DASH,
    KS_BLANK,
    KS_TIMER
};

volatile uint8_t key_status = KS_INIT;
uint16_t loop_out_inc = 0;
uint16_t play_time = 0;
uint16_t blank_time = 0;
uint8_t decode_value = 0;

static uint8_t PrevHIDReportBuffer[GENERIC_REPORT_SIZE];

USB_ClassInfo_HID_Device_t Generic_HID_Interface =
    {
        .Config =
            {
                .InterfaceNumber              = INTERFACE_ID_GenericHID,
                .ReportINEndpoint             =
                    {
                        .Address              = GENERIC_IN_EPADDR,
                        .Size                 = GENERIC_EPSIZE,
                        .Banks                = 1,
                    },
                .PrevReportINBuffer           = PrevHIDReportBuffer,
                .PrevReportINBufferSize       = sizeof(PrevHIDReportBuffer),
            },
    };


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

    init_buzz();

    SetupHardware();

    decode_timer_init();


    sei();

    if (key_status == KS_INIT){
        key_enable();
    }


    GlobalInterruptEnable();

    for (;;)
    {
        HID_Device_USBTask(&Generic_HID_Interface);
        USB_USBTask();
    }

}

void SetupHardware(void)
{
    /* Disable watchdog if enabled by bootloader/fuses */
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    /* Disable clock division */
    clock_prescale_set(clock_div_1);

    /* Hardware Initialization */
    USB_Init();
}

void init_buzz(void){
    DDRC |= _BV(PC6);
    PORTC &= ~_BV(PC6);
#ifdef USE_PWM_BUZZ
    // Buzz Init, PWM Ouput
    ICR1 = 0x6800; // 600Hz
    OCR1A = ICR1 >>1; // 50%

    TCCR1A |= _BV(WGM11);
    TCCR1B |= _BV(WGM12)| _BV(WGM13);
    TCCR1B |= (1 << CS10);
#endif
}


void on_buzz(void){
#ifdef USE_PWM_BUZZ
    TCCR1A |= _BV(COM1A1);
#else
    PORTC |= _BV(PC6);
#endif
}

void off_buzz(void){
#ifdef USE_PWM_BUZZ
    TCCR1A &= ~_BV(COM1A1);
#else
    PORTC &= ~_BV(PC6);
#endif
}

uint8_t key_timer_exec(void){
    loop_out_inc += 1;
    if (key_status == KS_DOT || key_status == KS_DASH){
        if (loop_out_inc <= play_time){
            on_buzz();
            return 0;
        } else {
            loop_out_inc = 0;
            off_buzz();
            key_status = KS_BLANK;
            return 0;
        }
    }
    else if (key_status == KS_BLANK){
        if (loop_out_inc <= blank_time){
            return 0;
        }else {
            key_status = KS_TIMER;
            return 0;
        }
    } else if (key_status == KS_TIMER) {
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
        _delay_us(20);
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

    loop_out_inc = 0;
    play_time = UnitTime;
    blank_time = UnitTime;
    key_status = KS_DOT;
}


void trigger_dash(void){
    loop_out_inc = 0;
    play_time = UnitTime * 3;
    blank_time = UnitTime;
    key_status = KS_DASH;
}

bool CALLBACK_HID_Device_CreateHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                         uint8_t* const ReportID,
                                         const uint8_t ReportType,
                                         void* ReportData,
                                         uint16_t* const ReportSize)
{
    uint8_t* Data        = (uint8_t*)ReportData;
    *ReportSize = GENERIC_REPORT_SIZE;
    Data[1] = key_status;
    Data[3] = UnitTime;
    Data[5] = decode_value;
    return false;
}

void CALLBACK_HID_Device_ProcessHIDReport(USB_ClassInfo_HID_Device_t* const HIDInterfaceInfo,
                                          const uint8_t ReportID,
                                          const uint8_t ReportType,
                                          const void* ReportData,
                                          const uint16_t ReportSize)
{
    uint8_t* Data       = (uint8_t*)ReportData;
    // TODO.
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    // USE LED.
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    // USE LED.
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= HID_Device_ConfigureEndpoints(&Generic_HID_Interface);

    USB_Device_EnableSOFEvents();

}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    HID_Device_ProcessControlRequest(&Generic_HID_Interface);
}

/** Event handler for the USB device Start Of Frame event. */
void EVENT_USB_Device_StartOfFrame(void)
{
    HID_Device_MillisecondElapsed(&Generic_HID_Interface);
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
    _delay_us(20);
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
    _delay_us(20);
    if ((PIND & _BV(PD4)) == 0)
    {
        trigger_dash();
    }
    else
    {
        key_enable();
    }
}

ISR(TIMER0_COMPA_vect)
{
    key_timer_exec();
    decode_value = decode_timer_exec(key_status);
}
