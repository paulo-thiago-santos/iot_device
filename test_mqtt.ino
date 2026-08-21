#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "SENAI_IOT";
const char* WIFI_PASSWORD = "senaiiot";
WiFiClient espClient;


const char* MQTT_SERVER = "192.168.10.111";
const int MQTT_PORT = 1883;
PubSubClient mqttClient(espClient);

String deviceID;
String mqttBaseTopic;

String getDeviceID()
{
  uint64_t chipid = ESP.getEfuseMac();
  char id[17];
  snprintf(id, sizeof(id), "%016llX", chipid);
  return String(id);
}

void conectarWiFi()
{
  Serial.print("Conectando ao WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  Serial.println();
  Serial.println("==============================");
  Serial.println("       MQTT RECEBIDO");
  Serial.println("==============================");
  Serial.print("Topic: ");
  Serial.println(topic);
  Serial.print("Mensagem: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("==============================");
}

void conectarMQTT()
{
  while (!mqttClient.connected())
  {
    Serial.print("Conectando ao MQTT...");
    String clientID = "ESP32-";
    clientID += deviceID;
    if (mqttClient.connect(clientID.c_str()))
    {
      Serial.println(" conectado!");
      String topicStatus = mqttBaseTopic + "status";
      mqttClient.publish(topicStatus.c_str(), "ONLINE", true);
      String topicCommand = mqttBaseTopic + "cmd/#";
      mqttClient.subscribe(topicCommand.c_str());
      Serial.print("Subscribe: ");
      Serial.println(topicCommand);
    }
    else
    {
      Serial.print(" falhou. Estado: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("==============================");
  Serial.println("      ESP32-C3 MQTT");
  Serial.println("==============================");
  deviceID = getDeviceID();
  mqttBaseTopic = deviceID + "/";
  Serial.print("Device ID: ");
  Serial.println(deviceID);
  Serial.print("Base MQTT: ");
  Serial.println(mqttBaseTopic);
  conectarWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttClient.setBufferSize(512);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    conectarWiFi();
  }
  if (!mqttClient.connected())
  {
    conectarMQTT();
  }
  mqttClient.loop();
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio >= 5000)
  {
    ultimoEnvio = millis();
    String topic = mqttBaseTopic + "teste";
    String mensagem = "ESP32 conectado";
    mqttClient.publish(topic.c_str(), mensagem.c_str());
    Serial.print("Publicado: ");
    Serial.print(topic);
    Serial.print(" -> ");
    Serial.println(mensagem);
  }
}
