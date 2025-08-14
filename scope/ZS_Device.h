/*** Copyright (C) 2019-2021 ZaidaTek and Andreas Riebesehl
**** This work is licensed under: Creative Commons Attribution-NoDerivatives 4.0 International Public License
**** For full license text, please visit: https://creativecommons.org/licenses/by-nd/4.0/legalcode
***/
#ifndef ZS_DEVICE_H_INCLUDED
#define ZS_DEVICE_H_INCLUDED

#include "ZS.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void ZSDEV_Capture(ZT_FLAG iState);
void ZSDEV_Record(ZT_FLAG iState);
void ZSDEV_Read(void);
void ZSDEV_LoadGUI(void);
void ZSDEV_FreeHardware(void);
void ZSDEV_FreeData(void);
void ZSDEV_Free(void);
void ZSDEV_Connect(const ZT_CHAR* iAddress, ZT_INDEX iSpeed, ZT_FLAG iChannels);
void ZSDEV_Disconnect(void);
void ZSDEV_TriggerInit(void);
void ZSDEV_TriggerLevel(ZT_I iIncrement);
void ZSDEV_TriggerFlank(ZT_FLAG iType);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif //ZS_DEVICE_H_INCLUDED

