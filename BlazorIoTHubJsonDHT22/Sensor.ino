#include <stddef.h>

#include <SimpleDHT.h>

// for DHT22, 
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT22 = 2;
SimpleDHT22 dht22(pinDHT22);

void SensorSetup()
{
  
}

float ReadSensor()
{
    float value =  137.035;
    return value;
}

float * ReadSensorValues()
{
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT22 failed, err="); 
    Serial.println(err);
    delay(2000);
    return;
  }
  float values[] = { temperature,0.0 ,humidity};
  return values;
}
