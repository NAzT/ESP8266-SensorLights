//#include <FS.h>

//Initialize File System
if(SPIFFS.begin())
{
  Serial.println("SPIFFS Initialize....ok");
  if(SPIFFS.exists("/default.ini")) 
  {
    Serial.println("Config file exists");
      if (!loadConfig()) {
        Serial.println("Failed to load config");
      } else {
        Serial.println("Config loaded");
      }
  }
  else
  {
    Serial.println("ERROR: no Config file");
  }
}
else
{
  Serial.println("SPIFFS Initialization...failed");
}
