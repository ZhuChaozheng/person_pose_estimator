# -*- coding:utf-8 -*-
import json
import re

from flask import Flask, request, render_template, jsonify

from server import socket_server

app = Flask(__name__)


@app.route('/')
def home():
    return render_template('index.html')


@app.route('/api/', methods=['GET', 'POST'])
def api():
        try:
            data = socket_server.request_data.pop()
            #print(123)
            # print(data)
        except IndexError:
            #data = 0
            web_data = {'core_node_name':'104', 'node':'N0040', 'status':'0'}
            data = str(web_data)
#        print( jsonify({"data":json.loads(data.replace("'", '"'))}))
        return jsonify({"data":json.loads(data.replace("'", '"'))})


if __name__ == '__main__':
    socket_server.run()
    app.run(host='0.0.0.0', debug=False, use_reloader=False)
