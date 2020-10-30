String JsonSensor(int sensorNo, float value, float values[], bool state, int valuesLen)
{
  //wait for serial connection to open (only necessary on some boards)
  while (!Serial);
  
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
