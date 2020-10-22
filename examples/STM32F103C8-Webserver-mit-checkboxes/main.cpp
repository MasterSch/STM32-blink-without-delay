
/*
 Für das STM32F103C8T6 Board mit den gelben ESP8266 Sockel: Serial2
 Für das STM32F103C8T6 Blue Pill: Serial1

 WiFiEsp example: WebServerLed
 
 A simple web server that lets you turn on and of an LED via a web page.
 This sketch will print the IP address of your ESP8266 module (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 13.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include <Arduino.h>
#include <WiFiEsp.h>
#include <TM1638GS.h>

void sendHomepage(WiFiEspClient client);
void WiFi_Traffic();
void sendErrpage();
int Find_Start(const char *, const char *);
void setRelay();
void DoIt();
void processKey();


const char* ssid = "zuhause";
const char* pass = "schmitt.obw@web.de";
int status = WL_IDLE_STATUS;
int LEDpin[] = {0,1,2,3,4,5,6,7,16,17}; // PA0-PA7,PB0,PB1

static char request[80], request1[80], website[400];
byte relayState = 0, status1 = 0, state = 0, keyOld = 0, keyNew = 0, keyPressTime = 0;
unsigned long starttime, prevTime = 0;
int waitTime = 0, x = 0, end = 0;
char displayBuffer[25] = "", Relay90 = '0';
const char* pos;

TM1638GS LedAndKey(19,20,21);   // PB3 PB4 PB5    STM32  strobe, clock, data

WiFiEspServer server(80);
// Initialize the client library
WiFiEspClient client;


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);	// initialize digital pin LED_BUILTIN as an output.
  digitalWrite(LED_BUILTIN, LOW); 

  int i = 0;
  do {
   pinMode(LEDpin[i], OUTPUT);
   i++;
} while (i < 10);
  digitalWrite(LEDpin[8], 1);  // LEDpin aus
  digitalWrite(LEDpin[9], 1);

  LedAndKey.reset();
  LedAndKey.set_Brightness(2);
  LedAndKey.set_Chars(0,"StArt...");
  setRelay();


  Serial.begin(115200);   // initialize serial for debugging
  Serial2.begin(115200);    // initialize serial for ESP module
  delay(3000);
  WiFi.init(&Serial2);    // initialize ESP module

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
  
  // Start the server
  server.begin();
  Serial.println("Server started");
 
 while (millis() - prevTime > 50)     // each 50ms tm1638 check
    prevTime += 50;
  
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("Use this URL : ");
  Serial.print("http://");
  sprintf(displayBuffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  Serial.println(displayBuffer);
  state = 1;
 
}



void loop() {

WiFi_Traffic();


 if (millis() - prevTime > 50)  {   // each 50ms tm1638 check
 //prevTime = millis();    // timer restart
 prevTime += 50;
 if (state > 0)   DoIt();
 processKey();
 }

}     //    end loop


void setRelay() {
int i = 0;
do {
if (bitRead(relayState,i) == 1)  digitalWrite(LEDpin[i], 0);
else digitalWrite(LEDpin[i], 1);
i++;
} while (i < 8);
//LedAndKey.setColorLEDs(0x00, relayState);  // <--  green
LedAndKey.setColorLEDs(relayState, 0x00);  // <--  red
}



void DoIt() {

switch (state)  {

case 1:     // init and display message
x = 0;
end = 0;
while (displayBuffer[x] != '\0')
x++;
if (x > 7) {
  end = x - 7;
  x = 0;
}
  else end = x;
LedAndKey.set_Chars(0, displayBuffer);
waitTime = 10;     // 20 * 50ms = 1 sec.
state++;
break;

case 2:     // wait
if (waitTime > 0) waitTime--;
else state++; 
break;

case 3:     //  shift left if needed
if (waitTime > 0) waitTime--; 
else {
waitTime = 5;
 
    if (x == end) {
    waitTime = 40;     // 40 * 50ms = 1 Sec.
    state++; 
    } else {
    pos = displayBuffer + x;
    LedAndKey.set_Chars(0, pos);
    x++;
    }
}
break;

case 4:     // wait again
 if (waitTime > 0) waitTime--;
 else state++; 
break;

case 5:     //  finished
 LedAndKey.clear_digits();
 state = 0;
break;

   }
}



void processKey() {
keyNew = LedAndKey.get_buttons();
if (keyNew == 0) {
   if ((keyPressTime > 0) && (keyPressTime < 8)) {  // short keypress
   strcpy(displayBuffer,"Relay ");
   x = 0;
   while (x < 8)  {
    if (bitRead(keyOld, x) == 1) break;
   x++;
   }
  sprintf(displayBuffer, "rELAy %d", x+1);
   if (bitRead(relayState, x) == 1) 
      strcat(displayBuffer, " Off");
   else    strcat(displayBuffer, " On");
   state = 1;
   relayState ^= keyOld;    // XOR toggle Bit
   setRelay();
     }
  keyOld = 0;       
  keyPressTime = 0;
} else {          // key s(till) pressed
if (keyPressTime == 9) {
IPAddress ip = WiFi.localIP();
sprintf(displayBuffer, "IP = %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
state = 1;
}
keyPressTime++;
keyOld = keyNew;
}   

}


void WiFi_Traffic() {
  char my_char;
  int htmlPtr = 0;
  int myIndex;
  int temp1 = 0;

  // Check if a client has connected
  client = server.available();
  if (!client)  {
    return;
  }

  unsigned long my_timeout = millis() + 40L;
  while (!client.available() && (millis() < my_timeout) ) delay(3);
  
  if (millis() > my_timeout)  {
    return;
  }
  //---------------------------------------------------------------------
  htmlPtr = 0;
  my_char = 0;
    // Read the first line of the request
  while (client.available() && my_char != '\r') {
    my_char = client.read();
    request[htmlPtr++] = my_char;
  }
  client.flush();
  request[htmlPtr] = 0;

  Serial.print("request: ");
  Serial.println(request);
 
 if(strcmp(request, request1) == 0) {
      Serial.print("refresh!!!"); 
      sendHomepage(client);
          // close the connection
    client.stop();
    Serial.println("Client disconnected");
     return;
 }

  // Match the request
    if (Find_Start("/favicon", request) > -1) {
    sendErrpage();
    } else {
if (Find_Start("GET / ", request) > -1) {
     sendHomepage(client);
   }
   else {
 myIndex = Find_Start("/?rel", request);
 if (myIndex > -1) {
   status1 = 0;
do  {
  Serial.print("myIndex = ");
  Serial.println(myIndex);
  my_char = request[myIndex + 6];
  Serial.print("relay # = ");
  Serial.println(my_char);
  temp1 = my_char - 48;
  if ((temp1 > 9) || (temp1 < 0))
  {
    Serial.println("--- error--- ");
    sendErrpage();
    return;
  }
  Serial.print("temp1 = ");
  Serial.println(temp1);
  my_char = request[myIndex + 7];
  Serial.print("trenner = ");
  Serial.println(my_char);
  if ((temp1 < 9) && (temp1 > 0)) bitSet(status1, temp1 - 1);
  myIndex+=6;
  } while (my_char == 38);   // = &
  temp1 = (relayState ^ status1);
  Serial.print("xor = ");
  Serial.println(temp1);
  htmlPtr = 0;
  while (temp1 > 0) {
    temp1 = temp1 >> 1;
    htmlPtr += 1;
  }
  Serial.print("Relay ");
  Serial.print(htmlPtr);
  sprintf(displayBuffer, "rELAy %d", htmlPtr);
  if (bitRead(status1, htmlPtr - 1) == 1)
   {
     Serial.println(" ein ");
     strcat(displayBuffer, " On");
   } else {  
     Serial.println(" aus ");
     strcat(displayBuffer, " Off");
   }
  state = 1;
  relayState = status1;
  Serial.print("relayState = ");
  Serial.println(relayState);
     setRelay();
  strcpy(request1, request);
  if (Relay90 == '0') Relay90 = '9'; else Relay90 = '0';
     sendHomepage(client);
     } else    sendErrpage();
    }  
  }

      // close the connection
    client.stop();
    Serial.println("Client disconnected");
}  


void sendErrpage()
 {
 Serial.println("");
 Serial.println("Sende 404 not found Fehler....");
 client.print(F("HTTP/1.1 404 Not found\r\n Content-Type:text/html\r\n Connection:close\r\n\r\n"));    
 client.print(F("<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"));
 client.print(F("<title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>"));
 }
//---------------------------------------------------------------------


int Find_Start(const char * such, const char * str) {
  int tmp = -1;
  int ww = strlen(str) - strlen(such);
  int ll = strlen(such);

  for (int i = 0; i <= ww && tmp == -1; i++) {
    if (strncmp(such, &str[i], ll) == 0) tmp = i;
  }
  return tmp;
}


void sendHomepage(WiFiEspClient client)
{
  //byte relayState = 103;
  int Relay90 = 0;

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

