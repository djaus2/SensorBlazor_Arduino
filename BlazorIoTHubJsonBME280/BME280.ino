#include <stddef.h>
// Using BME280 Library
// Ref: https://github.com/finitespace/BME280
// Environment Settings Example
#include <BME280I2C.h>

/**************************************************************************************
  Connecting the BME280 Sensor:
  Sensor              ->  Board
-------------------------------
  Vin (Voltage In)    ->  3.3V
  Gnd (Ground)        ->  Gnd
  SDA (Serial Data)   ->  A4 on Uno/Pro-Mini, 20 on Mega2560/Due, 2 Leonardo/Pro-Micro
  SCK (Serial Clock)  ->  A5 on Uno/Pro-Mini, 21 on Mega2560/Due, 3 Leonardo/Pro-Micro
**************************************************************************************/

  //Added this from one of the other repository headers:
   enum I2CAddr
   {
      I2CAddr_0x76 = 0x76,
      I2CAddr_0x77 = 0x77
   };

   BME280I2C::Settings settings(
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::OSR_X1,
   BME280::Mode_Forced,
   BME280::StandbyTime_1000ms,
   BME280::Filter_16,
   BME280::SpiEnable_False,
   //BME280I2C::I2CAddr_0x76
   I2CAddr::I2CAddr_0x77
    );

BME280I2C bme(settings);

void SensorSetup()
{
  Serial.println("Starting BME280 sensor!");
  // Following is in GetMacAddress()
  // Wire.begin();

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  // bme.chipID(); // Deprecated. See chipModel().
  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       //Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     //default:
       //Serial.println("Found UNKNOWN sensor! Error!");
  }
}

float ReadSensor()
{
    return NULL;
}

void ReadSensorValues(float values[])
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);
   
   //Puting either of the next two code segments bumps space from 88% to 103%
   //Doing in Loop() instead. Only 3% increase for second segment there.
   /* Serial.print("Temp: ");
   Serial.print(temp);
   Serial.println("°"+ String(tempUnit == BME280::TempUnit_Celsius ? "C" :"F"));
   Serial.print("\t\tHumidity: ");
   Serial.print(hum);
   Serial.println("% RH");
   Serial.print("\t\tPressure: ");
   Serial.print(pres);
   Serial.println(" Pa");*/
   
   /*Serial.print(temp);
   Serial.print(",");
   Serial.print(hum);
   Serial.print(",");
   Serial.println(pres);*/
  values[0] = temp;
  values[1] = hum;
  values[2] = pres;

}
