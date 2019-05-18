//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
//ESP8266WebServer serverWS(80);

  // Connect to WiFi network
  Serial.print("Connecting to WIFI ");
  Serial.print(sid);
  WiFi.setAutoConnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(host_name);
  WiFi.setSleepMode(WIFI_LIGHT_SLEEP);
  delay(100);

  WiFi.begin(sid.c_str(),pw.c_str());
  WiFi.disconnect(); // first cycle which may be broken
  delay(200);
  WiFi.begin(sid.c_str(),pw.c_str());
  delay(200);
  while (WiFi.status() != WL_CONNECTED) {
    wCounter += 1;
    delay(500);
    Serial.print(WiFi.status());
    if (wCounter > 400) {
      wCounter=0;
      noWiFi=true;
      Serial.println();
      Serial.println("Reset or power up will retry connecting");
      Serial.println("Running stand-alone - no WIFI");
      break;
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("..Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    // Start the server
    //    server.begin();
    serverWS.begin();
    Serial.println("Server started");
    ip = WiFi.localIP();
    mqtt.begin(mqtt_server.c_str(), client);
  }
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  WiFi.printDiag(Serial);
