// +build linux,cgo
package mxio

// #cgo CFLAGS: -I/usr/local/include
// #cgo LDFLAGS: ${SRCDIR}/clib/linux/libmxio_x64.a
// #cgo LDFLAGS: -L${SRCDIR}/clib/linux -lstdc++ -lpthread -lmxio_x64
/*
#include "gomxio.h"
*/
import "C"
import (
	"fmt"
	"net"
	"unsafe"
)

// cMAC2NetMAC converts a c char array to a hardward address.
func cMAC2NetMAC(m [6]C.char) (net.HardwareAddr, error) {
	return net.ParseMAC(
		fmt.Sprintf(
			"%02X:%02X:%02X:%02X:%02X:%02X",
			uint8(m[0]),
			uint8(m[2]),
			uint8(m[2]),
			uint8(m[3]),
			uint8(m[4]),
			uint8(m[5]),
		),
	)
}

// AutoSearch Will find the given device type on the given network.
func (m *Mxio) AutoSearch() ([]Device, error) {
	var c, e C.int
	var ifinfo *C.char
	if err := m.init(); err != nil {
		return nil, err
	}
	ml := C.autoSearch(C.int(m.Retry), C.int(m.Timeout), C.int(m.DeviceType), C.int(m.IF.Index), ifinfo, &c, &e)
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
		devices[i].ID = deviceid(d.nModuleID)
		devices[i].IP = net.ParseIP(C.GoString(&(d.szModuleIP[0])))
		devices[i].MAC, _ = cMAC2NetMAC(d.szMAC)
		devices[i].IP1 = net.ParseIP(C.GoString(&(d.szModuleIP1[0])))
		devices[i].MAC1, _ = cMAC2NetMAC(d.szMAC1)
		devices[i].LanUse = lan(d.bytLanUse)
	}
	return devices, nil
}
