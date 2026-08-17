#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_PIN 18 //21
#define SCL_PIN 19 //20

Adafruit_SSD1306 display(128, 32, &Wire, -1);

void setup() {

  Serial.begin(115200);
  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);

  Serial.println("Iniciando SSD1306...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("ERRO");
    while (1);
  }

  Serial.println("SSD1306 OK");

  delay(500);

  // Tela completamente branca
  display.cp437(true);
  display.clearDisplay();
  display.fillScreen(SSD1306_WHITE);
  display.display();

  Serial.println("TELA BRANCA ENVIADA");

  delay(3000);

  // Tela preta
  display.clearDisplay();
  display.display();

  delay(1000);

  // Texto
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);


  display.setCursor(0, 0);
  display.println("ESP32-C3");

  display.setCursor(0, 12);
  display.println("OLED 128x32");

  display.setCursor(0, 24);
  display.write(0x03);
  display.println("TESTE OK");

  display.display();
}

void loop() {
}