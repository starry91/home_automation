#!/usr/bin/env python3
import socket
import json
#import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev

def get_state(radio, pipes, conn):
    json_data = conn.recv(BUFFER_SIZE)
    data = json.loads(json_data)
    for key in data:
        room_name = key
        print(room_name)
    data = data[room_name]
    for app_type, value in data:
        appliance_type = app_type
        appliance_name = value
        print(appliance_name)
    '''
    if(room_name == "hall"):
        radio.openWritingPipe(pipes[0])
    radio.write("get")
    radio.write(appliance_name)
    radio.startListening()
    while(True):
       if(radio.available(0)):
            receivedMessage = []
            radio.read(receivedMessage, radio.getDynamicPayloadSize())
            print("Received: {}".format(receivedMessage))
            print("Translating our received Message into unicode characters...")
            string = ""
            for n in receivedMessage:
                if (n >= 32 and n <= 126):
                    string += chr(n)
            print("Our received message decodes to: {}".format(string))
            conn.send(string)
    '''
    conn.send(1)
    #radio.stopListening()
    return

def set_state(radio, pipes, conn):
    json_data = conn.recv(BUFFER_SIZE)
    data = json.loads(json_data)
    for key in data:
        room_name = key
    data = data[room_name]
    for app_type in data:
        appliance_type = app_type
    data = data[app_type]
    for app_name, value in data:
        appliance_name = app_name
        state = value   
    '''
    if(room_name == "hall"):
        radio.openWritingPipe(pipes[0])
    radio.write("set")
    radio.write(appliance_name)
    radio.write(state)
    radio.stopListening()
    '''
    return


TCP_IP = '127.0.0.1'
TCP_PORT = 8082
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

GPIO.setmode(GPIO.BCM)
 
TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response
pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]

radio = NRF24(GPIO, spidev.SpiDev())
radio.begin(0, 17)

radio.setPayloadSize(32)
radio.setChannel(0x76)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MIN)

radio.setAutoAck(True)
radio.enableDynamicPayloads()
radio.enableAckPayload()

radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1, pipes[1])
radio.printDetails()
#radio.startListening()

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)
 
conn, addr = s.accept()
s.settimeout(2)
print("server started\n")
print('Connection address:', addr)
while 1:
    func_call = conn.recv(BUFFER_SIZE)
    print(func_call)
    if len(func_call) <= 0:
        print("timed out")
    #conn.send(data)  # echo
    elif(func_call == "get"):
        print("get")
        get_state(radio, pipes, conn)
    elif(func_call == "set"):
        print("set")
        set_state(radio, pipes, conn)  
conn.close()







