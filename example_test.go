package mxio

import (
	"fmt"
	"net"

	"github.com/ARolek/mxio"
)

func ExampleAutoSearch() {
	// This will get all the devices on the first up and broadcastable interface.
	devices, err := mxio.AutoSearch()
	if err != nil {
		// Handle error correctly.
		fmt.Printf("Could not find devices because of %v\n", err)
		return
	}
	fmt.Printf("Found %v devices:\n", len(devices))
	for i, d := range devices {
		fmt.Printf("Device %v :\n%v\n", i, d)
	}
}

func ExampleAutoSearch_Interface() {
	// Search on the first Interface, for only the E1200_SERIES of devices
	m := mxio.Mxio{
		IF:         net.Interfaces()[0],
		DeviceType: mxio.E1200_SERIES,
	}
	devices, err := m.AutoSearch()
	if err != nil {
		// Handle error correctly.
		fmt.Printf("Could not find devices because of %v\n", err)
		return
	}
	fmt.Printf("Found %v devices:\n", len(devices))
	for i, d := range devices {
		fmt.Printf("Device %v :\n%v\n", i, d)
	}
}
