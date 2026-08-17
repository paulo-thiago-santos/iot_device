#include <WiFi.h>

const char* ssid = "WIFI";
const char* password = "password";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("     TESTE DE WIFI");
  Serial.println("==============================");

  Serial.print("Conectando em: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int tentativas = 0;

  while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {

    Serial.println("WiFi conectado!");
    Serial.println();

    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());

    Serial.print("Mascara: ");
    Serial.println(WiFi.subnetMask());

    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");

    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());

  } else {

    Serial.println();
    Serial.println("ERRO: nao foi possivel conectar ao WiFi.");

    Serial.print("Status: ");
    Serial.println(WiFi.status());
  }
}

void loop() {

  // Verifica a conexão a cada 5 segundos
  static unsigned long ultimoTeste = 0;

  if (millis() - ultimoTeste >= 5000) {

    ultimoTeste = millis();

    Serial.print("WiFi: ");

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("CONECTADO");
      Serial.print(" | IP: ");
      Serial.print(WiFi.localIP());
      Serial.print(" | RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    } else {
      Serial.println("DESCONECTADO");
    }
  }
}