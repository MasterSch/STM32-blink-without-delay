
/*
 WiFiEsp example: WebServerLed
 
 A simple web server that lets you turn on and of an LED via a web page.
 This sketch will print the IP address of your ESP8266 module (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 13.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include <Arduino.h>
#include <WiFiEsp.h>
#include <RingBufferGS.h>

void printWifiStatus();
void sendHttpResponse(WiFiEspClient client);


//char ssid[] = "zuhause";            // your network SSID (name)
//char pass[] = "schmitt.obw@web.de";        // your network password
const char* ssid = "zuhause";
const char* pass = "schmitt.obw@web.de";
int status = WL_IDLE_STATUS;

int ledStatus = LOW;

WiFiEspServer server(80);

// use a ring buffer to increase speed and reduce memory allocation
RingBufferGS buf(16);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);	// initialize digital pin LED_BUILTIN as an output.
  digitalWrite(LED_BUILTIN, LOW); 
  Serial.begin(115200);   // initialize serial for debugging
  Serial1.begin(115200);    // initialize serial for ESP module
  WiFi.init(&Serial1);    // initialize ESP module

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}


void loop()
{
  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    Serial.println("New client");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);
        
        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (buf.endsWith("GET /H")) {
          Serial.println("Turn led ON");
          ledStatus = HIGH;
          digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (buf.endsWith("GET /L")) {
          Serial.println("Turn led OFF");
          ledStatus = LOW;
          digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        }
      }
    }
    
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}


void sendHttpResponse(WiFiEspClient client)
{
  byte relayState = 103;
  int Relay90 = 0;
  char website[400] = "";
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
strcpy(website,"HTTP/1.1 200 OK\r\n Server: Arduino\r\n Content-Type:text/html\r\n Connection:close\r\n\r\n");
strcat(website,"<!DOCTYPE html>");
strcat(website,"<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
strcat(website,"<meta http-equiv=\"refresh\" content=\"10\"></head>");
strcat(website,"<body bgcolor=\"#adcede\"><font color=\"#000000\" face=\"VERDANA,ARIAL,HELVETICA\">");
strcat(website,"<center><h1>STM32F103C8 und ESP8266 Webserver</h1><hr>");
client.print(website);

client.print(F("<form method = get><center><body><br>"));
char j[2];
int i = 1; 
do {
strcpy(website,"<div>Relay ");
sprintf(j,"%d", i);
strcat(website, j);
strcat(website," <input type=\"checkbox\" name=\"rel\" value= \"");
strcat(website, j);
strcat(website,"\" onclick=\"submit();\"");
if (bitRead(relayState, i - 1) == 1) strcat(website,"checked=\"checked\"");
strcat(website,"> &nbsp;&nbsp;");
i++;
sprintf(j,"%d", i);
strcat(website, "<input type=\"checkbox\" name=\"rel\" value= \"");
strcat(website, j);
strcat(website,"\" onclick=\"submit();\"");
if (bitRead(relayState, i - 1) == 1) strcat(website,"checked=\"checked\""); 
strcat(website,"> Relay ");
strcat(website, j);
strcat(website,"</div><br><br>");
client.print(website);
delay(5);
i++; 
} while (i < 8);
strcpy(website,"<div>with <input type=\"checkbox\" name=\"rel\" value= \"");
sprintf(j,"%d", Relay90);
strcat(website, j);
strcat(website,"\" checked=\"checked\"> checkboxes</div></form>");
strcat(website,"<br><hr><br><center><a id=\"github\" href=\"https://github.com/MasterSch\">Github.com/MasterSch</a>\r\n\r\n</center></body></html>");
client.print(website); 
 //
 // The HTTP response ends with another blank line:
client.println();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
