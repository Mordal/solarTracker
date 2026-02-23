const apiHost = 'pieserver.myds.me';
const pageIsHttps = window.location.protocol === 'https:';

const dashboardConfig = {
  apiBaseCandidates: pageIsHttps
    ? [
      `https://${apiHost}:91`,
      `https://${apiHost}`,
      `http://${apiHost}:90`,
    ]
    : [
      `https://${apiHost}:91`,
      `https://${apiHost}`,
      `http://${apiHost}:90`,
    ],
//   mqttUrl: pageIsHttps ? `wss://${apiHost}:9001` : `ws://${apiHost}:9001`,

  mqttUrl: `wss://mqtt.${apiHost}`,

  mqttCdnFallbacks: [
    'https://unpkg.com/mqtt/dist/mqtt.min.js',
    'https://cdn.jsdelivr.net/npm/mqtt/dist/mqtt.min.js',
    'https://cdnjs.cloudflare.com/ajax/libs/mqtt/5.13.1/mqtt.min.js',
    'https://cdn.jsdelivr.net/npm/mqtt@5.10.3/dist/mqtt.min.js',
  ],
  mqttTopics: {
    FLAGS: 'flags',
    TILT: 'tiltMovementData',
    TURN: 'turnMovementData',
    FORCE: 'forceMovement',
    TIME: 'timeRemaining',
    SENSORS: 'sensorData',
    OTHER: 'otherData',
  },
  timers: [
    { id: 'draaien_AP', label: 'AP Draaien' },
    { id: 'kantelen_AP', label: 'AP Kantelen' },
    { id: 'Draaien_TO', label: 'Draaien Timeout' },
    { id: 'Kantelen_TO', label: 'Kantelen Timeout' },
    { id: 'LogbookTime', label: 'Logbook timer' },
    { id: 'Retry_Timer', label: 'Retry timer' },
    { id: 'Settings_Unlock', label: 'Settings unlock' },
    { id: 'goToPosition', label: 'Go to position' },
  ],
};

const appState = {
  apiBaseUrl: null,
  timeoutSettings: {},
  timers: {},
  forceHold: {
    LEFT_Force: false,
    RIGHT_Force: false,
    IN_Force: false,
    OUT_Force: false,
  },
};
