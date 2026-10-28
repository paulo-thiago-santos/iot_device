import paho.mqtt.client as mqtt

MQTT_BROKER = "broker.mqtt.cool"
MQTT_PORT = 1883
MQTT_TOPIC = "esp32/temp"


def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("Conectado ao broker MQTT!")
        client.subscribe(MQTT_TOPIC)
        print(f"Acompanhando o tópico: {MQTT_TOPIC}\n")
    else:
        print(f"Falha na conexão: {reason_code}")


def on_message(client, userdata, msg):
    temperatura = msg.payload.decode("utf-8")
    print(f"Temperatura recebida: {temperatura}")


client = mqtt.Client(
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2
)

client.on_connect = on_connect
client.on_message = on_message

print(f"Conectando a {MQTT_BROKER}...")

client.connect(MQTT_BROKER, MQTT_PORT, 60)

try:
    client.loop_forever()
except KeyboardInterrupt:
    print("\nMonitoramento encerrado.")
    client.disconnect()
