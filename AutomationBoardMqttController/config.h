#ifndef AUTOMATIONBOARDMQTTCONTROLLER_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_CONFIG_H_


#include "Debug.h"

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

// external libraries
#include <MemoryFree.h>
//#include <SwitchedDevice.h>
// #include <AutomationBoard.h>

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// global variable definitions
#if DEBUG_LEVEL > 0
const unsigned long BAUD_RATE     = 9600;
#endif

const byte BUFFER_SIZE            = 32;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];

const unsigned long STATUS_UPDATE_INTERVAL = 5UL * 60UL *1000UL;  // 5 minutes
unsigned long statusPreviousMillis = 0UL;

#include "ethernetConfig.h"
#include "mqttConfig.h"

#include "icscConfig.h"

// #include "alarmConfig.h"
// #include "relayConfig.h"
//
// void no_network_behaviour() {
//   relays_switch_off();
// }


#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_CONFIG_H_ */
