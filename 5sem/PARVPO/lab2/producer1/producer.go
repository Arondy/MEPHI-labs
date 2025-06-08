package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"math/rand"
	"net/http"
	"os"
	"strconv"
	"time"
)

func sendMessagesToBroker(pdType int, matrixSize int, serversNum int) {
	// brokerURL := "http://consumer:8080"
	//endURL := "http://consumer:8080/end"
	//endURL2 := "http://consumer2:8080/end"
	brokerURL := "http://nginx:80"
	//endURL := "http://nginx:80/end"
	i := 0
	for i < matrixSize*matrixSize {
		if i%5000 == 0 {
			fmt.Printf("Sending message %d...\n", i)
		}
		data := map[string]interface{}{
			"message_type":    pdType,
			"message_content": rand.Intn(1000) + 1,
			"count": i,
		}

		// Convert data to JSON
		jsonData, err := json.Marshal(data)
		if err != nil {
			fmt.Printf("Error encoding JSON: %v\n", err)
			continue
		}

		// Send POST request with JSON data
		response, err := http.Post(brokerURL, "application/json", bytes.NewBuffer(jsonData))
		if err != nil {
			fmt.Printf("Error sending message: %v\n", err)
			time.Sleep(1 * time.Second)
			continue
		}
		if response.StatusCode == -520 {
			continue
		}
		defer response.Body.Close()
		i++
	}
	
	// fmt.Printf("Send session is almost ended.\n")

	data := map[string]interface{}{
		"message_type":    pdType,
		"message_content": -1,
		"count": 0,
	}

	// Convert data to JSON
	jsonData, err := json.Marshal(data)
	if err != nil {
		fmt.Printf("Error encoding JSON: %v\n", err)
		return
	}

	// Send POST request with JSON data
	for i := 1; i <= serversNum; i++ {
		fmt.Printf("Sending /end from %d...\n", pdType)
		_, err = http.Post(fmt.Sprintf("http://consumer%d:8080/end", i), "application/json", bytes.NewBuffer(jsonData))
		if err != nil {
			fmt.Printf("Error sending message: %v\n", err)
			time.Sleep(1 * time.Second)
		}
	}
}

func main() {
	if len(os.Args) > 3 {
		arg1, err := strconv.Atoi(os.Args[1])
		if err != nil {
			fmt.Println("Invalid argument provided. Should be a producer type: 1 or 2")
			return
		}
		arg2, err := strconv.Atoi(os.Args[2])
		if err != nil {
			fmt.Println("Invalid argument provided. (1)")
			return
		}
		arg3, err := strconv.Atoi(os.Args[3])
        if err != nil {
            fmt.Println("Invalid argument provided. (2)")
            return
        }
		sendMessagesToBroker(arg1, arg2, arg3)
	} else {
		fmt.Println("No arguments provided. Should be provided producer type: 1 or 2 and number of messages to send")
	}
}
