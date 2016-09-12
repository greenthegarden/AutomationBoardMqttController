#ifndef AUTOMATIONBOARDMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_ETHERNET_CONFIG_H_


// Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>


// Update these with values suitable for your network.
byte mac[] = {  0xDE, 0xED, 0xED, 0xFE, 0xFE, 0xED };

EthernetClient ethernetClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

void ethernet_init()
{
  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  delay(NETWORK_STARTUP_DELAY);

  DEBUG_LOG(1, "IP:");
  DEBUG_LOG(1, Ethernet.localIP());
}


#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_ETHERNET_CONFIG_H_ */
