/*
 * =====================================================================================
 *
 *       Filename:  LUFAConfig.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/04/2020 10:16:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _LUFA_CONFIG_H_
#define _LUFA_CONFIG_H_
        #define GENERIC_REPORT_SIZE     8
        #define USE_STATIC_OPTIONS      (USB_DEVICE_OPT_FULLSPEED | USB_OPT_REG_ENABLED | USB_OPT_AUTO_PLL)
        #define USE_FLASH_DESCRIPTORS
        #define FIXED_CONTROL_ENDPOINT_SIZE      8
        #define FIXED_NUM_CONFIGURATIONS         1
#endif
