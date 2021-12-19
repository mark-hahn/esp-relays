#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

// uart programming adapter (number/name/color)
// GND (5 gnd blk)
// TX  (4 rx  red)
// RX  (3 tx  wht)
// D3  (2 dtr ylw)
// RST (1 rts orn) 

// wemos d1 mini pro pinout
// dx -> GPIO 
#define d0 16 // wake -- internally forced high at boot
#define d1 5
#define d2 4
#define d3 0  // (2 dtr ylw) low => program - pull-up
#define d4 2  // led s.b. high during boot - pull-up
#define d5 14
#define d6 12
#define d7 13

//#define d8 15 // s.b. low during boot - pull-down (not used here)

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

void setup(void){
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n\n');

  wifiMulti.addAP("hahn-fi", "90-NBVcvbasd");   // add Wi-Fi networks you want to connect to
//  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2");
//  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.println('.');
  }
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
  server.on("/", handleRoot);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started, waiting for relay commands... ");

  // start with all relays off 
  digitalWrite(d0, HIGH);
  digitalWrite(d1, HIGH); 
  digitalWrite(d2, HIGH); 
  digitalWrite(d3, HIGH); 
  digitalWrite(d4, HIGH); 
  digitalWrite(d5, HIGH); 
  digitalWrite(d6, HIGH); 
  digitalWrite(d7, HIGH); 

  pinMode(d0, OUTPUT); 
  pinMode(d1, OUTPUT); 
  pinMode(d2, OUTPUT); 
  pinMode(d3, OUTPUT); 
  pinMode(d4, OUTPUT); 
  pinMode(d5, OUTPUT); 
  pinMode(d6, OUTPUT); 
  pinMode(d7, OUTPUT);
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
  String cmdstr = server.arg("cmd");
  int cmd = cmdstr.toInt();
  switch (cmd) {
    case 10: digitalWrite(d0, HIGH); break;
    case 20: digitalWrite(d1, HIGH); break;
    case 30: digitalWrite(d2, HIGH); break;
    case 40: digitalWrite(d3, HIGH); break;
    case 50: digitalWrite(d4, HIGH); break;
    case 60: digitalWrite(d5, HIGH); break;
    case 70: digitalWrite(d6, HIGH); break;
    case 80: digitalWrite(d7, HIGH); break;

    case 11: digitalWrite(d0, LOW); break;
    case 21: digitalWrite(d1, LOW); break;
    case 31: digitalWrite(d2, LOW); break;
    case 41: digitalWrite(d3, LOW); break;
    case 51: digitalWrite(d4, LOW); break;
    case 61: digitalWrite(d5, LOW); break;
    case 71: digitalWrite(d6, LOW); break;
    case 81: digitalWrite(d7, LOW); break;
  }
  server.send(200, "text/plain", String(cmd));   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
