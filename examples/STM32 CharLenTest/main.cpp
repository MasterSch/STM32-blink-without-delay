#include <Arduino.h>


USBSerial USB;

char Test[] = "x";

char Top1[]="";
char Top2[]="HTTP/1.1 404 Not found\r\n Content-Type:text/html\r\n Connection:close\r\n\r\n";
char Top3[]="<html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
char Top4[]="<title>404 Not Found</title></head><body><h1>Not Found</h1><p>The requested URL was not found on this server.</p></body></html>";

char Body[]="<body><center><br><br><h1>ESP8266 WiFi Web Server V14 </h1><br><br>";

char ButtonLine[]="<a href=\"?rel1+\"><button class=\"an1\">Relay 1 ON </button></a><a href=\"?rel2-\"><button class=\"an0\">Relay 1 OFF</button></a><br><br>";

char Bottom[]="<br><br><hr><a id=\"google\" href=\"http://google.de\">Google.de</a>\r\n\r\n";


void setup()   
{

USB.begin(115200); //This pipes to the USB monitor
delay(3000);
USB.println("Hello world");  // Print "Hello World" to the USB Monitor

}

void loop()
{

int TestLen = sizeof(Test);
int Top1Len = sizeof(Top1), Top2Len = sizeof(Top2), Top3Len = sizeof(Top3), Top4Len = sizeof(Top4);
int BodyLen = sizeof(Body), ButtonLineLen = sizeof(ButtonLine),  BottomLen= sizeof(Bottom);

USB.println();
USB.println("Längenangaben sind 1 zu lang wegen abschliessender 0");
USB.print("       Test Länge  : ");
USB.println(TestLen);

USB.print("       Top1 Länge  : ");
USB.println(Top1Len);

USB.print("       Top2 Länge  : ");
USB.println(Top2Len);

USB.print("       Top3 Länge  : ");
USB.println(Top3Len);

USB.print("       Top4 Länge  : ");
USB.println(Top4Len);


USB.print("       Body Länge  : ");
USB.println(BodyLen);

USB.print(" Buttonline Länge  : ");
USB.println(ButtonLineLen);

USB.print("     Bottom Länge  : ");
USB.println(BottomLen);

USB.println();
USB.println();

int i = 0;
do  {
//  USB.print(i);
//  USB.print("  ");
  USB.print(ButtonLine[i]);
  i++;
}  while (i < ButtonLineLen);
USB.println();
delay(5000);
}





