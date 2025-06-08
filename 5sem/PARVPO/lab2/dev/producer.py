import random
import requests
import time
import sys

def send_messages_to_broker(PDtype):
    broker_url = 'http://localhost:8080'
    i = 0
    while i < 10 ** 2:
        if i % 1000 == 0:
            print(f"Sending message {i}...")
        try:
            # Modify the data sent in the request to match the consumer's expectations
            data = {
                'message_type': PDtype,
                'message_content': random.randint(1, 1000),
            }
            response = requests.post(broker_url, json=data)
            # print(f"Message sent: {data.get('message_content')}, got response: {response.text}")
            while response.text == "-520":
                response = requests.post(broker_url, json=data)
                # print(f"Message sent: {data.get('message_content')}, got response: {response.text}")
            else:
                i += 1
        except requests.exceptions.RequestException as E:
            print(f"Error sending message: {E}")
            time.sleep(1)

    try:
        # Modify the data sent in the request to match the consumer's expectations
        data = {
            'message_type': PDtype,
            'message_content': -1,
        }
        response = requests.post(broker_url + "/end", json=data)
        # print(f"Message sent: {data.get('message_content')}, got response: {response.text}")
        #    print(f"Message sent: {data.get('message_content')}, got response: {response.text}")
    except requests.exceptions.RequestException as E:
        print(f"Error sending message: {E}")
        time.sleep(1)

if __name__ == "__main__":
    if len(sys.argv) > 1:
        # Access the first argument (index 1) and print it
        arg = int(sys.argv[1])
        send_messages_to_broker(arg)
    else:
        print("No arguments provided. Sould be providet producer type: 1 or 2")
