// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"strings"
	"flag"
	"bufio"
	"fmt"
	"log"
	"net"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct{
	name, ip string
}

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
	nClients [] user
	hashes []client
	destination client
	oneUser = 0

)
func rm(s []user, i int) []user {
    s[i] = s[len(s)-1]
    return s[:len(s)-1]
}

func rmHash(clis []client, i int) []client {
	clis[i] = clis[len(clis)-1]
    return clis[:len(clis)-1]
}

func hash(cli client){
	for i := 0; i < len(hashes); i++ {
		if (hashes[i] == cli){
			return;
		}
	}
	hashes = append(hashes, cli)
}
func broadcaster(host string, port string) {
	fmt.Printf("irc-server > Server started in %s:%s\n", host, port)
	fmt.Printf("irc-server > Ready for recieving new clients\n")
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			if oneUser == 1 {
				for cli := range clients {
					if destination == cli {
						cli <- msg
					}
				}
				oneUser = 0

			} else {
				for cli := range clients {
					hash(cli)
					cli <- msg
				}
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	var users, msg_2send,user_name string
	var first_time = 1
	var other_user = ""
	
	who := conn.RemoteAddr().String()

	fmt.Printf("irc-server > New connected user, waiting for identification\n")

	ch <- "rc-server > Welcome to the simple IRC server. Press enter.\n"

	input := bufio.NewScanner(conn)
	for input.Scan() {
		input_arg := strings.Split(input.Text(), " ")
		if first_time == 1 {
			nClients = append(nClients, user {input_arg[0], who})
			user_name = input_arg[0]
			messages <- "Your username " + user_name + " is successfully logged"
			entering <- ch
			input_arg = input_arg[1:]
			first_time = 0
			fmt.Printf("irc-server > %s joined\n", user_name)
		}
		if input_arg[0] == "/users" {
			users = nClients[0].name
			for i := 1; i < len(nClients); i++ {
				users = users + ", " + nClients[i].name
			}
			ch <- "irc-server > " + users
		
		} else if input_arg[0] == "/msg" {
			if len(input_arg) == 3 {
				oneUser = 1
				other_user = input_arg[1]
				for i := 2; i < len(input_arg); i++{
					msg_2send = msg_2send + input_arg[i]
				}
				for i := 0; i < len(nClients); i++ {
					if nClients[i].name == other_user {
						destination = hashes[i]
					}
				}
				messages <- user_name + ": " + msg_2send
				msg_2send = ""
			} else {
				ch <-"Number of arguments wrong. Try: /msg <username> <message>"
			}
			
		} else if input_arg[0] == "/time" {
			tm := time.Now()
			ch <- "irc-server > " + tm.String()

		}else if input_arg[0] == "/user" {
			if len(input_arg) == 2 {
				var found= 0
				for i := 0; i < len(nClients); i++ {
					if nClients[i].name == input_arg[1] {
						found = 1
						ch <- "username: " + nClients[i].name + ", IP: " + nClients[i].ip
						
					}
				}
				if found ==0{
					ch <- "User not found"
				}
			} else {
				ch <- "Number of arguments wrong. Try: /user <username>"
			}
		} else {
			messages <- user_name + ": " + input.Text()
		}
		ch <- user_name + " > "

	}
	// NOTE: ignoring potential errors from input.Err()

	leaving <- ch
	fmt.Printf("irc-server > " + user_name + " left\n")
	for i := 0; i < len(nClients); i++{
		if nClients[i].name == user_name {
			nClients = rm(nClients, i)
			hashes = rmHash(hashes, i)
		}
	}
	messages <- user_name + " has left"
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		fmt.Fprintln(conn, msg) // NOTE: ignoring network errors
	}
}



//!-handleConn

//!+main
func main() {
	host := flag.String("host", "", "Host name")
	port := flag.String("port", "", "Port for chat")
	flag.Parse()
	listener, err := net.Listen("tcp", *host + ":" + *port)
	if err != nil {
		log.Fatal(err)
	}

	go broadcaster(*host, *port)
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
