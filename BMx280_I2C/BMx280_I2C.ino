//#include <b64.h>
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <ArduinoHttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <BMx280I2C.h>

// BMx280_I2C.ino
//
// shows how to use the BMP280 / BMx280 library with the sensor connected using I2C.
//
// Base upon the Example from:
// Gregor Christandl. Thx Gregor
// Source: https://bitbucket.org/christandlg/bmx280mi/src/master/
//
// connect the AS3935 to the Arduino like this:
//
// Arduino - BMP280 / BME280
// 3.3V ---- VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL
// some BMP280/BME280 modules break out the CSB and SDO pins as well: 
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x76)
// 5V ------ SDO (I2C Address 0x77)  <-- This setting
// other pins can be left unconnected.



#define DELAY  30
//Number of reads to do
#define MAXCOUNT 20
int Count = 0;

#define ETHER_I2C_ADDRESS 0x50
char serverAddress[] = "192.168.0.4";  // server address
int port = 80;
byte mac[] = { 0x00, 0x04, 0xA3, 0xD2, 0xBF, 0x3C };
EthernetClient ether;
HttpClient client = HttpClient(ether, serverAddress, port);



#define BME280_I2C_ADDRESS 0x77
//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280I2C bmx280(BME280_I2C_ADDRESS);

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);

	//wait for serial connection to open (only necessary on some boards)
	while (!Serial);
  Wire.begin();

  // Ethernet Setup /////////////////////////////
    while (Ethernet.begin(mac) != 1)
    {
        Serial.println("Error getting IP address via DHCP, trying again...");
        delay(15000);
    }


    // print your WiFi shield's IP address:
    IPAddress ip = Ethernet.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

	// BME Setup ///////////////////////////////////
  Serial.println("BME Begin");
	//begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
	//and reads compensation parameters.
	if (!bmx280.begin())
	{
		Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
		while (1);
	}

	if (bmx280.isBME280())
		Serial.println("sensor is a BME280");
	else
		Serial.println("sensor is a BMP280");

	//reset sensor to default parameters.
	bmx280.resetToDefaults();

	//by default sensing is disabled and must be enabled by setting a non-zero
	//oversampling setting.
	//set an oversampling setting for pressure and temperature measurements. 
	bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
	bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

	//if sensor is a BME280, set an oversampling setting for humidity measurements.
	if (bmx280.isBME280())
		bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
}

String JsonSensor(int sensorNo, float value, float values[], bool state, int valuesLen)
{
  Serial.println("Making POST request");

  String postData = "{";
  postData += "\"No\":";
  postData += sensorNo;
  postData += ",\"Id\":\"Sensor"; //In C# this is a new GUID
  postData += sensorNo;
  postData += "\"";
  postData += ",\"SensorType\":";
  postData += sensorNo;
  if (sensorNo < 4)
  {
      postData += ",\"Value\":";
      postData += value;
  }
  else if (sensorNo < 6)
  {
      postData += ",\"Values\":[";
      postData += values[0];
      postData += ",";
      postData += values[1];
      postData += ",";
      postData += values[2];
      postData +="]";
  }
  else
  {
      postData += ",\"State\":";
      postData += state;
  }
  postData += "}";

  Serial.println(postData);
  
  return postData;
}


float values[3];

void loop() {
	if (Count++ > MAXCOUNT)
	{
		delay(DELAY * 1000);
		return;
	}
  GetValues();
    /*Serial.println("-------------");
    Serial.println(values[0]);
    Serial.println(values[1]);
    Serial.println(values[2]);*/

    String postData = JsonSensor(5,NULL,values,false,3);
        String contentType = "application/json";
    client.post("/Sensor", contentType, postData);

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
  
    Serial.print("Status code (200=OK): ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
    Serial.println("-------------");
    delay(DELAY*1000);
}

void GetValues() {
  // put your main code here, to run repeatedly:
  float temp(NAN), hum(NAN), pres(NAN);
	delay(1000);

	//start a measurement
	if (!bmx280.measure())
	{
		Serial.println("could not start measurement, is a measurement already running?");
		return;
	}

	//wait for the measurement to finish
	do
	{
		delay(100);
	} while (!bmx280.hasValue());

  temp = bmx280.getTemperature();
  pres=bmx280.getPressure();
  hum=bmx280.getHumidity();

	Serial.print("Pressure: "); Serial.println(bmx280.getPressure());
	Serial.print("Pressure (64 bit): "); Serial.println(bmx280.getPressure64());
	Serial.print("Temperature: "); Serial.println(bmx280.getTemperature());

	//important: measurement data is read from the sensor in function hasValue() only. 
	//make sure to call get*() functions only after hasValue() has returned true. 
	if (bmx280.isBME280())
	{
		Serial.print("Humidity: "); 
		Serial.println(bmx280.getHumidity());
	}
 values[0] = temp;
  values[1] = hum;
  values[2] = pres;
}
