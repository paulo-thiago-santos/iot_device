import paho.mqtt.client as mqtt
from datetime import datetime

# =====================================================
# CONFIGURAÇÃO
# =====================================================

MQTT_HOST = "192.168.0.119"
MQTT_PORT = 1883

ARQUIVO_IDS = "lista.txt"


# =====================================================
# LER IDS
# =====================================================

def carregar_ids():
    with open(ARQUIVO_IDS, "r") as arquivo:

        ids = []

        for linha in arquivo:

            id_esp32 = linha.strip()

            # Ignora linhas vazias
            if not id_esp32:
                continue

            ids.append(id_esp32)

    return ids


# =====================================================
# CALLBACK DE CONEXÃO
# =====================================================

def on_connect(client, userdata, flags, reason_code, properties=None):

    print()
    print("========================================")
    print("Conectado ao servidor MQTT")
    print("========================================")

    ids = userdata["ids"]

    for device_id in ids:

        topic = f"{device_id}/status"

        client.subscribe(topic)

        print(f"Monitorando: {topic}")

    print()


# =====================================================
# CALLBACK DE MENSAGEM
# =====================================================

def on_message(client, userdata, msg):

    horario = datetime.now().strftime(
        "%Y-%m-%d %H:%M:%S"
    )

    device_id = msg.topic.split("/")[0]

    status = msg.payload.decode(
        "utf-8",
        errors="replace"
    )

    print(
        f"[{horario}] "
        f"{device_id} -> {status}"
    )


# =====================================================
# MAIN
# =====================================================

def main():

    ids = carregar_ids()

    if not ids:

        print("Nenhum ESP32 encontrado em lista.txt")

        return


    print("========================================")
    print("       MONITOR DE ESP32 MQTT")
    print("========================================")

    print(f"Dispositivos cadastrados: {len(ids)}")

    for device_id in ids:

        print(f"  - {device_id}")

    print()


    # -------------------------------------------------
    # CLIENT MQTT
    # -------------------------------------------------

    client = mqtt.Client(
        mqtt.CallbackAPIVersion.VERSION2,
        client_id="mqtt-status-monitor"
    )


    # Dados disponíveis nos callbacks
    client.user_data_set({
        "ids": ids
    })


    client.on_connect = on_connect
    client.on_message = on_message


    # -------------------------------------------------
    # CONECTAR
    # -------------------------------------------------

    print(
        f"Conectando ao MQTT "
        f"{MQTT_HOST}:{MQTT_PORT}..."
    )

    client.connect(
        MQTT_HOST,
        MQTT_PORT,
        60
    )


    print("Monitor iniciado.")
    print("Pressione Ctrl+C para sair.")
    print()


    try:

        client.loop_forever()

    except KeyboardInterrupt:

        print()
        print("Encerrando monitor...")

        client.disconnect()


# =====================================================
# EXECUÇÃO
# =====================================================

if __name__ == "__main__":
    main()