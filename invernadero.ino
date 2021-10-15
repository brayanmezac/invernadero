// * Incluimos las siguientes librerías
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#include "DHT.h"

#define DHTPIN 2     // Pin digital al caual vamos a conectar el sensor de temperatura

// Tipo de sensor que vamos a usar
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Parámetros para el servidor de Ubidots.*/
char const * TOKEN = "BBFF-AquiVaTuToken"; // Token asignado por Ubidots
char const * VARIABLE_LABEL_1 = "temperatura_dht"; // Asigna una etiqueta de variable única para enviar los datos
char const * VARIABLE_LABEL_2 = "humedad_dht"; // Asigna una etiqueta de variable única para enviar los datos de la humedad
int temperatura = 0;
int humedad = 0;
/* Dirección MAC para el módulo ethernet */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

Ubidots client(TOKEN);

void setup() {        
  Serial.begin(9600);
  //Inicializa el sensor de temperatura
  dht.begin();
  /* Inicializa la conexión Ethernet */
  Serial.print(F("Inicializando conexión ethernet..."));
  if (!Ethernet.begin(mac)) {
    Serial.println(F("ops hubo un problema"));
  } else {
    Serial.println(Ethernet.localIP());
  }
  // Esperamos un tiempo para el módulo arranque
  delay(2000);
  Serial.println(F("Placa ethernet lista!!!"));
}

void loop() {

  Ethernet.maintain();
  float valor_temperatura = dht.readTemperature();// Leemos la temperatura en grados celsius
  float valor_humedad = dht.readHumidity();//Leemos la humedad
  temperatura = valor_temperatura;
  humedad = valor_humedad;
  //Impimimos estos valores en el terminal serial
  Serial.print("Temperatura:");
  Serial.println(valor_temperatura);
  Serial.print("Humedad:");
  Serial.println(valor_humedad);
  /* Enviando datos a Ubidots*/
  client.add(VARIABLE_LABEL_1, valor_temperatura);
  client.add(VARIABLE_LABEL_2, valor_humedad);
  client.sendAll();
  delay(6000);
}