String JsonSensor(int sensorNo, String value, String values, bool state)
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
  if (value !="")
  {
    postData += ",\"Value\":";
    postData += value;
  }
  else if (values !="")
  { 
    postData += ",\"Values\":";
    postData += values;
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
