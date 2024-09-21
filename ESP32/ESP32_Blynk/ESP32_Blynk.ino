#define BLYNK_TEMPLATE_ID "TMPL6ns_u3Wgr"
#define BLYNK_TEMPLATE_NAME "Medical"
#define BLYNK_AUTH_TOKEN "FopjY2aXyI3vt5dxtDDBJApXk_m801JC"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "SoftwareSerial.h"

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Redmi Note 10 Pro";
char pass[] = "1sampai8";

SoftwareSerial mySerial(16, 17); // RX, TX

#define VPIN_suhu V0
#define VPIN_sys V1
#define VPIN_dia V2
#define VPIN_bpm V3

BlynkTimer timer;


void connect(){
  while(WiFi.status()!= WL_CONNECTED){
   WiFi.reconnect();
   delay(500);
  }
  Serial.println("Connected to WiFi");

  if(!Blynk.connected()){
    Blynk.connect();
    delay (500);
  }else{
    Serial.println("Connected to Blynk");
  }
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  connect();
}

void loop() {
  Blynk.run();
 if (mySerial.available() > 0) {
    String dataFromArduino = "";

    // Membaca data yang dikirim dari Arduino
    while (mySerial.available() > 0) {
      char incomingChar = mySerial.read();
      dataFromArduino += incomingChar;
      delay(10); // Hindari membaca terlalu cepat
    }

    dataFromArduino.trim();

    // Pisahkan data berdasarkan koma
    int firstCommaIndex = dataFromArduino.indexOf(',');
    int secondCommaIndex = dataFromArduino.indexOf(',', firstCommaIndex + 1);
    int thirdCommaIndex = dataFromArduino.indexOf(',', secondCommaIndex + 1);

    if (firstCommaIndex > 0 && secondCommaIndex > 0 && thirdCommaIndex > 0) {
      String suhu = dataFromArduino.substring(0, firstCommaIndex);
      String sys = dataFromArduino.substring(firstCommaIndex + 1, secondCommaIndex);
      String dia = dataFromArduino.substring(secondCommaIndex + 1, thirdCommaIndex);
      String bpm = dataFromArduino.substring(thirdCommaIndex + 1);

      // Tampilkan data di serial monitor
      Serial.println("Suhu Tubuh: " + suhu);
      Serial.println("SYS: " + sys);
      Serial.println("DIA: " + dia);
      Serial.println("BPM: " + bpm);
      Blynk.virtualWrite(VPIN_suhu, suhu);
      Blynk.virtualWrite(VPIN_sys, sys);
      Blynk.virtualWrite(VPIN_dia, dia);
      Blynk.virtualWrite(VPIN_bpm, bpm);
    } else {
      Serial.println("Error: Data tidak terformat dengan benar.");
    }
 }
  delay(1000); // Delay untuk loop
}
