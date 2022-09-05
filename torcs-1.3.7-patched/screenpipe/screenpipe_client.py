''' 
This file is used to: 
    1. receive image data from torcs
    2. display it in a dedicated window
'''
import numpy as np
import cv2
import etcd3
import time

# Set up etcd
etcd = etcd3.client(host='etcd', port=2379)

image_dataset = []

start = 0

def watch_callback(event):  
    width = (int)(etcd.get('/test/shared/width')[0])
    height = (int)(etcd.get('/test/shared/height')[0])
    red_array = list(etcd.get('/test/shared/red')[0])
    green_array = list(etcd.get('/test/shared/green')[0])
    blue_array = list(etcd.get('/test/shared/blue')[0])
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
    image = cv2.merge((blue, green, red))

    cv2.imshow('TORCS Image', image)
    cv2.waitKey(1)

etcd.add_watch_callback("/test/shared/red", watch_callback)

while True:
    None