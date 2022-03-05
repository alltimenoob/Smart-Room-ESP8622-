#include<FirebaseArduino.h>
#include<ESP8266WiFi.h>
#include<Espalexa.h>


#define WIFI_SSID "ZTE"
#define WIFI_PASSWORD "36988211"
#define DATABASE_URL "smart-room-9eac0-default-rtdb.firebaseio.com"

#define USER_EMAIL "mihirchauhan756@gmail.com"
#define USER_PASSWORD "VeryGood@123"

Espalexa Alexa;

const int lightPin = 12;
const int fanPin = 14;

long int LstartTime = 0;
long int LendTime = 0;
long int FstartTime = 0;
long int FendTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(lightPin,OUTPUT);
  pinMode(fanPin,OUTPUT);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  Alexa.addDevice("Mihir's Room Tubelight",lightChanged,EspalexaDeviceType::onoff);
  Alexa.addDevice("Mihir's Room Fan",fanChanged,EspalexaDeviceType::onoff);
  Alexa.begin();

  Firebase.begin("smart-room-9eac0-default-rtdb.firebaseio.com","HmoqLVVnZKKfGuZB3s7RwLLuTJ4FryxKjZ1eComH");
  Firebase.setBool("Light",false);
  Firebase.setBool("Fan",false);
}

void loop() {
  
  Alexa.loop();
}

void lightChanged(EspalexaDevice* device)
{
  bool firestate = Firebase.getBool("Light");
  if(device != NULL)
  {
    bool devicestate = device->getValue();

    Serial.println(devicestate);
   
    if(devicestate!=firestate)
    {
      firestate=!firestate;
      Firebase.setBool("Light",firestate);
    }
    
    if(devicestate && firestate)
    {
      digitalWrite(14,1);
      Serial.println("Light On");
      Firebase.setBool("Light",true);
      LstartTime = millis();
    }
    else if(!devicestate && !firestate)
    {
      digitalWrite(14,0);
      Serial.println("Light Off");
      Firebase.setBool("Light",false);
      LendTime = millis();
      Firebase.pushFloat("Light-Consumption",2.77778e-7 * (LendTime - LstartTime));
    }
  }
}

void fanChanged(EspalexaDevice* device)
{
  bool firestate = Firebase.getBool("Light");
  if(device != NULL)
  {
    bool devicestate = device->getValue();

    Serial.println(devicestate);
   
    if(devicestate!=firestate)
    {
      firestate=!firestate;
      Firebase.setBool("Fan",firestate); 
    }
    
    if(devicestate && firestate)
    {
      digitalWrite(12,1);
      Serial.println("Fan On");
      Firebase.setBool("Fan",true);
      FstartTime = millis();
    }
    else if(!devicestate && !firestate)
    {
      digitalWrite(12,0);
      Serial.println("Fan Off");
      Firebase.setBool("Fan",false);
      FendTime = millis();
      Firebase.pushFloat("Fan-Consumption",2.77778e-7 * (FendTime - FstartTime));
    }
  }
}
