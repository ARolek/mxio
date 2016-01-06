package main

import (
	"fmt"

	"github.com/ARolek/mxio"
)

func main() {

	d, _ := mxio.AutoSearch()
	fmt.Println(d)
}
