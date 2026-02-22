import json
import os
import signal
import sys
from urllib.parse import quote_plus

import paho.mqtt.client as mqtt
from pymongo import MongoClient

# MongoDB URI:
# mongodb://write:Reteip%401@192.168.0.205:27017/solarTrackerDB

DEFAULT_IP = "192.168.0.205"
DEFAULT_MONGO_PORT = 27017
DEFAULT_MONGO_DB = "solarTrackerDB"
DEFAULT_MONGO_COLLECTION_PREFIX = "logbook"
DEFAULT_MONGO_USER = "write"
DEFAULT_MONGO_PASSWORD = "Reteip@1"

DEFAULT_MQTT_PORT = 49111
DEFAULT_MQTT_CLIENT_ID = "python-logbook-writer"
DEFAULT_LOGBOOK_KEYS = [
    "flags",
    "sensors",
    "tilt",
    "turn",
    "force",
    "timeRemaining",
    "otherData",
    "wifi",
]

KEY_TO_TOPIC = {
    "flags": "flags",
    "sensors": "sensorData",
    "tilt": "tiltMovementData",
    "turn": "turnMovementData",
    "force": "forceMovement",
    "timeRemaining": "timeRemaining",
    "otherData": "otherData",
    "wifi": "wifiData",
}


class LogbookWriter:
    def __init__(self) -> None:
        host = os.getenv("SOLAR_HOST", DEFAULT_IP)
        mongo_port = int(os.getenv("MONGO_PORT", str(DEFAULT_MONGO_PORT)))
        mongo_db = os.getenv("MONGO_DB", DEFAULT_MONGO_DB)
        mongo_collection_prefix = os.getenv("MONGO_COLLECTION_PREFIX", DEFAULT_MONGO_COLLECTION_PREFIX)
        mongo_user = os.getenv("MONGO_USER", DEFAULT_MONGO_USER)
        mongo_password = os.getenv("MONGO_PASSWORD", DEFAULT_MONGO_PASSWORD)

        mqtt_host = os.getenv("MQTT_HOST", host)
        mqtt_port = int(os.getenv("MQTT_PORT", str(DEFAULT_MQTT_PORT)))
        mqtt_client_id = os.getenv("MQTT_CLIENT_ID", DEFAULT_MQTT_CLIENT_ID)

        keys_env = os.getenv("LOGBOOK_KEYS", ",".join(DEFAULT_LOGBOOK_KEYS))
        logbook_keys = [item.strip() for item in keys_env.split(",") if item.strip()]

        self.topics = self._resolve_topics(logbook_keys)
        self.mongo_collection_prefix = mongo_collection_prefix

        encoded_user = quote_plus(mongo_user)
        encoded_password = quote_plus(mongo_password)
        mongo_uri = f"mongodb://{encoded_user}:{encoded_password}@{host}:{mongo_port}/{mongo_db}"

        self.mongo_client = MongoClient(mongo_uri)
        self.mongo_db = self.mongo_client[mongo_db]
        self.collection_cache = {}

        self.mqtt_client = mqtt.Client(
            callback_api_version=mqtt.CallbackAPIVersion.VERSION2,
            client_id=mqtt_client_id,
            clean_session=True,
        )
        self.mqtt_client.on_connect = self._on_connect
        self.mqtt_client.on_disconnect = self._on_disconnect
        self.mqtt_client.on_message = self._on_message

        self.mqtt_host = mqtt_host
        self.mqtt_port = mqtt_port

    @staticmethod
    def _resolve_topics(logbook_keys: list[str]) -> list[str]:
        resolved = []
        seen = set()

        for key in logbook_keys:
            topic = KEY_TO_TOPIC.get(key, key)
            if not topic:
                continue
            if topic in seen:
                continue
            seen.add(topic)
            resolved.append(topic)

        return resolved

    @staticmethod
    def _decode_payload(payload_bytes: bytes):
        payload_text = payload_bytes.decode("utf-8", errors="replace")
        try:
            return json.loads(payload_text), True
        except json.JSONDecodeError:
            return payload_text, False

    @staticmethod
    def _sanitize_topic_for_collection(topic: str) -> str:
        sanitized = topic.replace("/", "_").replace(".", "_").replace("$", "_")
        while "__" in sanitized:
            sanitized = sanitized.replace("__", "_")
        return sanitized.strip("_") or "unknown"

    def _get_collection_for_topic(self, topic: str):
        topic_key = self._sanitize_topic_for_collection(topic)
        collection_name = f"{self.mongo_collection_prefix}_{topic_key}"

        if collection_name not in self.collection_cache:
            self.collection_cache[collection_name] = self.mongo_db[collection_name]

        return self.collection_cache[collection_name], collection_name

    def _on_connect(self, client, userdata, flags, reason_code, properties=None):
        if reason_code != 0:
            print(f"MQTT connect failed: rc={reason_code}")
            return

        print(f"Connected to MQTT {self.mqtt_host}:{self.mqtt_port}")
        for topic in self.topics:
            client.subscribe(topic, qos=1)
            print(f"Subscribed: {topic}")

    def _on_disconnect(self, client, userdata, disconnect_flags, reason_code, properties=None):
        print(f"MQTT disconnected: rc={reason_code}")

    def _on_message(self, client, userdata, msg):
        data, is_json = self._decode_payload(msg.payload)

        if not is_json:
            print(f"Skipped {msg.topic}: payload is not valid JSON")
            return

        if not isinstance(data, dict):
            print(f"Skipped {msg.topic}: JSON payload is not an object")
            return

        target_collection, collection_name = self._get_collection_for_topic(msg.topic)
        result = target_collection.insert_one(data)
        print(f"Saved {msg.topic} -> {collection_name} ({result.inserted_id})")

    def start(self) -> None:
        self.mongo_client.admin.command("ping")
        print("MongoDB connection ok")
        print(f"Writing per topic with prefix: {self.mongo_db.name}.{self.mongo_collection_prefix}_*")

        self.mqtt_client.connect(self.mqtt_host, self.mqtt_port, keepalive=60)
        self.mqtt_client.loop_forever()

    def stop(self) -> None:
        try:
            self.mqtt_client.disconnect()
        finally:
            self.mongo_client.close()


def main() -> None:
    writer = LogbookWriter()

    def shutdown_handler(signum, frame):
        print("Stopping...")
        writer.stop()
        sys.exit(0)

    signal.signal(signal.SIGINT, shutdown_handler)
    signal.signal(signal.SIGTERM, shutdown_handler)

    writer.start()


if __name__ == "__main__":
    main()
