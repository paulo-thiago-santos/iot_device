#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN   0
#define RST_PIN  4

#define SCK_PIN  1
#define MISO_PIN 3
#define MOSI_PIN 2

MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("   TESTE RC522 - ESP32-C3");
  Serial.println("==============================");

  // Inicializa SPI nos GPIOs definidos
  SPI.begin(
    SCK_PIN,
    MISO_PIN,
    MOSI_PIN,
    SS_PIN
  );

  // Inicializa RC522
  rfid.PCD_Init();

  delay(100);

  Serial.println("RC522 inicializado.");

  // Mostra versão do chip
  byte version = rfid.PCD_ReadRegister(MFRC522::VersionReg);

  Serial.print("VersionReg: 0x");

  if (version < 0x10)
    Serial.print("0");

  Serial.println(version, HEX);

  if (version == 0x00 || version == 0xFF) {
    Serial.println();
    Serial.println("ERRO: RC522 nao respondeu!");
    Serial.println("Verifique:");
    Serial.println("  SDA/SS -> GPIO7");
    Serial.println("  SCK    -> GPIO4");
    Serial.println("  MOSI   -> GPIO6");
    Serial.println("  MISO   -> GPIO5");
    Serial.println("  RST    -> GPIO2");
    Serial.println("  3.3V   -> 3V3");
    Serial.println("  GND    -> GND");
  } else {
    Serial.println("RC522 detectado!");
  }

  Serial.println();
  Serial.println("Aproxime um cartao...");
}

void loop() {

  // Verifica se existe um novo cartão
  if (!rfid.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }

  // Tenta ler o UID
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println();
  Serial.println("========== CARTAO ==========");

  Serial.print("UID: ");

  for (byte i = 0; i < rfid.uid.size; i++) {

    if (rfid.uid.uidByte[i] < 0x10)
      Serial.print("0");

    Serial.print(rfid.uid.uidByte[i], HEX);

    if (i < rfid.uid.size - 1)
      Serial.print(":");
  }

  Serial.println();

  Serial.print("UID Bytes: ");
  Serial.println(rfid.uid.size);

  Serial.print("SAK: 0x");

  if (rfid.PICC_GetType(rfid.uid.sak) == MFRC522::PICC_TYPE_UNKNOWN) {
    Serial.println("??");
  } else {
    Serial.println(rfid.uid.sak, HEX);
  }

  Serial.print("Tipo: ");
  MFRC522::PICC_Type piccType =
      rfid.PICC_GetType(rfid.uid.sak);

  Serial.println(rfid.PICC_GetTypeName(piccType));

  Serial.println("============================");

  // Encerra comunicação com o cartão
  rfid.PICC_HaltA();

  // Para criptografia, se utilizada
  rfid.PCD_StopCrypto1();

  delay(500);
}