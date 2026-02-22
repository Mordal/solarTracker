function decodeMqttPayload(message) {
  if (typeof message === 'string') return message;
  if (message instanceof Uint8Array) return new TextDecoder().decode(message);
  return String(message);
}

function routeMqttMessage(topic, payload) {
  try {
    const data = JSON.parse(payload);
    markDataFresh();

    switch (topic) {
      case dashboardConfig.mqttTopics.FLAGS:
        setFlags(data);
        break;
      case dashboardConfig.mqttTopics.TILT:
        setTiltMovement(data);
        break;
      case dashboardConfig.mqttTopics.TURN:
        setTurnMovement(data);
        break;
      case dashboardConfig.mqttTopics.TIME:
        setTimeRemaining(data);
        break;
      case dashboardConfig.mqttTopics.SENSORS:
        setSensors(data);
        break;
      case dashboardConfig.mqttTopics.OTHER:
        setOtherData(data);
        break;
      case dashboardConfig.mqttTopics.FORCE:
      default:
        break;
    }
  } catch (error) {
    console.warn(`MQTT parse error on ${topic}:`, error.message);
  }
}

function loadMqttScript(urls) {
  return new Promise((resolve, reject) => {
    if (!urls.length) {
      reject(new Error('No MQTT CDN available'));
      return;
    }

    const script = document.createElement('script');
    script.src = urls[0];

    script.onload = () => resolve();
    script.onerror = () => loadMqttScript(urls.slice(1)).then(resolve).catch(reject);

    document.head.appendChild(script);
  });
}

async function startMqtt() {
  try {
    await loadMqttScript([...dashboardConfig.mqttCdnFallbacks]);

    if (typeof mqtt === 'undefined') {
      throw new Error('MQTT library unavailable after load');
    }

    const client = mqtt.connect(dashboardConfig.mqttUrl);

    client.on('connect', () => {
      client.subscribe(Object.values(dashboardConfig.mqttTopics), (err) => {
        if (err) {
          console.error('MQTT subscribe error:', err.message || err);
        }
      });
    });

    client.on('message', (topic, message) => {
      routeMqttMessage(topic, decodeMqttPayload(message));
    });

    client.on('error', (err) => {
      console.warn('MQTT connection error:', err.message || err);
    });

    return client;
  } catch (error) {
    console.warn('MQTT startup failed, using API fallback:', error.message);
    return null;
  }
}
