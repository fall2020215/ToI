//----------------------------------------------------------------------------

//This sketch is combined from Adafruit TMP36 sensor and TMP36.h library
//Modified for Hackster.io project for the MKR1000 
//Since Arduino can't https, we need to use Pushingbox API (uses http)to run 
//the Google Script (uses https). Alternatly use Ivan's SecureWifi encryption

//----------------------------------------------------------------------------

#include <WiFi101.h>
#include "TMP36.h"


TMP36 tmp(A0,3.3);

const char WEBSITE[] = "api.pushingbox.com"; //pushingbox API server
const String devid = "vCA2DB0EB282CAF7"; //device ID on Pushingbox for our Scenario

const char* MY_SSID = "Frontier5200";
const char* MY_PWD =  "0851572087";


int status = WL_IDLE_STATUS;

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
    Serial.println(MY_SSID);
    //Connect to WPA/WPA2 network.Change this line if using open/WEP network
    status = WiFi.begin(MY_SSID, MY_PWD);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();
  
}

void loop() {

   // Wait between measurements.
  delay(10000);

  //prefer to use float, but package size or float conversion isnt working
  //will revise in future with a string fuction or float conversion function

  int celData = tmp.getTempC();
  int fehrData = tmp.getTempF();
  
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(celData) || isnan(fehrData))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(celData);
  Serial.print(" *C ");
  Serial.print(fehrData);
  Serial.print(" *F\t");

Serial.println("\nSending Data to Server..."); 
  // if you get a connection, report back via serial:
  
WiFiClient client;  //Instantiate WiFi object, can scope from here or Globally

    //API service using WiFi Client through PushingBox then relayed to Google
    if (client.connect(WEBSITE,80))
      { 
         client.print("GET /pushingbox?devid=" + devid
       + "&celData="      + (String) celData
       + "&fehrData="     + (String) fehrData
         );

      // HTTP 1.1 provides a persistent connection, allowing batched requests
      // or pipelined to an output buffer
      client.println(" HTTP/1.1"); 
      client.print("Host: ");
      client.println(WEBSITE);
      client.println("User-Agent: MKR1000/1.0");
      //for MKR1000, unlike esp8266, do not close connection
      client.println();
      Serial.println("\nData Sent"); 
      }
      else
      {
        Serial.println("\nCannot connect the server!\n");
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
