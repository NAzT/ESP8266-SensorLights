// HTTP requests are defined here

serverWS.on("/", handleRoot);
serverWS.on("/sensoren", handleSensoren);
serverWS.on("/timer",handleTimer);
serverWS.on("/timer2",handleTimer2);
serverWS.on("/loadconfig",handleloadconfig);
serverWS.on("/saveconfig",handlesaveconfig);
serverWS.on("/lamp",handleLampOnOFF);
serverWS.on("/favicon.ico", HTTP_GET, [](){serverWS.send(404, "text/html", "Not set");});
serverWS.on("/sensorenspeichern", HTTP_POST,handleSensorenSpeichern);
serverWS.on("/timerspeichern", HTTP_POST,handleTimerSpeichern);
serverWS.on("/timerspeichern2", HTTP_POST,handleTimer2Speichern);
serverWS.on("/index.css", [](){
  String path = "/index.css";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (serverWS.streamFile(dataFile, "text/css") != dataFile.size()) {  }
  dataFile.close();});
serverWS.on("/timer.js", [](){
  String path = "/timer.js";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (serverWS.streamFile(dataFile, "application/javascript") != dataFile.size()) {  }
  dataFile.close();});
serverWS.onNotFound(handleNotFound);
