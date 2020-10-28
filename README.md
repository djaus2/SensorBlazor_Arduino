# SensorBlazor_Arduino
Mirrors **SensorBlazor** repository .NET Core Console apps in that Arduino Sketches send Sensor data to Azure IoT Hub via the SensorBlazor service using Http POST.

## Sensor types:
From **SensorBlazor** Shared Project:
```
public enum SensorType {temperature,pressure,humidity,luminosity,accelerometer,environment,sswitch}
```
Indexed from zero.
## BlazorIoTHubJsonSim
Generates simulated sensor data and sends to the Blazor Service.

### Sample Serial Output
```
IP Address: 192.168.0.21
making POST request
Sensor Data: {
  "No": 0,
  "Id": "Sensor0",
  "SensorType": 0,
  "Value": 199.8
}
Status code: 200
Response: Sensor0
Wait 5 seconds
making POST request
Sensor Data: {
  "No": 1,
  "Id": "Sensor1",
  "SensorType": 1,
  "Value": 199.8
}
Status code: 200
Response: Sensor1
Wait 5 seconds
```

### Monitored IoT Hub Submissions
```
No.	Id	Sensor Type	Value	Values	State	TimeStamp
2	Sensor2	humidity	199.8	null	False.	0
1	Sensor1	pressure	199.8	null	False.	0
0	Sensor0	temperature	199.8	null	False.	0
```

## BlazorIoTHubTemplate
A template for sketches that read actuial sensor data

### Sample Sketch Serial Output
```
making POST request
Sensor Data: {
  "No": 0,
  "Id": "Sensor0",
  "SensorType": 0,
  "Value": 137.035
}
Status code: 200
Response: Sensor0
Wait 5 seconds
```

### Monitored IoT Hub Submissions
```
No.	Id	Sensor Type	Value	Values	State	TimeStamp
0	Sensor0	temperature	137.035	null	False.	0
0	Sensor0	temperature	137.035	null	False.	0
0	Sensor0	temperature	137.035	null	False.	0
```

## Real Sensors based upon the Template.
Added:   _(TB Tested, coded only thus far)_
- BME280
- DHT22
