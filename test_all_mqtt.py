
import json
import time
import threading
import paho.mqtt.client as mqtt

MQTT_SERVER = "broker.mqtt.cool"
MQTT_PORT = 1883

MQTT_ROOT = "mango/esp32c3/device01"

TOPIC_TELEMETRY = f"{MQTT_ROOT}/telemetry"
TOPIC_RFID = f"{MQTT_ROOT}/rfid"
TOPIC_COMMAND = f"{MQTT_ROOT}/command"
TOPIC_ALL = f"{MQTT_ROOT}/#"

CLIENT_ID = "PYTHON_TEST_DEVICE01"

connected = threading.Event()

def on_connect(client, userdata, flags, reason_code, properties):
    if reason_code == 0:
        print("[MQTT] Conectado ao broker!")
        client.subscribe(TOPIC_ALL)
        print(f"[MQTT] Inscrito em: {TOPIC_ALL}")
        connected.set()
    else:
        print(f"[MQTT] Falha na conexao: {reason_code}")

def on_disconnect(client, userdata, disconnect_flags, reason_code, properties):
    connected.clear()
    print(f"[MQTT] Desconectado: {reason_code}")

def on_message(client, userdata, msg):
    topic = msg.topic
    try:
        payload = json.loads(msg.payload.decode("utf-8"))
    except (json.JSONDecodeError, UnicodeDecodeError):
        print(f"\n[MQTT] {topic}: {msg.payload!r}")
        return
    print("\n" + "=" * 50)
    if topic == TOPIC_TELEMETRY:
        print("[ESP32] TELEMETRIA")
        print(f"Switch 1: {payload.get('sw1')}")
        print(f"Switch 2: {payload.get('sw2')}")
    elif topic == TOPIC_RFID:
        print("[ESP32] RFID")
        print(f"UID: {payload.get('uid')}")
    elif topic == TOPIC_COMMAND:
        print("[MQTT] COMANDO PUBLICADO")
        print(json.dumps(payload, indent=2, ensure_ascii=False))
    else:
        print(f"[MQTT] Topico: {topic}")
        print(json.dumps(payload, indent=2, ensure_ascii=False))
    print("=" * 50)

client = mqtt.Client(
    callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
    client_id=CLIENT_ID
)

client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message

def send_command(servo1, servo2, line1, line2, line3):
    if not connected.is_set():
        print("[ERRO] MQTT desconectado!")
        return
    payload = {
        "servo1": int(servo1),
        "servo2": int(servo2),
        "line1": line1,
        "line2": line2,
        "line3": line3
    }
    message = json.dumps(payload, ensure_ascii=False)
    result = client.publish(
        TOPIC_COMMAND,
        message,
        qos=0,
        retain=False
    )
    if result.rc == mqtt.MQTT_ERR_SUCCESS:
        print("[PYTHON] COMANDO ENVIADO")
        print(json.dumps(payload, indent=2, ensure_ascii=False))
    else:
        print("[ERRO] Falha ao publicar comando!")

def menu():

    while not connected.is_set():
        time.sleep(0.2)
    time.sleep(2)
    while True:
        s1 = int(input("Angulo Servo 1 (0-90): "))
        s2 = int(input("Angulo Servo 2 (0-90): "))
        l1 = input("Linha 1: ")
        l2 = input("Linha 2: ")
        l3 = input("Linha 3: ")
        if not (0 <= s1 <= 90 and 0 <= s2 <= 90):
            print("[ERRO] Angulo fora do intervalo!")
            continue
        send_command(s1, s2, l1, l2, l3)

while True:
    try:
        print("[MQTT] Conectando...")
        client.connect(
            MQTT_SERVER,
            MQTT_PORT,
            keepalive=60
        )
        client.loop_start()
        menu()
    except KeyboardInterrupt:
        print("Interrompido pelo usuario.")
    except Exception as e:
        print(f"Erro: {e}")
    finally:
        client.loop_stop()
        client.disconnect()
        print("Cliente MQTT encerrado.")
