package main

import (
	"io"
	"log"
	"net"
	"time"
	"flag"
	"os"
)

func handleConn(c net.Conn, timezone string) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, timezone + ": " + time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {

	nport := flag.String("port", "", "This is the number of the port")
	flag.Parse()

	listener, err := net.Listen("tcp", "localhost:" + *nport)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn, os.Getenv("TZ")) // handle connections concurrently
	}
}
