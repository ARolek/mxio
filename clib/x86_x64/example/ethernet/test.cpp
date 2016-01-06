/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
// test.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include "mxio.h"
#include <sys/time.h>

#define IPAddr		"192.168.127.254" 		//IP address for your ioLogik Device
#define Port		502						//Modbus TCP port


#define MOXA_PRINTF( STRING )	printf( STRING )

void CheckErr( int iRet, char * szFunctionName );		//check function execution result

char DbgStr[1024] = {'\0'};

int main(int   argc,   char   *argv[])
{
	int iRet;			//stored return code
	int	iHandle, iHandle2;		//stored handle for ioLogik 2000 serial
	BYTE bytStatus;	 	//stored connection status
	char RetString[64];
	char IPAddress[16] = {'\0'};
	DWORD dwTimeOut=5000;
	char bytInputValue[32] = {'\0'};
	char Password[8] = {'\0'};
	//char Password[8] = {0x31, 0x32, 0x33, 0x34, 0, 0, 0, 0};
	//==========================
	BYTE bytStartChannel = 0;
	BYTE bytCount = 8;
	double dValue[16] = {'\0'};
	WORD wValue[16] = {'\0'};
	DWORD dwValue[16] = {'\0'};
	WORD wHiValue[16] = {'\0'};
	WORD wLoValue[16] = {'\0'};
	BYTE bytValue[16] = {'\0'};
	int k;
	bool ALL_DO = false;
	bool ALL_DI = false;
	//=========================================================================
	//=========================================================================
	iRet = MXEIO_Init();
	CheckErr( iRet, (char*)"MXEIO_Init" );
	if(iRet == MXIO_OK)
	{
		sprintf( DbgStr, "MXEIO_Init Initiate the socket succeed.\n");
		MOXA_PRINTF( DbgStr);
	}
	int count;
        int nnumber;
        MODULE_LIST structML[];
        // 255 selects all devices except the AOPC_SERVER if we want to include that use 511
        iRet = MXIO_AutoSearch(255,3,5000,&nnumber, (char*)&structML);
        CheckErr( iRet, (char *)"MXIO_AutoSearch");
        if( iRet == MXIO_OK ){
        	printf("\n\ncount: %d, ip: %s\n\n",nnumber,structML[0].szModuleIP);
	}

	//==========================
	sprintf( DbgStr, "***** Press enter to Exit ***** : ");
	MOXA_PRINTF( DbgStr);
    gets(bytInputValue);
}
//  After each MXIO function call, the application checks whether the call succeed.
//  If a MXIO function call fails, return an error code.
//  If the call failed, this procedure prints an error message and exits.
void CheckErr( int iRet, char * szFunctionName )
{
	const char * szErrMsg;

	if( iRet != MXIO_OK )
	{

		switch( iRet )
		{
		case ILLEGAL_FUNCTION:
			szErrMsg = "ILLEGAL_FUNCTION";
			break;
		case ILLEGAL_DATA_ADDRESS:
			szErrMsg = "ILLEGAL_DATA_ADDRESS";
			break;
		case ILLEGAL_DATA_VALUE:
			szErrMsg = "ILLEGAL_DATA_VALUE";
			break;
		case SLAVE_DEVICE_FAILURE:
			szErrMsg = "SLAVE_DEVICE_FAILURE";
			break;
		case SLAVE_DEVICE_BUSY:
			szErrMsg = "SLAVE_DEVICE_BUSY";
			break;
		case EIO_TIME_OUT:
			szErrMsg = "EIO_TIME_OUT";
			break;
		case EIO_INIT_SOCKETS_FAIL:
			szErrMsg = "EIO_INIT_SOCKETS_FAIL";
			break;
		case EIO_CREATING_SOCKET_ERROR:
			szErrMsg = "EIO_CREATING_SOCKET_ERROR";
			break;
		case EIO_RESPONSE_BAD:
			szErrMsg = "EIO_RESPONSE_BAD";
			break;
		case EIO_SOCKET_DISCONNECT:
			szErrMsg = "EIO_SOCKET_DISCONNECT";
			break;
		case PROTOCOL_TYPE_ERROR:
			szErrMsg = "PROTOCOL_TYPE_ERROR";
			break;
		case SIO_OPEN_FAIL:
			szErrMsg = "SIO_OPEN_FAIL";
			break;
		case SIO_TIME_OUT:
			szErrMsg = "SIO_TIME_OUT";
			break;
		case SIO_CLOSE_FAIL:
			szErrMsg = "SIO_CLOSE_FAIL";
			break;
		case SIO_PURGE_COMM_FAIL:
			szErrMsg = "SIO_PURGE_COMM_FAIL";
			break;
		case SIO_FLUSH_FILE_BUFFERS_FAIL:
			szErrMsg = "SIO_FLUSH_FILE_BUFFERS_FAIL";
			break;
		case SIO_GET_COMM_STATE_FAIL:
			szErrMsg = "SIO_GET_COMM_STATE_FAIL";
			break;
		case SIO_SET_COMM_STATE_FAIL:
			szErrMsg = "SIO_SET_COMM_STATE_FAIL";
			break;
		case SIO_SETUP_COMM_FAIL:
			szErrMsg = "SIO_SETUP_COMM_FAIL";
			break;
		case SIO_SET_COMM_TIME_OUT_FAIL:
			szErrMsg = "SIO_SET_COMM_TIME_OUT_FAIL";
			break;
		case SIO_CLEAR_COMM_FAIL:
			szErrMsg = "SIO_CLEAR_COMM_FAIL";
			break;
		case SIO_RESPONSE_BAD:
			szErrMsg = "SIO_RESPONSE_BAD";
			break;
		case SIO_TRANSMISSION_MODE_ERROR:
			szErrMsg = "SIO_TRANSMISSION_MODE_ERROR";
			break;
		case PRODUCT_NOT_SUPPORT:
			szErrMsg = "PRODUCT_NOT_SUPPORT";
			break;
		case HANDLE_ERROR:
			szErrMsg = "HANDLE_ERROR";
			break;
		case SLOT_OUT_OF_RANGE:
			szErrMsg = "SLOT_OUT_OF_RANGE";
			break;
		case CHANNEL_OUT_OF_RANGE:
			szErrMsg = "CHANNEL_OUT_OF_RANGE";
			break;
		case COIL_TYPE_ERROR:
			szErrMsg = "COIL_TYPE_ERROR";
			break;
		case REGISTER_TYPE_ERROR:
			szErrMsg = "REGISTER_TYPE_ERROR";
			break;
		case FUNCTION_NOT_SUPPORT:
			szErrMsg = "FUNCTION_NOT_SUPPORT";
			break;
		case OUTPUT_VALUE_OUT_OF_RANGE:
			szErrMsg = "OUTPUT_VALUE_OUT_OF_RANGE";
			break;
		case INPUT_VALUE_OUT_OF_RANGE:
			szErrMsg = "INPUT_VALUE_OUT_OF_RANGE";
			break;
		case EIO_PASSWORD_INCORRECT:
			szErrMsg = "EIO_PASSWORD_INCORRECT";
			break;
		}

		sprintf( DbgStr,  "Function \"%s\" execution Fail. Error Message : %s\n", szFunctionName, szErrMsg );
		MOXA_PRINTF( DbgStr);
	}
}
