import paho.mqtt.client as mqtt

broker = '192.168.0.205'
port = 49111
topics = [
    "flags",
    "tiltMovementData",
    "turnMovementData",
    "forceMovement",
    "timeRemaining",
    "sensorData",
    "otherData",
]
client_id = f'python-mqtt-0001'


def on_connect(client, userdata, flags, rc, properties=None):
    print("CONNACK received with code %s." % rc)
    # Subscribe to topic on successful connection
    for topic in topics:
        client.subscribe(topic, qos=1)


def on_subscribe(client, userdata, mid, granted_qos, properties=None):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))


def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload.decode()))


# Create an MQTT client instance
client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
                     client_id=client_id, clean_session=True)

# Assign event callbacks
client.on_connect = on_connect
client.on_subscribe = on_subscribe
client.on_message = on_message

# Connect to the MQTT broker
client.connect(broker, port, 60)

# Start the network loop
client.loop_forever()
