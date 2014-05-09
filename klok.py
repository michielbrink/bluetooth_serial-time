#!/usr/bin/python
import os, time, serial, json

try:
    ser = serial.Serial(port='/dev/ttyACM0',baudrate=9600)
except Exception:
    print "cant connect"

data1 = ''
fps = 0.05

while 1 :


    data = [time.strftime('%I')[:1] ,time.strftime('%I')[1:] ,':' ,time.strftime('%M')[:1] ,time.strftime('%M')[1:] ,' ' ,time.strftime('%p')[:1] ,time.strftime('%p')[1:]]

    data = ','.join(map(str,data))
    data = '{'+data+'}'
    if data != data1 :
        ser.open()
        ser.isOpen()
        time.sleep(1)
        for char in data:
            ser.write(char)
            time.sleep(fps)
        ser.close()
        data1 = data
    time.sleep(1)


