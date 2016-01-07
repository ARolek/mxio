package main

import (
	"encoding/json"
	"errors"
	"flag"
	"fmt"
	"net"
	"os"
	"strings"

	"github.com/arolek/mxio"
)

var dtype = flag.String("device", "all", "Valid types: e4000, e2000, e4200, e1200, w5000, e1500, iopac8K, aopc")
var nif string

func init_interface() {
	ifaces, err := net.Interfaces()
	errout(err, 1)
	var nets []string
	var def string

	for _, iface := range ifaces {
		nets = append(nets, iface.Name)
		if def == "" && (iface.Flags&(net.FlagUp|net.FlagBroadcast)) != 0 {
			def = iface.Name
		}
	}
	flag.StringVar(&nif, "interface", def, "Valid interfaces are: "+strings.Join(nets, ", "))
}

func deviceTypeByName(name string) (mxio.DeviceType, error) {
	n := strings.ToLower(name)
	switch n {
	case "all":
		return mxio.ALL_DEVICES, nil
	case "e4000":
		return mxio.E4000_SERIES, nil
	case "e2000":
		return mxio.E2000_SERIES, nil
	case "e4200":
		return mxio.E4200_SERIES, nil
	case "e1200":
		return mxio.E1200_SERIES, nil
	case "w5000":
		return mxio.W5000_SERIES, nil
	case "e1500":
		return mxio.E1500_SERIES, nil
	case "iopac8000":
		return mxio.IOPac8000_SERIES, nil
	case "aopc":
		return mxio.AOPC_SERVER, nil
	}
	return mxio.ALL_DEVICES, errors.New(fmt.Sprintf("Unknown device type(%s)", n))
}
func errout(e error, c int) {
	if e != nil {
		fmt.Println(e)
		os.Exit(c)
	}
}

type jsonDevice struct {
	Name string `json:"name"`
	IP   string `json:"ip"`
	MAC  string `json:"mac"`
	IP1  string `json:"ip1"`
	MAC1 string `json:"mac1"`
	Lan  string `json:"lan"`
}

func newJsonDevice(d mxio.Device) jsonDevice {
	var ip, ip1 string
	if d.IP != nil {
		ip = d.IP.String()
	}
	if d.IP1 != nil {
		ip1 = d.IP1.String()
	}
	return jsonDevice{
		Name: d.ID.String(),
		IP:   ip,
		MAC:  d.MAC.String(),
		IP1:  ip1,
		MAC1: d.MAC1.String(),
		Lan:  d.LanUse.String(),
	}
}

func main() {
	init_interface()
	flag.Parse()
	var dmap = make(map[string]interface{})

	dt, err := deviceTypeByName(*dtype)
	ifn, err := net.InterfaceByName(nif)
	errout(err, 2)
	m := mxio.Mxio{
		IF:         ifn,
		DeviceType: dt,
	}
	d, err := m.AutoSearch()
	errout(err, 3)
	dmap["count"] = len(d)
	dl := make([]jsonDevice, 0, len(d))
	for _, dd := range d {
		dl = append(dl, newJsonDevice(dd))
	}
	dmap["devices"] = dl

	enc := json.NewEncoder(os.Stdout)
	errout(enc.Encode(dmap), 5)
}
