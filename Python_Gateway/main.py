import sys
from Adafruit_IO import MQTTClient
import time
from simple_ai import *
from uart import *

AIO_FEED_IDs = ["pump-state", "light-state", "pump-state-ack", "light-state-ack"]
AIO_USERNAME = "ntpt7921"
AIO_KEY = "change key"

def connected(client):
    print("Ket noi thanh cong ...")
    for topic in AIO_FEED_IDs:
        client.subscribe(topic, qos=1)

def subscribe(client , userdata , mid , granted_qos):
    print("Subscribe thanh cong ...")
    print("QOS", granted_qos)

def disconnected(client):
    print("Ngat ket noi ...")
    sys.exit (1)

def message(client , feed_id , payload):
    print("Nhan du lieu: " + payload + ", feed id: " + feed_id)
    if feed_id == 'pump-state':
        if int(payload) == 0:
            writeData('!0:P:0#')
        else:
            writeData('!0:P:1#')
    elif feed_id == 'light-state':
        if int(payload) == 0:
            writeData('!0:L:0#')
        else:
            writeData('!0:L:1#')

client = MQTTClient(AIO_USERNAME , AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()

counter_ai = 5
ai_result = ""
prev_ai_result = ""
sensor_type = 0

while True:
    counter_ai -= 1
    if counter_ai <= 0:
        counter_ai = 5
        prev_ai_result = ai_result
        ai_result = image_deterctor()
        print("AI output: ", ai_result)
        if ai_result != prev_ai_result:
            client.publish("ai", ai_result)

    readSerial(client)
    time.sleep(1)
