import webbrowser # allows the opening of google chrome as a viewer
from pynput.keyboard import Key, Listener # used to listen to keyboard input
import socket # used to send UDP packets
import time # used for delays
import _thread # used for simple multi-threading:
										# keyboard listener
										# UDP packet sender

webbrowser.open("http://pi-camera:8000")

On = 164  # sent when ^ is pressed
Off = 148 # duty cycle sent when nothing pressed
Rev = 132 # sent when \/ pressed

# UDP_IP = "192.168.86.174"
hostName = "rc-module"
UDP_IP = socket.gethostbyname(hostName)
UDP_PORT = 4210

print("UDP target IP: %s" % UDP_IP)
print("UDP target port: %s" % UDP_PORT)

driveInc = 0
steerInc = 0

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
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


def getVal(current, increment):
	if(increment == 0):
		return int((current + Off)/(2)-1)
	if(increment*(current-Off) > 0):
		current += increment
		current = current*7 + Off
		current /= 8
		return int(current)
	if(increment*(current-Off) <= 0):
		return current + 2*(increment)



def udp_loop():
	steer = Off
	drive = Off
	while(True):
		for thisKey in keyDict:
			if keyDict.get(thisKey)["send"]:
				if "steer" in keyDict.get(thisKey):
					steer = getVal(int(steer),keyDict.get(thisKey).get("steer"))
				if "drive" in keyDict.get(thisKey):
					drive = getVal(int(drive),keyDict.get(thisKey).get("drive"))
		print(bytes([drive,0,0,0,steer,0,0,0]))
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
		quit()

_thread.start_new_thread(udp_loop, ())
# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()