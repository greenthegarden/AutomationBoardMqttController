#ifndef AUTOMATIONBOARDMQTTCONTROLLER_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_CONFIG_H_


#include "Debug.h"

// external libraries
//#include <MemoryFree.h>
//#include <SwitchedDevice.h>
#include <AutomationBoard.h>


const byte BUFFER_SIZE            = 32;
char progBuffer[BUFFER_SIZE];
char charBuffer[BUFFER_SIZE];


// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const int BAUD_RATE               = 9600;

#include <TimeAlarms.h>
byte currentTimerRef = 255;


#include "ethernet_config.h"
#include "mqtt_config.h"
#include "relay_config.h"



#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_CONFIG_H_ */

