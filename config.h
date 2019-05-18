bool loadConfig() {
  int i,j;
  File configFile = SPIFFS.open("/default.ini", "r");
  if (!configFile) {
    Serial.println("Failed to open config file: default.ini");
    return false;
  }
  const size_t capacity = 15 * JSON_ARRAY_SIZE(4) + 12 * JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(9) + 250;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, configFile);
  if (error) {
    Serial.println(F("Failed to read file, using default configuration"));
    return false;
  }
  const char* Host_name=doc["hostname"]; // "Aussenlicht1"
  host_name = String(Host_name);
  Serial.println(host_name);
  const char* Sid=doc["SID"];
  sid = String(Sid);
  Serial.println(sid);
  const char* Pw=doc["PW"];
  pw = String(Pw);
  Serial.println(pw);
  const char* Mqtt_server=doc["MQTThost"]; // "192.168.2.90"
  mqtt_server = String(Mqtt_server);
  Serial.println(mqtt_server);
  const char* Mqtt_topic =doc["MQTTtopic"]; // "Aussenlicht1/status"
  mqtt_topic = String(Mqtt_topic);
  Serial.println(mqtt_topic);
  mqtt_port = doc["MQTTport"]; //1883
  Serial.println(mqtt_port);

  JsonArray sensorrelayJ = doc["sensorrelay"];
  for(i=0;i<4;i++) {
    sensorrelay[0][i]=sensorrelayJ[0]["S1"][i];
    sensorrelay[1][i]=sensorrelayJ[1]["S2"][i];
    sensorrelay[2][i]=sensorrelayJ[2]["S3"][i];
    sensorrelay[3][i]=sensorrelayJ[3]["S4"][i];
  }
  JsonArray timerlampJ = doc["timerlamp"];
  for(i=0;i<4;i++) {
    timermode[0][i]=timerlampJ[0]["S1"][i];
    timermode[1][i]=timerlampJ[1]["S2"][i];
    timermode[2][i]=timerlampJ[2]["S3"][i];
    timermode[3][i]=timerlampJ[3]["S4"][i];
  }
  JsonArray timerJ = doc["timer"];
  for(i=0;i<4;i++) {
    timer[0][i]=timerJ[0]["S1"][i];
    timer[1][i]=timerJ[1]["S2"][i];
    timer[2][i]=timerJ[2]["S3"][i];
    timer[3][i]=timerJ[3]["S4"][i];
  }
  return true;
}

bool saveConfig() {
  const size_t capacity = 15*JSON_ARRAY_SIZE(4) + 12*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(9)+250;
  DynamicJsonDocument doc(capacity);
  int i;

  doc["hostname"] = host_name;
  doc["SID"] = sid;
  doc["PW"] = pw;
  doc["MQTThost"] = mqtt_server;
  doc["MQTTport"] = mqtt_port;
  doc["MQTTtopic"] = mqtt_topic;
  JsonArray sensorrelayJ = doc.createNestedArray("sensorrelay");
  JsonObject sensorrelay_0 = sensorrelayJ.createNestedObject();
  JsonArray sensorrelay_0_S1 = sensorrelay_0.createNestedArray("S1");
  JsonObject sensorrelay_1 = sensorrelayJ.createNestedObject();
  JsonArray sensorrelay_1_S2 = sensorrelay_1.createNestedArray("S2");
  JsonObject sensorrelay_2 = sensorrelayJ.createNestedObject();
  JsonArray sensorrelay_2_S3 = sensorrelay_2.createNestedArray("S3");
  JsonObject sensorrelay_3 = sensorrelayJ.createNestedObject();
  JsonArray sensorrelay_3_S4 = sensorrelay_3.createNestedArray("S4");
  for (i=0;i<4;i++) {
    sensorrelay_0_S1.add(sensorrelay[0][i]);
    sensorrelay_1_S2.add(sensorrelay[1][i]);
    sensorrelay_2_S3.add(sensorrelay[2][i]);
    sensorrelay_3_S4.add(sensorrelay[3][i]);
  }
  JsonArray timerlampJ = doc.createNestedArray("timerlamp");
  JsonObject timerlamp_0 = timerlampJ.createNestedObject();
  JsonArray timerlamp_0_S1 = timerlamp_0.createNestedArray("S1");
  JsonObject timerlamp_1 = timerlampJ.createNestedObject();
  JsonArray timerlamp_1_S2 = timerlamp_1.createNestedArray("S2");
  JsonObject timerlamp_2 = timerlampJ.createNestedObject();
  JsonArray timerlamp_2_S3 = timerlamp_2.createNestedArray("S3");
  JsonObject timerlamp_3 = timerlampJ.createNestedObject();
  JsonArray timerlamp_3_S4 = timerlamp_3.createNestedArray("S4");
  for (i=0;i<4;i++) {
    timerlamp_0_S1.add(timermode[0][i]);
    timerlamp_1_S2.add(timermode[1][i]);
    timerlamp_2_S3.add(timermode[2][i]);
    timerlamp_3_S4.add(timermode[3][i]);
  }
  JsonArray timerJ = doc.createNestedArray("timer");
  JsonObject timer_0 = timerJ.createNestedObject();
  JsonArray timer_0_S1 = timer_0.createNestedArray("S1");
  JsonObject timer_1 = timerJ.createNestedObject();
  JsonArray timer_1_S2 = timer_1.createNestedArray("S2");
  JsonObject timer_2 = timerJ.createNestedObject();
  JsonArray timer_2_S3 = timer_2.createNestedArray("S3");
  JsonObject timer_3 = timerJ.createNestedObject();
  JsonArray timer_3_S4 = timer_3.createNestedArray("S4");
  for (i=0;i<4;i++) {
    timer_0_S1.add(timer[0][i]);
    timer_1_S2.add(timer[1][i]);
    timer_2_S3.add(timer[2][i]);
    timer_3_S4.add(timer[3][i]);
  }

  File configFile = SPIFFS.open("/default.ini", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }
  // Serialize JSON to file
  if (serializeJson(doc, configFile) == 0) {
    Serial.println(F("Failed to write to file"));
    configFile.close();
    return false;
  }
  // Close the file
  configFile.close();
  return true;
}
