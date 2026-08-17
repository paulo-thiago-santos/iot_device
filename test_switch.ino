#define SWITCH1_PIN 5
#define SWITCH2_PIN 6

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(SWITCH1_PIN, INPUT_PULLUP);
  pinMode(SWITCH2_PIN, INPUT_PULLUP);

  Serial.println();
  Serial.println("==========================");
  Serial.println("   TESTE DAS CHAVES");
  Serial.println("==========================");
  Serial.println("GPIO8 -> Chave 1");
  Serial.println("GPIO9 -> Chave 2");
  Serial.println();

  Serial.println("Chaves abertas = HIGH");
  Serial.println("Chaves fechadas = LOW");
  Serial.println();
}

void loop() {

  static int lastSwitch1 = HIGH;
  static int lastSwitch2 = HIGH;

  int switch1 = digitalRead(SWITCH1_PIN);
  int switch2 = digitalRead(SWITCH2_PIN);

  // Chave 1 mudou
  if (switch1 != lastSwitch1) {
    Serial.print("Chave 1: ");

    if (switch1 == LOW) {
      Serial.println("FECHADA");
    } else {
      Serial.println("ABERTA");
    }

    lastSwitch1 = switch1;
  }

  // Chave 2 mudou
  if (switch2 != lastSwitch2) {
    Serial.print("Chave 2: ");

    if (switch2 == LOW) {
      Serial.println("FECHADA");
    } else {
      Serial.println("ABERTA");
    }

    lastSwitch2 = switch2;
  }

  delay(20);
}