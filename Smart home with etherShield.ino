/*


###############################################################
Smart home with etherShield

C by Nikodem Bartnik   October 2014

###############################################################


*/


//libraries
#include "etherShield.h"
#include "ETHER_28J60.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


static uint8_t mac[6] = {0x54, 0x55, 0x58, 0x10, 0x01, 0x24};   // this just needs to be unique for your network, 
                                                                // so unless you have more than one of these boards
                                                                // connected, you should be fine with this value.
                                                           
static uint8_t ip[4] = {192, 168, 0, 101};                       // the IP address for your board. Check your home hub
                                                                // to find an IP address not in use and pick that
                                                                // this or 10.0.0.15 are likely formats for an address
                                                                // that will work.

static uint16_t port = 80;                                      // Use port 80 - the standard for HTTP

ETHER_28J60 ethernet;
double tempin;
double tempout;
boolean light;
boolean light2;
boolean PIR;
boolean rain;
int alarm;
boolean check;







void setup() {
  
 

   delay(200);
  ethernet.setup(mac, ip, port);
  
   //temperature sensor
   sensors.begin();
   sensors.setWaitForConversion(false); 
   sensors.setResolution(12); 
   sensors.requestTemperatures();
   
   // define pins
   pinMode(A0, INPUT);
   pinMode(5, OUTPUT);
   pinMode(6, OUTPUT);
   pinMode(3, INPUT);
   pinMode(4, INPUT);
   pinMode(7, INPUT);
 
}
 




void loop() {
  
  char* params;
  if (params = ethernet.serviceRequest()) {
    
    
    //when you type in browser http://192.168.0.101/all
    if (strcmp(params, "all") == 0){
      
    sensors.requestTemperatures();
    tempin = sensors.getTempCByIndex(0);
    sensors.requestTemperatures();
    tempout = sensors.getTempCByIndex(1);
    
    
    ethernet.print("<center>");
    ethernet.print("<font color='teal'>");
    ethernet.print("<h1>Smart Home</h1>"); 
    ethernet.print("</font>");
    ethernet.print("<br>");
    
    ethernet.print("<h2>Temp in: "); 
    ethernet.print(tempin); 
    ethernet.print("</h2>");
    
    ethernet.print("<h2>Temp out: "); 
    ethernet.print(tempout); 
    ethernet.print("</h2>");
    
    check = true;  
    ethernet.print("<h2>Alarm: "); 
    ethernet.print(alarm); 
    ethernet.print("</h2>");
    
    ethernet.print("<h2>Rain: "); 
    rain = digitalRead(4);
    if(rain==true){
      ethernet.print("NO"); 
    }
    if(rain==false){
     ethernet.print("YES");                   
    }
    ethernet.print("</h2>");
    
    
    ethernet.print("<h2>Window: "); 
    if(digitalRead(7)==HIGH){
      ethernet.print("CLOSE");
    }
    else{
      ethernet.print("OPEN");
    }
    ethernet.print("</h2>");
    
    
    ethernet.print("Lamp: ");
    ethernet.print("<a href='light'><button>Lamp</button></a>");
    
    
    ethernet.print("<br>");
    ethernet.print("<br>");
   
   ethernet.print("Instruction how make your smart home: <a href='http://www.instructables.com/member/Nikus/'>link</a>");
   ethernet.print("<br>");
   ethernet.print("C by Nikodem Bartnik");
   
   ethernet.print("</center>");
    
    
    
    
                          
    }
    
    
    
    // TEMP IN ********************
    if (strcmp(params, "tempin") == 0){
    sensors.requestTemperatures();
    tempin = sensors.getTempCByIndex(0)*100;
    ethernet.print(tempin);                        // print temperature in on ethernet
    }
    
    
    // TEMP OUT *******************
    if (strcmp(params, "tempout") == 0){
    sensors.requestTemperatures();
    tempout = sensors.getTempCByIndex(1)*100;
    ethernet.print(tempout);                      // print temperature out on ethernet
    }
    
    
    
    // ALARM **********************
    if (strcmp(params, "alarm") == 0){
      
      check = true;  
      
      ethernet.print(alarm); 
    

    }
    
    
    
    // RAIN ***********************
    if (strcmp(params, "rain") == 0){
    rain = digitalRead(4);
    if(rain==true){
      ethernet.print("NO"); 
    }
    if(rain==false){
     ethernet.print("YES");                   
    }
    }
    
    
    // WINDOW **********************
    if (strcmp(params, "window") == 0){
    if(digitalRead(7)==HIGH){
      ethernet.print("close");
    }
    else{
      ethernet.print("open");
    }
    }
    
    //LIGHT INTERNET****************
    if(strcmp(params, "light") == 0){
    ethernet.print("Lamp: ");
    ethernet.print(!light2);
    ethernet.print("<br>");
    ethernet.print("change: ");
    ethernet.print("<a href='light'><button>Lamp</button></a>");
    light2=!light2;
    digitalWrite(6, light2);
    
  
    }  
    
    

    
    
    // ETHERNET RESPOND ************
    ethernet.respond();
    delay(20);
    }
    
    
    
    // LIGHT ON CLAP ****************
    if(analogRead(A0)>350){
    light=!light;
    digitalWrite(5, light);
    delay(1000);
    }  
    
    
    //MOVE SENSOR*******************
    PIR = digitalRead(3);
    if(check==true){
    if(PIR==true){
       alarm = 1;
       check = false; 
       
    }
    if(PIR==false){
     alarm = 0;   
    }    
    }
  
  }



