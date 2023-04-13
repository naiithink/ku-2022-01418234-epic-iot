from flask import Flask
from flask import request

app = Flask(__name__)

humid = 0
temp = 0

@app.route('/')
def hello_world():
    global humid
    global temp

    humid = request.args.get('u', default='*', type=str)
    temp = request.args.get('t', default='*', type=str)
    return f'Hello from Flask!'

@app.route('/show')
def d():
    global humid
    global temp

    return f"""
    Humidity: {humid}\n
    Tempurature: {temp}\n"""

if __name__ == '__main__':
      app.run(host='0.0.0.0', port=80)
