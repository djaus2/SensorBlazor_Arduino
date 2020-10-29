#include <stddef.h>
// Using BME280 Library
// Ref: https://github.com/finitespace/BME280
// Environment Settings Example


/**************************************************************************************
/ GND ----- GND
// SDA ----- SDA
// SCL ----- SCL
// some BMP280/BME280 modules break out the CSB and SDO pins as well: 
// 5V ------ CSB (enables the I2C interface)
// GND ----- SDO (I2C Address 0x76)
// 5V ------ SDO (I2C Address 0x77)
// other pins can be left unconnected 
**************************************************************************************/

#include <Arduino.h>
//#include <Wire.h>

#include <BMx280I2C.h>

#define I2C_ADDRESS 0x77

//create a BMx280I2C object using the I2C interface with I2C Address 0x77
BMx280I2C bmx280(I2C_ADDRESS);

void SensorSetup()
{
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

float ReadSensor()
{
    return NULL;
}

void ReadSensorValues(float values[])
{
   float temp(NAN), hum(NAN), pres(NAN);

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
  
  /*
  Serial.print("Pressure: "); Serial.println(bmx280.getPressure());
  Serial.print("Pressure (64 bit): "); Serial.println(bmx280.getPressure64());
  Serial.print("Temperature: "); Serial.println(bmx280.getTemperature());

  //important: measurement data is read from the sensor in function hasValue() only. 
  //make sure to call get*() functions only after hasValue() has returned true. 
  if (bmx280.isBME280())
  {
    Serial.print("Humidity: "); 
    Serial.println(bmx280.getHumidity());
  }*/
   
   /*Serial.print(temp);
   Serial.print(",");
   Serial.print(hum);
   Serial.print(",");
   Serial.println(pres);*/
  values[0] = temp;
  values[1] = hum;
  values[2] = pres;

}
