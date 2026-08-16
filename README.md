# iot_device
Funcionamento do conjunto

A ESP32-C3 atua como o controlador central. O RC522 identifica o cartão RFID, enquanto os switches fornecem informações ou comandos relacionados aos mecanismos físicos. Com base nessas entradas, o microcontrolador pode determinar se uma operação deve ser autorizada e acionar o servo correspondente.

O OLED fornece uma interface local para o operador, permitindo apresentar mensagens como "Aproxime o cartão", "Cartão autorizado", "Acesso negado" e informações sobre o estado dos compartimentos.

A utilização da ESP32-C3 também permite que o hardware seja posteriormente integrado a uma rede Wi-Fi, possibilitando o envio dos eventos para um servidor, banco de dados ou dashboard.

## Simulação

O projeto pode ser testado e simulado no Wokwi:

[Simular projeto no Wokwi](https://wokwi.com/projects/472546405284788225)

## Bibliotecas

# ESP32Servo
# U8g2_for_Adafruit_GFX
# Adafruit_SSD1306_72x40
# MFRC522