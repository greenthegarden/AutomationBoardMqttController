#ifndef AUTOMATIONBOARDMQTTCONTROLLER_RELAY_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_RELAY_CONFIG_H_


#include <AutomationBoard.h>

const byte RELAY_PINS_USED[] = {RELAY_1, RELAY_2, RELAY_3, RELAY_4};

// returns 1 if relay connected to given pin is on, else returns 0
byte relay_state(byte idx)
{
  return(digitalRead(RELAY_PINS_USED[idx]));
}

// returns 1 if relay is currently on and switched off, else returns 0
byte relay_switch_off(byte idx)
{
  // only switch relay off if it is currently on
  if (relay_state(idx)) {
    digitalWrite(RELAY_PINS_USED[idx], LOW);
    publish_relay_state(idx, false);
//    DEBUG_LOG(1, "relay off");
//    progBuffer[0] = '\0';
//    strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[6])));
////    progBuffer[0] = '\0';
////    strcpy_P(progBuffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[6])));
////    char str[2];
//    charBuffer[0] = '\0';
////    sprintf(charBuffer, "%s%s%s", itoa(idx+1, str, 10), COMMAND_SEPARATOR, 'OFF');
//    sprintf(charBuffer, "%s%s%s", idx+1, COMMAND_SEPARATOR, 'OFF');
//    DEBUG_LOG(1, "progBuffer: ");
//    DEBUG_LOG(1, progBuffer);
//    DEBUG_LOG(1, "charBuffer: ");
//    DEBUG_LOG(1, charBuffer);
//    mqttClient.publish(progBuffer, charBuffer);
  }
  return 0;
}

// used as callback functions for Alarm

void relay1_switch_off() {
  byte relayIdx=0;
  relay_switch_off(relayIdx);
}

void relay2_switch_off() {
  byte relayIdx=1;
  relay_switch_off(relayIdx);
}

void relay3_switch_off() {
  byte relayIdx=2;
  relay_switch_off(relayIdx);
}

void relay4_switch_off() {
  byte relayIdx=3;
  relay_switch_off(relayIdx);
}

void relays_switch_off()
{
  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    if (relay_state(idx))
      relay_switch_off(idx);
  }
}

// returns 1 if relay is currently off and switched on, else returns 0
byte relay_switch_on(byte idx)
{
  if (!relay_state(idx)) {
    digitalWrite(RELAY_PINS_USED[idx], HIGH);
    publish_relay_state(idx, true);
//    DEBUG_LOG(1, "relay on");
//    progBuffer[0] = '\0';
//    strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[6])));
////    progBuffer[0] = '\0';
////    strcpy_P(progBuffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[6])));
//    // create message in format "idx,ON"
//    // add relay index
//    char str[2];
// //   itoa(idx+1, str, 10);
//    charBuffer[0] = '\0';
////    strcat(charBuffer, str);
////    strcat(charBuffer, COMMAND_SEPARATOR);
////    strcat(charBuffer, 'ON');
//    sprintf(charBuffer, "%s%s%s", itoa(idx+1, str, 10), COMMAND_SEPARATOR, 'ON');
//    DEBUG_LOG(1, "progBuffer: ");
//    DEBUG_LOG(1, progBuffer);
//    DEBUG_LOG(1, "charBuffer: ");
//    DEBUG_LOG(1, charBuffer);
//    mqttClient.publish(progBuffer, charBuffer);
    return 1;
  }
  return 0;
}

byte relay_switch_on_with_timer(byte idx, unsigned long duration)
{
  if (relay_switch_on(idx)) {
    currentTimerRef = Alarm.timerOnce(duration * SECS_PER_MIN, relays_switch_off);
    publish_alarm_id(currentTimerRef);
    return 1;
  }
  return 0;
}


#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_RELAY_CONFIG_H_ */

