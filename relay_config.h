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
    DEBUG_LOG(1, "relay off");
    publish_relay_state(idx, false);
    return 1;
  } else {
    DEBUG_LOG(1, "relay already off");
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

// used by callback as a void function to switch off relay which is currenlty on
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
    DEBUG_LOG(1, "relay on");
    publish_relay_state(idx, true);
    return 1;
  } else {
    DEBUG_LOG(1, "relay already on");
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

