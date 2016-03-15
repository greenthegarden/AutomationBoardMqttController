#ifndef AUTOMATIONBOARDMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define AUTOMATIONBOARDMQTTCONTROLLER_ETHERNET_CONFIG_H_


// Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>


// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xED, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 91);

EthernetClient ethernetClient;


#endif   /* AUTOMATIONBOARDMQTTCONTROLLER_ETHERNET_CONFIG_H_ */

