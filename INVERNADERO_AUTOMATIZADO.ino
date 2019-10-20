//LIBRERIAS TIEMPO REAL ////
#include <Wire.h> 
#include "RTClib.h"
RTC_DS1307 RTC;

//LIBRERIAS SENSOR DHT ////
#include "DHT.h" //Llamado de libreria DHT
#define DHTPin 5    // Pin al que se conecta el sensor DTH
#define DHTTYPE DHT22  
DHT dht(DHTPin, DHTTYPE);

float Humedad_Ambiente = 0; float Temperatura_Ambiente = 0;

float Temp_Ambient_Minima = 0; float Temp_Ambient_Maxima = 0;

float Humedad_Ambient_Minima = 0; float Humedad_Ambient_Maxima = 0;


//LIBRERIAS SENSOR SUELO ////
const int HumedadSuelo_Pin = A0; //Pin del sensor de humedad de suelo
int HumedadSuelo=0;
/*
  # the sensor value description
  # 0  ~300     dry soil
  # 300~700     humid soil
  # 700~950     in water
*/
int HumedadMinima=300; int HumedadMaxima=600;

//VARIABLES DE SALIDA ////
const int ElectroValvula = 3;
const int Bomba = 4;
const int Extractor = 5;
const int Resistencia =6;


void setup() 
{
   Serial.begin(9600);
   dht.begin();
   Wire.begin();
   RTC.begin();
   RTC.adjust(DateTime(__DATE__, __TIME__));
   
   pinMode(10, OUTPUT);
   pinMode(ElectroValvula, OUTPUT); pinMode(Bomba, OUTPUT); pinMode(Extractor, OUTPUT); pinMode(Resistencia, OUTPUT);
}
 
void loop() 
{
   Humedad_Ambiente = dht.readHumidity();
   Temperatura_Ambiente = dht.readTemperature();
   HumedadSuelo = analogRead(HumedadSuelo_Pin);
   Riego();
   Control_Temp();
}


void Riego()
{
  if(HumedadSuelo < HumedadMinima)
  {
    while(HumedadSuelo <= HumedadMaxima) { digitalWrite(ElectroValvula,HIGH); digitalWrite(Bomba,HIGH); HumedadSuelo = analogRead(HumedadSuelo_Pin);}
    digitalWrite(ElectroValvula,LOW); digitalWrite(Bomba,LOW);
  }
}

void Control_Temp()
{
  if (Temperatura_Ambiente >= Temp_Ambient_Maxima)
  {
    while(Temperatura_Ambiente <= Temp_Ambient_Minima) { digitalWrite(Extractor, HIGH); Temperatura_Ambiente = dht.readTemperature();}
    digitalWrite(Extractor, LOW);
  }

  if (Temperatura_Ambiente <= Temp_Ambient_Minima) 
  {
    while(Temperatura_Ambiente >= Temp_Ambient_Maxima) { digitalWrite(Resistencia, HIGH); Temperatura_Ambiente = dht.readTemperature();}
    digitalWrite(Resistencia, LOW);
   }
}

