#include <SPI.h>
#include <Ethernet.h>


int pin0 = 0;
int pin1=1;
int pin2=2;
int pin3=3;
int pin4=4;// analog pin
int tempc = 0;
int tempf=0;
int x=0;
int y=0;
int z=0;// temperature variables
int samples[8]; // variables to make a better precision
int maxi = -100,mini = 100; // to start max/min temperature
int i;
int led=8;




// Local Network Settings
byte mac[]     = { 0xc4, 0x8e, 0x08f, 0x0e, 0x44, 0x35 }; // Must be unique on local network
byte ip[]      = { 192,168,225,165 };                // Must be unique on local network
byte gateway[] = { 192,168,225,1};
byte subnet[]  = { 255, 255, 255, 0 };

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "1CYGO7G2YVVFAULW";    // Write API Key for a ThingSpeak Channel
const int updateInterval = 100;        // Time interval in milliseconds to update ThingSpeak   

// Variable Setup
long lastConnectionTime = 0; 
boolean lastConnected = false;
int failedCounter = 0;

// Initialize Arduino Ethernet Client
EthernetClient client;

void setup()
{
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  Ethernet.begin(mac, ip, gateway, subnet);
  delay(1000);
  Serial.print("ETHERNET SHIELD ip  is     : ");
  Serial.println(Ethernet.localIP()); 
   // Start Ethernet on Arduino
  startEthernet();
}

void loop()
{
  
 // tempc = ( analogRead(pin) * 100.0) / 1024.0;
    tempc=(1024/analogRead(pin0)*100);
    tempf= analogRead(pin1);
    x= analogRead(pin2);
    y= analogRead(pin3);
    z= analogRead(pin4);
    
    
  String analogPin0 = String(tempc);
  String analogPin1 = String(tempf);
  String analogPin2 = String(x);
  String analogPin3 = String(y);
  String analogPin4 = String(z);
  
   
       
  
  // Print Update Response to Serial Monitor
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }
    
  // Disconnect from ThingSpeak
  if (!client.connected() && lastConnected)
  {
    Serial.println();
    Serial.println("...disconnected.");
    Serial.println();
    
    client.stop();
  }
  
  // Update ThingSpeak
  if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("&field1="+analogPin0+"&field2="+analogPin1+"&field3="+analogPin2+"&field4="+analogPin3+"&field5="+analogPin4);
  }
  
  lastConnected = client.connected();
  
  
  if (tempc >350){
      digitalWrite(led, HIGH);
      
       if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("&field1="+analogPin0+"&field2="+analogPin1+"&field3="+analogPin2+"&field4="+analogPin3+"&field5="+analogPin4);
  }
      //delay(1000);
     }
          
   else if (tempf <1000){
      digitalWrite(led, HIGH);
      //delay(2000);
      
      
       if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("&field1="+analogPin0+"&field2="+analogPin1+"&field3="+analogPin2+"&field4="+analogPin3+"&field5="+analogPin4);
  }
  
  
  
     }
     
   else if (x <400){
      digitalWrite(led, HIGH);
   
     // delay(2000);
      
       if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("&field1="+analogPin0+"&field2="+analogPin1+"&field3="+analogPin2+"&field4="+analogPin3+"&field5="+analogPin4);
  }
  
   }
  
   else if (y>420){
      digitalWrite(led, HIGH);
   
     // delay(2000);
      
       if(!client.connected() && (millis() - lastConnectionTime > updateInterval))
  {
    updateThingSpeak("&field1="+analogPin0+"&field2="+analogPin1+"&field3="+analogPin2+"&field4="+analogPin3+"&field5="+analogPin4);
  }
  
  
  }
     
     
   else{
     digitalWrite(led, LOW);
     }
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  { 
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+writeAPIKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(tsData.length());
    client.print("\n\n");

    client.print(tsData);
    
    lastConnectionTime = millis();
    
    if (client.connected())
    {
      Serial.println("Connecting to ThingSpeak...");
      Serial.println();
      
      failedCounter = 0;
    }
    else
    {
      failedCounter++;
  
      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
}

void startEthernet()
{
  
  client.stop();

  Serial.println("Connecting Arduino to network...");
  Serial.println();  

  delay(1000);
  
  // Connect to network amd obtain an IP address using DHCP
  if (Ethernet.begin(mac) == 0)
  {
    Serial.println("DHCP Failed, reset Arduino to try again");
    Serial.println();
  }
  else {
    Serial.println("Arduino connected to network using DHCP");
    Serial.println();
    Serial.println("Data being uploaded to THINGSPEAK Server.......");
    Serial.println();
  }
  
  delay(1000);
}

