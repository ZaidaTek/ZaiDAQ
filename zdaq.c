/*** Copyright (C) 2025 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: Creative Commons Attribution-NoDerivatives 4.0 International Public License
**** For full license text, please visit: https://creativecommons.org/licenses/by-nd/4.0/legalcode
***/

#include <ZDX.h>

#define ZDQ_TITLE "zdaq"
#define ZDQ_VERSION "250906"

#define ZDQ_FORMAT_NONE 0x0
#define ZDQ_FORMAT_FLOAT 0x1 // TODO
#define ZDQ_FORMAT_BASE2 0x2
#define ZDQ_FORMAT_BASE10 0xa
#define ZDQ_FORMAT_BASE16 0x10
#define ZDQ_FILTER_NONE 0x0
#define ZDQ_FILTER_HEAD 0x1
#define ZDQ_FILTER_TAIL 0x100

#define ZDQ_DEFAULT_SYSTEM_SLEEP 4
#define ZDQ_DEFAULT_SYSTEM_PRINT 125
#define ZDQ_DEFAULT_DEVICE_TASK ZDX_TASK_ADC
#define ZDQ_DEFAULT_DEVICE_CONFIG 0xff
#define ZDQ_DEFAULT_DEVICE_BUFFER 131072
#define ZDQ_DEFAULT_DEVICE_RATE 1600
#define ZDQ_DEFAULT_DEVICE_SAMPLES (10 * ZDQ_DEFAULT_DEVICE_RATE / 100)
#define ZDQ_DEFAULT_SAMPLE_FORMAT ZDQ_FORMAT_BASE10
#define ZDQ_DEFAULT_SAMPLE_FILTER (ZDQ_FILTER_HEAD | ZDQ_FILTER_TAIL)
#ifdef ZTL_BUILD_WINDOWS
#define ZDQ_DEFAULT_PRINT_DELIMIT "\t"
#define ZDQ_DEFAULT_PRINT_NEWLINE "\r\n"
#else
#define ZDQ_DEFAULT_PRINT_DELIMIT "\t"
#define ZDQ_DEFAULT_PRINT_NEWLINE "\n"
#endif // OS Multiplexer

ZT_TIME gUserSleep;
ZT_TIME gUserPrint;
const ZT_CHAR *gUserDevice;
const char *gUser_Delimit;
ZT_FLAG gUserFormat;
ZT_FLAG gUserConfig;
ZT_FLAG gUserTask;
ZT_FLAG gUserFilter;
ZT_INDEX gUserRate;
ZT_INDEX gUserBuffer;
ZT_INDEX gUserSample;
ZT_WEAVE gUserWeave;
ZT_CHAR gUserData[4096]; // line cache
const ZT_CHAR* gUserPrintNL;
const ZT_CHAR* gUserPrintTab;

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#define ZDQ_ERR_NONE 0
#define ZDQ_ERR_UNKNOWN 1
#define ZDQ_ERR_SYS_DEV 2
#define ZDQ_ERR_SYS_MMD 3
#define ZDQ_ERR_SYS_MMB 4
#define ZDQ_ERR_SYS_CON 5
#define ZDQ_ERR_USR_UNK 6
#define ZDQ_ERR_USR_PRM 7
#define ZDQ_ERR_USR_DEV 8
#define ZDQ_ERR_LAST ZDQ_ERR_USR_DEV
#define ZDQ_ERR_COUNT ZDQ_ERR_LAST
const char *gSys_MessageError[ZDQ_ERR_COUNT] = {
	"err: unknown",
	"err: device, non-existent",
	"err: memory, allocation device",
	"err: memory, allocation buffer",
	"err: device, connection",
	"err: option, unknown",
	"err: option, requires parameter",
	"err: option, device unspecified",
};
#define ZDQ_WARN_NONE 0
#define ZDQ_WARN_UNKNOWN 1
#define ZDQ_WARN_QUIT 2
#define ZDQ_WARN_NOTASK 3
#define ZDQ_WARN_VERSION 4
#define ZDQ_WARN_HELP 5
#define ZDQ_WARN_LAST ZDQ_WARN_HELP
#define ZDQ_WARN_COUNT ZDQ_WARN_LAST
const char *gSys_MessageWarning[ZDQ_WARN_COUNT] = {
	"wrn: unknown",
	"wrn: interrupt",
	"wrn: no task specified",
	ZDQ_TITLE "-v" ZDQ_VERSION,
	"\
# Message-of-this-Version [" ZDQ_TITLE "-v" ZDQ_VERSION "]:\n\
##> \"Guten Tag!\" -- 'I.L.O.'\n\
# Usage:\n\
zdaq [options] <device>\n\
# Options:     ## note: '#'-commented options/values are unimplemented, but reserved\n\
main:\n\
 -v            ## system # version\n\
 -h            ## system # help\n\
 -T <uint,hex> ## device # task, (#0x10DII, #0x20: DIO, #0x21: PWM), 0x40: ADC, (#0x80: DAC) # def: 0x40\n\
 -c <uint,hex> ## device # config # def: 0xff == a0-a7\n\
 -r <uint,dec> ## device # rate/0.01Hz # def: 1600 == 16Hz\n\
 -l <uint,dec> ## sample # count, 0: continuous # def: 160\n\
more:\n\
 -e <uint,dec> ## system # idle/ms, 0: disable # def: 4\n\
 -b <uint,dec> ## system # flush/ms, 0: always # def: 125\n\
 -f <uint,hex> ## sample # format, 0x0: u32, (#0x1: flt), 0x2: bs2, 0xa: bs10, 0x10: bs16 # def: 0xa\n\
 -i <uint,hex> ## sample # filter, 0x1: head, 0x100: tail # def: 0x101 == head,tail\n\
 -n <str>      ## sample # newline # def: \"\\n\" [Windows: \"\\r\\n\"]\n\
 -d <str>      ## sample # delimit # def: \"\\t\"\n\
todo:\n\
#-u <uint,hex> ## device # unit/type, 0x100: AT328 # def: 0x100\n\
#-w <uint,dec> ## device # grace/ms # def: 3000\n\
#-M            ## sample # map samples to device ports, zero-fill unsampled channels\n\
#-O <str>      ## fscale # offset, matching no. of CSVs # def: \"0.0,0.0,...,0.0\"\n\
#-A <str>      ## fscale # factor, matching no. of CSVs # def: \"1.0,1.0,...,1.0\"\n\
# Examples:\n\
zdaq -c 0x8b -r 33333 -l 2001 '/dev/ttyUSB0'\n\
##> Reads 2001 samples on 4x channels a0,a1,a3,a7 at 333.33 Hz rate from '/dev/ttyUSB0'\n\
zdaq '/dev/ttyUSB0' > 'samples.tsv'\n\
##> Reads samples from '/dev/ttyUSB0' using default settings, which are written into 'samples.tsv'\n\
zdaq -d \",\" \"/dev/ttyUSB0\" > 'samples.csv'\n\
##> Same as previous, but uses comma- instead of tab-separation\n\
zdaq -d \",\" -n $'\\r\\n' \"/dev/ttyUSB0\" > \"samples.csv\"\n\
##> Same as previous, but uses CRLF instead of LF for newline-feeds\n\
zdaq -l 0 \"/dev/ttyUSB0\"\n\
##> Continous sampling\n\
zdaq \"\\\\.\\COM3\" \n\
##> [Windows:] Open device 'COM3' with default settings\n\
# Notes:\n\
##> head-filtering discards first ADC-sample\n\
##> tail-filtering discards trailing buffer samples\n\
##> filtering is disabled in continuous sampling\n\
##> host system does not always honor flush delay\n\
##> SIGINT/Ctrl+C is gracefully handled\n\
##> there is a 3s connection establishment delay\
"
};
unsigned int volatile gSys_NoHalt;
unsigned int volatile gSys_Error;
unsigned int volatile gSys_Warning;
void gSys_Interrupt(int iSignal) {(void)iSignal; gSys_NoHalt = 0x0; gSys_Warning = ZDQ_WARN_QUIT;};

#ifdef ZTL_BUILD_WINDOWS
#define ZDQ_DevicePathValid(iAddress) (0x1)
#else
#define ZDQ_DevicePathValid(iAddress) (ZTL_FileInfo(iAddress)->path != NULL)
#endif // OS Multiplexer

int main(int iArgC, char **iArgV) {
	// INIT
	gSys_NoHalt = 0x1;
	gSys_Error = ZDQ_ERR_NONE;
	gSys_Warning = ZDQ_WARN_NONE;
	
	gUserFormat = ZDQ_DEFAULT_SAMPLE_FORMAT;
	gUserSleep = ZDQ_DEFAULT_SYSTEM_SLEEP;
	gUserPrint = ZDQ_DEFAULT_SYSTEM_PRINT;
	
	gUserDevice = NULL;
	gUserTask = ZDQ_DEFAULT_DEVICE_TASK;
	gUserConfig = ZDQ_DEFAULT_DEVICE_CONFIG;
	gUserFilter = ZDQ_DEFAULT_SAMPLE_FILTER;
	gUserRate = ZDQ_DEFAULT_DEVICE_RATE;
	gUserBuffer = ZDQ_DEFAULT_DEVICE_BUFFER;
	gUserSample = ZDQ_DEFAULT_DEVICE_SAMPLES;
	
	gUserPrintNL = (const ZT_CHAR*)ZDQ_DEFAULT_PRINT_NEWLINE;
	gUserPrintTab = (const ZT_CHAR*)ZDQ_DEFAULT_PRINT_DELIMIT;
	
	ZTM8_Zero(&gUserWeave, sizeof(gUserWeave));
	gUserWeave.data.ptr = (void*)gUserData;
	gUserWeave.capacity = sizeof(gUserData);
	ZWV_Set(&gUserWeave);
	// SYSTEM
	setvbuf(stdout, NULL, _IONBF, 0);
	signal(SIGINT, gSys_Interrupt);
	opterr = 0;
	for (int lFlag; (lFlag = getopt(iArgC, iArgV, "vhc:r:l:i:e:b:n:d:f:T:")) != -1;) {
		switch (lFlag) {
			case 'v': gSys_Warning = ZDQ_WARN_VERSION; break;
			case 'h': gSys_Warning = ZDQ_WARN_HELP; break;
			case 'T': gUserTask = strtoul(optarg, NULL, 16); break;
			case 'c': gUserConfig = strtoul(optarg, NULL, 16); break;
			case 'r': gUserRate = strtoul(optarg, NULL, 10); break;
			case 'l': gUserSample = strtoul(optarg, NULL, 10); break;
			case 'e': gUserSleep = strtoul(optarg, NULL, 10); break;
			case 'b': gUserPrint = strtoul(optarg, NULL, 10); break;
			case 'f': gUserFormat = strtoul(optarg, NULL, 16); break;
			case 'i': gUserFilter = strtoul(optarg, NULL, 16); break;
			case 'n': gUserPrintNL = (const ZT_CHAR*)optarg; break;
			case 'd': gUserPrintTab = (const ZT_CHAR*)optarg; break;
			case '?':
				if (optopt == 'c' || optopt == 'r' || optopt == 'l' || optopt == 'i' || optopt == 'e' || optopt == 'b' || optopt == 'n' || optopt == 'd' || optopt == 'f' || optopt == 'T') {
					gSys_Error = ZDQ_ERR_USR_PRM;
				} else {
					gSys_Error = ZDQ_ERR_USR_UNK;
				}
				break;
			default:
				gSys_Error = ZDQ_ERR_UNKNOWN;
				break;
		}
	}
	// EXEC
	if (!gSys_Error && !(gSys_Warning == ZDQ_WARN_HELP || gSys_Warning == ZDQ_WARN_VERSION)) {
		if (optind < iArgC) {
			gUserDevice = (const ZT_CHAR*)iArgV[optind];
			if (ZDQ_DevicePathValid(gUserDevice)) {
				ZDX_DEVICE* lDevice;
				if ((lDevice = ZDX_New(gUserDevice, ZDX_DEVICE_TYPE_AT328P)) != NULL) {
					switch (gUserTask) {
						case ZDX_TASK_ADC: {
								ZDX_Assign(lDevice, ZDX_TASK_ADC, gUserConfig, gUserRate);
								lDevice->flag |= ZDX_DEVICE_FLAG_UNBUFFERED;
								ZDX_DATA* lBuffer;
								if ((lBuffer = ZDX_DataNew(lDevice, gUserBuffer)) != NULL) {
									ZDX_Connect(lDevice);
									if ((gSys_Error = (lDevice->interface.runtime != NULL) ? ZDQ_ERR_NONE : ZDQ_ERR_SYS_CON) != ZDQ_ERR_SYS_CON) {
										ZT_INDEX lAcquire = gUserSample;
										ZT_INDEX lChannels = ZTM_BitCount(gUserConfig);
										ZT_FLAG lRead = ZDQ_FILTER_HEAD;
										ZT_INDEX lCount = 0;
										ZT_TIME lNow = ZTL_Tick();
										ZT_TIME lPrint = lNow - gUserPrint;
										void (*lFormat)(ZT_U) = (gUserFormat == ZDQ_FORMAT_BASE16) ? &ZWV_AddHex : ((gUserFormat == ZDQ_FORMAT_BASE2) ? &ZWV_AddBin : &ZWV_AddUnsigned);
										do {
											lNow = ZTL_Tick();
											ZT_INDEX lCursorR = ZDX_DataGetCursor(lBuffer);
											if (ZDX_Read(lDevice, lBuffer)) {
												ZT_INDEX lCursorW = ZDX_DataGetCursor(lBuffer);
												while (lCursorR != lCursorW) {
													lRead |= lAcquire ? 0x0 : ZDQ_FILTER_TAIL;
													if (!gUserSample || !(lRead & gUserFilter)) {
														if (lAcquire) {--lAcquire;};
														ZT_U* lLine = ZDX_DataGetLine(lBuffer, lCursorR);
														if (gUserFormat) {
															ZWV_Empty();
															for (ZT_INDEX i = 0; i < lChannels; ++i) {if (i) {ZWV_Add(gUserPrintTab);} lFormat(lLine[i]);}
															ZWV_Add(gUserPrintNL);
															fwrite(gUserWeave.data.ptr, sizeof(ZT_CHAR), gUserWeave.data.length, stdout);
														} else {
															fwrite(lLine, sizeof(ZT_U), lChannels, stdout);
														}
													}
													if (lRead & ZDQ_FILTER_HEAD) {lRead &= ~ZDQ_FILTER_HEAD;};
													++lCount;
													++lCursorR;
													lCursorR %= lBuffer->block.yU;
												}
											} else if (gUserSleep) {
												ZTL_Sleep(gUserSleep);
											}
											if (!gSys_NoHalt || !(lNow - lPrint < gUserPrint)) {lPrint = lNow; fflush(stdout);}
										} while (gSys_NoHalt && (!gUserSample || lAcquire));
									}
									ZDX_DataFree(lBuffer);
								} else {
									gSys_Error = ZDQ_ERR_SYS_MMB;
								}
							}
							break;
						default:
							gSys_Warning = ZDQ_WARN_NOTASK;
							break;
					}
					ZDX_Free(lDevice);
				} else {
					gSys_Error = ZDQ_ERR_SYS_MMD;
				}
			} else {
				gSys_Error = ZDQ_ERR_SYS_DEV;
			}
		} else {
			gSys_Error = ZDQ_ERR_USR_DEV;
		}
	}
	// DIAG
	if (gSys_Warning) {fprintf((gSys_Warning == ZDQ_WARN_VERSION || gSys_Warning == ZDQ_WARN_HELP) ? stdout : stderr, "%s\n", gSys_MessageWarning[gSys_Warning - 1]);}
	if (gSys_Error) {fprintf(stderr, "%s\n", gSys_MessageError[gSys_Error - 1]);}
	// EXIT
    return gSys_Error;
}
