#!.venv/bin/python3

import sys					# used for testing.Any second arg triggers only key loggers
import webbrowser 	# allows the opening of google chrome as a viewer
from pynput.keyboard import Key, Listener 
										# used to listen to keyboard input
import socket 			# used to send UDP packets
import time 				# used for delays
import _thread 			# used for simple multi-threading:
										# keyboard listener
										# UDP packet sender

# Linda:
# 1. set up logging
# 2. create different test log files
#    a. perodic taps
#    b. consistent direction hold
#    c. other random key interactions
# 3. plot them (python ML/plotting is ideal, but Matlab works great, too)
IP_on = False

if (sys.argv == 1):
	IP_on = True

if (IP_on):
	webbrowser.open("http://pi.cam:8000")
	hostName = "rc-module"
	UDP_IP = socket.gethostbyname(hostName)
	UDP_PORT = 4210

	print("UDP target IP: %s" % UDP_IP)
	print("UDP target port: %s" % UDP_PORT)

	sock = socket.socket(socket.AF_INET, # Internet
											socket.SOCK_DGRAM) # UDP




# Linda: Wait for init.json, this would be an added feature
On = 164  # sent when ^ is pressed
Off = 148 # duty cycle sent when nothing pressed
Rev = 132 # sent when \/ pressed

driveInc = 0
steerInc = 0

logArray = []			# Linda							 
keyDict = { # data shared between processes
	Key.up: {
		"drive": 6,
		"send": 0,
		"key": "driveOff"
	},
	Key.down: {
		"drive": -3,
		"send": 0,
		"key": "driveOff"
	},
	Key.left: {
		"steer": 18,
		"send": 0,
		"key": "steerOff"
	},
	Key.right: {
		"steer": -9,
		"send": 0,
		"key": "steerOff"
	},
	"driveOff": {
		"drive": 0,
		"send": 0
	},
	"steerOff": {
		"steer": 0,
		"send": 0
	}
}


# def getVal(current, increment):
# 	if(increment == 0):
# 		return int((current + Off)/(2)-1)
# 	if(increment*(current-Off) > 0):
# 		current += increment
# 		current = current*7 + Off
# 		current /= 8
# 		return int(current)
# 	if(increment*(current-Off) <= 0):
# 		return current + 2*(increment)




damper = 0.3
def getVal(current, increment):
	distance = current - Off
	current += increment
	current -= damper*(distance)
	return int(current)



def udp_loop(IP_on):
	steer = Off
	drive = Off
	while(True):
		for thisKey in keyDict:
			if keyDict.get(thisKey)["send"]:
				if "steer" in keyDict.get(thisKey):
					steer = getVal(int(steer),keyDict.get(thisKey).get("steer"))
				if "drive" in keyDict.get(thisKey):
					drive = getVal(int(drive),keyDict.get(thisKey).get("drive"))
		print (bytes([drive%256,int(drive/256),0,0,steer%256,int((steer/256)%3),0,0]))
		# Linda - log info here
		if (IP_on):
			sock.sendto(bytes([drive,0,0,0,steer,0,0,0]), (UDP_IP, UDP_PORT))
		time.sleep(.15)



def on_press(key):
	if key in keyDict:
		keyDict.get(key)["send"] = 1
		keyDict.get(keyDict.get(key).get("key"))["send"] = 0

def on_release(key):
	if key in keyDict:
		keyDict.get(key)["send"] = 0
		keyDict.get(keyDict.get(key).get("key"))["send"] = 1
	if key == Key.esc:
		# Linda: save
		quit()



_thread.start_new_thread(udp_loop, (IP_on,))



# This is the keyboard listener. Uses:
#		on_press(key): which sets corresponding 
# 		keyDict (edict pun) definition
#		on_release(key): which resets corresponding 
# 		keyDict (edict pun) definition
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()


## Linda: Maybe you could separate the script into the following files:
#			main.py
#			keyboard.py
#			udp.py
#				add logging here
#			ml.py
#				take shared logging data and analyze here
# if done, you'll need to look at threading. you have the following options:
#				_thread - lightweight, simple, limited in ability
#				threading - https://realpython.com/intro-to-python-threading/
#				thread - https://www.tutorialspoint.com/python/python_multithreading.htm
#									^ I tend to trust tutorialspoint.com ^
