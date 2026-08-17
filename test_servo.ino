#include <ESP32Servo.h>

#define SERVO1_PIN 10
#define SERVO2_PIN 7

Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==========================");
  Serial.println("   TESTE DOS SERVOS");
  Serial.println("==========================");

  servo1.setPeriodHertz(50);
  servo2.setPeriodHertz(50);

  servo1.attach(SERVO1_PIN, 500, 2400);
  servo2.attach(SERVO2_PIN, 500, 2400);

  Serial.println("Servos inicializados.");

  // Posição inicial
  servo1.write(0);
  servo2.write(0);

  delay(1000);
}

void loop() {
  Serial.println("Posicao: 0 graus");
  servo1.write(0);
  servo2.write(0);
  delay(2000);

  Serial.println("Posicao: 15 graus");
  servo1.write(15);
  servo2.write(15);
  delay(2000);

  Serial.println("Posicao: 45 graus");
  servo1.write(45);
  servo2.write(45);
  delay(2000);

  Serial.println("Posicao: 90 graus");
  servo1.write(90);
  servo2.write(90);
  delay(2000);

}