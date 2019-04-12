package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"

	httpserver "github.com/beanbee/httpserver-go"
)

func main() {
	server := httpserver.NewServer("mytest", 3005).SetAsyncNum(20)

	server.HandlerAsyncRequst("GET", "/", asyncDemo)
	
	go func(){
		if err := server.Start(); err != nil {
			log.Printf("server failed: %v", err)
	    	}
	}()

	EndChannel := make(chan os.Signal)
	signal.Notify(EndChannel, syscall.SIGINT, syscall.SIGQUIT, syscall.SIGTERM)
	select {
	case output := <-EndChannel:
		log.Printf("end http server process by: %s", output)
		server.Stop()
	}
	close(EndChannel)
}

func asyncDemo(jsonIn []byte) (err error) {
	return nil
}
