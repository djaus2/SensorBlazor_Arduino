#include <SPI.h>
#include <Ethernet.h>

#define USESTATICMAC

#ifdef USESTATICMAC
// Enter static Mac:
byte mac[] = { 0x00, 0x04, 0xA3, 0xD2, 0xBF, 0x3C };
#endif

#ifndef USESTATICMAC
static uint8_t mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void GetMacAddress()
{
    Ref: https://www.freetronics.com.au/pages/setting-the-mac-address-using-the-mcp-24aa025e48
    
    Wire.begin();         // Join i2c bus (I2C address is optional for the master)

    for (int i = 0; i < 30; i++)
    {
        Serial.println(" ");
    }
    Serial.println("Starting test for MAC address ROM");
    Serial.print("Getting MAC: ");

    mac[0] = readRegister(0xFA);
    mac[1] = readRegister(0xFB);
    mac[2] = readRegister(0xFC);
    mac[3] = readRegister(0xFD);
    mac[4] = readRegister(0xFE);
    mac[5] = readRegister(0xFF);

    char tmpBuf[17];
    sprintf(tmpBuf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.println(tmpBuf);
}

#endif

void ethernetSetup() 
{
  //wait for serial connection to open (only necessary on some boards)
  while (!Serial);
  
  // Ethernet Setup /////////////////////////////
    while (Ethernet.begin(mac) != 1)
    {
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(15000);
    }

    // print your Ethernet IP address:
    IPAddress ip = Ethernet.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}
