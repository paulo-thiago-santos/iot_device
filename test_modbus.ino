#include <WiFi.h>
#include <ModbusIP_ESP8266.h>
#include <ESP32Servo.h>

const char* WIFI_SSID = "SENAI_IOT";
const char* WIFI_PASSWORD = "senaiiot";

ModbusIP mb;

const int COIL_BOTAO_0 = 0;
const int COIL_BOTAO_1 = 1;
const int REG_ANGULO_0 = 0;
const int REG_ANGULO_1 = 1;

bool estadoBotao_0 = false;
bool estadoBotao_1 = false;
int anguloServo_0 = 0;
int anguloServo_1 = 0;
int cont = 0;

const int PINO_BOTAO_0 = 6;
const int PINO_BOTAO_1 = 5;
const int PINO_SERVO_0 = 10;
const int PINO_SERVO_1 = 7;

Servo servo_0;
Servo servo_1;

void setup() {
  Serial.begin(115200);
  pinMode(PINO_BOTAO_0, INPUT_PULLUP);  // Botão com resistor pull-up interno
  pinMode(PINO_BOTAO_1, INPUT_PULLUP);  // Botão com resistor pull-up interno

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado, IP: " + WiFi.localIP().toString());

  mb.server();

  // Adiciona coils e registrador
  mb.addCoil(COIL_BOTAO_0, estadoBotao_0);
  mb.addCoil(COIL_BOTAO_1, estadoBotao_1);
  mb.addHreg(REG_ANGULO_0, anguloServo_0);
  mb.addHreg(REG_ANGULO_1, anguloServo_1);

  // Inicializa servo
  servo_0.attach(PINO_SERVO_0);
  servo_0.write(anguloServo_0);
  servo_1.attach(PINO_SERVO_1);
  servo_1.write(anguloServo_1);
}

void loop() {
  mb.task();

  // Atualiza estado do botão (invertido por pull-up)
  estadoBotao_0 = !digitalRead(PINO_BOTAO_0);
  mb.Coil(COIL_BOTAO_0, estadoBotao_0);
  estadoBotao_1 = !digitalRead(PINO_BOTAO_1);
  mb.Coil(COIL_BOTAO_1, estadoBotao_1);

  // Atualiza posição do servo com valor do registrador
  anguloServo_0 = constrain(mb.Hreg(REG_ANGULO_0), 0, 180);
  servo_0.write(anguloServo_0);
  anguloServo_1 = constrain(mb.Hreg(REG_ANGULO_1), 0, 180);
  servo_1.write(anguloServo_1);

  delay(10);
}