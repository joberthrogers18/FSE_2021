import random
import time
import threading
from paho.mqtt import client as mqtt_client

broker = 'test.mosquitto.org'
port = 1883
topic = "/python/mqtt"
client_id = f'python-mqtt-{random.randint(0, 1000)}'

class MqttController:
    def __init__(self, app, socketio):
        self.socketio = socketio
        self.app = app

    def connect_mqtt(self):
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)

        client = mqtt_client.Client(client_id)
        client.on_connect = on_connect
        client.connect(broker, port)
        return client


    def publish(self, client):
        msg_count = 0
        while True:
            time.sleep(1)
            msg = f"messages: {msg_count}"
            result = client.publish(topic, msg)
            # result: [0, 1]
            status = result[0]
            if status == 0:
                print(f"Send `{msg}` to topic `{topic}`")
            else:
                print(f"Failed to send message to topic {topic}")
            msg_count += 1


    def subscribe(self, client: mqtt_client, current_topic):
        def on_message(client, userdata, msg):
            print(f"Received `{msg.payload.decode()}` from `{msg.topic}` topic")

        client.subscribe(current_topic)
        client.on_message = on_message

    def run_publish(self, client):
        client.loop_start()
        self.publish(client)

    def run_subscribe(self, client, topic):
        self.subscribe(client, topic)
        # client.loop_forever()

    def run(self):
        temperature_ref = "fse2020/160121817/sala/temperatura"
        humidity_ref = "fse2020/160121817/sala/umidade"
        status_ref = "fse2020/160121817/sala/status"

        client_int = self.connect_mqtt()
        sub = threading.Thread(target=self.run_subscribe, args=(client_int, temperature_ref))
        sub.start()
        sub_2 = threading.Thread(target=self.run_subscribe, args=(client_int, humidity_ref))
        sub_2.start()
        sub_3 = threading.Thread(target=self.run_subscribe, args=(client_int, status_ref))
        sub_3.start()
        pub = threading.Thread(target=self.run_publish, args=(client_int,))
        pub.start()
        pub.join()
        sub.join()
        sub_2.join()
        sub_3.join()
