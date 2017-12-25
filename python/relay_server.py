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

GPIO.setmode(GPIO.BCM)
data_map = {'fan1': '1', 'light1': '2', 'fan2': '3', 'light2': '4', 'fan3': '5', 'light3': '6'}


class NrfCommunication:
    def __init__(self):
        self.radio = None
        # self.TCP_IP = '127.0.0.1'
        # self.TCP_PORT = 8082
        self.BUFFER_SIZE = 1024  # Normally 1024, but we want fast response
        self.pipes = [[0xF0, 0xF0, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE2], [0xF0, 0xF0, 0xF0, 0xF0, 0xE3]]
        # self.sock_desc = None

    def set_parameters(self):
        self.radio = NRF24(GPIO, spidev.SpiDev())
        self.radio.begin(0, 17)

        self.radio.setPayloadSize(32)
        self.radio.setChannel(0x76)
        self.radio.setDataRate(NRF24.BR_1MBPS)
        self.radio.setPALevel(NRF24.PA_MIN)

        self.radio.setAutoAck(True)
        self.radio.enableDynamicPayloads()
        self.radio.enableAckPayload()

        # radio.openWritingPipe(self.pipes[1])
        # radio.openReadingPipe(1, self.pipes[0])

    def get_state(self, room_name, appliance_name):
        if room_name == "hall":
            w_pipe = self.pipes[0]
        elif room_name == "dining":
            w_pipe = self.pipes[0]
        elif room_name == "bedroom":
            w_pipe = self.pipes[0]
        r_pipe = self.pipes[1]

        print("room name : {0} appliance name: {1}".format(room_name, appliance_name))
        msg = list('1' + data_map.get(appliance_name))
        while len(msg) < 32:
            msg.append(0)
        print("Message: {0}".format(msg))
        self.radio.stopListening()
        self.radio.openWritingPipe(w_pipe)
        self.radio.write(msg)
        self.radio.openReadingPipe(1, r_pipe)
        self.radio.startListening()

        while True:
            stat = self.radio.available()
            # print("Radio status: {0}".format(stat))
            if stat:
                receivedMessage = []
                self.radio.read(receivedMessage, self.radio.getDynamicPayloadSize())
                self.radio.stopListening()
                print("Received: {0}".format(receivedMessage))
                print("Translating our received Message into unicode characters...")
                string = ""
                for n in receivedMessage:
                    if 32 <= n <= 126:
                        string += chr(n)
                print("Our received message decodes to: {0}".format(len(string)))
                return int(string)

    def set_state(self, room_name, appliance_name, state):
        if room_name == "hall":
            w_pipe = self.pipes[0]
        elif room_name == "dining":
            w_pipe = self.pipes[0]
        elif room_name == "bedroom":
            w_pipe = self.pipes[0]
        r_pipe = self.pipes[1]

        message = list('0' + data_map.get(appliance_name) + str(state))
        while len(message) < 32:
            message.append(0)

        self.radio.stopListening()
        self.radio.openWritingPipe(w_pipe)
        self.radio.write(message)
        self.radio.openReadingPipe(1, r_pipe)
        self.radio.startListening()
        while True:
            if self.radio.available():
                receivedMessage = []
                self.radio.read(receivedMessage, self.radio.getDynamicPayloadSize())
                print("Received: {0}".format(receivedMessage))
                print("Translating our received Message into unicode characters...")
                string = ""
                for n in receivedMessage:
                    if (n >= 32 and n <= 126):
                        string += chr(n)
                print("Our received message decodes to: {0}".format(len(string)))
                return int(string)  # 0 if correct, 1 if error


class RelayServer(relay_server_pb2_grpc.RelayServerServicer):
    def get_state(self, request, context):
        transmitter = NrfCommunication()
        transmitter.set_parameters()
        print("GET:\n {0}".format(request))
        state = transmitter.get_state(request.room_name, request.appliance_name)
        print("GET rpc success. Returned state: {0}".format(state))
        return relay_server_pb2.State(value=state)

    def set_state(self, request, context):
        transmitter = NrfCommunication()
        transmitter.set_parameters()
        print("SET:\n {0}".format(request))
        exit_state = transmitter.set_state(request.room_name, request.appliance_name, request.state)
        print("SET rpc success. Exit state: {0}".format(exit_state))
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
