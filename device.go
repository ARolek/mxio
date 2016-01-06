package mxio

import "C"
import "net"

type deviceType C.int

const (
	E4000_SERIES     = deviceType(1)
	E2000_SERIES     = deviceType(2)
	E4200_SERIES     = deviceType(4)
	E1200_SERIES     = deviceType(8)
	W5000_SERIES     = deviceType(16)
	E1500_SERIES     = deviceType(64)
	IOPac8000_SERIES = deviceType(128)
	AOPC_SERVER      = deviceType(256)

	ALL_DEVICES = deviceType(511)
)

type lan uint8

const (
	Lan0 = lan(0)
	Lan1 = lan(1)
)

func (l lan) String() string {
	switch l {
	case Lan0:
		return "Lan0"
	case Lan1:
		return "Lan1"
	default:
		return "Unknown"
	}
}

// Device describes a device that could be on the network.
type Device struct {
	// This is the ID of the device.
	ID deviceid
	// The is the IP of the first network port on the device
	IP net.IP
	// This is the MAC address of the frist network port.
	MAC net.HardwareAddr
	// IP of the second network port.
	IP1 net.IP
	// MAC address of the second network port.
	MAC1 net.HardwareAddr
	// Which lan port is in use. These can be the valus Lan0 or Lan1
	LanUse lan
}
