#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your network";
const char* password =  "your password";
const char* mqttServer = "your raspberry IP";
const int mqttPort = 1883;
const char* mqttUser = "XXXX";
const char* mqttPassword = "XXXX";

WiFiClient espClient;
PubSubClient client(espClient);

int contador = 1;
char mensagem[30];

const char* topicoTemperatura = "temperatura";
const char* topicoUmidade = "umidade";

// --- DHT ---
#include <DHT.h>
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int umidade;
int temperatura;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Iniciando conexao com a rede WiFi..");
  }

  Serial.println("Conectado na rede WiFi!");
}

void loop()
{
//  umidade = dht.readHumidity();
//    Serial.print(F("Humidity: "));
//    Serial.println(umidade);
  
  
 
  //Faz a conexao com o broker MQTT
  reconectabroker();
  medirTemperaturaUmidade();
  publicarTemperaturaUmidadeNoTopico();
  
  sprintf(mensagem, "MQTT ESP32 - Mensagem no. %d", contador);
  Serial.print("Mensagem enviada: ");
  Serial.println(mensagem);

  //Envia a mensagem ao broker
  client.publish("mensagem", mensagem, true);
  Serial.println("Mensagem enviada com sucesso...");
  
  //Incrementa o contador
  contador++;
  
  //Aguarda 30 segundos para enviar uma nova mensagem
  delay(3000);
}


void medirTemperaturaUmidade() {
  umidade = dht.readHumidity();
  Serial.print(F("Humidity: "));
  Serial.println(umidade);
  delay(5000);
  temperatura = dht.readTemperature(false);
  Serial.print(F("Temperature: "));
  Serial.println(temperatura);
  delay(5000);
}

void publicarTemperaturaUmidadeNoTopico() {
  client.publish(topicoTemperatura, String(temperatura).c_str(), true);
  client.publish(topicoUmidade, String(umidade).c_str(), true);
}

void reconectabroker()
{
  //Conexao ao broker MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected())
  {
    Serial.println("Conectando ao broker MQTT...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword ))
    {
      Serial.println("Conectado ao broker!");
    }
    else
    {
      Serial.print("Falha na conexao ao broker - Estado: ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}
