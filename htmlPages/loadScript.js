// MQTT //
// Probeer deze CDNs in volgorde
const mqttCDNs = [
  'https://unpkg.com/mqtt/dist/mqtt.min.js',
  'https://cdnjs.cloudflare.com/ajax/libs/mqtt/4.3.7/mqtt.min.js',
  'https://cdn.jsdelivr.net/npm/mqtt@5.10.3/dist/mqtt.min.js',
];
const topics = {
  FLAGS: 'flags',
  TILT: 'tiltMovementData',
  TURN: 'turnMovementData',
  FORCE: 'forceMovement',
  TIME: 'timeRemaining',
  SENSORS: 'sensorData',
};

function loadMqttScript(urls, callback) {
  if (urls.length === 0) {
    console.error('Geen werkende MQTT CDN gevonden!');
    return;
  }

  const script = document.createElement('script');
  script.src = urls[0];
  script.onload = () => {
    console.log(`MQTT geladen vanaf: ${urls[0]}`);
    callback();
  };
  script.onerror = () => {
    console.warn(`Kon ${urls[0]} niet laden. Probeer volgende...`);
    loadMqttScript(urls.slice(1), callback); // Probeer de volgende URL
  };

  document.head.appendChild(script);
}

// Start de laadprocedure en verbind met MQTT zodra geladen
loadMqttScript(mqttCDNs, () => {
  if (typeof mqtt !== 'undefined') {
    const client = mqtt.connect(mqttUrl)

    client.on('connect', () => {
      console.log('Verbonden met MQTT broker');
      // Alle topics tegelijk subscriben
      client.subscribe(Object.values(topics), (err) => {
        if (err) {
          console.error('Fout bij subscriben:', err);
        } else {
          console.log('Succesvol geabonneerd op alle topics!');
        }
      });
    });

    client.on('message', (topic, message) => {
      if (topic == 'flags') {
        setFlags(JSON.parse(message));
      }
      if (topic == 'tiltMovementData') {
        setTiltMovement(JSON.parse(message));
      }
      if (topic == 'turnMovementData') {
        setTurnMovement(JSON.parse(message));
      }

      if (topic == 'sensorData') {
        setSensors(JSON.parse(message));
      }

      if (topic == 'forceMovement') {
        null;
        //setForceMovement(JSON.parse(message));
      }
      if (topic == 'timeRemaining') {
        setTimeRemaining(JSON.parse(message));
      }
    });

    client.on('error', (err) => {
      console.error('MQTT Fout:', err);
    });
  } else {
    console.error('MQTT library kon niet worden geladen.');
  }
});
