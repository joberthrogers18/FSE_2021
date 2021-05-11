import os
from flask import jsonify, render_template, request, make_response, json, send_from_directory
from flask_socketio import emit, send, join_room, leave_room

from json import loads

class Route:
    def __init__(self, app, socketio):
        self.app = app
        self.socketio = socketio

    def routes(self):
        @self.app.route('/')
        def main_route():
            return render_template('start.html', image='')