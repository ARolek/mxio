/******************************************************************************/
/*                                                                            */
/*    Example program : Activetag.cpp                                         */
/*                                                                            */
/*    Description:                                                            */
/*        1. Receive active message from ioLogik 1200 ethernet series.        */
/*        2. The message transmission can use TCP porotocol or UDP porotocol. */
/*                                                                            */
/*    List of MXIO Functions used in this example:                            */
/*        Message2K_Start, Message2K_Stop                                     */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mxio.h>
#include <unistd.h>
//====================================================================
void CALLBACK getTcp9900Message( IOLOGIKSTRUCT data[], WORD* wMutex );
void CheckErr( int iRet, char * szFunctionName );		//check function execution result
void CheckMsgType( UINT8 nType, char * szMsgTypeName, int& Len );
void CheckModuleType( WORD wHWID, char * szModuleName, int& Len );
void CheckSubMsgType( UINT16 nType, char * szMsgTypeName, int& Len );
void CheckChType( UINT8 wHWID, char * szChTypeName, int& Len );

char DbgStr[4096] = {'\0'};

#define MOXA_PRINTF( STRING)    printf( STRING)
#define CALLBACK
//====================================================================
int main()
{
    int iRet;            //stored return code
	int	iHandle=0, iHandle2=0;		//stored handle for ioLogik 2000 serial
	BYTE bytStatus;	 	//stored connection status
	char RetString[64] = {'\0'};
	DWORD dwTimeOut=5000;
	char bytInputValue[32] = {'\0'};
	char Password[8] = {'\0'};
	//==========================
	BYTE bytStartChannel = 0;
	BYTE bytCount = 8;
	double dValue[16] = {'\0'};
	WORD wValue[16] = {'\0'};
	DWORD dwValue[16] = {'\0'};
	WORD wHiValue[16] = {'\0'};
	WORD wLoValue[16] = {'\0'};
	BYTE bytValue[16] = {'\0'};
	int i, j, k, d, f, g, iCheck;
	bool ALL_DO = false;
	bool ALL_DI = false;
	WORD wType=0; //device model
	WORD wQueueSize = 1;
	char DbgStr2[6400] = {'\0'};
	WORD wTransInput = 0;
	int ToggleStatus_11 = 0;
	int ToggleStatus_41 = 0;
    //==========================
	//Active Tag Receive	
	{
		iRet = MXIO_Init_ActiveTag( 9900, 9500, 5000, 10000, getTcp9900Message, wQueueSize);
		CheckErr( iRet, (char*)"MXIO_Init_ActiveTag" );
		if(iRet == MXIO_OK)
			printf("MXIO_Init_ActiveTag() success.\r\n");
		else
		{
			printf("MXIO_Init_ActiveTag() fail.\r\n");
			return -1;
		}
		const int MAX_SUPPORT_DEVICE = 256;
		const int IP_INDEX = 0;
		const int MAC_INDEX = 4;
		ACTDEV_IO ActDevIo[MAX_SUPPORT_DEVICE] = {'\0'};
		WORD wDeviceCount = 0;
		char cMACLink_0[6] = {'\0'};
   		for( f=0; f < 6; f++)//18 * 10 secs = 180 secs
		{		
	   		usleep(10*1000*1000);    		
			{
				MXIO_ListDevs_ActiveTag( &wDeviceCount);
				printf(" ========= MXIO_ListDevs_ActiveTag() get %d Device. =========\r\n", wDeviceCount );

				//Get All Info (IP and MAC address ) that link to Active MXIO
				char * DeviceInfo = new char[wDeviceCount*10];
				iRet =MXIO_GetDevsInfo_ActiveTag( wDeviceCount, DeviceInfo);
				CheckErr( iRet, (char*)"MXIO_GetDevsInfo_ActiveTag" );
    			if(iRet == MXIO_OK)
    			{
    				printf("MXIO_GetDevsInfo_ActiveTag success,\r\n");
    				sprintf( DbgStr2, "MXIO_GetDevsInfo_ActiveTag succeed. Device Info:\n");
	        		MOXA_PRINTF( DbgStr2);
		    		for( i=0; i < wDeviceCount; i++)
					{
        				sprintf( DbgStr2, "[%d],IP=%d.%d.%d.%d, %02X-%02X-%02X-%02X-%02X-%02X\r\n", i,
        					(unsigned char)DeviceInfo[IP_INDEX+(i*10)], (unsigned char)DeviceInfo[IP_INDEX+1+(i*10)],    //IP
        					(unsigned char)DeviceInfo[IP_INDEX+2+(i*10)], (unsigned char)DeviceInfo[IP_INDEX+3+(i*10)],  //IP
        					(unsigned char)DeviceInfo[MAC_INDEX+(i*10)], (unsigned char)DeviceInfo[MAC_INDEX+1+(i*10)],  //MAC
            				(unsigned char)DeviceInfo[MAC_INDEX+2+(i*10)], (unsigned char)DeviceInfo[MAC_INDEX+3+(i*10)],//MAC
	        				(unsigned char)DeviceInfo[MAC_INDEX+4+(i*10)],(unsigned char)DeviceInfo[MAC_INDEX+5+(i*10)]);//MAC
		    			MOXA_PRINTF( DbgStr2);
					}
				}

    			//Make all list device connected
				for(k=0; k < wDeviceCount; k++)
				{
					//Check which MAC has already Connected
					for(i=0; i < MAX_SUPPORT_DEVICE; i++)
					{
    					if(ActDevIo[i].iHandle != 0)
						{
							//Check MAC is not exist.
							for(g=0, iCheck=0; g < 6; g++)
				            {
					            if(ActDevIo[i].szMAC[g] != DeviceInfo[MAC_INDEX+(k*10)+g])
						            break;
							    else
								    iCheck++;
                            }//for
	                        if(iCheck == 6)
		                    	break;
			            }
				    }//for--(i < MAX_SUPPORT_DEVICE)
					//Check is not found
                    if(i == MAX_SUPPORT_DEVICE)
	                {
		            	//Create Connection to new MAC address
			        	for(i=0; i < MAX_SUPPORT_DEVICE; i++)
				    	{
							if(ActDevIo[i].iHandle == 0)
                			{
                				for(g=0; g < 6; g++)
            						ActDevIo[i].szMAC[g] = DeviceInfo[MAC_INDEX+(k*10+g)];
	        					iRet = MXIO_Connect_ActiveTag( 5000, &ActDevIo[i].iHandle, ActDevIo[i].szMAC, 0, Password);//port and password don't care
		    					CheckErr( iRet, (char*)"MXIO_Connect_ActiveTag" );
			    				if(iRet == MXIO_OK)
								{
            						printf("MXIO_Connect_ActiveTag() [%03d]=%02X-%02X-%02X-%02X-%02X-%02X success.\r\n",k,
            							(unsigned char)ActDevIo[i].szMAC[0],(unsigned char)ActDevIo[i].szMAC[1],
										(unsigned char)ActDevIo[i].szMAC[2],(unsigned char)ActDevIo[i].szMAC[3],
										(unsigned char)ActDevIo[i].szMAC[4],(unsigned char)ActDevIo[i].szMAC[5]);
									break;
	               				}
		            		}
			        	}//for--(i < MAX_SUPPORT_DEVICE)
				    }
                }//for--(k < wDeviceCount)
				//==============================
				//Send Modbus Cmd
				WORD wType = 0;
				for(k=0; k <wDeviceCount; k++)
				{
					//Initial
					wType = 0;
					//==========================
					if(ActDevIo[k].iHandle != 0)
					{
						iRet = MXIO_GetModuleType( ActDevIo[k].iHandle, 0, &wType );
		    			CheckErr( iRet, (char*)"MXIO_GetModuleType" );
						if(iRet == MXIO_OK)
    					{
    						sprintf( DbgStr2,  "[%d] Module Type : %X ======================\n", k, wType );
    						MOXA_PRINTF( DbgStr2);
        				}
	    				else
						{
    						MXEIO_Disconnect(ActDevIo[k].iHandle);
    						ActDevIo[k].iHandle = 0;
    					}
        			}
	    			//==========================
					if(ActDevIo[k].iHandle != 0)
    				{
						BYTE bytRevision[5] = {'\0'};
						iRet = MXIO_ReadFirmwareRevision( ActDevIo[k].iHandle, bytRevision);
                    	CheckErr( iRet, (char*)"MXIO_ReadFirmwareRevision" );
	                	if(iRet == MXIO_OK)
		            	{
			        		sprintf( DbgStr2, "Firmware revision :V%01d.%01d, Release:%01d, build:%01d\r\n",bytRevision[0], bytRevision[1], bytRevision[2], bytRevision[3]);
				    		MOXA_PRINTF( DbgStr2);
						}
                    	else
	    				{
							MXEIO_Disconnect(ActDevIo[k].iHandle);
    						ActDevIo[k].iHandle = 0;
    					}
					}
                    //==========================
	                if(ActDevIo[k].iHandle != 0)
		            {
			        	WORD wGetFirmwareDate[2] = {'\0'};
				    	iRet = MXIO_ReadFirmwareDate( ActDevIo[k].iHandle, wGetFirmwareDate);
						CheckErr( iRet, (char*)"MXIO_ReadFirmwareDate" );
						if(iRet == MXIO_OK)
                    	{
	                		sprintf( DbgStr2, "Firmware Release Date:%04X/%02X/%02X\r\n",wGetFirmwareDate[1], (wGetFirmwareDate[0]>>8)&0xFF, (wGetFirmwareDate[0])&0xFF);
		            		MOXA_PRINTF( DbgStr2);
						}
						else
    					{
    						MXEIO_Disconnect(ActDevIo[k].iHandle);
    						ActDevIo[k].iHandle = 0;
        				}
					}
					//==========================
					if(ActDevIo[k].iHandle != 0)
					{
						//E1200 Series. Use E1K_XXX Function ***************
    					if((wType == 0x1210)||(wType == 0x1211)||(wType == 0x1212)||(wType == 0x1214)||(wType == 0x1240)||
							(wType == 0x1241)||(wType == 0x1242)||(wType == 0x1260)||(wType == 0x1262)||(wType == 0x1261))
						{										            								
							//
							if(wType == 0x1211 || wType == 0x1212 || wType == 0x1214 || wType == 0x1261)
							{
								DWORD dwValue[1] = {'\0'};
								if(wType == 0x1211)
									bytCount = 16;
								if(wType == 0x1212)
									bytCount = 8;
								if(wType == 0x1214)
									bytCount = 6;
								if(wType == 0x1261)
									bytCount = 12;

								bytStartChannel = 0;
								//==========================
                        		sprintf( DbgStr2, "***** Write DO channel to %s ***** : ", ToggleStatus_11?"ON":"OFF");

                               	if(ToggleStatus_11)
                               		dwValue[0] = 0xFFFFFFFF;
                               	else
                               		dwValue[0] = 0;

								ToggleStatus_11 = ToggleStatus_11?0:1;

                        		iRet = E1K_DO_Writes( ActDevIo[k].iHandle, bytStartChannel, bytCount, dwValue[0]);
                        		CheckErr( iRet, (char*)"E1K_DO_Writes" );
                        		if(iRet == MXIO_OK)
                        		{
                        			sprintf( DbgStr2, "E1K_DO_Writes success,DO %d Channels: %s\r\n", bytCount, ((dwValue[0]&0x01) >= 1)?"ON":"OFF");
                        			MOXA_PRINTF( DbgStr2);
                        		}
							}
							if(wType == 0x1241)
							{
								WORD wValue[4] = {'\0'};
								bytCount = 4;
								bytStartChannel = 0;
								//
								sprintf( DbgStr2, "***** Write AO channel to %d ***** : ", ToggleStatus_41?100:1000);
                        		MOXA_PRINTF( DbgStr2);

								for(int i=0; i < bytCount; i++ )
									wValue[i] =(ToggleStatus_41?100:1000)+500*i;

								ToggleStatus_41 = ToggleStatus_41?0:1;

								iRet = E1K_AO_WriteRaws(ActDevIo[k].iHandle, bytStartChannel, bytCount, &wValue[0]);
                        		CheckErr( iRet, (char*)"E1K_AO_WriteRaws" );
                        		if(iRet == MXIO_OK)
                        		{
                        			sprintf( DbgStr2, "E1K_AO_WriteRaws succeed.\n");
                        			MOXA_PRINTF( DbgStr2);
                        			for(int i=0; i < bytCount; i++ )
                        			{
                        				sprintf( DbgStr2, "ch[%d]=%d\n", i+bytStartChannel, wValue[i]);
                        				MOXA_PRINTF( DbgStr2);
                        			}
                        		}
								//
								/*sprintf( DbgStr2, "***** Read AO channel: *****");
                        		MOXA_PRINTF( DbgStr2);
								iRet = E1K_AO_Reads( ActDevIo[k].iHandle, bytStartChannel, bytCount, dValue);
								if(iRet == MXIO_OK)
				            	{
									for(int i=0; i < bytCount; i++ )
        							{
    									sprintf( DbgStr2, "ch[%d]=%.3f, ", i+bytStartChannel, dValue[i]);
    									MOXA_PRINTF( DbgStr2);
    								}
									MOXA_PRINTF("\n");
	        					}
		    					else
    		    				{
    								MXEIO_Disconnect(ActDevIo[k].iHandle);
    								ActDevIo[k].iHandle = 0;
    							}*/
							}
    						if(wType == 0x1242)
							{
								//
								DWORD dwValue = 0;
								bytCount = 4;
								bytStartChannel = 0;
								iRet = E1K_DI_Reads( ActDevIo[k].iHandle, bytStartChannel, bytCount, &dwValue);
								if(iRet == MXIO_OK)
				            	{
									for(int i=0; i < bytCount; i++ )
        							{
    									sprintf( DbgStr2, "ch[%d]=%s, ", i+bytStartChannel, (dwValue&(1 << (i-bytStartChannel)))?"ON":"OFF");
    									MOXA_PRINTF( DbgStr2);
    								}
									MOXA_PRINTF("\n");
	        					}
		    					else
			    				{
    								MXEIO_Disconnect(ActDevIo[k].iHandle);
    								ActDevIo[k].iHandle = 0;
    							}
							}
						}
    					//E2000 Series. Use E2K_XXX Function ***************
						if((wType == 0x2210)||(wType == 0x2212)||(wType == 0x2214)||(wType == 0x2240)||(wType == 0x2242)||
							(wType == 0x2260)||(wType == 0x2262))
						{
							if(iRet == MXIO_OK)
					    	{
								//sprintf( DbgStr2, "Firmware Release Date:%04X/%02X/%02X\r\n",wGetFirmwareDate[1], (wGetFirmwareDate[0]>>8)&0xFF, (wGetFirmwareDate[0])&0xFF);
                        		//MOXA_PRINTF( DbgStr2);
	    					}
							else
							{
    							MXEIO_Disconnect(ActDevIo[k].iHandle);
    							ActDevIo[k].iHandle = 0;
    						}
						}
    					//W5000 Series. Use W5K_XXX Function ***************
						if((wType == 0x5312)||(wType == 0x5340))
						{
							if(iRet == MXIO_OK)
				        	{
					    		//sprintf( DbgStr2, "Firmware Release Date:%04X/%02X/%02X\r\n",wGetFirmwareDate[1], (wGetFirmwareDate[0]>>8)&0xFF, (wGetFirmwareDate[0])&0xFF);
								//MOXA_PRINTF( DbgStr2);
    						}
        					else
	        				{
								MXEIO_Disconnect(ActDevIo[k].iHandle);
    							ActDevIo[k].iHandle = 0;
    						}
						}
					}
            		//==========================
				}//for--(k < wDeviceCount)
				//
    			delete [] DeviceInfo;				
			}
		}
		iRet = MXIO_Close_ActiveTag( );
		CheckErr( iRet, (char*)"MXIO_Close_ActiveTag" );
		if(iRet == MXIO_OK)
			printf("MXIO_Close_ActiveTag() success.\r\n");		 
	}
    return 0;
}
//====================================================================
void CALLBACK getTcp9900Message( IOLOGIKSTRUCT data[], WORD* wMutex )
{
	sprintf( DbgStr,  "Recv Active Tag!***********************\n");
    MOXA_PRINTF( DbgStr);

	int i, g, f, iSlot;
	if(data != NULL)
	{
		f=0;
		CheckModuleType( data->wHWID, &DbgStr[f], f);
		CheckMsgType( data->BytMsgType, &DbgStr[f], f);

		f += sprintf( &DbgStr[f],"IP:%d.%d.%d.%d, ",
			(data->dwSrcIP&0x000000FF),((data->dwSrcIP&0x0000FF00)>>8),
			((data->dwSrcIP&0x00FF0000)>>16),((data->dwSrcIP&0xFF000000)>>24));
		f += sprintf( &DbgStr[f],"MAC:%02X-%02X-%02X-%02X-%02X-%02X, ",
			data->BytSrcMAC[0],data->BytSrcMAC[1],data->BytSrcMAC[2],
			data->BytSrcMAC[3],data->BytSrcMAC[4],data->BytSrcMAC[5]);
		f += sprintf( &DbgStr[f],"Time:%d/%02d/%02d %02d:%02d:%02d.%03d\n",
    			data->wYear,data->BytMonth,data->BytDay,
    			data->BytHour,data->BytMin,data->BytSec,data->wMSec);


		MOXA_PRINTF(DbgStr);
		if(data->BytMsgType == ACTTAG_MSG_SYSTEM)
		{
			f=0;
			CheckSubMsgType( data->wMsgSubType, &DbgStr[f], f);
			MOXA_PRINTF( DbgStr);
		}
		else if(data->BytMsgType == ACTTAG_MSG_HEARTBEAT)
		{}
		else if(data->BytMsgType == ACTTAG_MSG_CONFIG || (data->BytMsgType == ACTTAG_MSG_IO_STATUS))
		{
			if((data->BytMsgType == ACTTAG_MSG_CONFIG))
			{
				f=0;
				f += sprintf( &DbgStr[f],"HeartBeat Timeout Seconds=%d\n", data->wMsgSubType);
    			MOXA_PRINTF( DbgStr);
				//
        		sprintf( &DbgStr[0],"Channel Type:\n    ");
    			MOXA_PRINTF( DbgStr);
        		for( iSlot=0, f=0; iSlot < (data->BytLastSlot+1); iSlot++)
        		{
            		for( g=0; g < data->BytLastCh[iSlot]; g++)
            		{
            			if((g!=0) && ((g%2)==0))
        	    			f += sprintf( &DbgStr[f],"\n    ");
            			f += sprintf( &DbgStr[f],"Ch%02d=", g );
            			CheckChType( data->BytChType[iSlot][g], &DbgStr[f], f);
            		}
            		f += sprintf( &DbgStr[f],"\n");
            		MOXA_PRINTF( DbgStr);
            	}
            	//bitwised
            	f=0;
            	f += sprintf( &DbgStr[f],"Channel Update Active Tag: 0=> Disable, 1=> Enable\n    ");
        		for( iSlot=0; iSlot < (data->BytLastSlot+1); iSlot++)
        		{
            		for( g=0; g < data->BytLastCh[iSlot]; g++)
            		{
            			if((g!=0) && ((g%4)==0))
        	    			f += sprintf( &DbgStr[f],"\n    ");
            			f += sprintf( &DbgStr[f],"Ch%02d=%s,  ", g, (data->BytChNumber[iSlot][g/8]&(1<<(g%8)))?" Enable":"Disable");
            		}
            		f += sprintf( &DbgStr[f],"\n");
            		MOXA_PRINTF( DbgStr);
            	}
            	//bitwised
            	f=0;
        		f += sprintf( &DbgStr[f],"Channel Lock Mode(Click&GO used): 0=> Unlocked, 1=> Locked\n    ");
            	for( iSlot=0; iSlot < (data->BytLastSlot+1); iSlot++)
        		{
            		for( g=0; g < data->BytLastCh[iSlot]; g++)
            		{
            			if((g!=0) && ((g%4)==0))
        	    			f += sprintf( &DbgStr[f],"\n    ");
            			f += sprintf( &DbgStr[f],"Ch%02d=%s,  ", g, (data->BytChLocked[iSlot][g/8]&(1<<(g%8)))?"Locked  ":"Unlocked");
            		}
            		f += sprintf( &DbgStr[f],"\n");
            		MOXA_PRINTF( DbgStr);
            	}
            }
    		//
			f=0;
    		f += sprintf( &DbgStr[f],"Channel Value: 0=> Off, 1=> On\n    ");
        	for( iSlot=0; iSlot < (data->BytLastSlot+1); iSlot++)
    		{
        		for( g=0; g < data->BytLastCh[iSlot]; g++)
        		{
        			if((g!=0) && ((g%4)==0))
    	    			f += sprintf( &DbgStr[f],"\n    ");
    	    		if( (data->BytChType[iSlot][g] == ACTTAG_DI_DI_DISABLE) ||
    	    			(data->BytChType[iSlot][g] == ACTTAG_DO_DO_DISABLE) ||
    	    			(data->BytChType[iSlot][g] == ACTTAG_AI_DISABLE) ||
            			(data->BytChType[iSlot][g] == ACTTAG_AO_DISABLE) ||
            			(data->BytChType[iSlot][g] == ACTTAG_TC_DISABLE) ||
            			(data->BytChType[iSlot][g] == ACTTAG_RTD_DISABLE) ||
            			(data->BytChType[iSlot][g] == ACTTAG_VIRTUAL_CH_MCONNECT) ||
            			(data->BytChType[iSlot][g] == ACTTAG_VIRTUAL_CH_DISABLE))
    	    			f += sprintf( &DbgStr[f],"Ch%02d= None,   ", g);
    				else if( data->BytChType[iSlot][g] < ACTTAG_AI_DISABLE)
        				f += sprintf( &DbgStr[f],"Ch%02d=%5d,   ", g, data->dwChValue[iSlot][g].iVal);
    				else
    					f += sprintf( &DbgStr[f],"Ch%02d=%.3f,   ", g, data->dwChValue[iSlot][g].fVal);
        		}
        		f += sprintf( &DbgStr[f],"\n");
				MOXA_PRINTF( DbgStr);
			}
		}
		else
		{
			sprintf( &DbgStr[0],"Undefine Message type: %d\n", data->BytMsgType);
        	MOXA_PRINTF( DbgStr);
		}
	}
	else
	{
		sprintf( &DbgStr[0],"Data = NULL, bypass\n");
        MOXA_PRINTF( DbgStr);
	}
	//Release Mutex
	*wMutex = 0;
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
		default:
			szErrMsg = "Unknown Error Value";
			break;
		}

		sprintf( DbgStr,  "Function \"%s\" execution Fail. Error Message : %s (%d)\n", szFunctionName, szErrMsg, iRet );
		MOXA_PRINTF( DbgStr);
	}
}

void CheckMsgType( UINT8 nType, char * szMsgTypeName, int& Len )
{
	switch(nType)
	{
		case ACTTAG_MSG_POWER_ON:
			Len += sprintf( szMsgTypeName,"nType=%s\n", "MSG_POWER_ON");
			break;
		case ACTTAG_MSG_HEARTBEAT:
			Len += sprintf( szMsgTypeName,"nType=%s\n", "MSG_HEARTBEAT(2)");
			break;
		case ACTTAG_MSG_CONFIG:
			Len += sprintf( szMsgTypeName,"nType=%s\n", "MSG_CONFIG(3)");
			break;
		case ACTTAG_MSG_IO_STATUS:
			Len += sprintf( szMsgTypeName,"nType=%s\n", "MSG_IO_STATUS(4)");
			break;
		case ACTTAG_MSG_SYSTEM:
			Len += sprintf( szMsgTypeName,"nType=%s\n", "MSG_SYSTEM");
			break;
		default:
			Len += sprintf( szMsgTypeName,"nType=%d, Unknowm Value\n", nType);
	}
}

void CheckSubMsgType( UINT16 nType, char * szMsgTypeName, int& Len )
{
	switch(nType)
	{
		case ACTTAG_MSG_SUB_HEARTBEAT_TIMEOUT:
			Len += sprintf( szMsgTypeName,"SubType=%s\n", "ACTTAG_MSG_SUB_HEARTBEAT_TIMEOUT");
			break;
		case ACTTAG_MSG_SUB_READWRITE_TIMEOUT:
			Len += sprintf( szMsgTypeName,"SubType=%s\n", "ACTTAG_MSG_SUB_READWRITE_TIMEOUT");
			break;
		case ACTTAG_MSG_SUB_CLIENT_DISCONNECT:
			Len += sprintf( szMsgTypeName,"SubType=%s\n", "ACTTAG_MSG_SUB_CLIENT_DISCONNECT");
			break;
		case ACTTAG_MSG_SUB_SERVER_DISCONNECT:
			Len += sprintf( szMsgTypeName,"SubType=%s\n", "ACTTAG_MSG_SUB_SERVER_DISCONNECT");
			break;
		default:
			Len += sprintf( szMsgTypeName,"SubType=%d, Unknowm Value\n", nType);
	}
}

void CheckModuleType( WORD wHWID, char * szModuleName, int& Len )
{
	switch(wHWID)
	{
		case ACTTAG_E2210_ID:
		case ACTTAG_E2210_V2_ID:
		case ACTTAG_E2210T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2210");
			break;
		case ACTTAG_E2212_ID:
		case ACTTAG_E2212T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2212");
			break;
		case ACTTAG_E2214_ID:
		case ACTTAG_E2214T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2214");
			break;
		case ACTTAG_E2240_ID:
		case ACTTAG_E2240_V2_ID:
		case ACTTAG_E2240T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2240");
			break;
		case ACTTAG_E2242_ID:
		case ACTTAG_E2242T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2242");
			break;
		case ACTTAG_E2260_ID:
		case ACTTAG_E2260T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2260");
			break;
		case ACTTAG_E2262_ID:
		case ACTTAG_E2262T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E2262");
			break;
		case ACTTAG_W5340_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5340");
			break;
		case ACTTAG_W5312_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5312");
			break;
		case ACTTAG_E4200_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E4200");
			break;
		case ACTTAG_E1210_ID:
		case ACTTAG_E1210T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1210");
			break;
		case ACTTAG_E1211_ID:
		case ACTTAG_E1211T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1211");
			break;
		case ACTTAG_E1212_ID:
		case ACTTAG_E1212T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1212");
			break;
		case ACTTAG_E1214_ID:
		case ACTTAG_E1214T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1214");
			break;
		case ACTTAG_E1240_ID:
		case ACTTAG_E1240T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1240");
			break;
		case ACTTAG_E1241_ID:
		case ACTTAG_E1241T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1241");
			break;
		case ACTTAG_E1242_ID:
		case ACTTAG_E1242T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1242");
			break;
		case ACTTAG_E1260_ID:
		case ACTTAG_E1260T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1260");
			break;
		case ACTTAG_E1261_ID:
		case ACTTAG_E1261T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1261");
			break;
		case ACTTAG_E1262_ID:
		case ACTTAG_E1262T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "E1262");
			break;
		case ACTTAG_W5340SLOT_ID:
		case ACTTAG_W5340TSLOT_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5340Slot");
			break;
		case ACTTAG_W5312SLOT_ID:
		case ACTTAG_W5312TSLOT_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5312Slot");
			break;
		case ACTTAG_W5341SLOT_ID:
			case ACTTAG_W5341TSLOT_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5341Slot");
			break;
		case ACTTAG_W5340_HSDPA_ID:
		case ACTTAG_W5340T_HSDPA_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5340-HSDPA");
			break;
		case ACTTAG_W5312_HSDPA_ID:
		case ACTTAG_W5312T_HSDPA_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5312-HSDPA");
			break;
		case ACTTAG_W5341_HSDPA_ID:
		case ACTTAG_W5341T_HSDPA_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5341-HSDPA");
			break;
		case ACTTAG_W5340_HSDPA_JPN_ID:
		case ACTTAG_W5340T_HSDPA_JPN_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5340-JPN-HSDPA");
			break;
		case ACTTAG_W5312_HSDPA_JPN_ID:
		case ACTTAG_W5312T_HSDPA_JPN_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5312-JPN-HSDPA");
			break;
		case ACTTAG_W5340_HSDPA_JPS_ID:
		case ACTTAG_W5340T_HSDPA_JPS_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5340-JPS-HSDPA");
			break;
		case ACTTAG_W5312_HSDPA_JPS_ID:
		case ACTTAG_W5312T_HSDPA_JPS_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "W5312-JPS-HSDPA");
			break;
		case ACTTAG_IOPAC_8020_T_ID:
			Len += sprintf( szModuleName,"nModuleType=%s\n", "ioPAC-8020T");
			break;
		default:
			Len += sprintf( szModuleName,"nModuleType=0x%X, Unknowm Module Type\n", wHWID);
	}
}

void CheckChType( UINT8 wHWID, char * szChTypeName, int& Len )
{
	switch(wHWID)
	{
		case ACTTAG_DI_DI:
			Len += sprintf( szChTypeName,"ChType=%s,", "DI_DI                 ");
			break;
		case ACTTAG_DO_DO:
			Len += sprintf( szChTypeName,"ChType=%s,", "DO_DO                 ");
			break;
		case ACTTAG_DI_CNT:
			Len += sprintf( szChTypeName,"ChType=%s,", "DI_CNT                ");
			break;
		case ACTTAG_DO_PULSE:
			Len += sprintf( szChTypeName,"ChType=%s,", "DO_PULSE              ");
			break;
		case ACTTAG_DI_DI_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "DI_DI_DISABLE         ");
			break;
		case ACTTAG_DO_DO_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "DO_DO_DISABLE         ");
			break;
		case ACTTAG_AI_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_DISABLE            ");
			break;
		case ACTTAG_AI_150_150MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_150_150MV          ");
			break;
		case ACTTAG_AI_500_500MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_500_500MV          ");
			break;
		case ACTTAG_AI_5_5V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_5_5V               ");
			break;
		case ACTTAG_AI_10_10V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_10_10V             ");
			break;
		case ACTTAG_AI_0_20MA:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_0_20MA             ");
			break;
		case ACTTAG_AI_4_20MA:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_4_20MA             ");
			break;
		case ACTTAG_AI_0_150MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_0_150MV            ");
			break;
		case ACTTAG_AI_0_500MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_0_500MV            ");
			break;
		case ACTTAG_AI_0_5V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_0_5V               ");
			break;
		case ACTTAG_AI_0_10V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AI_0_10V              ");
			break;
		case ACTTAG_AO_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "AO_DISABLE            ");
			break;
		case ACTTAG_AO_0_10V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AO_0_10V              ");
			break;
		case ACTTAG_AO_4_20MA:
			Len += sprintf( szChTypeName,"ChType=%s,", "AO_4_20MA             ");
			break;
		case ACTTAG_AO_0_20MA:
			Len += sprintf( szChTypeName,"ChType=%s,", "AO_0_20MA             ");
			break;
		case ACTTAG_AO_10_10V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AO_10_10V             ");
			break;
		case ACTTAG_AO_0_5V:
			Len += sprintf( szChTypeName,"ChType=%s,", "AO_0_5V               ");
			break;
		case ACTTAG_TC_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_DISABLE            ");
			break;
		case ACTTAG_TC_Type_K_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_K_C           ");
			break;
		case ACTTAG_TC_Type_J_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_J_C           ");
			break;
		case ACTTAG_TC_Type_T_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_T_C           ");
			break;
		case ACTTAG_TC_Type_B_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_B_C           ");
			break;
		case ACTTAG_TC_Type_R_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_R_C           ");
			break;
		case ACTTAG_TC_Type_S_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_S_C           ");
			break;
		case ACTTAG_TC_Type_E_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_E_C           ");
			break;
		case ACTTAG_TC_Type_N_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_N_C           ");
			break;
		case ACTTAG_TC_Type_L_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_L_C           ");
			break;
		case ACTTAG_TC_Type_U_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_U_C           ");
			break;
		case ACTTAG_TC_Type_C_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_C_C           ");
			break;
		case ACTTAG_TC_Type_D_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_D_C           ");
			break;
		case ACTTAG_TC_Type_K_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_K_F           ");
			break;
		case ACTTAG_TC_Type_J_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_J_F           ");
			break;
		case ACTTAG_TC_Type_T_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_T_F           ");
			break;
		case ACTTAG_TC_Type_B_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_B_F           ");
			break;
		case ACTTAG_TC_Type_R_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_R_F           ");
			break;
		case ACTTAG_TC_Type_S_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_S_F           ");
			break;
		case ACTTAG_TC_Type_E_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_E_F           ");
			break;
		case ACTTAG_TC_Type_N_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_N_F           ");
			break;
		case ACTTAG_TC_Type_L_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_L_F           ");
			break;
		case ACTTAG_TC_Type_U_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_U_F           ");
			break;
		case ACTTAG_TC_Type_C_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_C_F           ");
			break;
		case ACTTAG_TC_Type_D_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_Type_D_F           ");
			break;
		case ACTTAG_TC_VOLTAGE_78_126MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_VOLTAGE_78_126MV   ");
			break;
		case ACTTAG_TC_VOLTAGE_39_062MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_VOLTAGE_39_062MV   ");
			break;
		case ACTTAG_TC_VOLTAGE_19_532MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_VOLTAGE_19_532MV   ");
			break;
		case ACTTAG_TC_VOLTAGE_78MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_VOLTAGE_78MV       ");
			break;
		case ACTTAG_TC_VOLTAGE_32_7MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_VOLTAGE_32_7MV     ");
			break;
		case ACTTAG_TC_VOLTAGE_65_5MV:
			Len += sprintf( szChTypeName,"ChType=%s,", "TC_VOLTAGE_65_5MV     ");
			break;
		case ACTTAG_RTD_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_DISABLE           ");
			break;
		case ACTTAG_RTD_PT50_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT50_C            ");
			break;
		case ACTTAG_RTD_PT100_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT100_C           ");
			break;
		case ACTTAG_RTD_PT200_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT200_C           ");
			break;
		case ACTTAG_RTD_PT500_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT500_C           ");
			break;
		case ACTTAG_RTD_PT1000_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT1000_C          ");
			break;
		case ACTTAG_RTD_JPT100_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT100_C          ");
			break;
		case ACTTAG_RTD_JPT200_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT200_C          ");
			break;
		case ACTTAG_RTD_JPT500_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT500_C          ");
			break;
		case ACTTAG_RTD_JPT1000_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT1000_C         ");
			break;
		case ACTTAG_RTD_NI100_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI100_C           ");
			break;
		case ACTTAG_RTD_NI200_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI200_C           ");
			break;
		case ACTTAG_RTD_NI500_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI500_C           ");
			break;
		case ACTTAG_RTD_NI1000_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI1000_C          ");
			break;
		case ACTTAG_RTD_NI120_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI120_C           ");
			break;
		case ACTTAG_RTD_CU10_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_CU10_C            ");
			break;
		case ACTTAG_RTD_PT50_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT50_F            ");
			break;
		case ACTTAG_RTD_PT100_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT100_F           ");
			break;
		case ACTTAG_RTD_PT200_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT200_F           ");
			break;
		case ACTTAG_RTD_PT500_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT500_F           ");
			break;
		case ACTTAG_RTD_PT1000_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_PT1000_F          ");
			break;
		case ACTTAG_RTD_JPT100_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT100_F          ");
			break;
		case ACTTAG_RTD_JPT200_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT200_F          ");
			break;
		case ACTTAG_RTD_JPT500_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT500_F          ");
			break;
		case ACTTAG_RTD_JPT1000_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_JPT1000_F         ");
			break;
		case ACTTAG_RTD_NI100_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI100_F           ");
			break;
		case ACTTAG_RTD_NI200_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI200_F           ");
			break;
		case ACTTAG_RTD_NI500_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI500_F           ");
			break;
		case ACTTAG_RTD_NI1000_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI1000_F          ");
			break;
		case ACTTAG_RTD_NI120_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_NI120_F           ");
			break;
		case ACTTAG_RTD_CU10_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_CU10_F            ");
			break;
		case ACTTAG_RTD_RESISTANCE_1_310_:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_RESISTANCE_1_310_ ");
			break;
		case ACTTAG_RTD_RESISTANCE_1_620_:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_RESISTANCE_1_620_ ");
			break;
		case ACTTAG_RTD_RESISTANCE_1_1250_:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_RESISTANCE_1_1250_");
			break;
		case ACTTAG_RTD_RESISTANCE_1_2200_:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_RESISTANCE_1_2200_");
			break;
		case ACTTAG_RTD_RESISTANCE_1_2000_:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_RESISTANCE_1_2000_");
			break;
		case ACTTAG_RTD_RESISTANCE_1_327_:
			Len += sprintf( szChTypeName,"ChType=%s,", "RTD_RESISTANCE_1_327_ ");
			break;
		case ACTTAG_VIRTUAL_CH_AVG_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_AVG_C      ");
			break;
		case ACTTAG_VIRTUAL_CH_DIF_C:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_DIF_C      ");
			break;
		case ACTTAG_VIRTUAL_CH_AVG_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_AVG_F      ");
			break;
		case ACTTAG_VIRTUAL_CH_DIF_F:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_DIF_F      ");
			break;
		case ACTTAG_VIRTUAL_CH_MCONNECT:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_MCONNECT   ");
			break;
		case ACTTAG_VIRTUAL_CH_DISABLE:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_DISABLE    ");
			break;
		case ACTTAG_VIRTUAL_CH_VALUE:
			Len += sprintf( szChTypeName,"ChType=%s,", "VIRTUAL_CH_VALUE      ");
			break;
		case ACTTAG_SYSTEM_CONNECTION:
			Len += sprintf( szChTypeName,"ChType=%s,", "SYSTEM_CONNECTION     ");
			break;
		default:
			Len += sprintf( szChTypeName,"ChType=%s,", "Unknown               ");
			break;
	}
}
