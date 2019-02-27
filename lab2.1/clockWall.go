package main

import (
	"io"
	"log"
	"net"
	"os"
	"strings"
)

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}

func main() {

	p1 := strings.Split(os.Args[1], "=")

	p2 := strings.Split(os.Args[2], "=")

	p3 := strings.Split(os.Args[3], "=")

	con1, err1 := net.Dial("tcp", p1[1])
	if err1 != nil {
		log.Fatal(err1)
	}
	con2, err2 := net.Dial("tcp", p2[1])
	if err2 != nil {
		log.Fatal(err2)
	}
	con3, err3 := net.Dial("tcp", p3[1])
	if err3 != nil {
		log.Fatal(err3)
	}
	defer con1.Close()
	defer con2.Close()
	defer con3.Close()
	go mustCopy(os.Stdout, con3)
	go mustCopy(os.Stdout, con2)
	mustCopy(os.Stdout, con1)
}
