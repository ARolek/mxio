// +build linux,cgo
package mxio

// #cgo CFLAGS: -I/usr/local/include
// #cgo LDFLAGS: clib/linux/libmxio_x64.a
// #cgo LDFLAGS: -Lclib/linux -lstdc++ -lpthread -lmxio_x64
/*
#include "gomxio.h"
*/
import "C"

import (
	"errors"
	"fmt"
	"net"
	"unsafe"
)

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

type Device struct {
	ID     uint16
	IP     net.IP
	MAC    net.HardwareAddr
	IP1    net.IP
	MAC1   net.HardwareAddr
	LanUse lan
}

var NoInterfaces = errors.New("No interfaces found.")

type Mxio struct {
	IF         *net.Interface
	DeviceType deviceType
}

func AutoSearch() ([]Device, error) {
	var m Mxio
	return m.AutoSearch()
}

func (m *Mxio) init() error {
	if m == nil {
		return errors.New("Mxio can not be nil!")
	}
	if m.DeviceType == deviceType(0) {
		m.DeviceType = deviceType(511)
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
	}
	return nil
}

// AutoSearch
func (m *Mxio) AutoSearch() ([]Device, error) {
	var c, e C.int
	var ifinfo *C.char
	if err := m.init(); err != nil {
		return nil, err
	}

	ml := C.autoSearch(C.int(m.DeviceType), C.int(m.IF.Index), ifinfo, &c, &e)
	if ml == nil {
		err := errorType(e)
		if err == ok {
			return nil, nil
		}
		return nil, err
	}
	defer C.free(unsafe.Pointer(ml))
	count := int(c)
	mlSlice := (*[1 << 30]C.MODULE_LIST)(unsafe.Pointer(ml))[:count:256]

	devices := make([]Device, count, count)
	for i, d := range mlSlice {

		devices[i].ID = uint16(d.nModuleID)
		devices[i].IP = net.ParseIP(C.GoString(&(d.szModuleIP[0])))
		devices[i].MAC, _ = net.ParseMAC(
			fmt.Sprintf(
				"%02X:%02X:%02X:%02X:%02X:%02X",
				uint8(d.szMAC[0]),
				uint8(d.szMAC[1]),
				uint8(d.szMAC[2]),
				uint8(d.szMAC[3]),
				uint8(d.szMAC[4]),
				uint8(d.szMAC[5]),
			),
		)
		devices[i].IP1 = net.ParseIP(C.GoString(&(d.szModuleIP1[0])))
		devices[i].MAC1, _ = net.ParseMAC(
			fmt.Sprintf(
				"%02X:%02X:%02X:%02X:%02X:%02X",
				uint8(d.szMAC1[0]),
				uint8(d.szMAC1[1]),
				uint8(d.szMAC1[2]),
				uint8(d.szMAC1[3]),
				uint8(d.szMAC1[4]),
				uint8(d.szMAC1[5]),
			),
		)
		devices[i].LanUse = lan(d.bytLanUse)
	}
	return devices, nil
}
