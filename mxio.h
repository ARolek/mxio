/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.

    2013/4/11	Daniel Chang
    update
*/
//----------------------------------------------
#ifndef _MXIO_H
#define _MXIO_H
//----------------------------------------------
	//#include "StdAfx.h"
	//------------------------------------------
	#ifdef WINDOWS_TYPE
		#ifdef MXIO_DLL
			#ifndef MXIOEXPORT
    			#define MXIOEXPORT __declspec(dllexport)
    		#endif
	    #else
	    	#ifndef MXIOEXPORT
	    		#define MXIOEXPORT __declspec(dllimport)
	    	#endif
	    #endif
	    #define G_PACKED
	#else
		#define WORD unsigned short
	    #define DWORD unsigned int
	    #define LONG long
	    #define BYTE unsigned char
	    #define UINT unsigned int
	    #define MXIOEXPORT
	    #define CALLBACK
	    #define G_PACKED __attribute__ ((packed))
	#endif
	//
	#ifndef UINT8
		#define UINT8 unsigned char
	#endif
	#ifndef UINT16
		#define UINT16 unsigned short
	#endif
	#ifndef UINT32
		#define UINT32 unsigned int
	#endif

	//------------------------------------------
	#ifdef __cplusplus
		extern "C" {
	#endif
	//------------------------------------------
	#pragma pack(1)
	// ========================
	#define 	SUPPORT_MAX_SLOT		16
	#define 	SUPPORT_MAX_CHANNEL		64
//	const int SupportMaxChOfBit	= SUPPORT_MAX_CHANNEL>>3;
        #define         SupportMaxChOfBit SUPPORT_MAX_CHANNEL>>3
	//
	typedef union _ANALOG_VAL{
		float fVal;
		UINT32 iVal;
		UINT8 BytVal[4];
	} _ANALOG_VAL;
	//V1.2 OPC Tag DATA Struct
	typedef struct _IOLOGIKSTRUCT
    { 	//
        UINT8  BytMagic[4];  	// "DAC?"						//
        UINT16 wVersion;     	// 0x0100						//struct define of version 1.0.0
        UINT16 wLength;     	// nLength«áÄòBytes¼Æ¡A§tCRC	//
        UINT16 wHWID;											//for user to know which API to Read/write

        UINT32 dwSrcIP;
        UINT8 BytSrcMAC[6];
        UINT8 BytMsgType; 		// for AP side to known what kind of Message return
        UINT16 wMsgSubType;		//
        //------------------------
        UINT16 wYear; 			// tag timestamp
        UINT8 BytMonth;
        UINT8 BytDay;
        UINT8 BytHour;
        UINT8 BytMin;
        UINT8 BytSec;
        UINT16 wMSec;
        //-------------------------
        UINT8 BytLastSlot;		//add to notice the last slot, Range 0-16, 0=>myself only
        UINT8 BytLastCh[SUPPORT_MAX_SLOT];    	//add to notice the last Channel, Range 0-64, for each slot
        //-------------------------
        // support up to 16 slots and 64 channels //size:5408 bytes
        UINT8 BytChType[SUPPORT_MAX_SLOT][SUPPORT_MAX_CHANNEL] ;	  	// channel I/O type
        UINT16 wSlotID[SUPPORT_MAX_SLOT];								// Slot Module ID
        UINT8 BytChNumber[SUPPORT_MAX_SLOT][SupportMaxChOfBit] ;	   	// bitwised¡A1=Enable¡A0=Disable
        _ANALOG_VAL dwChValue[SUPPORT_MAX_SLOT][SUPPORT_MAX_CHANNEL] ;
        UINT8 BytChLocked[SUPPORT_MAX_SLOT][SupportMaxChOfBit] ;		// bitwised, 0=free, 1=locked
    	//-------------------------
        //UINT8 BytCrc;											//mark
    } G_PACKED IOLOGIKSTRUCT, *pIOLOGIKSTRUCT;
	//
	typedef struct _MODULE_LIST{
    	WORD nModuleID;     //Module ID of device
    	char szModuleIP[16];	//Save IP address of device
    	char szMAC[6];  	//Save MAC address of device
    	char szModuleIP1[16];	//Save 2nd IP address of device
    	char szMAC1[6];  	//Save 2nd MAC address of device
    	char bytLanUse;			//0 -> Lan0, 1 -> Lan1
	} G_PACKED MODULE_LIST, *p_MODULE_LIST;
	//
	typedef struct _ACTDEV_IO{
		//bool bLink;		//for check current link is not alive
		//int iIndex;		//for MAC address index of device list
		char szMAC[6];  //Save MAC address of device
		int iHandle;	//save Connection Handle
	}G_PACKED ACTDEV_IO, *pACTDEV_IO;

	//For W5340
    typedef struct _W5340_INFO{
    	char  szIP[16];		//"192.168.190.123"
    	char  szMACAddr[18];//"00-1C-25-E6-B6-2A"
    	BYTE bytStatus;
    }G_PACKED W5340_INFO, *pW5340_INFO;
    //
    #define AOPC_DEV_INFO	W5340_INFO
    #define pAOPC_DEV_INFO	pW5340_INFO

	#pragma pack()
	//------------------------------------------
	typedef void (CALLBACK * pfnCALLBACK)( BYTE bytData[], WORD wSize );
	typedef void (CALLBACK * pfnTagDataCALLBACK)( IOLOGIKSTRUCT ios[], WORD* wMutex);

	/*************************************************/
	/*                                               */
	/*     RS232 & RS485  Module Connect Command     */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK MXSIO_OpenCommport(
		char *	szCommport,
		DWORD	dwBaudrate,
		BYTE	bytDataFormat,
	    DWORD	dwTimeout,
		int *	hCommport );

	MXIOEXPORT int CALLBACK MXSIO_CloseCommport(
		int		hCommport );

	MXIOEXPORT int CALLBACK MXSIO_Connect(
		int		hCommport,
		BYTE	bytUnitID,
		BYTE	bytTransmissionMode,
		int *	hConnection);

	MXIOEXPORT int CALLBACK MXSIO_Disconnect(
		int		hConnection);

	/*************************************************/
	/*                                               */
	/*       Ethernet Module Connect Command         */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK MXEIO_Init();

	MXIOEXPORT void CALLBACK MXEIO_Exit();

	MXIOEXPORT int CALLBACK MXEIO_Connect(
		char *	szIP,
		WORD	wPort,
		DWORD	dwTimeOut,
		int *	hConnection );

	MXIOEXPORT int CALLBACK MXEIO_Disconnect(
		int		hConnection );

	MXIOEXPORT int CALLBACK MXEIO_CheckConnection(
		int		hConnection,
		DWORD	dwTimeOut,
		BYTE *	bytStatus );

	/*************************************************/
	/*                                               */
	/*                General Command                */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK MXIO_GetDllVersion ( );

	MXIOEXPORT int CALLBACK MXIO_GetDllBuildDate ( );

	MXIOEXPORT int CALLBACK MXIO_GetModuleType(
		int		hConnection,
		BYTE	bytSlot,
		WORD *	wType );

	MXIOEXPORT int CALLBACK MXIO_ReadFirmwareRevision (
		int		hConnection,
		BYTE	bytRevision[] );

	MXIOEXPORT int CALLBACK MXIO_ReadFirmwareDate(
		int		hConnection,
		WORD	wDate[] );

	MXIOEXPORT int CALLBACK MXIO_Restart(
		int		hConnection );

	MXIOEXPORT int CALLBACK MXIO_Reset(
		int		hConnection );
	/*************************************************/
	/*                                               */
	/*                Modbus Command                 */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK MXIO_ReadCoils(
		int		hConnection,
		BYTE	bytCoilType,
		WORD	wStartCoil,
		WORD	wCount,
		BYTE	bytCoils[] );

	MXIOEXPORT int CALLBACK MXIO_WriteCoils(
		int		hConnection,
		WORD	wStartCoil,
		WORD	wCount,
		BYTE	bytCoils[] );

	MXIOEXPORT int CALLBACK MXIO_ReadRegs(
		int		hConnection,
		BYTE	bytRegisterType,
		WORD	wStartRegister,
		WORD	wCount,
		WORD	wRegister[] );

	MXIOEXPORT int CALLBACK MXIO_WriteRegs(
		int		hConnection,
		WORD	wStartRegister,
		WORD	wCount,
		WORD	wRegister[] );

	/*************************************************/
	/*                                               */
	/*             Digital Input Command             */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK DI_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK DI_Read(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		BYTE *	bytValue );

	/*************************************************/
	/*                                               */
	/*            Digital Output Command             */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK DO_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK DO_Writes(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK DO_Read(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK DO_Write(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		BYTE	bytValue );

	MXIOEXPORT int CALLBACK DO_GetSafeValues(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK DO_SetSafeValues(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK DO_GetSafeValue(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK DO_SetSafeValue(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		BYTE	bytValue );

	//MXIOEXPORT int CALLBACK DO_RLY_GetSafeValue(
	//	int		hConnection,
	//	BYTE	bytSlot,
	//	BYTE	bytChannel,
	//	WORD *	wValue );
	//
	//MXIOEXPORT int CALLBACK DO_RLY_SetSafeValue(
	//	int		hConnection,
	//	BYTE	bytSlot,
	//	BYTE	bytChannel,
	//	WORD	wValue );
	//
	MXIOEXPORT int CALLBACK DO_GetSafeValues_W(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValues[]);

	MXIOEXPORT int CALLBACK DO_SetSafeValues_W(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValues[]);

	/*************************************************/
	/*                                               */
	/*             Analog Input Command              */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK AI_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AI_Read(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		double *dValue );

	MXIOEXPORT int CALLBACK AI_ReadRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AI_ReadRaw(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD *	wValue );

	/*************************************************/
	/*                                               */
	/*             Analog Output Command             */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK AO_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AO_Writes(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AO_Read(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		double *dValue );

	MXIOEXPORT int CALLBACK AO_Write(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		double  dValue );

	MXIOEXPORT int CALLBACK AO_ReadRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AO_WriteRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AO_ReadRaw(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD *	wValue );

	MXIOEXPORT int CALLBACK AO_WriteRaw(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD	wValue );

	MXIOEXPORT int CALLBACK AO_GetSafeValues(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AO_SetSafeValues(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AO_GetSafeValue(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		double *dValue );

	MXIOEXPORT int CALLBACK AO_SetSafeValue(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		double  dValue );

	MXIOEXPORT int CALLBACK AO_GetSafeRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AO_SetSafeRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AO_GetSafeRaw(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD *	wValue );

	MXIOEXPORT int CALLBACK AO_SetSafeRaw(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD	wValue );

	/*************************************************/
	/*                                               */
	/*                  RTD Command                  */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK RTD_Read(
		int hConnection,
		BYTE bytSlot,
		BYTE bytChannel,
		double * dValue ) ;

	MXIOEXPORT int CALLBACK RTD_Reads(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK RTD_ReadRaw(
		int hConnection,
		BYTE bytSlot,
		BYTE bytChannel,
		WORD * wValue );

	MXIOEXPORT int CALLBACK RTD_ReadRaws(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	/*************************************************/
	/*                                               */
	/*     ioLogik 2000 special Adapter command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK RTD2K_ResetMin(
		int hConnection,
		BYTE bytChannel );

	MXIOEXPORT int CALLBACK RTD2K_ResetMins(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount);

	MXIOEXPORT int CALLBACK RTD2K_ResetMax(
		int hConnection,
		BYTE bytChannel );

	MXIOEXPORT int CALLBACK RTD2K_ResetMaxs(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount);

	MXIOEXPORT int CALLBACK RTD2K_GetChannelStatus(
		int hConnection,
		BYTE bytChannel,
		BYTE * bytStatus );

	MXIOEXPORT int CALLBACK RTD2K_SetChannelStatus(
		int hConnection,
		BYTE bytChannel,
		BYTE bytStatus );

	MXIOEXPORT int CALLBACK RTD2K_GetChannelStatuses(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD * dwStatus );

	MXIOEXPORT int CALLBACK RTD2K_SetChannelStatuses(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwStatus );

	MXIOEXPORT int CALLBACK RTD2K_GetEngUnit(
		int hConnection,
		BYTE bytChannel,
		WORD * wEngUnit );

	MXIOEXPORT int CALLBACK RTD2K_SetEngUnit(
		int hConnection,
		BYTE bytChannel,
		WORD wEngUnit );

	MXIOEXPORT int CALLBACK RTD2K_GetEngUnits(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wEngUnit[] );

	MXIOEXPORT int CALLBACK RTD2K_SetEngUnits(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wEngUnit[] );

	MXIOEXPORT int CALLBACK RTD2K_GetSensorType(
		int hConnection,
		BYTE bytChannel,
		WORD * wSensorType );

	MXIOEXPORT int CALLBACK RTD2K_SetSensorType(
		int hConnection,
		BYTE bytChannel,
		WORD wSensorType );

	MXIOEXPORT int CALLBACK RTD2K_GetSensorTypes(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wSensorType[] );

	MXIOEXPORT int CALLBACK RTD2K_SetSensorTypes(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wSensorType[] );

	MXIOEXPORT int CALLBACK RTD2K_GetMathPar(
		int hConnection,
		BYTE bytChannel,
		WORD * wMathPar );

	MXIOEXPORT int CALLBACK RTD2K_SetMathPar(
		int hConnection,
		BYTE bytChannel,
		WORD wMathPar );

	MXIOEXPORT int CALLBACK RTD2K_GetMathPars(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wMathPar[] );

	MXIOEXPORT int CALLBACK RTD2K_SetMathPars(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wMathPar[] );

	MXIOEXPORT int CALLBACK RTD2K_ReadMinRaw(
		int hConnection,
		BYTE bytChannel,
		WORD * wValue );

	MXIOEXPORT int CALLBACK RTD2K_ReadMinRaws(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK RTD2K_ReadMaxRaw(
		int hConnection,
		BYTE bytChannel,
		WORD * wValue );

	MXIOEXPORT int CALLBACK RTD2K_ReadMaxRaws(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK RTD2K_ReadMin(
		int hConnection,
		BYTE bytChannel,
		double * dValue ) ;

	MXIOEXPORT int CALLBACK RTD2K_ReadMins(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK RTD2K_ReadMax(
		int hConnection,
		BYTE bytChannel,
		double * dValue ) ;

	MXIOEXPORT int CALLBACK RTD2K_ReadMaxs(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK RTD2K_SetChnAvg(
		int hConnection,
		BYTE bytChannel,
		BYTE bytChnIdx[],
		BYTE bytChCount );

	MXIOEXPORT int CALLBACK RTD2K_SetChnDev(
		int hConnection,
		BYTE bytChannel,
		BYTE bytChMinued,
		BYTE bytChSub  );

	/*************************************************/
	/*                                               */
	/*                  TC Command                  */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK TC_Read(
		int hConnection,
		BYTE bytSlot,
		BYTE bytChannel,
		double * dValue ) ;

	MXIOEXPORT int CALLBACK TC_Reads(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK TC_ReadRaw(
		int hConnection,
		BYTE bytSlot,
		BYTE bytChannel,
		WORD * wValue );

	MXIOEXPORT int CALLBACK TC_ReadRaws(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	/*************************************************/
	/*                                               */
	/*     ioLogik 2000 special Adapter command      */
	/*                                               */
	/*************************************************/
	//MXIOEXPORT int CALLBACK TC2K_Read(
	//	int hConnection,
	//	BYTE bytSlot,
	//	BYTE bytChannel,
	//	double * dValue ) ;
	//
	//MXIOEXPORT int CALLBACK TC2K_Reads(
	//	int hConnection,
	//	BYTE bytSlot,
	//	BYTE bytStartChannel,
	//	BYTE bytCount,
	//	double dValue[] ) ;

	MXIOEXPORT int CALLBACK TC2K_ReadRaw(
		int hConnection,
		BYTE bytChannel,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK TC2K_ReadRaws(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[] );

	MXIOEXPORT int CALLBACK TC2K_ResetMin(
		int hConnection,
		BYTE bytChannel );

	MXIOEXPORT int CALLBACK TC2K_ResetMins(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount);

	MXIOEXPORT int CALLBACK TC2K_ResetMax(
		int hConnection,
		BYTE bytChannel );

	MXIOEXPORT int CALLBACK TC2K_ResetMaxs(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount);

	MXIOEXPORT int CALLBACK TC2K_GetChannelStatus(
		int hConnection,
		BYTE bytChannel,
		BYTE * bytStatus );

	MXIOEXPORT int CALLBACK TC2K_SetChannelStatus(
		int hConnection,
		BYTE bytChannel,
		BYTE bytStatus );

	MXIOEXPORT int CALLBACK TC2K_GetChannelStatuses(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD * dwStatus );

	MXIOEXPORT int CALLBACK TC2K_SetChannelStatuses(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwStatus );

	MXIOEXPORT int CALLBACK TC2K_GetEngUnit(
		int hConnection,
		BYTE bytChannel,
		WORD * wEngUnit );

	MXIOEXPORT int CALLBACK TC2K_SetEngUnit(
		int hConnection,
		BYTE bytChannel,
		WORD wEngUnit );

	MXIOEXPORT int CALLBACK TC2K_GetEngUnits(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wEngUnit[] );

	MXIOEXPORT int CALLBACK TC2K_SetEngUnits(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wEngUnit[] );

	MXIOEXPORT int CALLBACK TC2K_GetSensorType(
		int hConnection,
		BYTE bytChannel,
		WORD * wSensorType );

	MXIOEXPORT int CALLBACK TC2K_SetSensorType(
		int hConnection,
		BYTE bytChannel,
		WORD wSensorType );

	MXIOEXPORT int CALLBACK TC2K_GetSensorTypes(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wSensorType[] );

	MXIOEXPORT int CALLBACK TC2K_SetSensorTypes(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wSensorType[] );

	MXIOEXPORT int CALLBACK TC2K_GetMathPar(
		int hConnection,
		BYTE bytChannel,
		WORD * wMathPar );

	MXIOEXPORT int CALLBACK TC2K_SetMathPar(
		int hConnection,
		BYTE bytChannel,
		WORD wMathPar );

	MXIOEXPORT int CALLBACK TC2K_GetMathPars(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wMathPar[] );

	MXIOEXPORT int CALLBACK TC2K_SetMathPars(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wMathPar[] );

	MXIOEXPORT int CALLBACK TC2K_ReadMinRaw(
		int hConnection,
		BYTE bytChannel,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK TC2K_ReadMinRaws(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[] );

	MXIOEXPORT int CALLBACK TC2K_ReadMaxRaw(
		int hConnection,
		BYTE bytChannel,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK TC2K_ReadMaxRaws(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[] );

	MXIOEXPORT int CALLBACK TC2K_ReadMin(
		int hConnection,
		BYTE bytChannel,
		double * dValue ) ;

	MXIOEXPORT int CALLBACK TC2K_ReadMins(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK TC2K_ReadMax(
		int hConnection,
		BYTE bytChannel,
		double * dValue ) ;

	MXIOEXPORT int CALLBACK TC2K_ReadMaxs(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK TC2K_SetChnAvg(
		int hConnection,
		BYTE bytChannel,
		BYTE bytChnIdx[],
		BYTE bytChCount );

	MXIOEXPORT int CALLBACK TC2K_SetChnDev(
		int hConnection,
		BYTE bytChannel,
		BYTE bytChMinued,
		BYTE bytChSub  );

	/*************************************************/
	/*                                               */
	/*      ioLogik 2000 special Module command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK Module2K_GetSafeStatus(
		int		hConnection,
		WORD *	wStatus );

	MXIOEXPORT int CALLBACK Module2K_ClearSafeStatus(
		int		hConnection );

	MXIOEXPORT int CALLBACK Module2K_GetInternalReg(
		int 	hConnection,
		BYTE    bytChannel,
		WORD *  wValue );

	MXIOEXPORT int CALLBACK Module2K_SetInternalReg(
		int 	hConnection,
		BYTE    bytChannel,
		WORD    wValue );

	MXIOEXPORT int CALLBACK Module2K_GetInternalRegs(
		int 	hConnection,
		BYTE    bytStartChannel,
		BYTE 	bytCount,
		WORD    wValue[] );

	MXIOEXPORT int CALLBACK Module2K_SetInternalRegs(
		int 	hConnection,
		BYTE    bytStartChannel,
		BYTE 	bytCount,
		WORD    wValue[] );

	/*************************************************/
	/*                                               */
	/*     ioLogik 4000 special Adapter command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK Adp4K_ReadStatus(
		int		hConnection,
		WORD *	wBusStatus,
		WORD *	wFPStatus,
		WORD *	wEWStatus,
		WORD *	wESStatus,
		WORD *	wECStatus );

	MXIOEXPORT int CALLBACK Adp4K_ClearStatus(
		int		hConnection );

	MXIOEXPORT int CALLBACK Adp4K_ReadFirmwareRevision(
		int		hConnection,
		WORD *	wRevision );

	MXIOEXPORT int CALLBACK Adp4K_ReadFirmwareDate(
		int		hConnection,
		WORD	wDate[] );

	MXIOEXPORT int CALLBACK Adp4K_ReadSlotAmount(
		int		hConnection,
		WORD *	wAmount );

	MXIOEXPORT int CALLBACK Adp4K_ReadAlarmedSlot(
		int		hConnection,
		DWORD * dwAlarm );

	/*************************************************/
	/*                                               */
	/*      ioLogik 4000 Digital output command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK DO4K_GetSafeActions(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wAction[] );

	MXIOEXPORT int CALLBACK DO4K_SetSafeActions(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wAction[] );

	MXIOEXPORT int CALLBACK DO4K_GetSafeAction(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD *	wAction );

	MXIOEXPORT int CALLBACK DO4K_SetSafeAction(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD	wAction );

	/*************************************************/
	/*                                               */
	/*      ioLogik 4000 Analog output command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK AO4K_GetSafeActions(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wAction[] );

	MXIOEXPORT int CALLBACK AO4K_SetSafeActions(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wAction[] );

	MXIOEXPORT int CALLBACK AO4K_GetSafeAction(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD *	wAction );

	MXIOEXPORT int CALLBACK AO4K_SetSafeAction(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytChannel,
		WORD	wAction );

	/*************************************************/
	/*                                               */
	/*      ioLogik 2000 Digital I/O  command        */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK DIO2K_GetIOMode(
		int hConnection,
		BYTE bytChannel,
		BYTE * bytMode );

	MXIOEXPORT int CALLBACK DIO2K_SetIOMode(
		int hConnection,
		BYTE bytChannel,
		BYTE bytMode );

	MXIOEXPORT int CALLBACK DIO2K_GetIOModes(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD * dwMode );

	MXIOEXPORT int CALLBACK DIO2K_SetIOModes(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwMode );

	/*************************************************/
	/*                                               */
	/*      ioLogik 2000 Digital Input command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK DI2K_GetModes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wMode[] );

	MXIOEXPORT int CALLBACK DI2K_SetModes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wMode[] );

	MXIOEXPORT int CALLBACK DI2K_GetMode(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wMode );

	MXIOEXPORT int CALLBACK DI2K_SetMode(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wMode );
	MXIOEXPORT int CALLBACK DI2K_GetFilters(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wFilter[] );

	MXIOEXPORT int CALLBACK DI2K_SetFilters(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wFilter[] );

	MXIOEXPORT int CALLBACK DI2K_GetFilter(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wFilter );

	MXIOEXPORT int CALLBACK DI2K_SetFilter(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wFilter );

	MXIOEXPORT int CALLBACK Cnt2K_GetSaveStatusesOnPowerFail(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwMode );

	MXIOEXPORT int CALLBACK Cnt2K_SetSaveStatusesOnPowerFail(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD   dwMode );

	/*************************************************/
	/*                                               */
	/*      ioLogik 2000 Digital output command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK DO2K_GetModes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wMode[] );

	MXIOEXPORT int CALLBACK DO2K_SetModes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wMode[] );

	MXIOEXPORT int CALLBACK DO2K_GetMode(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wMode );

	MXIOEXPORT int CALLBACK DO2K_SetMode(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wMode );

	MXIOEXPORT int CALLBACK DO2K_GetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK DO2K_SetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK DO2K_GetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK DO2K_SetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytValue );

	MXIOEXPORT int CALLBACK DO2K_GetPowerOnSeqDelaytimes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK DO2K_SetPowerOnSeqDelaytimes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	/*************************************************/
	/*                                               */
	/*         ioLogik 2000 Relay Count & Reset      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK RLY2K_GetResetTime(
		int hConnection,
		BYTE bytChannel,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK RLY2K_TotalCntRead(
		int hConnection,
		BYTE bytChannel,
		DWORD * dwValue);

	MXIOEXPORT int CALLBACK RLY2K_TotalCntReads(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[] );

	MXIOEXPORT int CALLBACK RLY2K_CurrentCntRead(
		int hConnection,
		BYTE bytChannel,
		DWORD * dwValue);

	MXIOEXPORT int CALLBACK RLY2K_CurrentCntReads(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[] );

	MXIOEXPORT int CALLBACK RLY2K_ResetCnt(
		int hConnection,
		BYTE bytChannel );

	MXIOEXPORT int CALLBACK RLY2K_ResetCnts(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount);

	/*************************************************/
	/*                                               */
	/*         ioLogik 2000 Counter command          */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK Cnt2K_Reads(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue[] );

	MXIOEXPORT int CALLBACK Cnt2K_Clears(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount );

	MXIOEXPORT int CALLBACK Cnt2K_Read(
		int		hConnection,
		BYTE	bytChannel,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK Cnt2K_Clear(
		int		hConnection,
		BYTE	bytChannel );

	MXIOEXPORT int CALLBACK Cnt2K_GetOverflows(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwStatus );

	MXIOEXPORT int CALLBACK Cnt2K_ClearOverflows(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount );

	MXIOEXPORT int CALLBACK Cnt2K_GetOverflow(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytStatus );

	MXIOEXPORT int CALLBACK Cnt2K_ClearOverflow(
		int		hConnection,
		BYTE	bytChannel );

	MXIOEXPORT int CALLBACK Cnt2K_GetFilters(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wFilter[] );

	MXIOEXPORT int CALLBACK Cnt2K_SetFilters(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wFilter[] );

	MXIOEXPORT int CALLBACK Cnt2K_GetFilter(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wFilter );

	MXIOEXPORT int CALLBACK Cnt2K_SetFilter(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wFilter );

	MXIOEXPORT int CALLBACK Cnt2K_GetStartStatuses(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwStatus );

	MXIOEXPORT int CALLBACK Cnt2K_SetStartStatuses(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwStatus );

	MXIOEXPORT int CALLBACK Cnt2K_GetStartStatus(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytStatus );

	MXIOEXPORT int CALLBACK Cnt2K_SetStartStatus(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytStatus );

	MXIOEXPORT int CALLBACK Cnt2K_GetTriggerTypes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwType );

	MXIOEXPORT int CALLBACK Cnt2K_SetTriggerTypes(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwType );

	MXIOEXPORT int CALLBACK Cnt2K_GetTriggerType(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytType );

	MXIOEXPORT int CALLBACK Cnt2K_SetTriggerType(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytType );

	MXIOEXPORT int CALLBACK Cnt2K_GetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwValue );

	MXIOEXPORT int CALLBACK Cnt2K_SetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK Cnt2K_GetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK Cnt2K_SetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytValue );

	MXIOEXPORT int CALLBACK Cnt2K_GetSafeValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwValue );

	MXIOEXPORT int CALLBACK Cnt2K_SetSafeValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK Cnt2K_GetSafeValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK Cnt2K_SetSafeValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytValue );

	MXIOEXPORT int CALLBACK Cnt2K_GetTriggerTypeWords(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD *	wType );

	MXIOEXPORT int CALLBACK Cnt2K_SetTriggerTypeWords(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD *  wType );

	MXIOEXPORT int CALLBACK Cnt2K_GetTriggerTypeWord(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wType );

	MXIOEXPORT int CALLBACK Cnt2K_SetTriggerTypeWord(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wType );
	/*************************************************/
	/*                                               */
	/*       ioLogik 2000 Pulse Output command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK Pulse2K_GetSignalWidths(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wHiWidth[],
		WORD	wLoWidth[] );

	MXIOEXPORT int CALLBACK Pulse2K_SetSignalWidths(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wHiWidth[],
		WORD	wLoWidth[] );

	MXIOEXPORT int CALLBACK Pulse2K_GetSignalWidth(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wHiWidth,
		WORD *	wLoWidth );

	MXIOEXPORT int CALLBACK Pulse2K_SetSignalWidth(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wHiWidth,
		WORD	wLoWidth );

	// 32 bit width
	MXIOEXPORT int CALLBACK Pulse2K_GetSignalWidths32(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwHiWidth[],
		DWORD	dwLoWidth[] );

	MXIOEXPORT int CALLBACK Pulse2K_SetSignalWidths32(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwHiWidth[],
		DWORD	dwLoWidth[] );

	MXIOEXPORT int CALLBACK Pulse2K_GetSignalWidth32(
		int		hConnection,
		BYTE	bytChannel,
		DWORD *	dwHiWidth,
		DWORD *	dwLoWidth );

	MXIOEXPORT int CALLBACK Pulse2K_SetSignalWidth32(
		int		hConnection,
		BYTE	bytChannel,
		DWORD	dwHiWidth,
		DWORD	dwLoWidth );


	MXIOEXPORT int CALLBACK Pulse2K_GetOutputCounts(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwOutputCounts[] );

	MXIOEXPORT int CALLBACK Pulse2K_SetOutputCounts(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwOutputCounts[] );

	MXIOEXPORT int CALLBACK Pulse2K_GetOutputCount(
		int		hConnection,
		BYTE	bytChannel,
		DWORD *	dwOutputCount );

	MXIOEXPORT int CALLBACK Pulse2K_SetOutputCount(
		int		hConnection,
		BYTE	bytChannel,
		DWORD	dwOutputCount );

	MXIOEXPORT int CALLBACK Pulse2K_GetStartStatuses(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwStatus );

	MXIOEXPORT int CALLBACK Pulse2K_SetStartStatuses(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwStatus );

	MXIOEXPORT int CALLBACK Pulse2K_GetStartStatus(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytStatus );

	MXIOEXPORT int CALLBACK Pulse2K_SetStartStatus(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytStatus );

	MXIOEXPORT int CALLBACK Pulse2K_GetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwValue );

	MXIOEXPORT int CALLBACK Pulse2K_SetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK Pulse2K_GetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK Pulse2K_SetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytValue );

	MXIOEXPORT int CALLBACK Pulse2K_GetSafeValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD *	dwValue );

	MXIOEXPORT int CALLBACK Pulse2K_SetSafeValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK Pulse2K_GetSafeValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE *	bytValue );

	MXIOEXPORT int CALLBACK Pulse2K_SetSafeValue(
		int		hConnection,
		BYTE	bytChannel,
		BYTE	bytValue );


	/*************************************************/
	/*                                               */
	/*       ioLogik 2000 Analog Input command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK AI2K_ReadMins(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AI2K_ReadMinRaws(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AI2K_ResetMins(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount);

	MXIOEXPORT int CALLBACK AI2K_ReadMin(
		int		hConnection,
		BYTE	bytChannel,
		double *dValue );

	MXIOEXPORT int CALLBACK AI2K_ReadMinRaw(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wValue );

	MXIOEXPORT int CALLBACK AI2K_ResetMin(
		int		hConnection,
		BYTE	bytChannel );

	MXIOEXPORT int CALLBACK AI2K_ReadMaxs(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AI2K_ReadMaxRaws(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AI2K_ResetMaxs(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount );

	MXIOEXPORT int CALLBACK AI2K_ReadMax(
		int		hConnection,
		BYTE	bytChannel,
		double *dValue );

	MXIOEXPORT int CALLBACK AI2K_ReadMaxRaw(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wValue );

	MXIOEXPORT int CALLBACK AI2K_ResetMax(
		int		hConnection,
		BYTE	bytChannel );

	MXIOEXPORT int CALLBACK AI2K_GetRanges(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wRange[] );

	MXIOEXPORT int CALLBACK AI2K_SetRanges(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wRange[] );

	MXIOEXPORT int CALLBACK AI2K_GetRange(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wRange );

	MXIOEXPORT int CALLBACK AI2K_SetRange(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wRange );

	MXIOEXPORT int CALLBACK AI2K_GetChannelStatus(
		int hConnection,
		BYTE bytChannel,
		 WORD * wValue );

	MXIOEXPORT int CALLBACK AI2K_SetChannelStatus(
		int hConnection,
		BYTE bytChannel,
		WORD wValue );

	MXIOEXPORT int CALLBACK AI2K_GetChannelStatuses(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD * wValue );


	MXIOEXPORT int CALLBACK AI2K_SetChannelStatuses(
		int hConnection,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD * wValue );

	//MXIOEXPORT int CALLBACK AI2K_GetPowerOnRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AI2K_SetPowerOnRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AI2K_GetPowerOnRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD *	wRange );
	//
	//MXIOEXPORT int CALLBACK AI2K_SetPowerOnRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD	wRange );
	//
	//MXIOEXPORT int CALLBACK AI2K_GetSafeRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AI2K_SetSafeRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AI2K_GetSafeRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD *	wRange );
	//
	//MXIOEXPORT int CALLBACK AI2K_SetSafeRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD	wRange );

	/*************************************************/
	/*                                               */
	/*      ioLogik 2000 Analog output command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK AO2K_GetRanges(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wRange[] );

	MXIOEXPORT int CALLBACK AO2K_SetRanges(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wRange[] );

	MXIOEXPORT int CALLBACK AO2K_GetRange(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wRange );

	MXIOEXPORT int CALLBACK AO2K_SetRange(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wRange );

	//MXIOEXPORT int CALLBACK AO2K_GetPowerOnRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AO2K_SetPowerOnRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AO2K_GetPowerOnRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD *	wRange );
	//
	//MXIOEXPORT int CALLBACK AO2K_SetPowerOnRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD	wRange );
	//
	//MXIOEXPORT int CALLBACK AO2K_GetSafeRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AO2K_SetSafeRanges(
	//	int		hConnection,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	WORD	wRange[] );
	//
	//MXIOEXPORT int CALLBACK AO2K_GetSafeRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD *	wRange );
	//
	//MXIOEXPORT int CALLBACK AO2K_SetSafeRange(
	//	int		hConnection,
	//	BYTE	bytChannel,
	//	WORD	wRange );

	MXIOEXPORT int CALLBACK AO2K_GetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AO2K_SetPowerOnValues(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK AO2K_GetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		double *dValue );

	MXIOEXPORT int CALLBACK AO2K_SetPowerOnValue(
		int		hConnection,
		BYTE	bytChannel,
		double  dValue );

	MXIOEXPORT int CALLBACK AO2K_GetPowerOnRaws(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AO2K_SetPowerOnRaws(
		int		hConnection,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK AO2K_GetPowerOnRaw(
		int		hConnection,
		BYTE	bytChannel,
		WORD *	wValue );

	MXIOEXPORT int CALLBACK AO2K_SetPowerOnRaw(
		int		hConnection,
		BYTE	bytChannel,
		WORD	wValue );

	/*************************************************/
	/*                                               */
	/*    ioLogik 2000 Click & Go Logic Command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK Logic2K_GetStartStatus(
		int		hConnection,
		WORD *	wStatus );

	MXIOEXPORT int CALLBACK Logic2K_SetStartStatus(
		int		hConnection,
		WORD	wStatus );

	/*************************************************/
	/*                                               */
	/*         ioLogik 2000 Message Command          */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK Message2K_Start(
		int				iProtocol,
		WORD			wPort,
		pfnCALLBACK		iProcAddress );

	MXIOEXPORT int CALLBACK Message2K_Stop(
		int		iProtocol );

	/*************************************************/
	/*                                               */
	/*     ioLogik 4200 special Adapter command      */
	/*                                               */
	/*************************************************/
	//MXIOEXPORT int CALLBACK E42_GetInternalReg(
	//	int 	hConnection,
	//	BYTE    bytChannel,
	//	WORD *  wValue );
	//
	//MXIOEXPORT int CALLBACK E42_SetInternalReg(
	//	int 	hConnection,
	//	BYTE    bytChannel,
	//	WORD    wValue );

	MXIOEXPORT int CALLBACK E42_GetInternalRegs(
		int 	hConnection,
		BYTE    bytStartChannel,
		BYTE 	bytCount,
		WORD    wValue[] );

	MXIOEXPORT int CALLBACK E42_SetInternalRegs(
		int 	hConnection,
		BYTE    bytStartChannel,
		BYTE 	bytCount,
		WORD    wValue[] );

	MXIOEXPORT int CALLBACK E42_GetIOMapMode(
		int 	hConnection,
		WORD  * wValue );

	MXIOEXPORT int CALLBACK E42_SetIOMapMode(
		int 	hConnection,
		WORD    wValue );

	MXIOEXPORT int CALLBACK E42_ReadStatus(
		int 	hConnection,
		WORD  * wState,
		WORD  * wLastErrorCode );

	MXIOEXPORT int CALLBACK E42_ClearStatus(
		int		hConnection );

	MXIOEXPORT int CALLBACK E42_ReadSlotAmount(
		int		hConnection,
		WORD *	wAmount );

	MXIOEXPORT int CALLBACK E42_AO_GetSafeActions(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wAction[] );

	MXIOEXPORT int CALLBACK E42_AO_SetSafeActions(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wAction[] );

	//MXIOEXPORT int CALLBACK E42_AO_GetPowerOnValues(
	//	int hConnection,
	//	BYTE bytSlot,
	//	BYTE bytStartChannel,
	//	BYTE bytCount,
	//	double dValue[] );
	//
	//MXIOEXPORT int CALLBACK E42_AO_SetPowerOnValues(
	//	int hConnection,
	//	BYTE bytSlot,
	//	BYTE bytStartChannel,
	//	BYTE bytCount,
	//	double dValue[] );

	MXIOEXPORT int CALLBACK E42_AO_GetPowerOnValues(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK E42_AO_SetPowerOnValues(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK E42_Message_Start(
		int				iProtocol,
		WORD			wPort,
		pfnCALLBACK		iProcAddress );

	MXIOEXPORT int CALLBACK E42_Message_Stop(
		int		iProtocol );

	MXIOEXPORT int CALLBACK E42_Logic_GetStartStatus(
		int hConnection,
		WORD *wStatus );

	MXIOEXPORT int CALLBACK E42_Logic_SetStartStatus(
		int hConnection,
		WORD wStatus );

	MXIOEXPORT int CALLBACK E42_ReadFirmwareRevision (
		int		hConnection,
		BYTE	bytRevision[] );

	MXIOEXPORT int CALLBACK E42_ReadFirmwareDate(
		int		hConnection,
		WORD	wDate[] );

	MXIOEXPORT int CALLBACK E42_DI_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK E42_DO_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD * dwValue );

	MXIOEXPORT int CALLBACK E42_DO_Writes(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		DWORD	dwValue );

	MXIOEXPORT int CALLBACK E42_AI_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK E42_AI_ReadRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK E42_AO_Reads(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK E42_AO_Writes(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		double	dValue[] );

	MXIOEXPORT int CALLBACK E42_AO_ReadRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK E42_AO_WriteRaws(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	//MXIOEXPORT int CALLBACK E42_AO_GetSafeValues(
	//	int		hConnection,
	//	BYTE	bytSlot,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	double	dValue[] );
	//
	//MXIOEXPORT int CALLBACK E42_AO_SetSafeValues(
	//	int		hConnection,
	//	BYTE	bytSlot,
	//	BYTE	bytStartChannel,
	//	BYTE	bytCount,
	//	double	dValue[] );

	MXIOEXPORT int CALLBACK E42_AO_GetFaultValues(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK E42_AO_SetFaultValues(
		int		hConnection,
		BYTE	bytSlot,
		BYTE	bytStartChannel,
		BYTE	bytCount,
		WORD	wValue[] );

	MXIOEXPORT int CALLBACK E42_RTD_Reads(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK E42_RTD_ReadRaws(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK E42_TC_Reads(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		double dValue[] ) ;

	MXIOEXPORT int CALLBACK E42_TC_ReadRaws(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		WORD wValue[] );

	MXIOEXPORT int CALLBACK E42_DO_GetSafeActions(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD wAction[] );

	MXIOEXPORT int CALLBACK E42_DO_SetSafeActions(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD wAction );

	MXIOEXPORT int CALLBACK E42_DO_GetFaultValues(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[]);

	MXIOEXPORT int CALLBACK E42_DO_SetFaultValues(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue);

	MXIOEXPORT int CALLBACK E42_DO_GetPowerOnValues(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue[] );

	MXIOEXPORT int CALLBACK E42_DO_SetPowerOnValues(
		int hConnection,
		BYTE bytSlot,
		BYTE bytStartChannel,
		BYTE bytCount,
		DWORD dwValue);

	MXIOEXPORT int CALLBACK E42_Modbus_List(
		int hConnection,
		char* FilePath);

	MXIOEXPORT int CALLBACK E42_RTD_GetEngUnit(
		int hConnection,
		BYTE bytSlot,
		WORD wEngUnit[] );

	MXIOEXPORT int CALLBACK E42_RTD_SetEngUnit(
		int hConnection,
		BYTE bytSlot,
		WORD wEngUnit );

	MXIOEXPORT int CALLBACK E42_RTD_GetSensorType(
		int hConnection,
		BYTE bytSlot,
		WORD wSensorType[] );

	MXIOEXPORT int CALLBACK E42_RTD_SetSensorType(
		int hConnection,
		BYTE bytSlot,
		WORD wSensorType );

	MXIOEXPORT int CALLBACK E42_TC_GetEngUnit(
		int hConnection,
		BYTE bytSlot,
		WORD wEngUnit[] );

	MXIOEXPORT int CALLBACK E42_TC_SetEngUnit(
		int hConnection,
		BYTE bytSlot,
		WORD wEngUnit );

	MXIOEXPORT int CALLBACK E42_TC_GetSensorType(
		int hConnection,
		BYTE bytSlot,
		WORD wSensorType[] );

	MXIOEXPORT int CALLBACK E42_TC_SetSensorType(
		int hConnection,
		BYTE bytSlot,
		WORD wSensorType );

	MXIOEXPORT int CALLBACK E42_GetWorkInternalRegs(
		int 	hConnection,
		BYTE    bytStartChannel,
		BYTE 	bytCount,
		WORD    wValue[] );

	MXIOEXPORT int CALLBACK E42_SetWorkInternalRegs(
		int 	hConnection,
		BYTE    bytStartChannel,
		BYTE 	bytCount,
		WORD    wValue[] );

	/*************************************************/
	/*                                               */
	/*     ioLogik W5xxx special Adapter command     */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK MXEIO_W5K_Connect( char * szIP , WORD wPort, DWORD dwTimeOut, int * hConnection, char * szMACAddr);
	MXIOEXPORT int CALLBACK W5K_DIO_GetIOModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwMode );
	MXIOEXPORT int CALLBACK W5K_DIO_SetIOModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwMode );
	MXIOEXPORT int CALLBACK W5K_DO_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_DO_Writes( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_DO_GetModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK W5K_DO_SetModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK W5K_DO_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_DO_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_DO_GetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_DO_SetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_RLY_GetResetTime( int hConnection,BYTE bytChannel, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_RLY_TotalCntReads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK W5K_RLY_CurrentCntReads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK W5K_RLY_ResetCnts( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK W5K_DI_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_DI_GetModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK W5K_DI_SetModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK W5K_DI_GetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK W5K_DI_SetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK W5K_Pulse_GetSignalWidths32( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwHiWidth[], DWORD dwLoWidth[] );
	MXIOEXPORT int CALLBACK W5K_Pulse_SetSignalWidths32( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwHiWidth[], DWORD dwLoWidth[] );
	MXIOEXPORT int CALLBACK W5K_Pulse_GetOutputCounts( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCount[] );
	MXIOEXPORT int CALLBACK W5K_Pulse_SetOutputCounts( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCount[] );
	MXIOEXPORT int CALLBACK W5K_Pulse_GetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus );
	MXIOEXPORT int CALLBACK W5K_Pulse_SetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus );
	MXIOEXPORT int CALLBACK W5K_Pulse_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *	dwValue );
	MXIOEXPORT int CALLBACK W5K_Pulse_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_Pulse_GetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_Pulse_SetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_Cnt_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK W5K_Cnt_Clears( int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetOverflows( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus );
	MXIOEXPORT int CALLBACK W5K_Cnt_ClearOverflows( int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK W5K_Cnt_SetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus );
	MXIOEXPORT int CALLBACK W5K_Cnt_SetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_Cnt_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK W5K_Cnt_SetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetTriggerTypeWords( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wType );
	MXIOEXPORT int CALLBACK W5K_Cnt_SetTriggerTypeWords( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wType );
	MXIOEXPORT int CALLBACK W5K_Cnt_GetSaveStatusesOnPowerFail( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwMode );
	MXIOEXPORT int CALLBACK W5K_Cnt_SetSaveStatusesOnPowerFail( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwMode );
	MXIOEXPORT int CALLBACK W5K_AI_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK W5K_AI_ReadRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMins( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMinRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_AI_ResetMins( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK W5K_AI_ReadMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMaxRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_AI_ResetMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK W5K_AI_GetRanges( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK W5K_AI_SetRanges( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK W5K_AI_GetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wValue );
	MXIOEXPORT int CALLBACK W5K_AI_SetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wValue );
	//
	MXIOEXPORT int CALLBACK W5K_Message_Start(	int	iProtocol, WORD	wPort, pfnCALLBACK iProcAddress );
	MXIOEXPORT int CALLBACK W5K_Message_Stop( int iProtocol );
	MXIOEXPORT int CALLBACK W5K_Logic_GetStartStatus( int hConnection, WORD * wStatus );
	MXIOEXPORT int CALLBACK W5K_Logic_SetStartStatus( int hConnection, WORD wStatus );
	MXIOEXPORT int CALLBACK W5K_GetInternalRegs( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_SetInternalRegs( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_GetGprsSignal( int hConnection, WORD * wSignal);
	//MXIOEXPORT int CALLBACK W5K_GetCallerID( int hConnection, BYTE bytChannel, WORD wValue[] );
	//MXIOEXPORT int CALLBACK W5K_SetCallerID( int hConnection, BYTE bytChannel, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_GetSafeStatus(	int	hConnection, WORD *	wStatus );
	MXIOEXPORT int CALLBACK W5K_ClearSafeStatus( int hConnection );
	MXIOEXPORT int CALLBACK W5K_ListOpcDevices( char * szIP, DWORD dwTimeOut, WORD * wDeviceCount);
	MXIOEXPORT int CALLBACK W5K_GetOpcDevicesInfo( char * szIP, DWORD dwTimeOut, WORD wDeviceCount, char szDeviceInfo[]);
	MXIOEXPORT int CALLBACK W5K_GetOpcHostName( char * szIP, DWORD dwTimeOut, char szAliasName[]);
	/*************************************************/
	/*                                               */
	/*     ioLogik E1200 Series command              */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK MXEIO_E1K_Connect( char * szIP , WORD wPort, DWORD dwTimeOut, int * hConnection, char *Password);
	/////===ioLogik E1200 DI Command  ===/////
	MXIOEXPORT int CALLBACK E1K_DI_Reads(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK E1K_DI_GetModes(int hConnection, BYTE	bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK E1K_DI_SetModes( int	hConnection, BYTE bytStartChannel, BYTE bytCount,WORD wMode[] );
	MXIOEXPORT int CALLBACK E1K_DI_GetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount,WORD wFilter[] );
	MXIOEXPORT int CALLBACK E1K_DI_SetFilters(int hConnection,BYTE bytStartChannel, BYTE bytCount,WORD	wFilter[] );
	/////===ioLogik E1200 DO Command  ===/////
	MXIOEXPORT int CALLBACK E1K_DO_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwValue );
	MXIOEXPORT int CALLBACK E1K_DO_Writes( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK E1K_DO_GetSafeValues_W( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValues[]);
	MXIOEXPORT int CALLBACK E1K_DO_SetSafeValues_W( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValues[]);
	MXIOEXPORT int CALLBACK E1K_DO_GetModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK E1K_DO_SetModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK E1K_DO_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwValue );
	MXIOEXPORT int CALLBACK E1K_DO_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK E1K_DO_GetPowerOnSeqDelaytimes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_DO_SetPowerOnSeqDelaytimes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	/////===ioLogik E1200 special Command  ===/////
	MXIOEXPORT int CALLBACK E1K_GetSafeStatus( int	hConnection, WORD *wStatus );
	MXIOEXPORT int CALLBACK E1K_ClearSafeStatus( int hConnection );
	/////===ioLogik E1200 DIO Command  ===/////
	MXIOEXPORT int CALLBACK E1K_DIO_GetIOModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwMode );
	/////===ioLogik E1200 AI Command  ===/////
	MXIOEXPORT int CALLBACK E1K_AI_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, double	dValue[] );
	MXIOEXPORT int CALLBACK E1K_AI_ReadRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AI_ReadMins( int hConnection, BYTE bytStartChannel, BYTE bytCount, double	dValue[] );
	MXIOEXPORT int CALLBACK E1K_AI_ReadMinRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AI_ResetMins( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK E1K_AI_ReadMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount, double	dValue[] );
	MXIOEXPORT int CALLBACK E1K_AI_ReadMaxRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AI_ResetMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK E1K_AI_GetRanges( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK E1K_AI_GetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD *wValue );
	MXIOEXPORT int CALLBACK E1K_AI_SetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD *wValue );
	/////===ioLogik E1200 COUNT Command  ===/////
	MXIOEXPORT int CALLBACK E1K_Cnt_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK E1K_Cnt_Clears( int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetOverflows( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwStatus );
	MXIOEXPORT int CALLBACK E1K_Cnt_ClearOverflows( int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK E1K_Cnt_SetFilters( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwStatus );
	MXIOEXPORT int CALLBACK E1K_Cnt_SetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwValue );
	MXIOEXPORT int CALLBACK E1K_Cnt_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwValue );
	MXIOEXPORT int CALLBACK E1K_Cnt_SetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetTriggerTypeWords( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD *wType );
	MXIOEXPORT int CALLBACK E1K_Cnt_SetTriggerTypeWords( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD *wType );
	MXIOEXPORT int CALLBACK E1K_Cnt_GetSaveStatusesOnPowerFail( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwMode );
	MXIOEXPORT int CALLBACK E1K_Cnt_SetSaveStatusesOnPowerFail( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwMode );
	/////===ioLogik E1200 Relay Command  ===/////
	MXIOEXPORT int CALLBACK E1K_RLY_TotalCntReads( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	/////===ioLogik E1200 Pulse Command  ===/////
	MXIOEXPORT int CALLBACK E1K_Pulse_GetSignalWidths( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wHiWidth[], WORD wLoWidth[] );
	MXIOEXPORT int CALLBACK E1K_Pulse_SetSignalWidths( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wHiWidth[], WORD wLoWidth[] );
	MXIOEXPORT int CALLBACK E1K_Pulse_GetOutputCounts( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCounts[] );
	MXIOEXPORT int CALLBACK E1K_Pulse_SetOutputCounts( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCounts[] );
	MXIOEXPORT int CALLBACK E1K_Pulse_GetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwStatus );
	MXIOEXPORT int CALLBACK E1K_Pulse_SetStartStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus );
	MXIOEXPORT int CALLBACK E1K_Pulse_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwValue );
	MXIOEXPORT int CALLBACK E1K_Pulse_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK E1K_Pulse_GetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *dwValue );
	MXIOEXPORT int CALLBACK E1K_Pulse_SetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	//E4200
	MXIOEXPORT int CALLBACK E42_ClearSafeStatus( int hConnection );
	/////===ioLogik W53xx+Expansion Module Command  ===/////
	MXIOEXPORT int CALLBACK W5K_ReadStatus( int hConnection, WORD * wState, WORD * wLastErrorCode );
	MXIOEXPORT int CALLBACK W5K_Exp_Reconnect( int hConnection );
	MXIOEXPORT int CALLBACK W5K_Exp_Status( int hConnection, WORD wState[] );
	MXIOEXPORT int CALLBACK W5K_ReadSlotAmount( int hConnection, WORD * wAmount );
	MXIOEXPORT int CALLBACK W5K_ReadLastSlotIndex( int hConnection, WORD * wLastSlotIndex );
	MXIOEXPORT int CALLBACK W5K_Modbus_List( int hConnection, char* FilePath);
	MXIOEXPORT int CALLBACK W5K_DIO_GetIOModes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwMode, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DIO_SetIOModes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwMode, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_DO_Reads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_DO_Writes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_DO_GetModes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DO_SetModes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DO_GetPowerOnValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DO_SetPowerOnValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DO_GetSafeValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DO_SetSafeValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_RLY_TotalCntReads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_RLY_CurrentCntReads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_RLY_ResetCnts_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_DI_Reads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_DI_GetModes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DI_SetModes_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DI_GetFilters_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_DI_SetFilters_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_GetSignalWidths32_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwHiWidth[], DWORD dwLoWidth[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_SetSignalWidths32_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwHiWidth[], DWORD dwLoWidth[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_GetOutputCounts_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCount[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_SetOutputCounts_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCount[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_Pulse_GetStartStatuses_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_Pulse_SetStartStatuses_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_GetPowerOnValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD *	dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_SetPowerOnValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_GetSafeValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Pulse_SetSafeValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_Cnt_Reads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_Cnt_Clears_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetOverflows_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_ClearOverflows_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetFilters_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_SetFilters_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetStartStatuses_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_SetStartStatuses_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetPowerOnValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_SetPowerOnValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetSafeValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_SetSafeValues_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetTriggerTypeWords_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wType, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_SetTriggerTypeWords_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wType, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_GetSaveStatusesOnPowerFail_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwMode, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_Cnt_SetSaveStatusesOnPowerFail_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwMode, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_Reads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_ReadRaws_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMins_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMinRaws_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_AI_ResetMins_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMaxs_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_ReadMaxRaws_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_AI_ResetMaxs_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_AI_GetRanges_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_AI_SetRanges_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[], BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_AI_GetChannelStatuses_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wValue, BYTE BytSlot );
	//MXIOEXPORT int CALLBACK W5K_AI_SetChannelStatuses_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD * wValue, BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_VC_Reads_Ex( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[], BYTE BytSlot );
	MXIOEXPORT int CALLBACK W5K_GetWorkInternalRegs( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK W5K_SetWorkInternalRegs( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );

	/////===ioLogik E1200 TC Command  ===/////
	MXIOEXPORT int CALLBACK E1K_TC_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_TC_ReadRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_TC_ResetMins( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK E1K_TC_ResetMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK E1K_TC_GetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus );
	MXIOEXPORT int CALLBACK E1K_TC_SetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus );
	MXIOEXPORT int CALLBACK E1K_TC_GetEngUnits( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wEngUnit[] );
	MXIOEXPORT int CALLBACK E1K_TC_SetEngUnits( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wEngUnit[] );
	MXIOEXPORT int CALLBACK E1K_TC_GetSensorTypes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wSensorType[] );
	MXIOEXPORT int CALLBACK E1K_TC_SetSensorTypes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wSensorType[] );
	MXIOEXPORT int CALLBACK E1K_TC_ReadMinRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK E1K_TC_ReadMaxRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK E1K_TC_ReadMins( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_TC_ReadMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	/////===ioLogik E1200 RTD Command  ===/////
	MXIOEXPORT int CALLBACK E1K_RTD_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_RTD_ReadRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_RTD_ResetMins( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK E1K_RTD_ResetMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK E1K_RTD_GetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwStatus );
	MXIOEXPORT int CALLBACK E1K_RTD_SetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwStatus );
	MXIOEXPORT int CALLBACK E1K_RTD_GetEngUnits( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wEngUnit[] );
	MXIOEXPORT int CALLBACK E1K_RTD_SetEngUnits( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wEngUnit[] );
	MXIOEXPORT int CALLBACK E1K_RTD_GetSensorTypes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wSensorType[] );
	MXIOEXPORT int CALLBACK E1K_RTD_SetSensorTypes( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wSensorType[] );
	MXIOEXPORT int CALLBACK E1K_RTD_ReadMinRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_RTD_ReadMaxRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_RTD_ReadMins( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_RTD_ReadMaxs( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	/////===ioLogik E1200 AO Command  ===/////
	MXIOEXPORT int CALLBACK E1K_AO_Reads( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_Writes( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_ReadRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_WriteRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_GetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_SetSafeValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_GetSafeRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_SetSafeRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_GetRanges( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK E1K_AO_SetRanges( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK E1K_AO_GetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_SetPowerOnValues( int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_GetPowerOnRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK E1K_AO_SetPowerOnRaws( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	//MXIOEXPORT int CALLBACK E1K_AO_SetScaleSelect( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	//V2.7.0.0 Added
	/////===ioLogik Special Module command for fix modbus address /////===
	MXIOEXPORT int CALLBACK MXIO_ReadCoils_Ex( int hConnection, BYTE bytCoilType, WORD wStartCoil, WORD wCount, BYTE bytCoils[] );
	MXIOEXPORT int CALLBACK MXIO_WriteCoils_Ex( int	hConnection, WORD wStartCoil, WORD wCount, BYTE	bytCoils[] );
	MXIOEXPORT int CALLBACK MXIO_ReadRegs_Ex( int hConnection, BYTE bytRegisterType, WORD wStartRegister, WORD wCount, WORD wRegister[] );
	MXIOEXPORT int CALLBACK MXIO_WriteRegs_Ex( int hConnection, WORD wStartRegister, WORD wCount, WORD wRegister[] );
	//V2.8.0.0 Added
	MXIOEXPORT int CALLBACK ACTTAG_Modbus_Start( int iProtocol, WORD wPort, pfnCALLBACK iProcAddress, DWORD dwTimeout );
	MXIOEXPORT int CALLBACK ACTTAG_Modbus_Stop( int iProtocol );
	MXIOEXPORT int CALLBACK ACTTAG_TagData_Start( int iProtocol, WORD wPort, pfnTagDataCALLBACK iProcAddress, DWORD dwTimeout );
	MXIOEXPORT int CALLBACK ACTTAG_TagData_Stop( int iProtocol );

	MXIOEXPORT int CALLBACK MXIO_Init_ActiveTag( WORD wDataPort, WORD wCmdPort, DWORD dwToleranceTimeout, DWORD dwCmdTimeout, pfnTagDataCALLBACK iProcAddress, WORD wSize);
	MXIOEXPORT int CALLBACK MXIO_Init_ActiveTag_Ex( WORD wDataPort, WORD wCmdPort, DWORD dwToleranceTimeout, DWORD dwCmdTimeout, pfnCALLBACK iProcAddress, WORD wSize);
	MXIOEXPORT int CALLBACK MXIO_ListDevs_ActiveTag( WORD * wDeviceCount);
	MXIOEXPORT int CALLBACK MXIO_GetDevsInfo_ActiveTag( WORD wDeviceCount, char szDeviceInfo[]);
	MXIOEXPORT int CALLBACK MXIO_Close_ActiveTag(void);
	//MXIOEXPORT int CALLBACK MXIO_SetQueueSize_ActiveTag( WORD wSize);
	//MXIOEXPORT int CALLBACK MXIO_GetQueueSize_ActiveTag( WORD * wSize);
	//MXIOEXPORT int CALLBACK MXIO_SetCmdTimeout_ActiveTag( int hConnection, DWORD dwTimeOut);
	//MXIOEXPORT int CALLBACK MXIO_GetCmdTimeout_ActiveTag( int hConnection, DWORD * dwTimeOut);
	#ifdef ARM_WINCE
		MXIOEXPORT int CALLBACK MXIO_Connect_ActiveTag( DWORD dwTimeOut, int __unaligned * hConnection, char * szMACAddr, WORD wPort, char * Password);
	#else
		MXIOEXPORT int CALLBACK MXIO_Connect_ActiveTag( DWORD dwTimeOut, int * hConnection, char * szMACAddr, WORD wPort, char * Password);
	#endif
	MXIOEXPORT int CALLBACK MXIO_RelLock_ActiveTag(void);
	//
	MXIOEXPORT int CALLBACK MXIO_GetSubType( int hConnection, BYTE bytSlot, DWORD * dwSubType);
	//
	//MXIOEXPORT int CALLBACK MXIO_AutoSearch(int nType, int nRetryCount, int nTimeout, int* nNumber, MODULE_LIST * struML);
	MXIOEXPORT int CALLBACK MXIO_AutoSearch(int nType, int nRetryCount, int nTimeout, int* nNumber, char struML[]);
	//MXIOEXPORT int CALLBACK MXIO_Get_LockCh(int hConnection, BYTE bytSlot, WORD * wDeviceCount);
	/////===ioLogik E1200 DIO Command  ===/////
	MXIOEXPORT int CALLBACK E1K_DIO_SetIOModes( int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwMode );
	//for change duplicate IP
	MXIOEXPORT int CALLBACK MXIO_ListIF( WORD * wIFCount);
	MXIOEXPORT int CALLBACK MXIO_GetIFInfo( WORD wIFCount, char IFInfo[]);
	MXIOEXPORT int CALLBACK MXIO_SelectIF( WORD wIFCount, char IFInfo[], DWORD dwIFIndex);
	//for E1200 Series Only
	MXIOEXPORT int CALLBACK MXIO_ChangeDupIP( char* szIP, WORD wPort, char* szMACAddr, DWORD dwTimeOut, char *Password, char* szNewIP);
	//E1200 Series AO Channel Enable/Disable
	MXIOEXPORT int CALLBACK E1K_AO_GetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD *wValue );
	MXIOEXPORT int CALLBACK E1K_AO_SetChannelStatuses( int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD *wValue );
    //V2.10.0.0 Added=================================		
	/*************************************************/
	/*                                               */
	/*      ioLogik R1000 Digital I/O command        */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_DIO_GetIOModes(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );

	/*************************************************/
	/*                                               */
	/*      ioLogik R1000 Digital Input command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_DI_Reads(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_DI_GetModes(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK R1K_DI_SetModes(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK R1K_DI_GetFilters(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK R1K_DI_SetFilters(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
    /*************************************************/
	/*                                               */
	/*         ioLogik R1000 Counter command          */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_Cnt_Reads(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK R1K_Cnt_Clears(int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetOverflows(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_ClearOverflows(int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetFilters(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK R1K_Cnt_SetFilters(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wFilter[] );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetStartStatuses(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_SetStartStatuses(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetTriggerTypeWords(int hConnection,  BYTE bytStartChannel, BYTE bytCount, WORD wType[] );
	MXIOEXPORT int CALLBACK R1K_Cnt_SetTriggerTypeWords(int hConnection,  BYTE bytStartChannel, BYTE bytCount, WORD wType[] );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetPowerOnValues(int hConnection,  BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_SetPowerOnValues(int hConnection,  BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetSafeValues(int hConnection,  BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_SetSafeValues(int hConnection,  BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_GetSaveStatusesOnPowerFail(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Cnt_SetSaveStatusesOnPowerFail(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	/*************************************************/
	/*                                               */
	/*      ioLogik R1000 Digital output command     */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_DO_Reads(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_DO_Writes(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_DO_SetSafeValues_W(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[]);
	MXIOEXPORT int CALLBACK R1K_DO_GetSafeValues_W(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[]);
	MXIOEXPORT int CALLBACK R1K_DO_GetModes(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK R1K_DO_SetModes(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wMode[] );
	MXIOEXPORT int CALLBACK R1K_DO_GetPowerOnValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_DO_SetPowerOnValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_DO_GetPowerOnSeqDelaytimes(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_DO_SetPowerOnSeqDelaytimes(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );

	/*************************************************/
	/*                                               */
	/*         ioLogik R1000 Relay Count & Reset      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_RLY_TotalCntReads(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
	MXIOEXPORT int CALLBACK R1K_RLY_CurrentCntReads(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue[] );
    MXIOEXPORT int CALLBACK R1K_RLY_ResetCnts(int hConnection, BYTE bytStartChannel, BYTE bytCount);

	/*************************************************/
	/*                                               */
	/*       ioLogik R1000 Pulse Output command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_Pulse_GetSignalWidths(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wHiWidth[], WORD wLoWidth[] );
	MXIOEXPORT int CALLBACK R1K_Pulse_SetSignalWidths(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wHiWidth[], WORD wLoWidth[] );
	MXIOEXPORT int CALLBACK R1K_Pulse_GetOutputCounts(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCount[] );
	MXIOEXPORT int CALLBACK R1K_Pulse_SetOutputCounts(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwOutputCount[] );
	MXIOEXPORT int CALLBACK R1K_Pulse_GetStartStatuses(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Pulse_SetStartStatuses(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_Pulse_GetPowerOnValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Pulse_SetPowerOnValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );
	MXIOEXPORT int CALLBACK R1K_Pulse_GetSafeValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD * dwValue );
	MXIOEXPORT int CALLBACK R1K_Pulse_SetSafeValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, DWORD dwValue );

	/*************************************************/
	/*                                               */
	/*       ioLogik R1000 Analog Input command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_AI_Reads(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AI_ReadRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AI_ReadMins(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AI_ReadMinRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AI_ResetMins(int hConnection, BYTE bytStartChannel, BYTE bytCount);
	MXIOEXPORT int CALLBACK R1K_AI_ReadMaxs(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AI_ReadMaxRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AI_ResetMaxs(int hConnection, BYTE bytStartChannel, BYTE bytCount );
	MXIOEXPORT int CALLBACK R1K_AI_GetRanges(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );

	/*************************************************/
	/*                                               */
	/*      ioLogik R1000 Analog output command       */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_AO_Reads(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_Writes(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_ReadRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_WriteRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_GetSafeValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_SetSafeValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_GetSafeRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_SetSafeRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_GetRanges(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK R1K_AO_SetRanges(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wRange[] );
	MXIOEXPORT int CALLBACK R1K_AO_GetPowerOnValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_SetPowerOnValues(int hConnection, BYTE bytStartChannel, BYTE bytCount, double dValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_GetPowerOnRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	MXIOEXPORT int CALLBACK R1K_AO_SetPowerOnRaws(int hConnection, BYTE bytStartChannel, BYTE bytCount, WORD wValue[] );
	/*************************************************/
	/*                                               */
	/*      ioLogik R1000 special Module command      */
	/*                                               */
	/*************************************************/
	MXIOEXPORT int CALLBACK R1K_GetSafeStatus(int hConnection, WORD * wStatus );
	MXIOEXPORT int CALLBACK R1K_ClearSafeStatus(int hConnection);		
	
	#ifdef __cplusplus
		}
	#endif
	//---------------------------------------------------------------------------
	// Error values generated by the classed in the driver toolkit
	/*************************************************/
	/*                                               */
	/*                  Error Code                   */
	/*                                               */
	/*************************************************/
	#define MXIO_OK								0

	#define ILLEGAL_FUNCTION					1001
	#define ILLEGAL_DATA_ADDRESS				1002
	#define ILLEGAL_DATA_VALUE					1003
	#define SLAVE_DEVICE_FAILURE				1004
	#define SLAVE_DEVICE_BUSY					1006

	#define EIO_TIME_OUT						2001
	#define EIO_INIT_SOCKETS_FAIL				2002
	#define EIO_CREATING_SOCKET_ERROR			2003
	#define EIO_RESPONSE_BAD					2004
	#define EIO_SOCKET_DISCONNECT				2005
	#define PROTOCOL_TYPE_ERROR					2006
	#define EIO_PASSWORD_INCORRECT				2007

	#define SIO_OPEN_FAIL						3001
	#define SIO_TIME_OUT					    3002
	#define SIO_CLOSE_FAIL						3003
	#define SIO_PURGE_COMM_FAIL					3004
	#define SIO_FLUSH_FILE_BUFFERS_FAIL			3005
	#define SIO_GET_COMM_STATE_FAIL				3006
	#define SIO_SET_COMM_STATE_FAIL				3007
	#define SIO_SETUP_COMM_FAIL					3008
	#define SIO_SET_COMM_TIME_OUT_FAIL			3009
	#define SIO_CLEAR_COMM_FAIL					3010
	#define SIO_RESPONSE_BAD					3011
	#define SIO_TRANSMISSION_MODE_ERROR			3012
	#define SIO_BAUDRATE_NOT_SUPPORT            3013

	#define PRODUCT_NOT_SUPPORT					4001
	#define HANDLE_ERROR						4002
	#define SLOT_OUT_OF_RANGE					4003
	#define CHANNEL_OUT_OF_RANGE				4004
	#define COIL_TYPE_ERROR						4005
	#define REGISTER_TYPE_ERROR					4006
	#define FUNCTION_NOT_SUPPORT				4007
	#define OUTPUT_VALUE_OUT_OF_RANGE			4008
	#define INPUT_VALUE_OUT_OF_RANGE			4009
	#define SLOT_NOT_EXIST						4010
	#define FIRMWARE_NOT_SUPPORT				4011
	#define CREATE_MUTEX_FAIL					4012

	#define ENUM_NET_INTERFACE_FAIL				5000
	#define ADD_INFO_TABLE_FAIL					5001
	#define UNKNOWN_NET_INTERFACE_FAIL			5002
	#define TABLE_NET_INTERFACE_FAIL			5003
	//
	/*************************************************/
	/*                                               */
	/*              Data Format Setting              */
	/*                                               */
	/*************************************************/
	/* Data length define	*/
	#define BIT_5								0x00
	#define BIT_6								0x01
	#define BIT_7								0x02
	#define BIT_8								0x03

	/* Stop bits define	*/
	#define STOP_1								0x00
	#define STOP_2								0x04

	/* Parity define	*/
	#define P_EVEN								0x18
	#define P_ODD								0x08
	#define P_SPC								0x38
	#define P_MRK								0x28
	#define P_NONE								0x00

	/*************************************************/
	/*                                               */
	/*        Modbus Transmission Mode Setting       */
	/*                                               */
	/*************************************************/
	#define MODBUS_RTU							0x0
	#define MODBUS_ASCII						0x1

	/*************************************************/
	/*                                               */
	/*            Check Connection Status            */
	/*                                               */
	/*************************************************/
	#define CHECK_CONNECTION_OK					0x0
	#define CHECK_CONNECTION_FAIL				0x1
	#define CHECK_CONNECTION_TIME_OUT			0x2

	/*************************************************/
	/*                                               */
	/*            Modbus Coil Type Setting           */
	/*                                               */
	/*************************************************/
	#define COIL_INPUT							0x2
	#define COIL_OUTPUT							0x1

	/*************************************************/
	/*                                               */
	/*            Modbus Coil Type Setting           */
	/*                                               */
	/*************************************************/
	#define REGISTER_INPUT						0x4
	#define REGISTER_OUTPUT						0x3

	/*************************************************/
	/*                                               */
	/*             ioLogik 4000 Bus Status           */
	/*                                               */
	/*************************************************/
	#define NORMAL_OPERATION					0x0
	#define	BUS_STANDYBY						0x1
	#define BUS_COMMUNICATION_FAULT				0x2
	#define SLOT_CONFIGURATION_FAILED			0x3
	#define NO_EXPANSION_SLOT					0x4


	/*************************************************/
	/*                                               */
	/*        ioLogik 4000 Field Power Status        */
	/*                                               */
	/*************************************************/
	#define FIELD_POWER_ON						0x0
	#define FIELD_POWER_OFF						0x1

	/*************************************************/
	/*                                               */
	/*         ioLogik 4000 Watchdog Status          */
	/*                                               */
	/*************************************************/
	#define WATCHDOG_NO_ERROR					0x0
	#define WATCHDOG_ERROR						0x1

	/*************************************************/
	/*                                               */
	/*   ioLogik 4000 Modbus Error Setup Status      */
	/*                                               */
	/*************************************************/
	#define SETUP_NO_ERROR						0x0
	#define SETUP_ERROR							0x1

	/*************************************************/
	/*                                               */
	/*   ioLogik 4000 Modbus Error Check Status      */
	/*                                               */
	/*************************************************/
	#define CHECKSUM_NO_ERROR					0x0
	#define CHECKSUM_ERROR						0x1

	/*************************************************/
	/*                                               */
	/*         ioLogik 4000 AO Safe Action           */
	/*                                               */
	/*************************************************/
	#define SAFE_VALUE							0x00		//Suport AO & DO module
	#define HOLD_LAST_STATE						0x01		//Suport AO & DO module
	#define LOW_LIMIT							0x02		//Only suport AO module
	#define HIGH_LIMIT							0x03		//Only suport AO module
	/*************************************************/
	/*                                               */
	/*               Protocol Type                   */
	/*                                               */
	/*************************************************/
	#define PROTOCOL_TCP						0x01
	#define PROTOCOL_UDP						0x02
	/*************************************************/
	/*                                               */
	/*               W53xx BUS STATE                 */
	/*                                               */
	/*************************************************/
	#define BUS_INIT							0x00
	#define BUS_INIT_NET						0x01
	#define BUS_INIT_FAULT						0x02
	#define BUS_IO		   					    0x03
	#define BUS_IO_FAULT						0x04
	#define BUS_IDLE							0x05
	/*************************************************/
	/*                                               */
	/*      W53xx Expansion Module Error Code        */
	/*                                               */
	/*************************************************/
	#define EXPASION_NO_ERROR					0x00
	#define EXPASION_COMMUNICATION_ERROR		0x01
	#define EXPASION_MODBUS_ERROR				0x02
	#define EXPASION_PENDING_ERROR				0x03
	#define EXPASION_COMBINATION_ERROR			0x04
	#define EXPASION_OUT_OF_MEMORY_ERROR		0x05
	/*************************************************/
	/*                                               */
	/*         W53xx Expansion Module State	         */
	/*                                               */
	/*************************************************/
	#define EXPASION_OFFLINE					0x00
	#define EXPASION_ONLINE						0x01
	#define EXPASION_UNMATCH_TYPE				0x02
	#define EXPASION_RETURN_MODBUS_EXCEPTION	0X03
	#define EXPASION_NETWORK_ERROR				0X04
	/*************************************************/
	/*                                               */
	/*         Active Tag Define -- nModuleType      */
	/*                                               */
	/*************************************************/
	#define ACTTAG_E2210_ID						0x2210
	#define ACTTAG_E2210_V2_ID					0x2211
	#define ACTTAG_E2212_ID						0x2212
    #define ACTTAG_E2214_ID						0x2214
    #define ACTTAG_E2240_ID						0x2240
    #define ACTTAG_E2240_V2_ID					0x2241
    #define ACTTAG_E2242_ID						0x2242
    #define ACTTAG_E2260_ID						0x2260
    #define ACTTAG_E2262_ID						0x2262

    #define ACTTAG_E1210_ID						0xE210
    #define ACTTAG_E1211_ID						0xE211
    #define ACTTAG_E1212_ID						0xE212
    #define ACTTAG_E1214_ID						0xE214
    #define ACTTAG_E1240_ID						0xE240
    #define ACTTAG_E1241_ID						0xE241
    #define ACTTAG_E1242_ID						0xE242
    #define ACTTAG_E1260_ID						0xE260
	#define ACTTAG_E1262_ID						0xE262
	#define ACTTAG_E1261_ID						0x5A

	#define ACTTAG_E2210T_ID					0x150
	#define ACTTAG_E2212T_ID					0x151
    #define ACTTAG_E2214T_ID					0x152
    #define ACTTAG_E2240T_ID					0x153
    #define ACTTAG_E2242T_ID					0x154
    #define ACTTAG_E2260T_ID					0x155
    #define ACTTAG_E2262T_ID					0x156

    #define ACTTAG_E1210T_ID					0x50
    #define ACTTAG_E1211T_ID					0x51
    #define ACTTAG_E1212T_ID					0x52
    #define ACTTAG_E1214T_ID					0x53
    #define ACTTAG_E1240T_ID					0x54
    #define ACTTAG_E1241T_ID					0x55
    #define ACTTAG_E1242T_ID					0x56
    #define ACTTAG_E1260T_ID					0x57
	#define ACTTAG_E1262T_ID					0x58
	#define ACTTAG_E1261T_ID					0x59

	#define ACTTAG_W5340_ID						0x5340
    #define ACTTAG_W5312_ID						0x5312
    #define ACTTAG_E4200_ID						0x4200

    #define ACTTAG_W5340TSLOT_ID                0x100
	#define ACTTAG_W5312TSLOT_ID                0x101
	#define ACTTAG_W5340SLOT_ID                 0x102
	#define ACTTAG_W5312SLOT_ID                 0x103
	#define ACTTAG_W5341SLOT_ID                 0x104
	#define ACTTAG_W5341TSLOT_ID                0x105

	#define ACTTAG_W5340_HSDPA_ID               0x106
    #define ACTTAG_W5312_HSDPA_ID               0x107
    #define ACTTAG_W5341_HSDPA_ID               0x108
    #define ACTTAG_W5340T_HSDPA_ID              0x109
    #define ACTTAG_W5312T_HSDPA_ID              0x10A
    #define ACTTAG_W5341T_HSDPA_ID              0x10B
    #define ACTTAG_W5340_HSDPA_JPN_ID           0x10C
    #define ACTTAG_W5340T_HSDPA_JPN_ID          0x10E
    #define ACTTAG_W5312_HSDPA_JPN_ID           0x10D
    #define ACTTAG_W5312T_HSDPA_JPN_ID          0x10F
    #define ACTTAG_W5340_HSDPA_JPS_ID           0x110
    #define ACTTAG_W5340T_HSDPA_JPS_ID          0x112
    #define ACTTAG_W5312_HSDPA_JPS_ID           0x111
    #define ACTTAG_W5312T_HSDPA_JPS_ID          0x113
    #define ACTTAG_W5340_AP_ID			        0x114

	#define ACTTAG_IOPAC_8020_T_ID		        0x180

	#define ACTTAG_IOPAC_8020_5_RJ45_C_T_ID 	0x181
	#define ACTTAG_IOPAC_8020_5_M12_C_T_ID		0x182
	#define ACTTAG_IOPAC_8020_9_RJ45_C_T_ID 	0x183
	#define ACTTAG_IOPAC_8020_9_M12_C_T_ID		0x184
	#define ACTTAG_IOPAC_8020_5_RJ45_IEC_T_ID 	0x185
	#define ACTTAG_IOPAC_8020_5_M12_IEC_T_ID	0x186
	#define ACTTAG_IOPAC_8020_9_RJ45_IEC_T_ID 	0x187
	#define ACTTAG_IOPAC_8020_9_M12_IEC_T_ID	0x188
	#define ACTTAG_IOPAC_8020_5_RJ45_C_ID		0x201
	#define ACTTAG_IOPAC_8020_5_M12_C_ID		0x202
	#define ACTTAG_IOPAC_8020_9_RJ45_C_ID		0x203
	#define ACTTAG_IOPAC_8020_9_M12_C_ID		0x204
	#define ACTTAG_IOPAC_8020_5_RJ45_IEC_ID		0x205
	#define ACTTAG_IOPAC_8020_5_M12_IEC_ID		0x206
	#define ACTTAG_IOPAC_8020_9_RJ45_IEC_ID		0x207
	#define ACTTAG_IOPAC_8020_9_M12_IEC_ID		0x208

	#define ACTTAG_E1510_ID						0x220
	#define ACTTAG_E1510_T_ID					0x221
	#define ACTTAG_E1512_ID						0x222
	#define ACTTAG_E1512_T_ID					0x223
	#define ACTTAG_E1261H_ID					0x340
    #define ACTTAG_E1261H_T_ID					0x341
    #define ACTTAG_E1263H_ID					0x342
    #define ACTTAG_E1263H_T_ID					0x343

	#define ACTTAG_E1213_ID						0x5B
	#define ACTTAG_E1213T_ID					0x5C

	#define ACTTAG_W5348_GPRS_C_ID				0x115
    #define ACTTAG_W5348_HSDPA_C_ID				0x116
    #define ACTTAG_W5348_GPRS_IEC_ID			0x117
    #define ACTTAG_W5348_HSDPA_IEC_ID			0x118
    #define ACTTAG_W5348_GPRS_C_T_ID			0x119
    #define ACTTAG_W5348_HSDPA_C_T_ID			0x11A
    #define ACTTAG_W5348_GPRS_IEC_T_ID			0x11B
    #define ACTTAG_W5348_HSDPA_IEC_T_ID			0x11C

    #define ACTTAG_AOPC_SERVER_ID   			0xA09C
    //
	#define ACTTAG_E1510_CT_T_ID				0x224
	#define ACTTAG_E1512_CT_T_ID				0x225
	#define ACTTAG_E1213_CT_ID    			    0x5D
	#define ACTTAG_E1213_CT_T_ID    			0x5E

	/*************************************************/
	/*                                               */
	/*         Active Tag Define -- BytMsgType       */
	/*                                               */
	/*************************************************/
	#define ACTTAG_MSG_POWER_ON					1
	#define ACTTAG_MSG_HEARTBEAT				2
	#define ACTTAG_MSG_CONFIG					3
	#define ACTTAG_MSG_IO_STATUS				4
	#define ACTTAG_MSG_SYSTEM					5
	/*************************************************/
	/*                                               */
	/*         Active Tag Define -- BytMsgSubType    */
	/*                                               */
	/*************************************************/
	//under ACTTAG_MSG_SYSTEM
	#define ACTTAG_MSG_SUB_HEARTBEAT_TIMEOUT	1
	#define ACTTAG_MSG_SUB_READWRITE_TIMEOUT	2
	#define ACTTAG_MSG_SUB_CLIENT_DISCONNECT	3
	#define ACTTAG_MSG_SUB_SERVER_DISCONNECT	4

	/*************************************************/
	/*                                               */
	/*         Active Tag Define -- BytChType        */
	/*                                               */
	/*************************************************/
	// 0~9=DIO mode
    #define ACTTAG_DI_DI						0
    #define ACTTAG_DO_DO						1
    #define ACTTAG_DI_CNT						2
    #define ACTTAG_DO_PULSE						3
    #define ACTTAG_DI_DI_DISABLE				4
    #define ACTTAG_DO_DO_DISABLE				5
    #define ACTTAG_RLY_DO                  		6
    #define ACTTAG_RLY_PULSE               		7

    // 10~49=AIO range
    #define ACTTAG_AI_DISABLE					10
    //AI 11
    #define ACTTAG_AI_150_150MV					11
    #define ACTTAG_AI_500_500MV					12
    #define ACTTAG_AI_5_5V						13
    #define ACTTAG_AI_10_10V					14
    #define ACTTAG_AI_0_20MA					15
    #define ACTTAG_AI_4_20MA					16
    #define ACTTAG_AI_0_150MV					17
    #define ACTTAG_AI_0_500MV					18
    #define ACTTAG_AI_0_5V						19
    #define ACTTAG_AI_0_10V						20
    //AO
    #define ACTTAG_AO_DISABLE					30
    #define ACTTAG_AO_0_10V						31
    #define ACTTAG_AO_4_20MA					32
    #define ACTTAG_AO_0_20MA					33
    #define ACTTAG_AO_10_10V					34
    #define ACTTAG_AO_0_5V						35

    // 50~99=TC
    #define ACTTAG_TC_DISABLE					50
    //Celsius (C)
    #define ACTTAG_TC_Type_K_C					51
    #define ACTTAG_TC_Type_J_C					52
    #define ACTTAG_TC_Type_T_C					53
    #define ACTTAG_TC_Type_B_C					54
    #define ACTTAG_TC_Type_R_C					55
    #define ACTTAG_TC_Type_S_C					56
    #define ACTTAG_TC_Type_E_C					57
    #define ACTTAG_TC_Type_N_C					58
    #define ACTTAG_TC_Type_L_C					59
    #define ACTTAG_TC_Type_U_C					60
    #define ACTTAG_TC_Type_C_C					61
    #define ACTTAG_TC_Type_D_C					62

    //Fahrenheit (F)
    #define ACTTAG_TC_Type_K_F					71
    #define ACTTAG_TC_Type_J_F					72
    #define ACTTAG_TC_Type_T_F					73
    #define ACTTAG_TC_Type_B_F					74
    #define ACTTAG_TC_Type_R_F					75
    #define ACTTAG_TC_Type_S_F					76
    #define ACTTAG_TC_Type_E_F					77
    #define ACTTAG_TC_Type_N_F					78
    #define ACTTAG_TC_Type_L_F					79
    #define ACTTAG_TC_Type_U_F					80
    #define ACTTAG_TC_Type_C_F					81
    #define ACTTAG_TC_Type_D_F					82

    #define ACTTAG_TC_VOLTAGE_78_126MV       	91
    #define ACTTAG_TC_VOLTAGE_39_062MV       	92
    #define ACTTAG_TC_VOLTAGE_19_532MV       	93
    #define ACTTAG_TC_VOLTAGE_78MV		     	94
    #define ACTTAG_TC_VOLTAGE_32_7MV	     	95
    #define ACTTAG_TC_VOLTAGE_65_5MV         	96

    // 100~149=RTD
    #define ACTTAG_RTD_DISABLE					100
    //Celsius (C)
    #define ACTTAG_RTD_PT50_C					101
    #define ACTTAG_RTD_PT100_C					102
    #define ACTTAG_RTD_PT200_C					103
    #define ACTTAG_RTD_PT500_C					104
    #define ACTTAG_RTD_PT1000_C					105
    #define ACTTAG_RTD_JPT100_C					106
    #define ACTTAG_RTD_JPT200_C					107
    #define ACTTAG_RTD_JPT500_C					108
    #define ACTTAG_RTD_JPT1000_C				109
    #define ACTTAG_RTD_NI100_C					110
    #define ACTTAG_RTD_NI200_C					111
    #define ACTTAG_RTD_NI500_C					112
    #define ACTTAG_RTD_NI1000_C					113
    #define ACTTAG_RTD_NI120_C					114
    #define ACTTAG_RTD_CU10_C					115

    //Fahrenheit (F)
    #define ACTTAG_RTD_PT50_F					121
    #define ACTTAG_RTD_PT100_F					122
    #define ACTTAG_RTD_PT200_F					123
    #define ACTTAG_RTD_PT500_F					124
    #define ACTTAG_RTD_PT1000_F					125
    #define ACTTAG_RTD_JPT100_F					126
    #define ACTTAG_RTD_JPT200_F					127
    #define ACTTAG_RTD_JPT500_F					128
    #define ACTTAG_RTD_JPT1000_F				129
    #define ACTTAG_RTD_NI100_F					130
    #define ACTTAG_RTD_NI200_F					131
    #define ACTTAG_RTD_NI500_F					132
    #define ACTTAG_RTD_NI1000_F					133
    #define ACTTAG_RTD_NI120_F					134
    #define ACTTAG_RTD_CU10_F					135

    #define ACTTAG_RTD_RESISTANCE_1_310_        141
    #define ACTTAG_RTD_RESISTANCE_1_620_	    142
    #define ACTTAG_RTD_RESISTANCE_1_1250_	    143
    #define ACTTAG_RTD_RESISTANCE_1_2200_	    144
    #define ACTTAG_RTD_RESISTANCE_1_2000_       145
    #define ACTTAG_RTD_RESISTANCE_1_327_        146

    //Virtual channel
    #define ACTTAG_VIRTUAL_CH_AVG_C      		201
    #define ACTTAG_VIRTUAL_CH_DIF_C      		202
    #define ACTTAG_VIRTUAL_CH_AVG_F      		203
    #define ACTTAG_VIRTUAL_CH_DIF_F      		204
    #define ACTTAG_VIRTUAL_CH_MCONNECT   		205	//slot expansion module connection
    #define ACTTAG_VIRTUAL_CH_DISABLE    		206
    #define ACTTAG_VIRTUAL_CH_VALUE      		207

    #define ACTTAG_SYSTEM_CONNECTION     		255	//SYSTEM CONNECTION TAG

    /*************************************************/
	/*                                               */
	/*         Module Sub Type Define                */
	/*                                               */
	/*************************************************/
	#define TYPE_GPRS							1
	#define TYPE_HSDPA							2
	//
	#define TYPE_WIDE_TEMP						1
	/*************************************************/
	/*                                               */
	/*         Module Series Define                  */
	/*                                               */
	/*************************************************/
	#define E4000_SERIES						1
	#define E2000_SERIES						2
	#define E4200_SERIES						4
	#define E1200_SERIES						8
	#define W5000_SERIES						16
	#define E1500_SERIES						64
	#define AOPC_SERVER 						256
	//---------------------------------------------------------------------------
	//for network Interface information
	#define 	MX_IF_ONE_IF_SIZE 							282		//
    #define		MX_IF_INDEX_NUM 							0		//SIZE:4  (DWORD)
    #define		MX_IF_INDEX_IP  							4		//SIZE:16 (STRING)
    #define		MX_IF_INDEX_MAC 							20		//SIZE:6
    #define		MX_IF_INDEX_DESC							26		//SIZE:256(STRING)
	//---------------------------------------------------------------------------
	#define 	MX_ML_MODULE_LIST_SIZE						47
	#define 	MX_ML_INDEX_WHWID							0
	#define 	MX_ML_INDEX_SZIP0							2
	#define 	MX_ML_INDEX_SZMAC0							18
	#define 	MX_ML_INDEX_SZIP1							24
	#define 	MX_ML_INDEX_SZMAC1							40
	#define 	MX_ML_INDEX_BYTLANUSE						46
	//---------------------------------------------------------------------------
#endif
