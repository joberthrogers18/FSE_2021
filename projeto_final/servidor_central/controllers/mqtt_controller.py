import random
import time
import threading
from paho.mqtt import client as mqtt_client
import json

broker = 'test.mosquitto.org'
port = 1883
topic = "/python/mqtt"
client_id = f'python-mqtt-{random.randint(0, 1000)}'

class MqttController:
    def __init__(self, app, socketio):
        self.socketio = socketio
        self.app = app
        self.unregisteredDevices = []
        self.registeredDevices = []
        self.client = None

    def connect_mqtt(self):
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)

        client = mqtt_client.Client(client_id)
        client.on_connect = on_connect
        client.connect(broker, port)
        self.client = client
        return client

    def publish_message(self, msg):
        topic = "fse2020/160121817/" + str(msg["comodo"])
        print(topic)

        result = self.client.publish(topic, json.dumps(msg, indent = 4))
        status = result[0]
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")

    def publish(self, client, msg):
        msg_count = 0
        while True:
            time.sleep(1)
            msg = f"messages: {msg_count}"
            result = client.publish(topic, msg)
            # result: [0, 1]
            status = result[0]
            # if status == 0:
            #     print(f"Send `{msg}` to topic `{topic}`")
            # else:
            #     print(f"Failed to send message to topic {topic}")
            msg_count += 1


    def subscribe(self, client: mqtt_client, current_topic):
        def on_message(client, userdata, msg):
            print(current_topic)
            # self.socketio.emit(
            #     str(current_topic),
            #     {
            #         current_topic: msg.payload.decode()
            #     }
            # )
            # print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

        client.subscribe(current_topic)
        client.on_message = on_message

    def run_publish(self, client):
        client.loop_start()
        self.publish(client, {})

    def run_subscribe(self, client, current_topic):
        def on_message(client, userdata, msg):
            name_topic = str(msg.topic).split("/")[-1]
            # self.socketio.emit(
            #     str(msg.topic),
            #     {
            #         name_topic: msg.payload.decode("utf-8")
            #     }
            # )
            print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

        client.subscribe(current_topic)
        client.on_message = on_message

    def run_subscribe_devices(self, client, current_topic):
        def on_message(client, userdata, msg):
            name_topic = str(msg.topic).split("/")[-1]

            if name_topic not in self.unregisteredDevices and name_topic not in self.registeredDevices:
                self.unregisteredDevices.append(name_topic)

            self.socketio.emit(
                "devices_unregistered",
                {
                    'unregistered': self.unregisteredDevices
                }
            )
            print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

        client.subscribe(current_topic)
        client.on_message = on_message

    def run(self):
        temperature_ref = "fse2020/160121817/sala/temperatura"
        humidity_ref = "fse2020/160121817/sala/umidade"
        status_ref = "fse2020/160121817/sala/status"
        topic_device = "fse2020/160121817/dispositivos/+"

        client_int = self.connect_mqtt()
        devices = threading.Thread(target=self.run_subscribe_devices, args=(client_int, topic_device))
        devices.start()
        devices.join()
        # sub = threading.Thread(target=self.run_subscribe, args=(client_int, temperature_ref,))
        # sub.start()
        # sub_2 = threading.Thread(target=self.run_subscribe, args=(client_int, humidity_ref,))
        # sub_2.start()
        # sub_3 = threading.Thread(target=self.run_subscribe, args=(client_int, status_ref,))
        # sub_3.start()
        pub = threading.Thread(target=self.run_publish, args=(client_int,))
        pub.start()
        pub.join()
        # sub.join()
        # sub_2.join()
        # sub_3.join()
