#ifndef AUTOMATIONBOARDMQTTCONTROLLER_MQTT_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>

// MQTT parameters
byte mqttServerAddr[]             = { 192, 168, 1, 55 };  // Pi eth0 interface
char mqttClientId[]               = "automationboard";
const int MQTT_PORT               = 1883;
//#define MQTT_MAX_PACKET_SIZE        168
//#define MQTT_KEEPALIVE              300

long lastReconnectAttempt         = 0;
const char COMMAND_SEPARATOR      = ',';

char message[BUFFER_SIZE];


// Status topics

const char CONNECTED_STATUS[]  PROGMEM = "ab/status/connected";
const char IP_ADDR_STATUS[]    PROGMEM = "ab/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "ab/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "ab/status/memory";
const char TIME_STATUS[]       PROGMEM = "ab/status/time";
const char RELAY_STATUS[]      PROGMEM = "ab/status/relay";

PGM_P const STATUS_TOPICS[]    PROGMEM = { CONNECTED_STATUS,    // idx = 0
                                           IP_ADDR_STATUS,      // idx = 1
                                           UPTIME_STATUS,       // idx = 2
                                           MEMORY_STATUS,       // idx = 3
                                           TIME_STATUS,         // idx = 4
                                           RELAY_STATUS,        // idx = 5
                                          };

// Relayduino Input topics

const char ANALOG_IN_1_INPUT[]       PROGMEM = "ab/input/analog_in_1";
const char ANALOG_IN_2_INPUT[]       PROGMEM = "ab/input/analog_in_2";
const char ANALOG_IN_3_INPUT[]       PROGMEM = "ab/input/analog_in_3";

const char OPTO_INPUT_1_INPUT[]      PROGMEM = "ab/input/opto_input_1";
const char OPTO_INPUT_2_INPUT[]      PROGMEM = "ab/input/opto_input_2";
const char OPTO_INPUT_3_INPUT[]      PROGMEM = "ab/input/opto_input_3";
const char OPTO_INPUT_4_INPUT[]      PROGMEM = "ab/input/opto_input_4";

PGM_P const INPUT_TOPICS[]     PROGMEM = { ANALOG_IN_1_INPUT,     // idx = 0
                                           ANALOG_IN_2_INPUT,     // idx = 1
                                           ANALOG_IN_3_INPUT,     // idx = 2
                                           OPTO_INPUT_1_INPUT,    // idx = 3
                                           OPTO_INPUT_2_INPUT,    // idx = 4
                                           OPTO_INPUT_3_INPUT,    // idx = 5
                                           OPTO_INPUT_4_INPUT,    // idx = 6
                                          };

// Request topics

const char STATE_REQUEST[]     PROGMEM = "ab/request/relay_state";

PGM_P const REQUEST_TOPICS[]   PROGMEM = { STATE_REQUEST,          // idx = 0
                                         };


// Control topics

const char RELAY_CONTROL[]   PROGMEM = "ab/control/relay";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,     // idx = 0
                                         };


// callback function definition
void callback(char*, uint8_t*, unsigned int);

PubSubClient   mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);

void publish_connected()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqttClient.publish(progBuffer, "");
}

void publish_ip_address()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqttClient.publish(progBuffer, "192.168.1.91");
}

void publish_uptime()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  charBuffer[0] = '\0';
  ltoa(millis(), charBuffer, 10);
  mqttClient.publish(progBuffer, charBuffer);
}

void publish_alarm_id(byte ref = 255)
{
  if (ref == 255)
    ref = Alarm.getTriggeredAlarmId();
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[5])));
  char str[4];
  mqttClient.publish(progBuffer, itoa(ref, str, 10));
}



#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_MQTT_CONFIG_H_ */
