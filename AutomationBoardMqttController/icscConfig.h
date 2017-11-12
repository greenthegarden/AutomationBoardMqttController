#include <ICSC.h>
#include <SoftwareSerial.h>


// on automation boatd
// D2 RS485 RO      Receiver out
// D3 RS485 DI      Driver in (the transmitter pin)
// D4 RS485 RE/DE   Receiver Enable pin (pulled low to receive data)/Driver Enable pin (pulled high while transmitting data)


const byte RS485_RO_PIN = 2; // D2
const byte RS485_DI_PIN = 3; // D3
const byte RS485_DE_PIN = 4; // D4

SoftwareSerial icscSerial(RS485_RO_PIN, RS485_DI_PIN); // RX, TX

const char station = 3;  // change station number here (BEFORE uploading to each node)

ICSC icsc(icscSerial, station, RS485_DE_PIN);

void print_data(unsigned char src, char command, unsigned char len, char *data)
{
  Serial.println(data);
}
