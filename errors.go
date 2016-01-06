package mxio

type errorType int

const (
	ok = errorType(0)

	ILLEGAL_FUNCTION     = errorType(1001)
	ILLEGAL_DATA_ADDRESS = errorType(1002)
	ILLEGAL_DATA_VALUE   = errorType(1003)
	SLAVE_DEVICE_FAILURE = errorType(1004)
	SLAVE_DEVICE_BUSY    = errorType(1006)

	EIO_TIME_OUT              = errorType(2001)
	EIO_INIT_SOCKETS_FAIL     = errorType(2002)
	EIO_CREATING_SOCKET_ERROR = errorType(2003)
	EIO_RESPONSE_BAD          = errorType(2004)
	EIO_SOCKET_DISCONNECT     = errorType(2005)
	PROTOCOL_TYPE_ERROR       = errorType(2006)
	EIO_PASSWORD_INCORRECT    = errorType(2007)

	SIO_OPEN_FAIL               = errorType(3001)
	SIO_TIME_OUT                = errorType(3002)
	SIO_CLOSE_FAIL              = errorType(3003)
	SIO_PURGE_COMM_FAIL         = errorType(3004)
	SIO_FLUSH_FILE_BUFFERS_FAIL = errorType(3005)
	SIO_GET_COMM_STATE_FAIL     = errorType(3006)
	SIO_SET_COMM_STATE_FAIL     = errorType(3007)
	SIO_SETUP_COMM_FAIL         = errorType(3008)
	SIO_SET_COMM_TIME_OUT_FAIL  = errorType(3009)
	SIO_CLEAR_COMM_FAIL         = errorType(3010)
	SIO_RESPONSE_BAD            = errorType(3011)
	SIO_TRANSMISSION_MODE_ERROR = errorType(3012)
	SIO_BAUDRATE_NOT_SUPPORT    = errorType(3013)

	PRODUCT_NOT_SUPPORT       = errorType(4001)
	HANDLE_ERROR              = errorType(4002)
	SLOT_OUT_OF_RANGE         = errorType(4003)
	CHANNEL_OUT_OF_RANGE      = errorType(4004)
	COIL_TYPE_ERROR           = errorType(4005)
	REGISTER_TYPE_ERROR       = errorType(4006)
	FUNCTION_NOT_SUPPORT      = errorType(4007)
	OUTPUT_VALUE_OUT_OF_RANGE = errorType(4008)
	INPUT_VALUE_OUT_OF_RANGE  = errorType(4009)
	SLOT_NOT_EXIST            = errorType(4010)
	FIRMARE_NOT_SUPPORT       = errorType(4011)
	CREATE_MUTEX_FAIL         = errorType(4012)

	ENUM_NET_INTERFACE_FAIL    = errorType(5000)
	ADD_INFO_TABLE_FAIL        = errorType(5001)
	UNKNOWN_NET_INTERFACE_FAIL = errorType(5002)
	TABLE_NET_INTERFACE_FAIL   = errorType(5003)
)

func (err errorType) Error() string {
	switch err {
	case ok:
		return "No Error: Funciton call was successful."
	case ILLEGAL_FUNCTION:
		return "Illegal Function: The function code received in the query is not an allowable action for the server (or slave)."
	case ILLEGAL_DATA_ADDRESS:
		return "Illegal Data Address: The data address received in the query is not an allowable address for the server (or slave)."
	case ILLEGAL_DATA_VALUE:
		return "Illegal Data Value: A value contained in the query data field is not an allowable value for the server (or slave)."
	case SLAVE_DEVICE_FAILURE:
		return "Slave Device Failure: An unrecoverable error occurred while the server (or slave) was attempting to perform the requested action."
	case SLAVE_DEVICE_BUSY:
		return "Slave Device Busy: Specialized use in conjuction with programming commands. The server (or slave) is engaged in processing a long-duration program command. The client (or master) should retransmit the message alter when the server (or slave) is free"

	case EIO_TIME_OUT:
		return "EIO Time Out: The following situation may cause an EIO_TIME_OUT: 1. Open Socket timeout. 2. Send command to the I/O Server timeout. 3. I/O Server Response timeout."
	case EIO_INIT_SOCKETS_FAIL:
		return "EIO Init Sockets Fail: The error occurred when the Windows system couldn't complete SOCKET INIT."
	case EIO_CREATING_SOCKET_ERROR:
		return "EIO Creating Socket Error: The error occurred when the Windows system couldn't initiate the Socket."
	case EIO_RESPONSE_BAD:
		return "EIO Response Bad: The data received from Ethernet I/O server is incorrect."
	case EIO_SOCKET_DISCONNECT:
		return "EIO Socket Disconnect: The network connection from host computer is down."
	case PROTOCOL_TYPE_ERROR:
		return "Protocol Type Error."
	case EIO_PASSWORD_INCORRECT:
		return "EIO Password Incorrect"
	case SIO_OPEN_FAIL:
		return "SIO Open Fail: Open COM port Fail."
	case SIO_TIME_OUT:
		return "SIO Time Out: Unable to communicate to the COM port in the designated time."
	case SIO_CLOSE_FAIL:
		return "SIO Close Fail: Unable to close the COM port."
	case SIO_PURGE_COMM_FAIL:
		return "SIO Purge Comm Fail: Purge COM port error"
	case SIO_FLUSH_FILE_BUFFERS_FAIL:
		return "SIO Flush File Buffers Fail: Flush File Buffers error"
	case SIO_GET_COMM_STATE_FAIL:
		return "SIO Get Comm State Fail: Get COM port Status error"
	case SIO_SET_COMM_STATE_FAIL:
		return "SIO Set Comm State Fail: Set COM Port Status error"
	case SIO_SETUP_COMM_FAIL:
		return "SIO Setup Comm Fail: Setup COM port error"
	case SIO_SET_COMM_TIME_OUT_FAIL:
		return "SIO Set Comm Time Out Fail: Set cOM port read timeout and write timeout fail."
	case SIO_CLEAR_COMM_FAIL:
		return "SIO Clear Comm Fail: Clear COM port"
	case SIO_RESPONSE_BAD:
		return "SIO Response Bad: The data received from the Serial I/O server is incorrect"
	case SIO_TRANSMISSION_MODE_ERROR:
		return "SIO Transmission Mode Error: Modbus transmission parameter error while calling MXSIO_Connect()."
	case SIO_BAUDRATE_NOT_SUPPORT:
		return "SIO Baudrate Not Support: Serial Communication baudrate not supported."

	case PRODUCT_NOT_SUPPORT:
		return "Product Not Support: The I/O module is not supported by this version of MXIO DLL."
	case HANDLE_ERROR:
		return "Handle Error"
	case SLOT_OUT_OF_RANGE:
		return "Slot Out Of Range"
	case CHANNEL_OUT_OF_RANGE:
		return "Channel Out Of Range"
	case COIL_TYPE_ERROR:
		return "Coil Type Error"
	case REGISTER_TYPE_ERROR:
		return "Register Type Error"
	case FUNCTION_NOT_SUPPORT:
		return "Function Not Support: Function is not supported for designated I/O module."
	case OUTPUT_VALUE_OUT_OF_RANGE:
		return "Output Value Out Of Range: The output value is out of the output range."
	case INPUT_VALUE_OUT_OF_RANGE:
		return "Input Value Out Of Range: The input value is out of the input range."
	case SLOT_NOT_EXIST:
		return "Slot Not Exist: The slot does not exist."
	case FIRMARE_NOT_SUPPORT:
		return "Firmare Not Support: The device firmware does not support this function."
	case CREATE_MUTEX_FAIL:
		return "Create Mutex Fail: Active Message create mutex failed."
	case ENUM_NET_INTERFACE_FAIL:
		return "Enum Net Interface Fail: Enum network interfae failed."
	case ADD_INFO_TABLE_FAIL:
		return "Add Info Table Fail: Failed to add information from the route or the arp table."
	case UNKNOWN_NET_INTERFACE_FAIL:
		return "Unknown Net Interface Fail: Failed to select the network interface."
	case TABLE_NET_INTERFACE_FAIL:
		return "Table Net Interface Fail: Failed to remove infromation from the route or the arp table."

	}
	return "Unknown error."
}
