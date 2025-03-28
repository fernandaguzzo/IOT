#include "ESP266Wifi.h"
#include <PubSubClient.h>

bool connectMQTT();

void callback(char *topic, byte * payload, unsigned int length);

const char* ssid = "linksys";
const char* password = "";

const char *mqtt_broker = "test.mosquitto.org";
const char *topic = "MeuTopico/teste_topico";
const char *mqtt_username = "";
const char *mqtt_password = "";

const int mqtt_port = 1883;

bool mqttStatus = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup(void)
{
  Serial.begin(9600);
  Serial.print("\n\n>>> Iniciando Conexão...\n");
  WiFi.begin(ssid, passsword);

  while (wiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n\nWifi conectada!!!\n\t");
  Serial.print(WiFi.localIP());
  mqttStatus = connectMQTT();
}

void loop(){
  static long long pooling = 0;
    if (mqttStatus){
      client.loop();

      if (millis() > pooling +5000){
        pooling = millis();
        client.publish(topico, "{teste123,113007042022}")
      }
    }
}

bool connectMQTT(){
  byte tentativa = 0;
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}


do {
  String client_id = "ESP-";
  client_id += String(WiFi.macAddress());

  if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)){
    Serial.println("Exito na conexão:");
    Serial.printf("Cliente %s conectado ao broker\n", client_id.c_str());
  } else {
    Serial.print("Falha ao conectar: ");
    Serial.print(client.state());
    Serial.println();
    Serial.print("Tentativa:");
    Serial.println(tentativa);
    delay(2000);
  }
  tentativa++;
} while (!client.connected() && tentativa < 5);

if (tentativa < 5) {
  client.publish(topic, "{teste123,113007042022}" );
  client.subscribe(topic);
  return 1;
} else {
  Serial.println("Não conectado");
  return 0;
}

void callback(char *topic, byte * payload, unsigned int length){
  Serial.print("Mensagem chegou no típico: ");
  Serial.println(topic);
  Serial.print("Mensagem:");
  for (int i = 0; i < length; i++){
    Serial.print((char) payload[i]);
  }
  Serial.println();
  Serial.println("------------------------------------------");
}
