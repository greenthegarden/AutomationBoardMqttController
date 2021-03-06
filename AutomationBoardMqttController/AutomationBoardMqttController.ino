#include "config.h"

boolean mqtt_connect()
{
  DEBUG_LOG(1, "Attempting MQTT connection ...");
  if (mqttClient.connect(mqttClientId)) {
    DEBUG_LOG(1, "  connected");
    // Once connected, publish an announcement ...
    publish_connected();
    publish_configuration();
    publish_status();
    // ... and subscribe to topics (should have list)
    mqttClient.subscribe("ab/control/#");
    mqttClient.subscribe("ab/request/#");
  } else {
      DEBUG_LOG(1, "failed, rc = ");
      DEBUG_LOG(1, mqttClient.state());
  }
  return mqttClient.connected();
}

// void callback(char* topic, uint8_t* payload, unsigned int payloadLength)
// {
//   // handle message arrived
//   /* topic = part of the variable header:has topic name of the topic where the publish received
//        NOTE: variable header does not contain the 2 bytes with the
//             publish msg ID
//       payload = pointer to the first item of the buffer array that
//                 contains the message tha was published
//                EXAMPLE of payload: lights,1
//       length = the length of the payload, until which index of payload
//   */
//
//   DEBUG_LOG(1, "Payload length is");
//   DEBUG_LOG(1, payloadLength);
//
//   // Copy the payload to the new buffer
//   char* message = (char*)malloc((sizeof(char) * payloadLength) + 1); // get the size of the bytes and store in memory
//   memcpy(message, payload, payloadLength * sizeof(char));        // copy the memory
//   message[payloadLength * sizeof(char)] = '\0';                  // add terminating character
//
//   DEBUG_LOG(1, "Message with topic");
//   DEBUG_LOG(1, topic);
//   DEBUG_LOG(1, "arrived with payload");
//   DEBUG_LOG(1, message);
//
//   byte topicIdx = 0;
//   boolean requestTopicFound = false;
//   boolean controlTopicFound = false;
//
//   // find if topic is matched
//   // first check request topics
//   for (byte idx = 0; idx < ARRAY_SIZE(REQUEST_TOPICS); idx++) {
//     topicBuffer[0] = '\0';
//     strcpy_P(topicBuffer, (PGM_P)pgm_read_word(&(REQUEST_TOPICS[idx])));
//     if (strcmp(topic, topicBuffer) == 0) {
//       topicIdx = idx;
//       requestTopicFound = true;
//       break;
//     }
//   }
//   if (requestTopicFound) {
//     if (topicIdx == UPTIME_REQUEST_IDX) {
//       DEBUG_LOG(1, "UPTIME_REQUEST topic arrived");
//     } else if (topicIdx == STATE_REQUEST_IDX) {  // topic is STATE_REQUEST
//       DEBUG_LOG(1, "STATE_REQUEST topic arrived");
//     } else {  // unknown request topic has arrived - ignore!!
//       DEBUG_LOG(1, "Unknown request topic arrived");
//     }
//   } else {
//     // check control topics
//     topicIdx = 0;
//     // find if topic is matched
//     for (byte idx = 0; idx < ARRAY_SIZE(CONTROL_TOPICS); idx++) {
//       topicBuffer[0] = '\0';
//       strcpy_P(topicBuffer, (PGM_P)pgm_read_word(&(CONTROL_TOPICS[idx])));
//       if (strcmp(topic, topicBuffer) == 0) {
//         topicIdx = idx;
//         controlTopicFound = true;
//         break;
//       }
//     }
//
//     if (controlTopicFound) {
//       DEBUG_LOG(1, "Control topic index");
//       DEBUG_LOG(1, topicIdx);
//       switch (topicIdx) {
//         case RELAY_CONTROL_IDX :
//           DEBUG_LOG(1, "RELAY_CONTROL topic arrived");
//           // message is expected to be in format "relay,duration"
//           // get relay and duration from message
//           // see http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
//           { char* separator = strchr(message, COMMAND_SEPARATOR);
//           DEBUG_LOG(1, "separator: ");
//           DEBUG_LOG(1, separator);
//           if (separator != 0) {
//             byte relayIdx = atoi(message)-1;
//             DEBUG_LOG(1, "relayIdx: ");
//             DEBUG_LOG(1, relayIdx);
//             ++separator;
//             unsigned long duration = atoi(separator);
//             DEBUG_LOG(1, "duration: ");
//             DEBUG_LOG(1, duration);
//             if (duration>0) {
//               relay_switch_on_with_timer(relayIdx, duration);
//             } else {
//               relay_switch_off(relayIdx);
//             }
//           }
//           }
//           break;
//         default :
//           DEBUG_LOG(1, "Unknown control topic arrived");
//       }
//     }
//   }
//
//   // free memory assigned to message
//   free(message);
// }
//
// // process serial input from XBee
// char buffer [32];
// int cnt = 0;
// boolean ready = false;

// #include <ArduinoJson.h>
// const byte JSON_BUFFER_SIZE = 200;
//
// void ParseLine
// {
//   StaticJsonBuffer<JSON_BUFFER_SIZE> jsonBuffer;
//   JsonObject& root = jsonBuffer.parseObject(buffer);
//   if root["weather"]
//   const char* sensor = root["sensor"];
// }

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
#if DEBUG_LEVEL > 0
  Serial.begin(9600);
#endif

  DEBUG_LOG(1, "AUTOMATIONBOARD");

  icscSerial.begin(115200);

  icsc.begin();
  icsc.registerCommand('P', &print_data);

  ethernet_init();

//  AutomationBoard().init();
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  icsc.process();

  // // require an Alarm.delay in order to allow alarms to work
  // Alarm.delay(0);
  //
  unsigned long now = millis();

  if (!mqttClient.connected()) {
    mqttClientConnected = false;
    if (now - lastReconnectAttempt > RECONNECTION_ATTEMPT_INTERVAL) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
        mqttClientConnected = true;
      }
    }
  } else {
    // Client connected
    mqttClient.loop();
  }

  if (now - statusPreviousMillis >= STATUS_UPDATE_INTERVAL) {
    if (mqttClientConnected) {
      statusPreviousMillis = now;
      publish_status();
    }
  }

  // if (ready) {
  //   ParseLine();
  //   ready = false;
  // } else while (Serial.available()) {
  //   char c = Serial.read();
  //   buffer[cnt++] = c;
  //   if ((c == '\n') || (cnt == sizeof(buffer)-1)) {
  //       buffer[cnt] = '\0';
  //       cnt = 0;
  //       ready = true;
  //   }
  //   JsonObject& root = jsonBuffer.parseObject(buffer);
  //   // Test if parsing succeeds.
  //   if (!root.success()) {
  //     Serial.println("parseObject() failed");
  //     return;
  //   }
  //   // Fetch values.
  //   //
  //   // Most of the time, you can rely on the implicit casts.
  //   // In other case, you can do root["time"].as<long>();
  //   const char* source = root[F("source")];
  //   const char* type = root[F("type")];
  //   if (strcmp(source, "weather") == 0) {
  //     if (strcmp(type, "init") == 0) {
  //       client.publish("home/weather/init", buffer);
  //     }
  //     if (strcmp(type, "report") == 0) {
  //       client.publish("home/weather/report", buffer);
  //     }
  //   }
  // }
}
