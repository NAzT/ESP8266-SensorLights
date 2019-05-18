// Handling WiFi and HTTP for window shades

void Handle_Check_Client(uint32_t deltaTime) { 
  if (WiFi.status() != WL_CONNECTED) {
//    taskManager.StopTask(&Check_Client_Task);
//    taskManager.StartTask(&Connect_Client_Task);
  } else {
    serverWS.handleClient();
  }
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += serverWS.uri();
  message += "\nMethod: ";
  message += (serverWS.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += serverWS.args();
  message += "\n";
  for (uint8_t i=0; i<serverWS.args(); i++){
    message += " " + serverWS.argName(i) + ": " + serverWS.arg(i) + "\n";
  }
  serverWS.send(404, "text/plain", message);
  Serial.println(message);
}

void sendhtml(String filename,String val[]) {
  String htmltemp;
  char html[500];
  const char sep=10;
  int index=0;
  if (SPIFFS.exists(filename) || SPIFFS.exists(filename)) {
    File file = SPIFFS.open(filename, "r");
    serverWS.setContentLength(CONTENT_LENGTH_UNKNOWN);
    serverWS.send(200, "text/html","");
    while(file.available()) {
      htmltemp=file.readStringUntil(sep);
      if (htmltemp.indexOf("%s")>=0){
        sprintf(html,htmltemp.c_str(),val[index].c_str());
        index++;
        serverWS.sendContent(html);
      }
      else {
        serverWS.sendContent(htmltemp);
      }
    }
    file.close();
  }
  else {
    handleNotFound();
  }
} 

// functions to handle http requests
void handlesaveconfig() {
  String htmlanswer="<html><head><title>Config</title>";
  String htmlfailed="<meta http-equiv='refresh' content='5; URL=/'></head>\<body><b>Fehler beim Speichern</b></body></html>";
  String htmlsaved="<meta http-equiv='refresh' content='2; URL=/'></head>\<body><b>Config wurde gespeichert</b></body></html>";
  Serial.println("Config Speichern");
  if (saveConfig()) {
    htmlanswer+=htmlsaved;
  } 
  else {
    htmlanswer+=htmlfailed;
  }
  serverWS.send ( 200, "text/html", htmlanswer );
}
void handleloadconfig() {
  String htmlanswer="<html><head><title>Config</title>";
  String htmlfailed="<meta http-equiv='refresh' content='5; URL=/'></head>\<body><b>Fehler beim Laden</b></body></html>";
  String htmlsaved="<meta http-equiv='refresh' content='2; URL=/'></head>\<body><b>Config wurde geladen</b></body></html>";
  Serial.println("Config Speichern");
  if (loadConfig()) {
    htmlanswer+=htmlsaved;
  } 
  else {
    htmlanswer+=htmlfailed;
  }
  serverWS.send ( 200, "text/html", htmlanswer );
}
void handleSensorenSpeichern() {
  String htmlanswer="<html><head><title>Sensor Zuordnung</title>\<meta http-equiv='refresh' content='2; URL=/'></head>\<body><b>Sensor Zuordnung gespeichert</b></body></html>";
  String val,argpost[17],post;
  char sep=13;
  int endpost,k,index2,index=0;
  Serial.println("SensorenSpeichern");
  post=serverWS.arg(0);
  for (k=0;k<17;k++) {
    index2=post.indexOf(sep,index);
    argpost[k]=post.substring(index,index2);
    index=index2+2;
    if(argpost[k].substring(0,3)=="a=S") {
      endpost=k;
      argpost[k]="";
      k=16;
      Serial.print(endpost);
      Serial.println(" Zuordnungen");
    }
  }
  k=0;
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++){
      val="s"+String(i+1)+String(j+1)+"=1";
//      Serial.print(argpost[k]);
//      Serial.print("-");
      if (argpost[k]==val) {
        sensorrelay[i][j] = true;
//        Serial.println("true ");
        k++;
      }
      else
      {
        sensorrelay[i][j] = false;
//        Serial.println("false ");
      }
    }
  }
  serverWS.send ( 200, "text/html", htmlanswer );
}

void handleTimerSpeichern() {
  String htmlanswer="<html><head><title>Timer Zuordnung</title>\<meta http-equiv='refresh' content='2; URL=/'></head>\<body><b>Timer Zuordnung gespeichert</b></body></html>";
  String val,argpost[17],post;
  char sep=13;
  int endpost,k,index2,index=0;
  Serial.println("TimerSpeichern");
  post=serverWS.arg(0);
  for (k=0;k<17;k++) {
    index2=post.indexOf(sep,index);
    argpost[k]=post.substring(index,index2);
    index=index2+2;
    if(argpost[k].substring(0,3)=="a=S") {
      endpost=k;
      argpost[k]="";
      k=16;
      Serial.print(endpost);
      Serial.println(" Zuordnungen");
    }
  }
  k=0;
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++){
      val="t"+String(i+1)+String(j+1)+"=1";
//      Serial.print(argpost[k]);
//      Serial.print("-");
      if (argpost[k]==val) {
        timermode[i][j] = true;
//        Serial.println("true ");
        k++;
      }
      else
      {
        timermode[i][j] = false;
//        Serial.println("false ");
      }
    }
  }
  serverWS.send ( 200, "text/html", htmlanswer );
}

void handleTimer2Speichern() {
  String htmlanswer="<html><head><title>Timer Länge</title>\<meta http-equiv='refresh' content='2; URL=/'></head>\<body><b>Timer Länge gespeichert</b></body></html>";
  String val,argpost[17],post;
  char sep=13;
  int endpost,k,index2,index=0;
  Serial.println("TimerSpeichern");
  post=serverWS.arg(0);
  for (k=0;k<17;k++) {
    index2=post.indexOf(sep,index);
    argpost[k]=post.substring(index,index2);
    index=index2+2;
    if(argpost[k].substring(0,3)=="a=S") {
      endpost=k;
      argpost[k]="";
      k=16;
      Serial.print(endpost);
      Serial.println(" Zuordnungen");
    }
  }
  k=0;
  for (uint8_t i=0; i<4; i++) {
    for (uint8_t j=0; j<4; j++){
      val="l"+String(i+1)+String(j+1);
//      Serial.print(argpost[k]);
//      Serial.print("-");
      if (argpost[k].substring(0,3)==val) {
        timer[i][j] = argpost[k].substring(4).toInt()*10;
        k++;
      }
      else
      {
//        Serial.println("false ");
      }
    }
  }
  serverWS.send ( 200, "text/html", htmlanswer );
}

void handleLampOnOFF() {
  String post;
  int Lamp;
  bool val;
  Serial.println("handleLampOnOFF");
  post=serverWS.arg(0);
  Serial.println(post.length());
  Lamp=post.substring(0,1).toInt();
  val=(post.substring(2)=="true");
  Serial.print(Lamp);
  Serial.print(":");
  Serial.println(val);
  relayon[4][Lamp-1]=val;
  if (val) {
    digitalWrite(relaypin[Lamp-1], LOW);   // turn the lamp on (LOW is the voltage level)
  }
  serverWS.send ( 200, "text/html", "Ok" );
  Lamp_Publish();
}
// Web Pages
void handleRoot() {
  String checked="checked";
  String val[26];
  String filename="/index.html";
  char temp[40];
  int i,j, index=6;
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  Serial.println("root");
  // define values to upload as string
  sprintf(temp,"%u:%02u:%02u",hr,min % 60,sec % 60);
  val[0]=temp;
  val[1]=host_name;
  sprintf(temp,"%u.%u.%u.%u",ip[0],ip[1],ip[2],ip[3]);
  val[2]=temp;
  val[3]=String(mqtt_server)+":"+String(mqtt_port);
  val[4]=mqtt_topic;
  val[5]=CONFIGFILE;
  for (i=0;i<5;i++) {
    for (j=0;j<4;j++) {
      if (relayon[i][j]){
        val[index]= checked;
      }
      else {
        val[index]="";
      }
      index++;
    }
  } 
  Serial.print(index);
  Serial.println(" values inserted");
  sendhtml(filename,val);
} 

void handleSensoren() {
  String checked="checked";
  String val[18];
  String filename="/sensoren.html";
  char temp[40];
  int i,j, index=2;
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  Serial.println("Sensoren");
  // define values to upload as string
  sprintf(temp,"%u:%02u:%02u",hr,min % 60,sec % 60);
  val[0]=temp;
  val[1]=host_name;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      if (sensorrelay[i][j]){
        val[index]=checked;
      }
      else {
        val[index]="";
      }
      index++;
    }
  }
  Serial.print(index);
  Serial.println(" values inserted");
  sendhtml(filename,val);
}

void handleTimer() {
  String checked="checked";
  String val[18];
  String filename="/timer.html";
  char temp[20];
  int i,j,help, index=2;
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  Serial.println("Timer");
  // define values to upload as string
  sprintf(temp,"%u:%02u:%02u",hr,min % 60,sec % 60);
  val[0]=temp;
  val[1]=host_name;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      if (timermode[i][j]){
        val[index]="checked";
      }
      else {
        val[index]="";
      }
      index++;
    }
  }
  Serial.print(index);
  Serial.println(" values inserted");
  sendhtml(filename,val);
}

void handleTimer2() {
  String val[18];
  String checked="checked";
  String filename="/timer2.html";
  char temp[20];
  int i,j,help, index=2;
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  Serial.println("Timer2");
  // define values to upload as string
  sprintf(temp,"%u:%02u:%02u",hr,min % 60,sec % 60);
  val[0]=temp;
  val[1]=host_name;
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      help=timer[i][j]/10;
      val[index]=help;
      index++;
    }
  }
  Serial.print(index);
  Serial.println(" values inserted");
  sendhtml(filename,val);
}
