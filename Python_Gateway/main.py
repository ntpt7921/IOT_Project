import sys
from Adafruit_IO import MQTTClient
import time
import random
from simple_ai import *
from uart import *

AIO_FEED_IDs = ["nutnhan1", "nutnhan2"]
AIO_USERNAME = "Alphazir"
AIO_KEY = "aio_VAKQ79QeUlNuNdXc3Sru5GN8YTDS"

def connected(client):
    print("Ket noi thanh cong ...")
    for topic in AIO_FEED_IDs:
        client.subscribe(topic)

def subscribe(client , userdata , mid , granted_qos):
    print("Subscribe thanh cong ...")

def disconnected(client):
    print("Ngat ket noi ...")
    sys.exit (1)

def message(client , feed_id , payload):
    print("Nhan du lieu: " + payload + ", feed id: " + feed_id)
    if feed_id == 'nutnhan1':
        if payload == 0:
            writeData('1')
        else:
            writeData('2')
    elif feed_id == 'nutnhan2':
        if payload == 0:
            writeData('3')
        else:
            writeData('4')

client = MQTTClient(AIO_USERNAME , AIO_KEY)
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message
client.on_subscribe = subscribe
client.connect()
client.loop_background()
counter = 10
counter_ai = 5
ai_result = ""
prev_ai_result = ""
while True:
    counter -= 1
    if counter <= 0:
        counter = 10
        #TODO
        # print("Random data is publishing...")
        # temp = random.randint(10, 20)
        # client.publish("cambien1", temp)
        # humi = random.randint(50, 70)
        # client.publish("cambien2", humi)
        # light = random.randint(100, 500)
        # client.publish("cambien3", light)

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