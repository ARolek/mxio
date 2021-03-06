package mxio

import "C"
import "net"

type DeviceType C.int

const (
	E4000_SERIES     = DeviceType(1)
	E2000_SERIES     = DeviceType(2)
	E4200_SERIES     = DeviceType(4)
	E1200_SERIES     = DeviceType(8)
	W5000_SERIES     = DeviceType(16)
	E1500_SERIES     = DeviceType(64)
	IOPac8000_SERIES = DeviceType(128)
	AOPC_SERVER      = DeviceType(256)

	ALL_DEVICES = DeviceType(511)
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

func (d Device) LanIP() net.IP {
	if d.LanUse == Lan0 {
		return d.IP
	}
	return d.IP1
}

func (d Device) LanMAC() net.HardwareAddr {
	if d.LanUse == Lan0 {
		return d.MAC
	}
	return d.MAC1
}
