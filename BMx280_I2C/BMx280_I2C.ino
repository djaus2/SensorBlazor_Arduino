#include <Wire.h>
#include <ArduinoHttpClient.h>
#include <EthernetClient.h>

//#define USESTATICMAC
#define DELAY  30
//Number of reads to do
#define MAXCOUNT 20
char serverAddress[] = "192.168.0.4";  // server address
int port = 80;
int Count = 0;

EthernetClient ether;
HttpClient client = HttpClient(ether, serverAddress, port);

void ethernetSetup();
void sensorSetup();
String sensorRead();
String JsonSensor(int sensorNo, String value, String values, bool state);

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
	//wait for serial connection to open (only necessary on some boards)
	while (!Serial);

	ethernetSetup();
	sensorSetup();
}
	
void loop() {
	if (Count++ > MAXCOUNT)
	{
		delay(DELAY * 1000);
		return;
	}
	  String values = sensorRead();
    /*Serial.println("-------------");
    Serial.println(values[0]);
    Serial.println(values[1]);
    Serial.println(values[2]);*/

    String postData = JsonSensor(5,"",values,false);
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
