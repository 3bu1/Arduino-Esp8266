#include <SoftwareSerial.h>
 
#define DEBUG true
 
SoftwareSerial esp8266(6,7); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  Serial.begin(9600);
  esp8266.begin(115200); // your esp's baud rate might be different
  
 
#if 0

    

  //sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  //sendData("AT+CIPSTART=TCP,api.thingspeak.com,80\r\n",1000,DEBUG); // turn on server on port 80
  //sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
 

  
#endif
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point + Station
  sendData("AT+CIPMODE=0\r\n",1000,DEBUG); // Set transfer mode,normal(0) or transparent transmission(1).
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  //sendData("AT+CWJAP=\"tribhuv\",\"tribhuvan\"\r\n",1000,DEBUG); // configure ssid & password
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  delay(3000);
  ssid();
  //sendData("AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80\r\n",3000,DEBUG); // configure for TCP connections
  
 
   
}
 
void loop()
{
    
  if(esp8266.available()) // check if the esp is sending a message 
  {
    /*
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    } */
    
    if(esp8266.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
     
     String webpage = "<h1>Hello<h2> 3bu1!</h2> </h1><h3>Greetings From ESP 8266 - 01</h3></br>"+ssid();
 
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,3000,DEBUG);
    }
  }
  
}

String ssid()
{

 char inv = '"';
  String add = "AT+CIPSTART";
  add += "=0,";
  add += inv;
  add += "TCP";
  add += inv;
  add += ",";
  add += inv;
  add += "api.thingspeak.com";  // Host_server_name
  add += inv;
  add += ",";
  add += "80";
  add += "\r\n";
#if 1
  sendData(add, 3000, DEBUG);  // Establishing a TCP Connection. ( AT+CIPSTART = "TCP","api.thingspeak.com",80 )
 delay(2000);

  String rest = "AT+CIPSEND=0,90";
  rest += "\r\n";
  sendData(rest, 3000, DEBUG); //Number of characters to send. ( "AT+CIPSEND=90" )

 delay(2000);
  String hostt = "GET /apps/thinghttp/send_request?api_key=XC06ZEW76HP2NA35";
  hostt += "\r\n";
  hostt += "Host:api.thingspeak.com";
  hostt += "\r\n";
  String ssid = sendData(hostt, 3000, DEBUG);  // GET request ( GET /apps/thinghttp/send_request?api_key=XXXXXXXXXXXXXXXX 
                                            //               Host: Host_server_name ) 
                                            
  #else
  String ts = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
  
   byte dataLength = ts.length();
  for (int i=0; i<dataLength; i++){
  Serial.print(ts[i], DEC);
   Serial.print(' ');
  }
    String ssid =sendData(ts, 3000, DEBUG);
  #endif
  

  
  return (ssid);
}

 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}

