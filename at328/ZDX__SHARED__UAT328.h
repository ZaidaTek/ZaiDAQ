/*** Copyright (C) 2019-2025 ZaidaTek, Andreas Riebesehl
**** This work is licensed under: Creative Commons Attribution-NoDerivatives 4.0 International Public License
**** For full license text, please visit: https://creativecommons.org/licenses/by-nd/4.0/legalcode
***/
#ifndef ZDX__SHARED__UAT328_H_INCLUDED
#define ZDX__SHARED__UAT328_H_INCLUDED
// USER
#define ZDX_AT328_BUFFER 16
#define ZDX_AT328_TASK (ZDX_TASK_DIO | ZDX_TASK_PWM | ZDX_TASK_ADC)
#define ZDX_AT328_DIO_MASK 0xff
#define ZDX_AT328_PWM_MASK 0xff
#define ZDX_AT328_PWM_RATE_MIN 100
#define ZDX_AT328_PWM_RATE_MAX 50000
#define ZDX_AT328_ADC_MASK 0xff
#define ZDX_AT328_ADC_RESOLUTION 10
#define ZDX_AT328_ADC_RATE_MIN 100
#define ZDX_AT328_ADC_RATE_MAX 10000000
// COMPOUND
#define ZDX_AT328_ADC_CHANNELS (\
(ZDX_AT328_ADC_MASK & 0x80 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x40 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x20 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x10 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x8 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x4 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x2 ? 1 : 0) + \
(ZDX_AT328_ADC_MASK & 0x1 ? 1 : 0))

#endif // ZDX__SHARED__UAT328_H_INCLUDED
