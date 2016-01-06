package mxio

import (
	"errors"
	"net"
)

type Mxio struct {
	// The interface to use. You can use net.Interfaces() to get a list of
	// interfaces in the system.
	IF *net.Interface
	// The device types to search for. The default value is to search for
	// all device types.
	DeviceType DeviceType
}

// Finds all devices types on the first network interface that is up and can Boradcast.
func AutoSearch() ([]Device, error) {
	var m Mxio
	return m.AutoSearch()
}

// init — we just want to make sure that the default values are good values.
// As the zero values don't make sense, this make the zero values acceptable values.
// If a network interface to use is not provided, we use the first device that is
// ip and able to Broadcast. If not devices are found we return an error of NoInterfaces.
func (m *Mxio) init() error {
	if m == nil {
		return errors.New("Mxio can not be nil!")
	}
	if m.DeviceType == DeviceType(0) {
		m.DeviceType = ALL_DEVICES
	}
	if m.IF == nil {
		interfaces, err := net.Interfaces()
		if len(interfaces) == 0 {
			return NoInterfaces
		}
		if err != nil {
			return err
		}
		for _, ifc := range interfaces {
			if (ifc.Flags & (net.FlagUp | net.FlagBroadcast)) != 0 {
				m.IF = &ifc
				break
			}
		}
		if m.IF == nil {
			return NoInterfaces
		}
	}
	return nil
}
