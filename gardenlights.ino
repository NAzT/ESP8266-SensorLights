
// gardenlight 
// switch lights by sensor input
// on-time of lights could be selected for each timer/lamp combination independently 
// to sensor on/off or to timer.
// each sensor can switch more than one light

#define FIRMWARE    "Firmware V 0.0.1"
#define HOST_NAME   "Aussenlicht1"
#define MQTT_SERVER "192.168.2.90"
#define SID         "klaushome"
#define PW          "Johann1965"
#define MQTT_TOPIC  "/status"
#define MQTT_PORT   1883
#define CONFIGFILE  "config.ini"

// Pin 2 has an LED connected on ESP8266 D1 mini boards.
#define RelayPin1 16 
#define RelayPin2 14
#define RelayPin3 12 
#define RelayPin4 13
#define AButtonPin 5
#define BButtonPin 4
#define CButtonPin 0
#define DButtonPin 2

String host_name=HOST_NAME, mqtt_server=MQTT_SERVER, sid = SID, pw=PW;
String mqtt_topic=MQTT_TOPIC;
int mqtt_port=MQTT_PORT;
uint8_t sensorpin[4]={AButtonPin,BButtonPin,CButtonPin,DButtonPin};
uint8_t relaypin[4]={RelayPin1,RelayPin2,RelayPin3,RelayPin4};
//define which sensor switches which relais
bool sensorrelay[4][4]={{true,true,false,false},{true,true,false,false},{false,false,false,false},{false,false,false,false}};
//define which sensor/relay action is based on sensor-on time (false) or on timer (true)
bool timermode[4][4]={{true,false,false,false},{false,true,false,false},{false,false,false,false},{false,false,false,false}};
//timer time in 1/10 seconds, can be set via webpage
int timer[4][4]={{100,100,100,100},{100,100,100,100},{100,100,100,100},{100,1000,100,100}};
//relay status (which sensor has switched which relay)
bool relayon[5][4]={{false,false,false,false},{false,false,false,false},{false,false,false,false},{false,false,false,false},{false,false,false,false}};
//actual timer counters for each lamp, will count down to zero in 1/10 second steps
int lamptimeron[4][4];
int wCounter = 0;
bool noWiFi = false;
int8_t MQTT_Status;

// include libraries
#include <Task.h>
#include <ESP8266WiFi.h>
/*#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"*/
#include <MQTT.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include <ArduinoJson.h>

// include sub files
#include "ButtonTask.h" // this implements the button task
#include "LampTimerTask.h"
#include "config.h"

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
WiFiClient client;
MQTTClient mqtt(256);

//Adafruit_MQTT_Client mqtt(&client, mqtt_server.c_str(), mqtt_port, "", AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'shadestatus' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

// Adafruit_MQTT_Publish lampstatus = Adafruit_MQTT_Publish(&mqtt, topic.c_str());

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).

void MQTT_connect();

TaskManager taskManager;
IPAddress ip;

// WiFiServer server(1337);  // 80
ESP8266WebServer serverWS(80);

//forward declare Handles for Tasks
void HandleButtonChanged(ButtonState state, uint8_t sensorno);
void Handle_Check_Client(uint32_t deltaTime);
void Handle_MQTT_Connect(uint32_t deltaTime);
void Handle_MQTT_Publish(uint32_t deltaTime);
 
// foreward delcare functions passed to task constructors now required
ButtonTask AButtonTask(HandleButtonChanged, 0); 
ButtonTask BButtonTask(HandleButtonChanged, 1);
ButtonTask CButtonTask(HandleButtonChanged, 2); 
ButtonTask DButtonTask(HandleButtonChanged, 3);
TaskLampTimer LampTimerTask(MsToTaskTime(100));
FunctionTask Check_Client_Task(Handle_Check_Client, MsToTaskTime(8));
FunctionTask Connect_MQTT_Task(Handle_MQTT_Connect, MsToTaskTime(2000));
FunctionTask Publish_MQTT_Task(Handle_MQTT_Publish, MsToTaskTime(120000));

#include "handleWIFI.h"

void setup()
{
  Serial.begin(115200);
  delay(500);  // überspielt Zeichen beim Reset
  Serial.println();
  Serial.println("Starting...");
  Serial.println(FIRMWARE);
  
  #include "FSstart.h"
  #include "wifistart.h"
  #include "HTTP_Server_Requests.h";
    
  taskManager.StartTask(&AButtonTask); 
  taskManager.StartTask(&BButtonTask); 
  taskManager.StartTask(&CButtonTask); 
  taskManager.StartTask(&DButtonTask);
  taskManager.StartTask(&LampTimerTask);
  taskManager.StartTask(&Check_Client_Task);  //listening to WiFi  
  taskManager.StartTask(&Connect_MQTT_Task); //connect to mqtt server
  taskManager.StopTask(&Publish_MQTT_Task); //MQTT publizieren   
  Serial.println("Running...");      
}

void loop()
{
    taskManager.Loop();
}

void Lamp_Publish() {
  String msg_MQTT;
  if (mqtt.connected()) {
    msg_MQTT = "{\"Lamp\":{\"1\":"+String(1-digitalRead(relaypin[0]))+",\"2\":"+String(1-digitalRead(relaypin[1]))+",\"3\":"+String(1-digitalRead(relaypin[2]))+ ",\"4\":"+String(1-digitalRead(relaypin[3]))+"}}";
    if (! mqtt.publish(mqtt_topic.c_str(),msg_MQTT.c_str())) {
      Serial.println("Failed");
    } else {
      Serial.println("Published Lamp_Publish!");
    }
  } else {
    taskManager.StartTask(&Connect_MQTT_Task);
  } 
 }

void Sensor_Publish(ButtonState state, uint8_t sensorno) {
  String msg_MQTT;
  if (mqtt.connected()) {
  msg_MQTT = "{\"Sensor\":{\""+String(sensorno+1)+"\":"+String(state)+"}}";  
  if (!mqtt.publish(mqtt_topic.c_str(),msg_MQTT.c_str())) {
    Serial.println("Failed");
  } else {
    Serial.print("Published: Sensor #");
    Serial.print(sensorno);
    Serial.print(":");
    Serial.println(state);
  }
  } else {
    taskManager.StartTask(&Connect_MQTT_Task);
  } 
 }

void HandleButtonChanged(ButtonState state, uint8_t sensorno)
{
  int i;
    // apply on press
  if (state & ButtonState_Pressed) // any state that is pressed
  {
    if (state == ButtonState_Pressed)
    {
      for (i=0;i<4;i++) {
        if (sensorrelay[sensorno][i])
        {
          digitalWrite(relaypin[i], LOW);   // turn the lamp on (LOW is the voltage level)
          relayon[sensorno][i]=true;
          if (timermode[sensorno][i])
          {
            lamptimeron[sensorno][i]=timer[sensorno][i];
          }
        } 
      }
      Sensor_Publish(state, sensorno);
      Lamp_Publish();
      Serial.print("Pressed - ");
      Serial.println(sensorno);
    }
  }
  else if (state == ButtonState_Released)
  {
    for (i=0;i<4;i++) {
      if (sensorrelay[sensorno][i]&&!timermode[sensorno][i])
      {
//       turn the LED off - will be done in the LampTimerTask
        relayon[sensorno][i]=false;
      }
    }
    Sensor_Publish(state, sensorno);
    Serial.print("Released - ");
    Serial.println(sensorno);
  }
}
#include "MQTT_Task.h";
