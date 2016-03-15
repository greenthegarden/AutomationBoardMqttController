#!/usr/bin/env python

#---------------------------------------------------------------------------------------
# Load configuration values
#
#---------------------------------------------------------------------------------------

# see https://wiki.python.org/moin/ConfigParserShootout
from configobj import ConfigObj
config = ConfigObj('AutomationBoardMqttController.cfg')

print("{0}".format("Automation Board MQTT Controller Simulator"))


#---------------------------------------------------------------------------------------
# Modules and methods to support MQTT
#
#---------------------------------------------------------------------------------------

import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc) :
	print("Connected with result code "+str(rc))
	# Subscribing in on_connect() means that if the connection is lost
	# the subscriptions will be renewed when reconnecting.
	print("Subscribing to topics ...")
	for topic in config['mqtt_topics']['TOPICS'] :
#	topic = config['mqtt_topics']['TOPIC']
		client.subscribe(topic)
		print("{0}".format(topic))

def on_subscribe(client, userdata, mid, granted_qos) :
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_message(client, userdata, message) :
#    print(time.clock())
	print(message.topic+" "+str(message.payload))
	if (message.topic == "ab/control/relay_1") :
		if (message.payload == "1") :
			switchRelayOn()
		elif (message.payload == "0") :
			switchRelayOff()
	else :
		print("unexpected message received!!")
#    print("message received: topic is {0} with payload {1}".format(message.topic, message.payload))

def on_publish(client, userdata, mid) :
    print("mid: {0}".format(str(mid)))

def on_disconnect(client, userdata, rc) :
	print("Disconnect returned:")
	print("client: {0}".format(str(client)))
	print("userdata: {0}".format(str(userdata)))
	print("result: {0}".format(str(rc)))

def on_log(client, userdata, level, buf) :
    print("{0}".format(buf))

client               = mqtt.Client()
client.on_connect    = on_connect
client.on_subscribe  = on_subscribe
client.on_message    = on_message
client.on_publish    = on_publish
client.on_disconnect = on_disconnect
# Uncomment to enable debug messages
#client.on_log       = on_log

client.connect(
               config['mqtt_configuration']['MQTT_BROKER_IP'],
               int(config['mqtt_configuration']['MQTT_BROKER_PORT']),
               int(config['mqtt_configuration']['MQTT_BROKER_PORT_TIMEOUT']),
               )

client.loop_start()


def tidyupAndExit() :
	running = False       #Stop thread1
	# Disconnect mqtt client			mqttc.loop_stop()
	client.disconnect()
	print("Bye")
	exit(0)

command_types_valid_inputs = ['n', 'f']
#led_control_valid_inputs   = ['0', '1']

print("Enter 'n' or 'f' to switch relay on or off, respectively: ")

# Loop continuously
while True :
	try :
		command_type = raw_input()
		if command_type in command_types_valid_inputs :
			if command_type is 'n' :
				client.publish("ab/control/relay_1", "ON");
			elif command_type is 'f' :
				client.publish("ab/control/relay_1", "OFF");
			else :
				print("Should not be here!!")
		else :
			print("Invalid command type entered")
	except KeyboardInterrupt :      #Triggered by pressing Ctrl+C
		tidyupAndExit()
