package main

import "fmt"

func main() {
	ch := make(chan string)

	go func() {
		ch <- "Hola desde la goroutine"
	}()

	mensaje := <-ch
	fmt.Println(mensaje)
}
