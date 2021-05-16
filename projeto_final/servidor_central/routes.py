import os
from flask import jsonify, render_template, request, make_response, json, send_from_directory
from flask_socketio import emit, send, join_room, leave_room
from controllers.mqtt_controller import MqttController
from json import loads
import threading

class Route:
    def __init__(self, app, socketio):
        self.app = app
        self.socketio = socketio
        self.mqtt = MqttController(app, socketio)
        main_run = threading.Thread(target=self.mqtt.run, args=())
        main_run.start()
        # main_run.join()

    def routes(self):
        @self.app.route('/')
        def main_route():
            r = threading.Timer(3.0, self.mqtt.get_register_values, ())
            r.start()
            # self.mqtt.get_register_values()
            return render_template('start.html')
        
        @self.socketio.on('publish_channel')
        def on_publish_device(data):
            self.mqtt.publish_message(data)