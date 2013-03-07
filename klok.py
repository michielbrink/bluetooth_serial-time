#!/usr/bin/python
import os, sys, time, serial

portt='/dev/rfcomm5'

executable = sys.executable
args = sys.argv[:]
args.insert(0, sys.executable)

os.system("rfcomm connect rfcomm5 00:12:08:17:20:73 1 &")

try:
  ser = serial.Serial(
		port=portt,
		baudrate=9600
	)
except Exception:
		print "you have not port " + portt + " connected"
		time.sleep(5)
		os.execvp(executable, args)

ser.open()
ser.isOpen()

input=1
while 1 :
	# get keyboard input
	input = raw_input(">> ")

	# send the character to the device
	try:
		ser.write(input + '\r\n')
	except Exception:
		print "you have not port " + portt + " connected"
		time.sleep(5)
		os.execvp(executable, args)
