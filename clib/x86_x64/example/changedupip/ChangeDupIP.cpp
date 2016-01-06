/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.
*/
// ChangeDupIP.cpp : Defines the entry point for the console application.
//

#include "stdio.h"
#include "stdlib.h"
#include "mxio.h"
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//=========================================================================
#define Port		502						//Modbus TCP port
//=========================================================================
void CheckErr( int iRet, char * szFunctionName );		//check function execution result
void CheckModuleType( WORD wHWID, char * szModuleName, int& Len );
//=========================================================================
int main(int   argc,   char   *argv[]){

	int iRet;
	DWORD dwTimeOut=5000;		
	char Password[8] = {'\0'};
	char EthIFName[] = "eth0";
	//==========================
    iRet = MXIO_GetDllVersion();
	printf("MXIO_GetDllVersion DLL Version:%01d.%01d.%01d.%01d\r\n",(iRet>>12)&0xF, (iRet>>8)&0xF,(iRet>>4)&0xF,(iRet)&0xF);
	//==========================
    iRet = MXIO_GetDllBuildDate();
	printf("MXIO_GetDllBuildDate DLL release date:%04X/%02X/%02X\r\n",(iRet>>16)&0xFFFF, (iRet>>8)&0xFF, (iRet)&0xFF);
	//==========================
	iRet = MXEIO_Init();
	CheckErr( iRet, (char*)"MXEIO_Init" );
	if(iRet == MXIO_OK)
		printf("MXEIO_Init Initiate the socket succeed.\n");
	//=========================================================================
	MODULE_LIST Module_List[256] = {'\0'};
	int iDeviceCount = 0, i=0, f=0, h=0;
	WORD wIFCount = 0;
	iRet = MXIO_ListIF( &wIFCount);
	CheckErr( iRet, (char*)"MXIO_ListIF" );
	if(iRet == MXIO_OK)
	{
		printf("MXIO_ListIF() success. %d network Interface.\n", wIFCount);
		char *IFInfo = NULL;
		DWORD IF_INDEX = 0;
		if(wIFCount)
			IFInfo = new char[wIFCount*MX_IF_ONE_IF_SIZE];
		//
		iRet = MXIO_GetIFInfo( wIFCount, IFInfo);
		CheckErr( iRet, (char*)"MXIO_GetIFInfo" );
		if(iRet == MXIO_OK)
		{
			printf("MXIO_GetIFInfo() success. %d network Interface.\n", wIFCount);
			for(i = 0; i < wIFCount; i++)
			{
    	    	/* Show the device name and IP address */
    			printf("[%d] %s, %02x:%02x:%02x:%02x:%02x:%02x, %s\n",
    		        IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_NUM], &IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_IP],
    			    (unsigned char)IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_MAC],(unsigned char)IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_MAC+1],
    			    (unsigned char)IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_MAC+2], (unsigned char)IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_MAC+3],
    			    (unsigned char)IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_MAC+4],(unsigned char)IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_MAC+5],
    				&IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_DESC]);
    			//check which one network IF that we want.//save user specific IF information
				if(!(memcmp(&IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_DESC], EthIFName, strlen(EthIFName))))
					IF_INDEX = IFInfo[i*MX_IF_ONE_IF_SIZE+MX_IF_INDEX_NUM];    			
    		}
		}
		//
		if(IFInfo != NULL)
		{
    		iRet = MXIO_SelectIF(wIFCount, IFInfo, IF_INDEX);
    		CheckErr( iRet, (char*)"MXIO_SelectIF" );
    		if(iRet == MXIO_OK)
    			printf("MXIO_SelectIF() success. select Index = %d network Interface.\n", IF_INDEX);
    		//
			delete [] IFInfo;
		}
	}
	//
	iRet = MXIO_AutoSearch( E1200_SERIES, 2, 2000, &iDeviceCount, (char*)Module_List);
	CheckErr( iRet, (char*)"MXIO_AutoSearch" );
	if(iRet == MXIO_OK)
	{
		printf( "Found %d device!\n", iDeviceCount);
		
		//Cfeck duplicate IP address
		unsigned char MACAddress0[6] = {'\0'};
		unsigned char MACAddress1[6] = {'\0'};
		char cmpIPAddress0[16] = {"\0"};
		char cmpIPAddress1[16] = {"\0"};
		char DbgStr[128] = {'\0'};
		for( i=0; i < iDeviceCount; i++)
		{
			if(Module_List[i].bytLanUse)
			{
				memcpy(MACAddress0, Module_List[i].szMAC1, 6);
				strcpy(cmpIPAddress0, Module_List[i].szModuleIP1);
			}
			else
			{
				memcpy(MACAddress0, Module_List[i].szMAC, 6);
				strcpy(cmpIPAddress0, Module_List[i].szModuleIP);
			}
			CheckModuleType( Module_List[i].nModuleID, &DbgStr[0], h);			
			printf( "[%d]=%s,%s,%02X-%02X-%02X-%02X-%02X-%02X\n", i, DbgStr, cmpIPAddress0,
				MACAddress0[0],MACAddress0[1],
				MACAddress0[2],MACAddress0[3],
				MACAddress0[4],MACAddress0[5]);
			
			//
			if((i+1) < iDeviceCount)
			{				
				for( f=i+1; f < iDeviceCount; f++)
    			{
    				if(Module_List[f].bytLanUse)
    				{
    					memcpy(MACAddress1, Module_List[f].szMAC1, 6);
    					strcpy(cmpIPAddress1, Module_List[f].szModuleIP1);						
    				}
    				else
    				{
    					memcpy(MACAddress1, Module_List[f].szMAC, 6);
    					strcpy(cmpIPAddress1, Module_List[f].szModuleIP);
    				}					
    				//Compare
    				if(!(strcmp(cmpIPAddress0, cmpIPAddress1)))
    				{
    					printf( "Found duplicate IP in [%d] and [%d] IP=%s\n", i, f, cmpIPAddress0);
    					
    					printf( "[%d] MAC=%02X-%02X-%02X-%02X-%02X-%02X, [%d] MAC=%02X-%02X-%02X-%02X-%02X-%02X\n",
    						i, 	MACAddress0[0], MACAddress0[1], MACAddress0[2],
    							MACAddress0[3],	MACAddress0[4], MACAddress0[5],
    						f, 	MACAddress1[0], MACAddress1[1], MACAddress1[2],
    							MACAddress1[3], MACAddress1[4], MACAddress1[5]);
    					
    					//
    					char newIPAddress[16] = "192.168.123.123";
                    	iRet = MXIO_ChangeDupIP(  cmpIPAddress0,	 //IP address
                           						  Port,				 //TCP port number
                           						  (char*)MACAddress0,//MAC address
                           						  dwTimeOut,		 //timeout
                           						  Password,			 //login password
                           						  newIPAddress);	 //new IP address
                    	CheckErr( iRet, (char*)"MXIO_ChangeDupIP" );
                    	if(iRet == MXIO_OK)
                    	{
                    		printf("MXIO_ChangeDupIP() success and device restarted.\n");
                    	}
    				}
    			}//for
    		}
		}
	}	
	//=========================================================================
	MXEIO_Exit();
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
		case SLOT_NOT_EXIST:
			szErrMsg = "SLOT_NOT_EXIST";
			break;
		case FIRMWARE_NOT_SUPPORT:
			szErrMsg = "FIRMWARE_NOT_SUPPORT";
			break; 
		case CREATE_MUTEX_FAIL:
			szErrMsg = "CREATE_MUTEX_FAIL";
			break; 
		case ENUM_NET_INTERFACE_FAIL:
			szErrMsg = "ENUM_NET_INTERFACE_FAIL";
			break; 
		case ADD_INFO_TABLE_FAIL:
			szErrMsg = "ADD_INFO_TABLE_FAIL";
			break; 
		case UNKNOWN_NET_INTERFACE_FAIL:
			szErrMsg = "UNKNOWN_NET_INTERFACE_FAIL";
			break; 
		case TABLE_NET_INTERFACE_FAIL:
			szErrMsg = "TABLE_NET_INTERFACE_FAIL";
			break; 			
		default:
			szErrMsg = "Unknown Error Value";
			break;
		}
		printf( "Function \"%s\" execution Fail. Error Message : %s\n", szFunctionName, szErrMsg );
	}
}
void CheckModuleType( WORD wHWID, char * szModuleName, int& Len )
{
	int i=0;
	typedef struct _HWID_NAME{
		WORD wHWIDNum;
		const char* szMName;
	}HWID_NAME, *pHWID_NAME;

	HWID_NAME wHWIDArray[] = {
		{ACTTAG_E1210_ID,			"E1210"},
		{ACTTAG_E1210T_ID,			"E1210-T"},
		{ACTTAG_E1211_ID,			"E1211"},
		{ACTTAG_E1211T_ID,			"E1211-T"},
		{ACTTAG_E1212_ID,			"E1212"},
		{ACTTAG_E1212T_ID,			"E1212-T"},
		{ACTTAG_E1214_ID,			"E1214"},
		{ACTTAG_E1214T_ID,			"E1214-T"},
		{ACTTAG_E1240_ID,			"E1240"},
		{ACTTAG_E1240T_ID,			"E1240-T"},
		{ACTTAG_E1241_ID,			"E1241"},
		{ACTTAG_E1241T_ID,			"E1241-T"},
		{ACTTAG_E1242_ID,			"E1242"},
		{ACTTAG_E1242T_ID,			"E1242-T"},
		{ACTTAG_E1260_ID,			"E1260"},
		{ACTTAG_E1260T_ID,			"E1260-T"},
		{ACTTAG_E1262_ID,			"E1262"},
		{ACTTAG_E1262T_ID,			"E1262-T"},
		{ACTTAG_E1261_ID,			"E1261-WP"},
		{ACTTAG_E1261T_ID,			"E1261-WP-T"},
		{ACTTAG_E1213_ID,			"E1213"},
		{ACTTAG_E1213T_ID,			"E1213-T"},
		//
		{ACTTAG_E2210_ID,			"E2210"},
		{ACTTAG_E2210_V2_ID,		"E2210"},
		{ACTTAG_E2210T_ID,			"E2210-T"},
		{ACTTAG_E2212_ID,			"E2212"},
		{ACTTAG_E2212T_ID,			"E2212-T"},
		{ACTTAG_E2214_ID,			"E2214"},
		{ACTTAG_E2214T_ID,			"E2214-T"},
		{ACTTAG_E2240_ID,			"E2240"},
		{ACTTAG_E2240_V2_ID,		"E2240"},
		{ACTTAG_E2240T_ID,			"E2240-T"},
		{ACTTAG_E2242_ID,			"E2242"},
		{ACTTAG_E2242T_ID,			"E2242-T"},
		{ACTTAG_E2260_ID,			"E2260"},
		{ACTTAG_E2260T_ID,			"E2260-T"},
		{ACTTAG_E2262_ID,			"E2262"},
		{ACTTAG_E2262T_ID,			"E2262-T"},
		//
		{ACTTAG_W5340_ID,			"W5340"},
		{ACTTAG_W5312_ID,			"W5312"},
		{ACTTAG_W5340SLOT_ID,		"W5340"},
		{ACTTAG_W5340TSLOT_ID,		"W5340-T"},
		{ACTTAG_W5312SLOT_ID,		"W5312"},
		{ACTTAG_W5312TSLOT_ID,		"W5312-T"},
		{ACTTAG_W5341SLOT_ID,		"W5341"},
		{ACTTAG_W5341TSLOT_ID,		"W5341-T"},
		{ACTTAG_W5340_HSDPA_ID,		"W5340-HSDPA"},
		{ACTTAG_W5340T_HSDPA_ID,	"W5340-HSDPA-T"},
		{ACTTAG_W5312_HSDPA_ID,		"W5312-HSDPA"},
		{ACTTAG_W5312T_HSDPA_ID,	"W5312-HSDPA-T"},
		{ACTTAG_W5341_HSDPA_ID,		"W5341-HSDPA"},
		{ACTTAG_W5341T_HSDPA_ID,	"W5341-HSDPA-T"},
		{ACTTAG_W5340_HSDPA_JPN_ID,	"W5340-JPN-HSDPA"},
		{ACTTAG_W5340T_HSDPA_JPN_ID,"W5340-JPN-HSDPA-T"},
		{ACTTAG_W5340_HSDPA_JPS_ID,	"W5340-JPS-HSDPA"},
		{ACTTAG_W5340T_HSDPA_JPS_ID,"W5340-JPS-HSDPA-T"},
		{ACTTAG_W5312_HSDPA_JPN_ID,	"W5312-JPN-HSDPA"},
		{ACTTAG_W5312T_HSDPA_JPN_ID,"W5312-JPN-HSDPA-T"},
		{ACTTAG_W5312_HSDPA_JPS_ID,	"W5312-JPS-HSDPA"},
		{ACTTAG_W5312T_HSDPA_JPS_ID,"W5312-JPS-HSDPA-T"},
		//
		{ACTTAG_E4200_ID,			"E4200"},
		//
		{ACTTAG_E1510_ID,			"E1510"},
		{ACTTAG_E1510_T_ID,			"E1510-T"},
		{ACTTAG_E1512_ID,			"E1512"},
		{ACTTAG_E1512_T_ID,			"E1512-T"},
		{ACTTAG_E1261H_ID,			"E1261H"},
		{ACTTAG_E1261H_T_ID,		"E1261H-T"},
		{ACTTAG_E1263H_ID,			"E1263H"},
		{ACTTAG_E1263H_T_ID,		"E1263H-T"},
		//
		{ACTTAG_IOPAC_8020_T_ID					,	"ioPAC-8020-T"},
		//PGM
		{ACTTAG_IOPAC_8020_5_RJ45_C_T_ID 	    ,	"ioPAC-8020-5-RJ45-C-T"},
		{ACTTAG_IOPAC_8020_5_M12_C_T_ID		    ,	"ioPAC-8020-5-M12-C-T"},
		{ACTTAG_IOPAC_8020_9_RJ45_C_T_ID 	    ,	"ioPAC-8020-9-RJ45-C-T"},
		{ACTTAG_IOPAC_8020_9_M12_C_T_ID		    ,	"ioPAC-8020-9-M12_C_T"},
		{ACTTAG_IOPAC_8020_5_RJ45_IEC_T_ID 	    ,	"ioPAC-8020-5-RJ45-IEC-T"},
		{ACTTAG_IOPAC_8020_5_M12_IEC_T_ID	    ,	"ioPAC-8020-5-M12-IEC-T"},
		{ACTTAG_IOPAC_8020_9_RJ45_IEC_T_ID 	    ,	"ioPAC-8020-9-RJ45-IEC-T"},
		{ACTTAG_IOPAC_8020_9_M12_IEC_T_ID	    ,	"ioPAC-8020-9-M12-IEC-T"},
		{ACTTAG_IOPAC_8020_5_RJ45_C_ID		    ,	"ioPAC-8020-5-RJ45-C"},
		{ACTTAG_IOPAC_8020_5_M12_C_ID		    ,	"ioPAC-8020-5-M12-C"},
		{ACTTAG_IOPAC_8020_9_RJ45_C_ID		    ,	"ioPAC-8020-9-RJ45-C"},
		{ACTTAG_IOPAC_8020_9_M12_C_ID		    ,	"ioPAC-8020-9-M12-C"},
		{ACTTAG_IOPAC_8020_5_RJ45_IEC_ID		,	"ioPAC-8020-5-RJ45-IEC"},
		{ACTTAG_IOPAC_8020_5_M12_IEC_ID		    ,	"ioPAC-8020-5-M12-IEC"},
		{ACTTAG_IOPAC_8020_9_RJ45_IEC_ID		,	"ioPAC-8020-9-RJ45-IEC"},
		{ACTTAG_IOPAC_8020_9_M12_IEC_ID		    ,	"ioPAC-8020-9-M12-IEC"},
		//
		{ACTTAG_W5348_GPRS_C_ID					,   "W5348-GPRS-C"},
		{ACTTAG_W5348_HSDPA_C_ID				,   "W5348-HSDPA-C"},
		{ACTTAG_W5348_GPRS_IEC_ID				,   "W5348-GPRS-IEC"},
		{ACTTAG_W5348_HSDPA_IEC_ID				,   "W5348-HSDPA-IEC"},
		{ACTTAG_W5348_GPRS_C_T_ID				,   "W5348-GPRS-C-T"},
		{ACTTAG_W5348_HSDPA_C_T_ID				,   "W5348-HSDPA-C-T"},
		{ACTTAG_W5348_GPRS_IEC_T_ID				,   "W5348-GPRS-IEC-T"},
		{ACTTAG_W5348_HSDPA_IEC_T_ID			,	"W5348-HSDPA-IEC-T"},
	};

	for(i=0; i < (sizeof(wHWIDArray)/sizeof(wHWIDArray[0])); i++)
	{
		if(wHWID == wHWIDArray[i].wHWIDNum)
		{
			Len += sprintf( szModuleName,"%s", wHWIDArray[i].szMName);
			break;
		}
	}
	if(i >= (sizeof(wHWIDArray)/sizeof(wHWIDArray[0])))
		Len += sprintf( szModuleName,":Unknown ID(0x%X)\n", wHWID);
}
