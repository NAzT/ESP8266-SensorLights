# ESP8266-SensorLights
Sensor driven garden lights controlled by ESP8266 and MQTT reporting via Mosquito to homekit
# Why?
I have several lights and motion sensors im my garden area. Currently I can connect one or more lights with each sensor, but it is always hard wired. Adjusting the timer length has to be done directly at the motion sensor as well as the sensitivity for darkness.
# What?
This program based on an ESP D1 mini or pro is designed to support up-to 4 relays (for the lamps) and up-to 4 sensors.
- Each sensor can switch each lamp.
- For each sensor/lamp it could be selected to operate in soft timer mode or as long as the sensor is on.
- Soft timer value can be set for each conbination.
- Web interface allows the manipulation (see pics).
- values can be saved in a JSON config file and will be read during reboot.
The status of the lights and sensors is reported via MQTT (Mosquito as broker) to NODE-RED. NODE-RED sends then the status to Apple homekit.
For the lamps is an override function programmed, that you can switch the lamps ON from homekit, but OFF does not override the sensor activity.
# Platform
Arduino IDE
# Libraries
+Makuna/Task

  all funktions are running in tasks
  - Sensor check
  - Lamp timers
  - MQTT publish
  - MQTT reconnect
  - Webserver for webpages
  
+SPIFFS

  Web pages and the config is stored in files in the SPIFFS.
  
+ArduinoJSON v6

  A very nice tool to read, store and interprete JSON data
  
+256dpi/arduino-mqtt

  I used this MQTT library, because I can define the MQTT-server and topic(s) in the loop or setup, if necessary. In the config are the MQTT data stored. Currently I do not support password for MQTT.
# Web Pages
  The web pages are stored as html in the SPIFFS. The length of the web page(s) is not limited (except by SPIFFS). I read them line by line and publish them - so it is important to have CR/LF at the end of each line. You can have in each line one "%s". The routine is looking for the %s and substituting it with a string you specify.
