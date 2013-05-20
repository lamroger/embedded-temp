//ARDUINO 1.0+ ONLY
//ARDUINO 1.0+ ONLY
//http://bildr.org/2011/06/arduino-ethernet-client/
#include <Ethernet.h>
#include <SPI.h>

////////////////////////////////////////////////////////////////////////
//CONFIGURE
////////////////////////////////////////////////////////////////////////
char server[] ="limitless-headland-1164.herokuapp.com"; //Address of the server you will connect to
//char server[] ="mrlamroger.bol.ucla.edu";
//The location to go to on the server
//make sure to keep HTTP/1.0 at the end, this is telling it what type of file it is
String location = "/testlamp HTTP/1.0";
//String location = "/arduino HTTP/1.0";

// if need to change the MAC address (Very Rare)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
////////////////////////////////////////////////////////////////////////

EthernetClient client;

char inString[32]; // string for incoming serial data
String variable[32];
String value[32];
int stringPos = 0; // string index counter
boolean startRead = false; // is reading?
boolean readingFirst = true;
int count = 0; //number of variables

void setup(){
  Ethernet.begin(mac);
  Serial.begin(9600);
}

void loop(){
  String pageValue = connectAndRead(); //connect to the server and read the output
  Serial.println(pageValue);
  int i;
  for (i=0; i<count; i++){
    Serial.println(variable[i]);
    //Serial.println(value[i]); 
  }
  delay(20000); //wait 20 seconds before connecting again
}

String connectAndRead(){
  //connect to the server
  
  //Initialize client
  client.stop();
  client.flush();
  Serial.println("connecting...");

  //port 80 is typical of a www page
  if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /testlamp HTTP/1.1");
    client.println("Host: limitless-headland-1164.herokuapp.com");
//    client.println("GET /arduino HTTP/1.1");
//    client.println("Host: mrlamroger.bol.ucla.edu");
    client.println("Connection: close");
    client.println();

    //Connected - Read the page
    return readPage(); //go and read the output
    
   
  }else{
    Serial.println("connection failed");
  }

}

String readPage(){
  //read the page, and capture & return everything between '<' and '>'

  stringPos = 0;
  memset( &inString, 0, 32 ); //clear inString memory
  count = 0;

  while(true){
    if (client.available()) {
      Serial.println("Client is available");
      while (char c = client.read()) {
        if (c == '>') {
          client.stop();
          client.flush();
          Serial.println("disconnecting.");
          return "End";
        } else 
        
        if (c == '[' ) { //'<' is our begining character
          startRead = true; //Ready to start reading the part 
          memset( &inString, 0, 32 );
          stringPos = 0;
        } else if(startRead){
          if(c != ']'){ //'>' is our ending character
            //Serial.print(c);
            inString[stringPos] = c;
            stringPos ++;
          } else {
  //          Serial.println(inString);
  //          //got what we need here! We can disconnect now
  //          if(readingFirst){
  //            String temp = inString;
  //            Serial.println(temp);
  //            variable[count] = inString;
  //            readingFirst = false;
  //          } else {
  //            String temp = inString;
  //            Serial.println(temp);
  //            value[count] = temp;
  //            readingFirst = true;
  //          }            
  //          startRead = false;
            
            variable[count] = inString;
            //Serial.println(variable[count]);
            count = count + 1;
            //memset( &inString, 0, 32 );
            //Serial.println("Test");
            startRead = false;
            //client.stop();
            //client.flush();
            //Serial.println("disconnecting.");
            //return variable[count - 1];
          } 
        }
      }
    }
  }
}
