import serial.tools.list_ports

def getPort():
    try:
        ports = serial.tools.list_ports.comports()
        N = len(ports)
        commPort = "None"
        for i in range(0, N):
            port = ports[i]
            strPort = str(port)
            if "CP210x" in strPort:
                splitPort = strPort.split(" ")
                commPort = (splitPort[0])
        return commPort
        # return "COM5"
    except Exception as e:
        print("Error:", e)
        return None

try:
    port = getPort()
    if port is None:
        raise Exception("No valid serial port selected.")
    ser = serial.Serial(port=port, baudrate=115200)
    print("Serial PORT: " + str(ser))
except Exception as e:
    print("Error:", e)


mess = ""
def processData(client, data):
    try:
        data = data.replace("!", "")
        data = data.replace("#", "")
        splitData = data.split(":")
        print(splitData)
        if splitData[1] == "T":
            client.publish("cambien1", splitData[2])
        elif splitData[1] == "H":
            client.publish("cambien2", splitData[2])
        elif splitData[1] == "L":
            client.publish("cambien3", splitData[2])
    except Exception as e:
        print("Error:", e)

mess = ""
def readSerial(client):
    try:
        bytesToRead = ser.inWaiting()
        if (bytesToRead > 0):
            global mess
            mess = mess + ser.read(bytesToRead).decode("UTF-8")
            while ("#" in mess) and ("!" in mess):
                start = mess.find("!")
                end = mess.find("#")
                processData(client, mess[start:end + 1])
                if (end == len(mess)):
                    mess = ""
                else:
                    mess = mess[end+1:]
    except Exception as e:
        print("Error:", e)

def writeData(data):
    try:
        ser.write(str(data).encode())
    except Exception as e:
        print("Error:", e)