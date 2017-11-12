#ifndef AUTOMATIONBOARDMQTTCONTROLLER_MQTT_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>

// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 1);        // openHAB
const char mqttClientId[]                         = "automationboard";
const int MQTT_PORT                               = 1883;

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

boolean mqttClientConnected                       = false;

const char COMMAND_SEPARATOR                      = ',';

// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length) {}

PubSubClient mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);

// MQTT topic definitions

// MQTT payloads
const char MQTT_PAYLOAD_CONNECTED[]   PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_ERROR[]       PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[]       PROGMEM = "START";
const char MQTT_PAYLOAD_END[]         PROGMEM = "END";
const char MQTT_PAYLOAD_SLEEP[]       PROGMEM = "SLEEP";
const char MQTT_PAYLOAD_OK[]          PROGMEM = "OK";

PGM_P const MQTT_PAYLOADS[]           PROGMEM = { MQTT_PAYLOAD_CONNECTED,   // idx = 0
                                                  MQTT_PAYLOAD_OK,          // idx = 1
                                                  MQTT_PAYLOAD_ERROR,       // idx = 2
                                                  MQTT_PAYLOAD_START,       // idx = 3
                                                  MQTT_PAYLOAD_END,         // idx = 4
                                                  MQTT_PAYLOAD_SLEEP,       // idx = 5
                                                };

/* MQTT_PAYLOADS indices, must match table above */
typedef enum {
  MQTT_PAYLOAD_CONNECTED_IDX = 0,
  MQTT_PAYLOAD_OK_IDX        = 1,
  MQTT_PAYLOAD_ERROR_IDX     = 2,
  MQTT_PAYLOAD_START_IDX     = 3,
  MQTT_PAYLOAD_END_IDX       = 4,
  MQTT_PAYLOAD_SLEEP_IDX     = 5,
} mqtt_payloads;

// Status topics
const char MQTT_STATUS[]       PROGMEM = "ab/status/mqtt";
const char VERSION_STATUS[]    PROGMEM = "ab/status/version";
const char INTERVAL_STATUS[]   PROGMEM = "ab/status/interval";
const char IP_ADDR_STATUS[]    PROGMEM = "ab/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "ab/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "ab/status/memory";
const char TIME_STATUS[]       PROGMEM = "ab/status/time";
const char ALARM_STATUS[]      PROGMEM = "ab/status/alarm";
const char RELAY_STATUS[]      PROGMEM = "ab/status/relay";
const char FLOWRATE_STATUS[]   PROGMEM = "ab/status/flowrate";

PGM_P const STATUS_TOPICS[]    PROGMEM = { MQTT_STATUS,         // idx = 0
                                           VERSION_STATUS,      // idx = 1
                                           INTERVAL_STATUS,     // idx = 2
                                           IP_ADDR_STATUS,      // idx = 3
                                           UPTIME_STATUS,       // idx = 4
                                           MEMORY_STATUS,       // idx = 5
                                           TIME_STATUS,         // idx = 6
                                           ALARM_STATUS,        // idx = 7
                                           RELAY_STATUS,        // idx = 8
                                           FLOWRATE_STATUS,
                                         };

// STATUS_TOPICS indices, must match table above
typedef enum {
  MQTT_STATUS_IDX          = 0,
  VERSION_STATUS_IDX       = 1,
  INTERVAL_STATUS_IDX      = 2,
  IP_ADDR_STATUS_IDX       = 3,
  UPTIME_STATUS_IDX        = 4,
  MEMORY_STATUS_IDX        = 5,
  TIME_STATUS_IDX          = 6,
  ALARM_STATUS_IDX         = 7,
  RELAY_STATUS_IDX         = 8,
  FLOWRATE_STATUS_IDX      = 9,
} status_topics;

// Automation Board Input topics

//const char ANALOG_IN_1_INPUT[]       PROGMEM = "ab/input/analog_in_1";
//const char ANALOG_IN_2_INPUT[]       PROGMEM = "ab/input/analog_in_2";
//const char ANALOG_IN_3_INPUT[]       PROGMEM = "ab/input/analog_in_3";
//
//const char OPTO_INPUT_1_INPUT[]      PROGMEM = "ab/input/opto_input_1";
//const char OPTO_INPUT_2_INPUT[]      PROGMEM = "ab/input/opto_input_2";
//const char OPTO_INPUT_3_INPUT[]      PROGMEM = "ab/input/opto_input_3";
//const char OPTO_INPUT_4_INPUT[]      PROGMEM = "ab/input/opto_input_4";
//
//PGM_P const INPUT_TOPICS[]     PROGMEM = { ANALOG_IN_1_INPUT,    // idx = 0
//                                           ANALOG_IN_2_INPUT,    // idx = 1
//                                           ANALOG_IN_3_INPUT,    // idx = 2
//                                           OPTO_INPUT_1_INPUT,   // idx = 3
//                                           OPTO_INPUT_2_INPUT,   // idx = 4
//                                           OPTO_INPUT_3_INPUT,   // idx = 5
//                                           OPTO_INPUT_4_INPUT,   // idx = 6
//                                          };

// Request topics
const char UPTIME_REQUEST[]    PROGMEM = "ab/request/uptime";
const char STATE_REQUEST[]     PROGMEM = "ab/request/relay_state";

PGM_P const REQUEST_TOPICS[]   PROGMEM = { UPTIME_REQUEST,         // idx = 0
                                           STATE_REQUEST,          // idx = 1
                                         };
// REQUEST_TOPICS indices, must match table above
typedef enum {
  UPTIME_REQUEST_IDX          = 0,
  STATE_REQUEST_IDX           = 1,
} request_topics;

// Control topics
const char RELAY_CONTROL[]     PROGMEM = "ab/control/relay";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,          // idx = 0
                                         };
// INPUT_TOPICS indices, must match table above
typedef enum {
  RELAY_CONTROL_IDX          = 0,
} control_topics;

void publish_connected() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_version() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[VERSION_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  sprintf(payloadBuffer, "%i.%i", VERSION_MAJOR, VERSION_MINOR);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_status_interval() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[INTERVAL_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(STATUS_UPDATE_INTERVAL, payloadBuffer, 10));
}

void publish_ip_address() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  IPAddress ip = Ethernet.localIP();
//  strcpy(payloadBuffer, Ethernet.localIP());
  sprintf(payloadBuffer,
    "%d%c%d%c%d%c%d",
    ip[0], '.',
    ip[1], '.',
    ip[2], '.',
    ip[3]
  );
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_uptime() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
}

void publish_memory() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MEMORY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
}

// void publish_relay_state(byte relayIdx, boolean relayState) {
//   payloadBuffer[0] = '\0';
//   if (relayState) { // relay ON
//     DEBUG_LOG(1, "relay on");
//     sprintf(payloadBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 1);
//   } else {
//     DEBUG_LOG(1, "relay off");
//     sprintf(payloadBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 0);
//   }
//   DEBUG_LOG(1, "payloadBuffer: ");
//   DEBUG_LOG(1, payloadBuffer);
//   topicBuffer[0] = '\0';
//   strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[RELAY_STATUS_IDX])));
//   // create message in format "idx,ON"
//   // add relay index
//   DEBUG_LOG(1, "topicBuffer: ");
//   DEBUG_LOG(1, topicBuffer);
//   mqttClient.publish(topicBuffer, payloadBuffer);
// }

void publish_configuration() {
  publish_version();
  publish_status_interval();
  publish_ip_address();
}

void publish_status() {
  publish_uptime();
  publish_memory();
}


#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_MQTT_CONFIG_H_ */
