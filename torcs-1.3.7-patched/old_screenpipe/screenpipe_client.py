''' 
This file is used to: 
    1. receive image data from torcs
    2. display the game image
'''
import zmq
import torcs_data_pb2
import matplotlib.pyplot as plt
import numpy as np
import cv2
from datetime import date, datetime

context = zmq.Context()
socket = context.socket(zmq.SUB)
port = "5555"
socket.setsockopt(zmq.SUBSCRIBE, b"")
socket.connect("tcp://localhost:5555")
print("Connecting to server tcp://localhost:5555")

# Set up protobuf class
serialized_data = torcs_data_pb2.TorcsData()

image_dataset = []
start = datetime.now()

while True:
    # Receive data and parse it
    print("Trying to receive")
    message = socket.recv()
    print("Received something")
    serialized_data.ParseFromString(message)

    
    width = list(serialized_data.width)[0]
    height = list(serialized_data.height)[0]
    save_flag = list(serialized_data.save_flag)[0]
    red_array = list(serialized_data.red)[0]
    green_array = list(serialized_data.green)[0]
    blue_array = list(serialized_data.blue)[0]
    red = np.empty(len(red_array), dtype = np.uint8)
    green = np.empty(len(green_array), dtype = np.uint8)
    blue = np.empty(len(blue_array), dtype = np.uint8)

    # Convert uchar to uint8
    for j in range(width * height):
        red[j] = np.uint8(red_array[j])
        green[j] = np.uint8(green_array[j])
        blue[j] = np.uint8(blue_array[j])

    red = red.reshape(height, width)
    green = green.reshape(height, width)
    blue = blue.reshape(height, width)
    image = cv2.merge([blue, green, red])

    finish = datetime.now()
    tdelta = finish - start
    start = finish
    print("Time from last frame " + str(tdelta.total_seconds()))
    cv2.imshow('TORCS Image', image)
    cv2.waitKey(1)

    print("[width, height] = [{}, {}]".format(width, height))