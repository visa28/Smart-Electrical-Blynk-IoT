
#define BLYNK_TEMPLATE_ID "TMPL62NwaKMGh"
#define BLYNK_TEMPLATE_NAME "mq2"
#define BLYNK_AUTH_TOKEN "KY7eN-CCJvjTQfyMn_w_pXlUJZ_wFBif"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <PZEM004Tv30.h>

PZEM004Tv30 pzem(15, 13); //15=D8(Tx), 13=D7(Rx)

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Bintang";
char pass[] = "bintang68";

float Tegangan, Arus, Power, Energy;
int relay1 = 04;
int relay2 = 05;
int relay3 = 00;
int mq2 = A0;
int flame = 16;
int rain = 02;
int buzzer = 10;

int data = 0;
int sensorThres =100;

BlynkTimer timer;

void setup(){
  pinMode(mq2, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(flame, INPUT);
  pinMode(rain, INPUT);
  pinMode(buzzer, OUTPUT);
    Serial.begin(9600);
   Blynk.begin(auth, ssid, pass);
   //dht begin();
   timer.setInterval(2500L, gassensor);
   timer.setInterval(2500L, flamesensor);
   timer.setInterval(2500L, rainsensor);
}

void gassensor(){
  int data = analogRead(mq2);
  Blynk.virtualWrite(V0, data);
    Serial.print("Pin A0: ");
    Serial.println(data);

    if(data > 600){
      Blynk.logEvent("indikator_gas","PERINGATAN! KEBOCORAN GAS TERDETEKSI");
        digitalWrite(buzzer, HIGH);
    }
  // put your setup code here, to run once:
}

void flamesensor(){
  bool value = digitalRead(flame);
  if(value == 1){
    Blynk.logEvent("indikator_api","PERINGATAN! API TERDETEKSI");
    digitalWrite(buzzer, HIGH);
  } else if(value == 0){
    digitalWrite(buzzer, LOW);
  }
}

void rainsensor(){
  bool value = digitalRead(rain);
  if (value == 1){
    Blynk.virtualWrite(V1, "Air Tidak Terdeteksi");
    digitalWrite(buzzer, LOW);
  } else if (value == 0){
    Blynk.virtualWrite(V1, "Air Terdeteksi");
    Blynk.logEvent("indikator_tetesan_air","PERINGATAN! TETESAN AIR TERDETEKSI");
    digitalWrite(buzzer, HIGH);
  }
}

void loop() {
 //baca nilai tegangan(V)
  Tegangan = pzem.voltage();
  //jika gagal membaca tegangan
    if(isnan(Tegangan)){
      Serial.println("Gagal Membaca Nilai Tegangan");
    } else if(!isnan(Tegangan)){
      Serial.print("Tegangan: ");
      Serial.print(Tegangan);
      Serial.println("V");
    }
 //baca nilai arus(A)
  Arus = pzem.current();
  //jika gagal membaca arus
    if(isnan(Arus)){
      Serial.println("Gagal Membaca Nilai Arus");
    } else if(!isnan(Arus)){
      Serial.print("Arus: ");
      Serial.print(Arus);
      Serial.println("A");
    }
 //baca nilai power(V)
  Power = pzem.power();
  //jika gagal membaca power
    if(isnan(Power)){
      Serial.println("Gagal Membaca Nilai Power");
    } else if(!isnan(Power)){
      Serial.print("Power: ");
      Serial.print(Power);
      Serial.println("kW");
    }
 //baca nilai energy(V)
  Energy = pzem.energy();
  //jika gagal membaca energy
    if(isnan(Energy)){
      Serial.println("Gagal Membaca Nilai Energy");
    } else if(!isnan(Energy)){
      Serial.print("Energy: ");
      Serial.print(Energy);
      Serial.println("kWh");
    }
  
  Serial.println();
  
  Blynk.virtualWrite(V2, Power);
  Blynk.virtualWrite(V3, Energy);
  Blynk.virtualWrite(V4, Tegangan);
  Blynk.virtualWrite(V5, Arus);
  
  Blynk.run();
  delay(1000);
  timer.run();
}
