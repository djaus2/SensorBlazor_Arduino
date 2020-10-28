#include <SimpleDHT.h>

/*
  By David Jones.
  Post Simulated Sensor Data to a Blazor Service that onforwards it to an Azure IoT Hub.
  Do not need IoT Hub Connection details. Only authority to access the service. In Repo access is Public.
  The Blazor Service on GitHub: https://github.com/djaus2/SensorBlazor
  Mirroring Blazor App to monitor submissions: https://github.com/djaus2/BlazorD2CMessages
  Based uponSimple POST client for ArduinoHttpClient library by Tom Igoe
  and a request body,
 */

 /* Test Hardware:
  * Freetronics: EtherTen https://www.freetronics.com.au/products/etherten 
  * Uses Ethrenet not WiFi
 */

#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Wire.h>
#include <ArduinoHttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

void SensorSetup();
float ReadSensor();
float * ReadSensorValues();

#define SENSORTYPE 0
// From public enum SensorType {temperature,pressure,humidity,luminosity,accelerometer,environment,sswitch}
// Indexed from zero.

//Delay after POST done in seconds
#define DELAY  5
//Number of reads to do
#define MAXCOUNT 10

#define I2C_ADDRESS 0x50
#include <Wire.h>

#ifdef USESTATICMAC
// Enter static Mac:
byte mac[] = { 0x00, 0x04, 0xA3, 0xD2, 0xBF, 0x3C };
#endif

#ifndef USESTATICMAC
static uint8_t mac[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
#endif

//Set the Blazor Service IPAddress and Port:
char serverAddress[] = "192.168.0.4";  // server address
int port = 80;

// These next two are not currently used.
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30 * 1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

int Count; //Count the loops. A diffrent sensor for each.

EthernetClient ether;
HttpClient client = HttpClient(ether, serverAddress, port);

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

byte readRegister(byte r)
{
    unsigned char v;
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(r);  // Register to read
    Wire.endTransmission();

    Wire.requestFrom(I2C_ADDRESS, 1); // Read a byte
    while (!Wire.available())
    {
        // Wait
    }
    v = Wire.read();
    return v;
}

void setup() {
    Serial.begin(9600);

    #ifndef USESTATICMAC
    GetMacAddress();
    #endif

    while (Ethernet.begin(mac) != 1)
    {
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(15000);
    }


    // print your WiFi shield's IP address:
    IPAddress ip = Ethernet.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    SensorSetup();
    
    Count =0;
}

/*
 * Create a Json string for a Sensor's data
 * Sensor type depends upon sensorNo
 */


String JsonSensor(int sensorNo, float value, float* values, bool state)
{
  StaticJsonDocument<64> Sensor;
  Serial.println("making POST request");
 
  Sensor["No"] = sensorNo;
  String sensorType = "Sensor";
  sensorType += sensorNo;
  Sensor["Id"] = sensorType;
  Sensor["SensorType"] = sensorNo;
  if (sensorNo<4)
  {
    if (value != NULL)
        Sensor["Value"] = value;
  }
  else if (sensorNo<6)
  {
      if (values != NULL)
      {
          JsonArray values = Sensor.createNestedArray("Values");
          int len = sizeof(values)/sizeof(values[0]);
          for (int i=0;i<len; i++)
            values.add(values[i]);
      }
  }
  else
  {
    Sensor["State"] = state;
  }
  String postData;
  serializeJsonPretty(Sensor,postData);
  Serial.print("Sensor Data: ");
  Serial.println(postData);
  
  return postData;
}



void loop() {
  if (Count < MAXCOUNT)
  {
    String postData;
    float value = ReadSensor();
    float* values = ReadSensorValues();
    Serial.print(values[0]);
    Serial.print(",");
    Serial.println(values[2]);
    postData = JsonSensor(SENSORTYPE,value,values,false);
    Count++;
    String contentType = "application/json";
    client.post("/Sensor", contentType, postData);

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
  
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    Serial.println("Wait 5 seconds");
    delay(DELAY*1000);
  }
  else
  {
    
  }
}
