// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"flag"
	"bytes"
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {
	server := flag.String("server", "", "Host for chat")
	user := flag.String("user", "", "User's name")
	flag.Parse()
	conn, err := net.Dial("tcp", *server)
	if err != nil {
		log.Fatal(err)
	}
	io.Copy(conn, bytes.NewBufferString(*user + " "))
	done := make(chan struct{})
	go func() {
		io.Copy(os.Stdout, conn) // NOTE: ignoring errors
		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	mustCopy(conn, os.Stdin)
	conn.Close()
	<-done // wait for background goroutine to finish
}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
