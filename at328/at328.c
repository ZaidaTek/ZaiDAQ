/*** Copyright (C) 2019-2025 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: Creative Commons Attribution-NoDerivatives 4.0 International Public License
**** For full license text, please visit: https://creativecommons.org/licenses/by-nd/4.0/legalcode
***/

// v250906
// WARNING: THE EMBEDDED CODE HERE HAS BECOME INCOMPATIBLE WITH ZAIDASCOPE-v210603 / ZDAQ-v250831 !!!
// DO NOT USE THIS FOR USE WITH THAT SOFTWARE, INSTEAD USE THE INCLUDED EMBEDDED CODE IN THE RELEASE FILES
// THESE ARE THE FIRST STEPS IN UNIFYING OVERLAPPING #defines IN EMBEDDED/APPLICATION CODE

#include <avr/io.h>

#include "ZDX-SHARED-DEFINE.h"
#include "ZDX-SHARED-UAT328.h"

unsigned char gBuffer[ZDX_AT328_BUFFER];

void ZDX_AT328_ADC_InitClock(const unsigned long *iTimer, const unsigned char *iChannels) {
	unsigned long lPrescale = 16UL * 0x2UL * (unsigned long)*iChannels;
	unsigned char i = 0;
	while ((*iTimer > lPrescale) && (i < 7)) {
		++i;
		lPrescale <<= 1;
	}
	ADCSRB = 0x0;
	ADCSRA = 0x80 | i;
}
void ZDX_AT328_ADC_InitTimer(const unsigned long *iTimer) {
	unsigned int lTimer;
	unsigned char lPrescale;
	if (*iTimer < 0x10000UL) {
		lTimer = *iTimer;
		lPrescale = 0x1;
	} else if (*iTimer < 0x80000UL) {
		lTimer = *iTimer >> 3;
		lPrescale = 0x2;
	} else if (*iTimer < 0x400000UL) {
		lTimer = *iTimer >> 6;
		lPrescale = 0x3;
	} else if (*iTimer < 0x1000000UL) {
		lTimer = *iTimer >> 8;
		lPrescale = 0x4;
	} else {
		lTimer = *iTimer >> 10;
		lPrescale = 0x5;
	}
	OCR1A = lTimer;
	TCCR1A = 0x0;
	TCCR1B = 0x8 | lPrescale;
}
void ZDX_AT328_ADC_InitMain(unsigned char *oPort, unsigned char *oCount) {
	unsigned char lADC_Config = gBuffer[8];
	unsigned long lADC_Timer = (unsigned long)gBuffer[1] << 24;
	lADC_Timer |= (unsigned long)gBuffer[2] << 16;
	lADC_Timer |= (unsigned long)gBuffer[3] << 8;
	lADC_Timer |= (unsigned long)gBuffer[4];
	*oCount = 0;
	for (unsigned char i = 0; i < ZDX_AT328_ADC_CHANNELS; i++) {if (lADC_Config & (1 << i)) {oPort[(*oCount)++] = i;}}
	UCSR0B = 0x8;
	DIDR0 = 0x3f;
	DIDR1 = 0x3;
	ADMUX |= 0x40;
	ZDX_AT328_ADC_InitClock(&lADC_Timer, oCount);
	ZDX_AT328_ADC_InitTimer(&lADC_Timer);
}
void ZDX_AT328_ADC_Main(void) {
	unsigned char lPort[ZDX_AT328_ADC_CHANNELS];
	unsigned char lChannels;
	ZDX_AT328_ADC_InitMain(lPort, &lChannels);
	unsigned char lTransmit;
	unsigned char lRead = 0;
	#define mZDX_AT328_ADC_Main_INCREMENT() lTransmit = lRead; if (++lRead == lChannels) {lRead = 0;} ADMUX &= 0xf0; ADMUX |= lPort[lRead]
	mZDX_AT328_ADC_Main_INCREMENT();
	while(0x1) {
		unsigned char lAcquire = lChannels;
		do {
			if (!(ADCSRA & 0x40)) {
				unsigned char lSampleL = ADCL;
				unsigned char lSampleH = ADCH;
				ADCSRA |= 0x40;
				while (!(UCSR0A & 0x20));
				UDR0 = 0x80 | (lTransmit << 3) | (lSampleH << 1) | (lSampleL >> 7);
				while (!(UCSR0A & 0x20));
				UDR0 = lSampleL & 0x7f;
				mZDX_AT328_ADC_Main_INCREMENT();
				--lAcquire;
			}
		} while (lAcquire);
		while (!(TIFR1 & 0x2));
		TIFR1 |= 0x2;
	}
	#undef mZDX_AT328_ADC_Main_INCREMENT
}
void ZDX_AT328_PWM_Init(const unsigned char *iConfig, const unsigned int *iTimer) {
	DDRB = (*iConfig) & 0x3;
	DDRD = (*iConfig) & 0xfc;
	OCR1A = *iTimer; // shouldn't this be one less?
}
#define mZDX_AT328_PWM_MAIN(CONFIG, DEPTH, TYPE) \
	unsigned char lCache = 0x0;\
	unsigned char lConfig = *(CONFIG);\
	unsigned TYPE lMax = *(DEPTH);\
	unsigned TYPE lPhase = lMax;\
	unsigned TYPE lDuty[8] = {0x0};\
	TCCR1A = 0x0;\
	TCCR1B = 0x9;\
	while (0x1) {\
		if (TIFR1 & 0x2) {\
			TIFR1 |= 0x2;\
			++lPhase;\
			lPhase &= lMax;\
			unsigned char lOut;\
			for (unsigned char i = 8; i;) {lOut <<= 1; lOut |= (lDuty[--i] >= lPhase);}\
			lOut &= lConfig;\
			PORTB = lOut & 0x3;\
			PORTD = lOut & 0xfc;\
		}\
		if (UCSR0A & 0x80) {\
			unsigned char lBuffer = UDR0;\
			if (lCache) {\
				if (!(lBuffer & 0x80)) {lDuty[(lCache >> 3) & 0x7] = ((unsigned TYPE)(lCache & 0x7) << 7) | (unsigned TYPE)lBuffer;}\
				lCache = 0x0;\
			} else if (lBuffer & 0x80) {\
				lCache = lBuffer;\
			}\
		}\
	}
void ZDX_AT328_PWM_Main8(const unsigned char *iConfig, const unsigned int *iDepth) {
	mZDX_AT328_PWM_MAIN(iConfig, iDepth, char);
}
void ZDX_AT328_PWM_Main16(const unsigned char *iConfig, const unsigned int *iDepth) {
	mZDX_AT328_PWM_MAIN(iConfig, iDepth, int);
}
#undef mZDX_AT328_PWM_MAIN
void ZDX_AT328_PWM_Main(void) {
	unsigned int lDepth;
	unsigned int lTimer;
	unsigned char lConfig = gBuffer[1];
	lTimer = (unsigned int)gBuffer[2] << 8;
	lTimer |= (unsigned int)gBuffer[3];
	lDepth = (unsigned int)gBuffer[4] << 8;
	lDepth |= (unsigned int)gBuffer[5];
	ZDX_AT328_PWM_Init(&lConfig, &lTimer);
	if (lDepth > 0xff) {
		ZDX_AT328_PWM_Main16(&lConfig, &lDepth);
	} else {
		ZDX_AT328_PWM_Main8(&lConfig, &lDepth);
	}
}
void ZDX_AT328_DIO_Write(unsigned char iState) {
	unsigned char lPortB = iState & 0x3;
	unsigned char lPortD = iState & 0xfc;
	PORTB &= lPortB;
	PORTB |= lPortB;
	PORTD &= lPortD;
	PORTD |= lPortD;
}
void ZDX_AT328_DIO_Init(unsigned char *iConfig, unsigned char *iState) {
	ZDX_AT328_DIO_Write(*iState); // before live
	DDRB = (*iConfig) & 0x3;
	DDRD = (*iConfig) & 0xfc;
}
void ZDX_AT328_DIO_Main(void) {
	ZDX_AT328_DIO_Init(&gBuffer[1], &gBuffer[2]);
	unsigned char lConfig = gBuffer[1];
	while(0x1) {if (UCSR0A & 0x80) {ZDX_AT328_DIO_Write(lConfig & UDR0);}}
}
void ZDX_AT328_MainInit(void) {
	UBRR0L = 0x0;
	UBRR0H = 0x0;
	UCSR0A = 0x2;
	UCSR0B = 0x18;
	UCSR0C = 0x6;
}
unsigned char ZDX_AT328_MainMode(void) {
	unsigned char lMode = 0xff;
	unsigned char lWait = 0x1;
	unsigned char lReceived = 0;
	while (lWait) {
		if (UCSR0A & 0x80) {
			gBuffer[lReceived++] = UDR0;
			if (lReceived == 1) {lMode = gBuffer[0];}
			switch (lMode) {
				case ZDX_TASK_DIO: lWait = lReceived < 3; break;
				case ZDX_TASK_PWM: lWait = lReceived < 6; break;
				case ZDX_TASK_ADC: lWait = lReceived < 9; break;
				default: lWait = 0x0; break;
			}
		} else {
			__asm__("nop\n\t");
		}
	}
	return lMode;
}
int main() {
	ZDX_AT328_MainInit();
	switch (ZDX_AT328_MainMode()) {
		case ZDX_TASK_DIO: ZDX_AT328_DIO_Main(); break;
		case ZDX_TASK_PWM: ZDX_AT328_PWM_Main(); break;
		case ZDX_TASK_ADC: ZDX_AT328_ADC_Main(); break;
		default: break;
	}
	return 0x0;
}
