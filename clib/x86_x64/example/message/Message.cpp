/******************************************************************************/
/*                                                                            */
/*    Example program : Message.cpp                                           */
/*                                                                            */
/*    Description:                                                            */
/*        1. Receive active message from ioLogik 2000 ethernet series.        */
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

void getTcpMessage( BYTE data[], WORD wSize );          //TCP call back function
void getUdpMessage( BYTE data[], WORD wSize );          //UDP call back function

void CheckErr( int iRet, char * szFunctionName );       //check function execution result

int main()
{
    int iRet;            //stored return code

    //********************************
    // start TCP & UDP active message
    //********************************
    iRet = Message2K_Start( PROTOCOL_TCP,               //start TCP message
                            9000,                       //TCP port number
                            getTcpMessage );            //call back functioin

    CheckErr( iRet, (char*)"Message2K_Start" );

    printf("Start to receive active TCP message.\n");

    iRet = Message2K_Start( PROTOCOL_UDP,               //start UDP message
                            9000,                       //UDP port number
                            getUdpMessage );            //call back function

    CheckErr( iRet, (char*)"Message2K_Start" );

    printf("Start to receive active UDP message.\n");

    sleep( 20 );

    Message2K_Stop( PROTOCOL_TCP );        //stop TCP message

    printf("Stop to receive active TCP message.\n");

    Message2K_Stop( PROTOCOL_UDP );        //stop UDP message

    printf("Stop to receive active UDP message.\n");

    return 0;
}

void getTcpMessage( BYTE data[], WORD wSize )
{
    //data[wSize] = 0;
    printf( "TCP Message : %s\n", data );
}

void getUdpMessage( BYTE data[], WORD wSize )
{
    //data[wSize] = 0;
    printf( "UDP Message : %s\n", data );
}

//  After each MXIO function call, the application checks whether the call succeeded.
//  If the call failed, this procedure prints an error message and exits.
void CheckErr( int iRet, char * szFunctionName )
{
    if( iRet != MXIO_OK )
    {
        const char * szErrMsg;

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
        }

        printf( "Function \"%s\" execution Fail. Error Message : %s\n", szFunctionName, szErrMsg );

        exit(1);
    }
}
