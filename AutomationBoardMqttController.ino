#include "config.h"

boolean mqtt_connect()
{
  DEBUG_LOG(1, "Attempting MQTT connection ...");
  if (mqttClient.connect(mqttClientId)) {
    DEBUG_LOG(1, "  connected");
    // Once connected, publish an announcement...
    publish_connected();
    publish_ip_address();
    // ... and subscribe to topics (should have list)
    mqttClient.subscribe("ab/control/#");
  } else {
      DEBUG_LOG(1, "failed, rc=");
      DEBUG_LOG(1, mqttClient.state());
  }
  return mqttClient.connected();
}

void callback(char* topic, uint8_t* payload, unsigned int payloadLength)
{
  // handle message arrived
  /* topic = part of the variable header:has topic name of the topic where the publish received
       NOTE: variable header does not contain the 2 bytes with the
            publish msg ID
      payload = pointer to the first item of the buffer array that
                contains the message tha was published
               EXAMPLE of payload: lights,1
      length = the length of the payload, until which index of payload
  */

  DEBUG_LOG(1, "Payload length is");
  DEBUG_LOG(1, payloadLength);

  // Copy the payload to the new buffer
  char* message = (char*)malloc((sizeof(char) * payloadLength) + 1); // get the size of the bytes and store in memory
  memcpy(message, payload, payloadLength * sizeof(char));        // copy the memory
  message[payloadLength * sizeof(char)] = '\0';                  // add terminating character

  DEBUG_LOG(1, "Message with topic");
  DEBUG_LOG(1, topic);
  DEBUG_LOG(1, "arrived with payload");
  DEBUG_LOG(1, message);

  byte topicIdx = 0;
  boolean requestTopicFound = false;
  boolean controlTopicFound = false;

  // find if topic is matched
  // first check request topics
  for (byte i = 0; i < ARRAY_SIZE(REQUEST_TOPICS); i++) {
    progBuffer[0] = '\0';
    strcpy_P(progBuffer, (PGM_P)pgm_read_word(&(REQUEST_TOPICS[i])));
    if (strcmp(topic, progBuffer) == 0) {
      topicIdx = i;
      requestTopicFound = true;
      break;
    }
  }
  if (requestTopicFound) {
    if (topicIdx == 0) {  // topic is STATE_REQUEST
    } else {  // unknown request topic has arrived - ignore!!
      DEBUG_LOG(1, "Unknown request topic arrived");
    }
  } else {
    // check control topics
    topicIdx = 0;
    // find if topic is matched
    for (byte i = 0; i < ARRAY_SIZE(CONTROL_TOPICS); i++) {
      progBuffer[0] = '\0';
      strcpy_P(progBuffer, (PGM_P)pgm_read_word(&(CONTROL_TOPICS[i])));
      if (strcmp(topic, progBuffer) == 0) {
        topicIdx = i;
        controlTopicFound = true;
        break;
      }
    }

    if (controlTopicFound) {
      DEBUG_LOG(1, "Control topic index");
      DEBUG_LOG(1, topicIdx);
      switch (topicIdx) {
        case 0 :    // RELAY_1_CONTROL
          DEBUG_LOG(1, "RELAY_1_CONTROL topic arrived");
          if (strcmp(message, "ON") == 0)
            relay_switch_on(0);
          else if (strcmp(message, "OFF") == 0)
            relay_switch_off(0);
          break;
        case 1 :    // topic is RELAY_2_CONTROL
          DEBUG_LOG(1, "RELAY_2_CONTROL topic arrived");
          if (strcmp(message, "ON") == 0)
            relay_switch_on(1);
          else if (strcmp(message, "OFF") == 0)
            relay_switch_off(1);
          break;
        case 2 :    // topic is RELAY_3_CONTROL
          DEBUG_LOG(1, "RELAY_3_CONTROL topic arrived");
          if (strcmp(message, "ON") == 0)
            relay_switch_on(2);
          else if (strcmp(message, "OFF") == 0)
            relay_switch_off(2);
          break;
        case 4 :    // topic is RELAY_4_CONTROL
          DEBUG_LOG(1, "RELAY_4_CONTROL topic arrived");
          if (strcmp(message, "ON") == 0)
            relay_switch_on(3);
          else if (strcmp(message, "OFF") == 0)
            relay_switch_off(3);
          break;
        default :   // unknown control topic has arrived - ignore!!
          DEBUG_LOG(1, "Unknown control topic arrived");
          break;
      }
    }
  }

  // free memory assigned to message
  free(message);
}

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
#if DEBUG_LEVEL > 0
  Serial.begin(BAUD_RATE);
#endif

  // Configure Ethernet
  Ethernet.begin(mac, ip);
  delay(1500);

  // configure relay pins as outputs and set to LOW
  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    pinMode(RELAY_PINS_USED[idx], OUTPUT);
    digitalWrite(RELAY_PINS_USED[idx], LOW);
  }

  DEBUG_LOG(3, "Number of relays is ");
  DEBUG_LOG(3, ARRAY_SIZE(RELAY_PINS_USED));
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  if (!mqttClient.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    mqttClient.loop();
  }
}

