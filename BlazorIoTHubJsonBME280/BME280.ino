// Using BME280 Library
// Ref: https://github.com/finitespace/BME280
// Environment Settings Example
#include <BME280I2C.h>

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
   I2CAddr::I2CAddr_0x76
);

BME280I2C bme(settings);

void SensorSetup()
{
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
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}

float ReadSensor()
{
    return NULL;
}

float * ReadSensorValues()
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   Serial.print("Temp: ");
   Serial.print(temp);
   Serial.println("°"+ String(tempUnit == BME280::TempUnit_Celsius ? "C" :"F"));
   Serial.print("\t\tHumidity: ");
   Serial.print(hum);
   Serial.println("% RH");
   Serial.print("\t\tPressure: ");
   Serial.print(pres);
   Serial.println(" Pa");
   float values[] = { temp,pres,hum };
   return values;
}
