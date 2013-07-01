/* Code for CloudHouse Greenhouse, based on the following:

DHT humidity/temperature sensor sketch from Adafruit -http://learn.adafruit.com/dht/using-a-dhtxx-sensor
JSON examples from Interactive Matter Lab: http://interactive-matter.eu/how-to/ajson-arduino-json-library/
Wifi Hub exaample: http://arduino.cc/en/Tutorial/WiFiCosmClientString/
Requires
DHT Library: https://github.com/adafruit/DHT-sensor-library
JSON Library: https://github.com/interactive-matter/aJson
Code must be configured with WiFi network SSID and server address and login
*/ 
#include "DHT.h"
#include "aJSON.h"   
#include <SPI.h>
#include <WiFi.h>

#define DHTTYPE DHT11
#define USERAGENT      "My Arduino Project"     // user agent is the project name

char ssid[] = "Atlas Networks";      //  your network SSID (name) 

int status = WL_IDLE_STATUS;

// initialize the library instance:
WiFiClient client;


char server[] = "cloudgreenhouse.com";   // name address for server

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  //delay between updates to pachube.com

float humidity;
float temperature;
int light;
byte mac[6];
unsigned long int time;
char *json;

int DHTPIN = 2; 	
int redPin = 5;
int greenPin = 3;
int bluePin = 4;
int lightPin = 0;
int moistPin = 1;
int lightVal = 0;
int moisture = 0;


DHT dht(DHTPIN, DHTTYPE); // initialize DHT sensor 


void setup() {
  Serial.begin(9600); 
  
  dht.begin();  // star DHT sensor
  
    // Setup LED
  pinMode(redPin, OUTPUT); 
  pinMode(bluePin, OUTPUT);  
  pinMode(greenPin, OUTPUT);
  
  digitalWrite(redPin,HIGH); // turn on LED as red
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  } 
  // you're connected now, so print out the status:
  printWifiStatus(); 
  
  WiFi.macAddress(mac); // get mac address
 }

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  light = analogRead(lightPin);
  moisture = analogRead(moistPin);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT");
  }

 time = millis();
 
 int s_time = time;
 char mac_str[12];
 
 sprintf(mac_str, "%x%x%x%x%x%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6]);

// Setup json object with data recorded from sensors

 aJsonObject *root = aJson.createObject();
 aJson.addStringToObject(root, "id", mac_str);
 aJson.addNumberToObject(root, "stamp", s_time);
 aJson.addNumberToObject(root, "humidity", humidity);
 aJson.addNumberToObject(root, "temperature", temperature);
 aJson.addNumberToObject(root, "light", light);
aJson.addNumberToObject(root, "moisture", moisture); 	
 json = aJson.print(root);
 if (json != NULL) {
  Serial.println(json);
 }
 
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    digitalWrite(greenPin, HIGH); // turn on LED as green
    digitalWrite(bluePin, LOW); // turn off LED as blue
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    digitalWrite(bluePin, HIGH); // turn on LED as blue
    digitalWrite(greenPin, LOW); // turn off green LED
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data: 
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    sendData(json);
  }
  
  // store the state of the connection for next time through
  // the loop:
 lastConnected = client.connected();

//clear the json object to prepare for next time through the loop
 aJson.deleteItem(root);
 free(json); 
}

// this method makes a HTTP connection to the server:
void sendData(String thisData) {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("GET /up.php?json=");
    client.println(thisData);
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.print("Content-Length: ");
    client.println(thisData.length());

    // last pieces of the HTTP PUT request:
    client.println("Content-Type: text/csv");
    client.println("Connection: close");
    client.println();

    // here's the actual content of the PUT request:
    //client.println(thisData);
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    digitalWrite(bluePin, HIGH); // turn on LED as blue
    digitalWrite(greenPin, LOW); // turn off green LED
  }
  // note the time that the connection was made or attempted:
  lastConnectionTime = millis();
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  digitalWrite(redPin, LOW); // turn on LED as blue
  digitalWrite(bluePin, HIGH); // turn on LED as red   
  
}

