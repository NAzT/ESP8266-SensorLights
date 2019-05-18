// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.

void Handle_MQTT_Connect(uint32_t deltaTime) {

  // Exit if already connected.
  if (mqtt.connected()) {
    taskManager.StopTask(&Connect_MQTT_Task);
    taskManager.StartTask(&Publish_MQTT_Task);
    Serial.println("MQTT connection already established");
  } 
  else {
    taskManager.StopTask(&Publish_MQTT_Task);
    Serial.print("Connecting to MQTT-Server ");
    Serial.println(mqtt_server);
    if (!mqtt.connect(mqtt_server.c_str(),"","")) { // connect will return true for connected
      Serial.println("Retrying MQTT connection later");
      mqtt.disconnect();
    }
    else {
      Serial.println("MQTT Connected! - Start publishing");
      taskManager.StopTask(&Connect_MQTT_Task);
      taskManager.StartTask(&Publish_MQTT_Task);
      Handle_MQTT_Publish(MsToTaskTime(120000));
    }
  }
}

void Handle_MQTT_Publish(uint32_t deltaTime) {
  String msg_MQTT;
  if (mqtt.connected()) {
    msg_MQTT = "{\"Lamp\":{\"1\":"+String(1-digitalRead(relaypin[0]))+",\"2\":"+String(1-digitalRead(relaypin[1]))+",\"3\":"+String(1-digitalRead(relaypin[2]))+ ",\"4\":"+String(1-digitalRead(relaypin[3]))+"}";
    msg_MQTT = msg_MQTT + ",\"Sensor\":{\"1\":"+String(1-digitalRead(sensorpin[0]))+",\"2\":"+String(1-digitalRead(sensorpin[1]))+",\"3\":"+String(1-digitalRead(sensorpin[2]))+ ",\"4\":"+String(1-digitalRead(sensorpin[3]))+"}}";
    if (!mqtt.publish(mqtt_topic.c_str(),msg_MQTT.c_str())) {
      Serial.println("Failed to publish!");
    } else {
      Serial.println("Published.");
    }
  } else {
    taskManager.StopTask(&Publish_MQTT_Task);
    taskManager.StartTask(&Connect_MQTT_Task);
    Serial.println("MQTT trying to reconnect");
  }
}


 
