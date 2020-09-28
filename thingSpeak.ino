/*
This sketch is a combination of ADAFruits DHT sketch, WiFi101 Webclient
and The arduino example from ThingSpeak
Modified by Stephen Borsay for the MKR1000, feel free to use
 */

#include <SPI.h> //you don't need this as we arn't using the shiled just chip
#include <WiFi101.h>
#include "TMP36.h"


TMP36 tmp(A0,3.3);

String apiKey ="U960YDSZZ6BKSM9G"; // api from ThingSpeak

char ssid[] = "Frontier5200"; //  your network SSID (name)
char pass[] = "0851572087";    //your network password
int keyIndex = 0;     // your network key Index number (needed only for WEP)

//#define WEBSITE "api.thingspeak.com"

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.thingspeak.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    //Connect to WPA/WPA2 network.Change this line if using open/WEP network
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();
  
}

void loop() {

   // Wait a few seconds between measurements.
  delay(2000);

  //prefer to use float, but package size or float conversion isnt working
  //will revise in future with a string fuction or float conversion function

 int celData = tmp.getTempC();
 int fehrData = tmp.getTempF();

  // Check if any reads failed and exit early (to try again).
  if (isnan(celData) || isnan(fehrData))
  {
    Serial.println("Failed to read from TMP36 sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(celData);
  Serial.print(" *C ");
  Serial.print(fehrData);
  Serial.print(" *F\t");


    Serial.println("\nStarting connection to server..."); 
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("**********Connected to server*************\n");


          client.print(F("POST "));
          client.print("/update?key=apiKey&field1=" 
          
          + (String) celData
          +  "&field2=" +(String) fehrData
                                   );
                                      
          String tsData = "field1="   //need the length to give to ThingSpeak
          + (String) celData
          +  "&field2=" +(String) fehrData

        ; 


          client.print("POST /update HTTP/1.1\n");  
          client.print("Host: api.thingspeak.com\n");
          client.print("Connection: close\n");
          client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
          client.print("Content-Type: application/x-www-form-urlencoded\n");
          client.print("Content-Length: ");
          client.print(tsData.length());  //send out data string legth to ts
          client.print("\n\n");
          client.print(tsData);
         client.stop();
          delay(1000);
    } 
    else
    {
      if(status)
      setup();
      
      Serial.println("\n!!!!!!!!!!!Cannot Connect the server!!!!!!!!!!\n"); 
  
      
    }

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
}
