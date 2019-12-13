#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "config.h"

WiFiClient wifiClient;
PubSubClient client(wifiClient);
SoftwareSerial ser(D1,D2,false,8);

void setup() {
  Serial.begin(9600);
  ser.begin(9600);

  wifi_reconnect();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(msgReceived);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED) {  
    wifi_reconnect();
  }
  if(!client.connected()) {
    mqtt_reconnect();
  }
  
  client.loop();
}

void wifi_reconnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  //WiFi.begin(SSID);
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(HOSTNAME,MQTT_USER,MQTT_PASS)) {
      Serial.println("connected");
      client.publish("home/clock","Connected");
      client.subscribe("home/clock/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

/* Clock face positions:
 *  0: Home
 *  1: Lessons
 *  2: Concert
 *  3: Lost
 *  4: Tavern
 *  5: Mortal Peril
 *  6: Opera
 *  7: Siblings
 *  8: Travel
 *  9: Doctors
 *  10: Work
 *  11: Parents
 *  12: School
 */

void sendData(char target, char loc[]) {
  int pos=3; // default to Lost
  if (strcasecmp(loc,"Home") == 0) {
    pos = 0;
  }
  else if (strcasecmp(loc,"Lessons") == 0) {
    pos = 1;
  }
  else if (strcasecmp(loc,"Concert") == 0) {
    pos = 2;
  }
  else if (strcasecmp(loc,"not_home") == 0 or strcasecmp(loc,"Lost") == 0) {
    pos = 3;
  }
  else if (strcasecmp(loc,"Tavern") == 0) {
    pos = 4;
  }
  else if (strcasecmp(loc,"Mortal Peril") == 0) {
    pos = 5;
  }
  else if (strcasecmp(loc,"Opera") == 0) {
    pos = 6;
  }
  else if (strcasecmp(loc,"Siblings") == 0) {
    pos = 7;
  }
  else if (strcasecmp(loc,"Travel") == 0) {
    pos = 8;
  }
  else if (strcasecmp(loc,"Doctors") == 0) {
    pos = 9;
  }
  else if (strcasecmp(loc,"Work") == 0) {
    pos = 10;
  }
  else if (strcasecmp(loc,"Parents") == 0) {
    pos = 11;
  }
  else if (strcasecmp(loc,"School") == 0) {
    pos = 12;
  }

  Serial.print(loc);
  Serial.print(" - Sending: ");
  Serial.print(target);
  Serial.println(pos);

  ser.write(target);
  ser.write(pos);
  ser.write('#');
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  char target = 0;
  if(strcmp(topic,"home/clock/personA") == 0) {
    target = 'A';
  }
  else if(strcmp(topic,"home/clock/personB") == 0) {
    target = 'B';
  }
  else if(strcmp(topic,"home/clock/personC") == 0) {
    target = 'C';
  }
  else if(strcmp(topic,"home/clock/personD") == 0) {
    target = 'D';
  }
  else {
    target = 0;
  }

  char message[length+1];
  for (int i = 0; i < length; i++) {
    message[i] = payload[i];
  }
  message[length] = '\0';

  Serial.print("Location update: ");
  Serial.print(target);
  Serial.print(" is at ");
  Serial.println(message);

  if(target != 0) {
    sendData(target,message);
  }
}
