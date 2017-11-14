#!/usr/bin/env python3

from concurrent import futures
import time

import grpc

import relay_server_pb2
import relay_server_pb2_grpc
import socket
import json
import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
_ONE_DAY_IN_SECONDS = 60 * 60 * 24

data_map = {'fan1': '1', 'light1': '2', 'fan2': '3', 'light2': '4', 'fan3': '5', 'light3': '6'}

class NrfCommunication:
    def __init__(self):
        self.radio = None
        #self.TCP_IP = '127.0.0.1'
        #self.TCP_PORT = 8082
        self.BUFFER_SIZE = 1024  # Normally 1024, but we want fast response
        self.pipes = [[0xF0, 0xF0, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE2], [0xF0, 0xF0, 0xF0, 0xF0, 0xE3]]
        #self.sock_desc = None

    def set_parameters(self):
        radio = NRF24(GPIO, spidev.SpiDev())
        radio.begin(0, 17)

        radio.setPayloadSize(32)
        radio.setChannel(0x76)
        radio.setDataRate(NRF24.BR_1MBPS)
        radio.setPALevel(NRF24.PA_MIN)

        radio.setAutoAck(True)
        radio.enableDynamicPayloads()
        radio.enableAckPayload()

        #radio.openWritingPipe(self.pipes[1])
        #radio.openReadingPipe(1, self.pipes[0])
        radio.printDetails()

    '''def start_server(self):
        self.set_parameters()
        sock_desc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock_desc.bind((self.TCP_IP, self.TCP_PORT))
        sock_desc.listen(1)'''

    def get_state(self, room_name, appliance_type, appliance_name):
        if room_name == "hall":
            w_pipe = self.pipes[0]
        elif room_name == "dining":
            w_pipe = self.pipes[1]
        elif room_name == "bedroom":
            w_pipe = self.pipes[2]
        self.radio.stopListening()
        self.radio.openWritingPipe(w_pipe)
        self.radio.write('1' + appliance_name)
        self.radio.openReadingPipe(1, self.pipes[w_pipe])
        self.radio.startListening()
        while True:
            if self.radio.available(0):
                receivedMessage = []
                self.radio.read(receivedMessage, self.radio.getDynamicPayloadSize())
                print("Received: {}".format(receivedMessage))
                print("Translating our received Message into unicode characters...")
                string = ""
                for n in receivedMessage:
                    if (n >= 32 and n <= 126):
                        string += chr(n)
                print("Our received message decodes to: {}".format(string))
                return int(string)

    def set_state(self, room_name, appliance_type, appliance_name, state):
        if room_name == "hall":
            w_pipe = self.pipes[0]
        elif room_name == "dining":
            w_pipe = self.pipes[1]
        elif room_name == "bedroom":
            w_pipe = self.pipes[2]
        self.radio.stopListening()
        self.radio.openWritingPipe(w_pipe)
        self.radio.write('0' + data_map.get(appliance_name) + str(state))
        self.radio.openReadingPipe(1, self.pipes[w_pipe])
        self.radio.startListening()
        while True:
            if self.radio.available(0):
                receivedMessage = []
                self.radio.read(receivedMessage, self.radio.getDynamicPayloadSize())
                print("Received: {}".format(receivedMessage))
                print("Translating our received Message into unicode characters...")
                string = ""
                for n in receivedMessage:
                    if (n >= 32 and n <= 126):
                        string += chr(n)
                print("Our received message decodes to: {}".format(string))
                return int(string) #0 if correct, 1 if error

class RelayServer(relay_server_pb2_grpc.RelayServerServicer):

    def get_state(self, request, context):
        transmitter = NrfCommunication()
        transmitter.set_parameters()
        state = transmitter.get_state()
        return relay_server_pb2.State(value=state)

    def set_state(self, request, context):
        transmitter = NrfCommunication()
        transmitter.set_parameters()
        exit_state = transmitter.get_state()
        return relay_server_pb2.Response(value=exit_state)


def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
    relay_server_pb2_grpc.add_RelayServerServicer_to_server(RelayServer(), server)
    server.add_insecure_port('[::]:50051')
    server.start()
    try:
        while True:
            time.sleep(_ONE_DAY_IN_SECONDS)
    except KeyboardInterrupt:
        server.stop(0)

if __name__ == '__main__':
    serve()