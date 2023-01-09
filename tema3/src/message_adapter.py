import sys, os, json
import paho.mqtt.client as mqtt
from datetime import datetime

from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import ASYNCHRONOUS

if 'DEBUG_DATA_FLOW' in os.environ and (os.environ['DEBUG_DATA_FLOW'] == True or os.environ['DEBUG_DATA_FLOW'] == 'true' or os.environ['DEBUG_DATA_FLOW'] == 'TRUE'):
    sys.stdout = sys.__stdout__
else:
    sys.stdout = open(os.devnull, 'w')

db_token = "initial-token"
db_org = "my-org"
db_bucket = "my-bucket"

db_client = InfluxDBClient(url="http://influxdb:8086", token=db_token)
write_api = db_client.write_api(write_options=ASYNCHRONOUS)


def on_connect(client, userdata, flags, rc):
    print('Adapter connected')

def on_message(client, userdata, msg):
    json_data = json.loads(msg.payload)
    time_now = datetime.now()
    print(f'{time_now} Received a message by topic [{msg.topic}]')

    timestamp = datetime.utcnow()
    if 'timestamp' in json_data:
        utc_time_string = json_data['timestamp']
        print(f'{time_now} Data timestamp is: {utc_time_string}')
        timestamp = datetime.strptime(datetime.strptime(utc_time_string, '%Y-%m-%dT%H:%M:%S%z').strftime("%Y-%m-%d %H:%M:%S.%f"), '%Y-%m-%d %H:%M:%S.%f')
    else:
        print(f'{time_now} Data timestamp is NOW')

    timeseries_prefix = '.'.join(msg.topic.split('/'))
    for key, value in json_data.items():
        if type(value) == int or type(value) == float:
            point = Point(f'{timeseries_prefix}.{key}').tag('location', msg.topic.split('/')[0]).field(key, value).time(timestamp, WritePrecision.S)
            write_api.write(db_bucket, db_org, point)
            print(f'{time_now} {timeseries_prefix}.{key} {value}')


def main():
    brokerhost = "messagebroker"

    client = mqtt.Client('message_adapter_client')
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(brokerhost, 1883, 60)

    client.loop_start()

    client.subscribe('#')

    while True:
        pass


if __name__ == '__main__':
    main()