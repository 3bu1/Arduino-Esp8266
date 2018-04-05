#include <SoftwareSerial.h>
 
#define DEBUG true
 String ssidpw = "";
SoftwareSerial esp8266(6,7); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
void setup()
{
  Serial.begin(9600);
  esp8266.begin(9600); // your esp's baud rate might be different

 
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point + Station
  sendData("AT+CIPMODE=0\r\n",1000,DEBUG); // Set transfer mode,normal(0) or transparent transmission(1).
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  
   delay(3000);
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
  delay(3000);

  
  
  //sendData("AT+CWJAP="+ssid()+"\r\n",2000,DEBUG); // configure ssid & password
   
}
 
void loop()
{
 
  if(esp8266.available()) // check if the esp is sending a message 
  {

  #if 1     
    if(esp8266.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
      String webpage ="\"";                           // the ASCII decimal value and 0 (the first decimal number) starts at 48 
       esp8266.find("ssid=");
      
      while(esp8266.available()){
         char c = esp8266.read(); // read the next character.
        if(c != '&'){
     
      webpage += c;
      }else{
        webpage += "\",\"";
         break;
      }
      }
      esp8266.find("pwd=");
      
      while(esp8266.available()){
         char c = esp8266.read(); // read the next character.
        if(c != ' '){
     
      webpage += c;
      }else{
        webpage += "\"";
         break;
      }
      }
      Serial.println(webpage);
ssidpw = webpage;
    }
    #endif  
    sendData("AT+CWJAP="+ssidpw+"\r\n",2000,DEBUG); // configure ssid & password
    delay(2000);
    while(1){
      
    }
  }

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
        
        // The esp has dacta so display its output to the serial window 
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

