#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
#include <OneWire.h>
#include <DallasTemperature.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0E, 0xCA, 0xCB}; // MAC address for your Ethernet shield

char xivelyKey[] = "XvYdRvp0B4h51CQyir6Q0GazBDDlD2m8fp4efVMgLZ2eurEF";  // Your Xively key to let you upload data

#define ONE_WIRE_BUS 8   //data wire is plugged into pin 8 on the Arduino
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);  //Setup a oneWire instance to communicate with any OneWire devices.

DallasTemperature sensors(&oneWire);  //pass our oneWire reference to the Dallas Temperature.

DeviceAddress OutsideThermometer = { 0x28, 0xB8, 0x6E, 0xDE, 0x04, 0x00, 0x00, 0x81};

char sensorId[] = "sensor_reading";  // Define the strings for our datastream IDs
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId, strlen(sensorId), DATASTREAM_FLOAT),
};

XivelyFeed feed(676748944, datastreams, 1 /* number of datastreams */);  // Finally, wrap the datastreams into a feed

EthernetClient client;
XivelyClient xivelyclient(client);



void setup() {

  Serial.begin(9600);  // put your setup code here, to run once:

  Serial.println("Starting single datastream upload to Xively...");
  Serial.println();

  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
  sensors.begin();
  sensors.setResolution(OutsideThermometer, TEMPERATURE_PRECISION);
}

void loop() {
  sensors.requestTemperatures();
  int sensorValue = (sensors.getTempCByIndex(0));  //this is teh code from MBurton
  datastreams[0].setFloat(sensorValue);

  Serial.print("Read sensor value ");
  Serial.println(datastreams[0].getFloat());

  Serial.println("Uploading it to Xively");
  int ret = xivelyclient.put(feed, xivelyKey);
  Serial.print("xivelyclient.put returned ");
  Serial.println(ret);

  Serial.println();
  delay(15000);
}
------------------------------------
------------------------------------
char xivelyKey[] = "YOUR_XIVELY_API_KEY";
// Should be something like "HsNiCoe_Es2YYWltKeRFPZL2xhqSAKxIV21aV3lTL2h5OD0g"
#define FEED_ID XXXXXX
// The 3 to 6-digit number (like 504 or 104097), that identifies the Xively Feed you're using

// For datastreams of floats:
char myFloatStream[] = "humidity";
// For datastreams of ints:
char myIntStream[] = "temperature";
// For datastreams of Strings:
String myStringStream("my_thoughts_on_the_temperature");
// For datastreams of char buffers:
char myCharBufferStream[] = "more_thoughts";  // ID of the array
const int bufferSize = 140;                   // size of the array
char bufferValue[bufferSize];                 // the array of chars itself

XivelyDatastream datastreams[] = {
  // Float datastreams are set up like this:
  XivelyDatastream(myFloatStream, strlen(myFloatStream), DATASTREAM_FLOAT),
  // Int datastreams are set up like this:
  XivelyDatastream(myIntStream, strlen(myIntStream), DATASTREAM_INT),
  // String datastreams are set up like this:
  XivelyDatastream(myStringStream, DATASTREAM_STRING),
  // Char buffer datastreams are set up like this:
  XivelyDatastream(myCharBufferStream, strlen(myCharBufferStream), DATASTREAM_BUFFER, bufferValue, bufferSize),
};

XivelyFeed feed(FEED_ID, datastreams, 4);
EthernetClient client;
XivelyClient xivelyclient(client);

datastreams[0].setFloat(1.5);                           // Push a float datapoint
datastreams[1].setInt(23);                              // Push an int datapoint
datastreams[2].setString("Pretty comfy temperature");   // Push a String datapoint
datastreams[3].setBuffer("But quite dry");              // Push a char buffer datapoint
