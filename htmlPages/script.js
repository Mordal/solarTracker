window.onload = function () {
  getSettings();
  registerClient();
  setInterval(registerClient, 25000); // 25000 milliseconden = 25 seconden
};

let currentSettings = {};
let timers = {};

let forceLeftButton = false;
let forceRightButton = false;
let forceInButton = false;
let forceOutButton = false;

const baseUrl = 'http://pieserver.myds.me:90';

// MQTT //

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
    console.warn(`Kon niet laden: ${urls[0]}. Probeer volgende...`);
    loadMqttScript(urls.slice(1), callback); // Probeer de volgende URL
  };

  document.head.appendChild(script);
}

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

// Start de laadprocedure en verbind met MQTT zodra geladen
loadMqttScript(mqttCDNs, () => {
  if (typeof mqtt !== 'undefined') {
    const client = mqtt.connect('ws://pieserver.myds.me:9001');

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

// const client = mqtt.connect('ws://pieserver.myds.me:9001');

// client.on('connect', () => {
//   console.log('Verbonden met MQTT broker');
//   // Alle topics tegelijk subscriben
//   client.subscribe(Object.values(topics), (err) => {
//     if (err) {
//       console.error('Fout bij subscriben:', err);
//     } else {
//       console.log('Succesvol geabonneerd op alle topics!');
//     }
//   });
// });

async function registerClient() {
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), 2000); // 2 sec timeout

  try {
    const response = await fetch(`${baseUrl}/API/CLIENTCONNECTED`, {
      signal: controller.signal,
    });
    clearTimeout(timeout); // Timeout annuleren als de response op tijd komt
    return;
  } catch (error) {
    if (error.name === 'AbortError') {
      console.warn('Request timed out!');
    } else {
      console.error('Fetch error:', error);
    }
  }
}

async function getSettings() {
  const response = await fetch(`${baseUrl}/API/SETTINGS`);
  currentSettings = await response.json();
}

// Selecteer de knop
const button = document.getElementById('StopButton');
// Voeg een click-eventlistener toe om de actieve klasse te toggelen
button.addEventListener('click', function () {
  // Toggle de klasse 'active' + de tekst van de knop
  if (button.innerHTML === 'STOP') {
    button.innerHTML = 'START';
  } else {
    button.innerHTML = 'STOP';
  }
  button.classList.toggle('active');
});

// General mouseup event - To capture button release
document.addEventListener('mouseup', () => {
  const forceButtons = [
    {
      id: 'LEFT_Force',
      button: forceLeftButton,
    },
    {
      id: 'RIGHT_Force',
      button: forceRightButton,
    },
    {
      id: 'IN_Force',
      button: forceInButton,
    },
    {
      id: 'OUT_Force',
      button: forceOutButton,
    },
  ];

  // Itereer door alle force-buttons
  forceButtons.forEach(({ id, button }) => {
    if (button) {
      forceLeftButton = false;
      forceRightButton = false;
      forceInButton = false;
      forceOutButton = false;
      forceMovement_Deactivate(id);
    }
  });
});

function forceMovement_Activate(id) {
  const statusCell = document.getElementById(id);
  const actions = {
    LEFT_Force: () => (forceLeftButton = true),
    RIGHT_Force: () => (forceRightButton = true),
    IN_Force: () => (forceInButton = true),
    OUT_Force: () => (forceOutButton = true),
  };

  if (actions[id]) {
    actions[id](); // Voer de bijbehorende actie uit
    statusCell.classList.add('green'); // Voeg de groene class toe
    sendRequest(id, true); // Stuur de request
  }
}

function forceMovement_Deactivate(id) {
  document.getElementById(id).classList.remove('green');
  sendRequest(id, false);
}

function sendRequest(id, state) {
  fetch(`${baseUrl}/CONTROL`, {
    method: 'POST',
    body: JSON.stringify({ [id]: state }),
  });
  //   .then((response) => {
  //     console.log(response);
  //   });
}

// UNLOCK FORM //
document.getElementById('unlockBtn').addEventListener('click', async function (event) {
  // Voorkom dat de pagina herlaadt
  event.preventDefault();

  // Verkrijg de waarde van het inputveld
  const unlockCode = document.getElementById('unlockInput').value;
  document.querySelector('input[name="Unlock"]').value = '';

  // Gebruik Fetch API om een POST-verzoek te sturen naar /UNLOCK
  fetch(`${baseUrl}/UNLOCK`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded',
    },
    body: encodeURIComponent(unlockCode),
  })
    .then((response) => {
      return response.text(); // Wacht op de tekst van de response
    })
    .then((responseText) => {
      console.log(responseText); // Hier krijg je de response body als tekst
      document.getElementById('unlockInput').value = responseText;
    })
    .catch((error) => {
      console.error('Fout:', error);
    });
});

// GoToPosition //
document
  .getElementById('Draaien-Positie_Btn')
  .addEventListener('click', async function (event) {
    event.preventDefault();

    let inputValue = document.getElementById('Draaien-Positie_Input').value;
    inputValue = validatePositionInput(inputValue);

    if (inputValue === null) return;

    fetch(`${baseUrl}/CONTROL`, {
      method: 'POST',
      body: JSON.stringify({ TURN_Position: inputValue }),
    });
  });

document
  .getElementById('Kantelen-Positie_Btn')
  .addEventListener('click', async function (event) {
    event.preventDefault();

    let inputValue = document.getElementById('Kantelen-Positie_Input').value;
    inputValue = validatePositionInput(inputValue);
    if (inputValue === null) return;

    fetch(`${baseUrl}/CONTROL`, {
      method: 'POST',
      body: JSON.stringify({ TILT_Position: inputValue }),
    });
  });

// function setTimer(timerId) {
//   const timeoutInput = document.getElementById(`${timerId}Timeout`);
//   const progressElement = document.getElementById(`${timerId}Progress`);
//   const progressText = document.getElementById(`${timerId}ProgressText`);
//   const totalDuration = parseInt(timeoutInput.value) || 0;
//   let remainingTime = totalDuration;

//   if (totalDuration > 0) {
//     progressElement.style.width = '100%';
//     progressText.textContent = `${remainingTime}/${totalDuration}`;
//     const interval = setInterval(() => {
//       remainingTime -= 0.1; // update elke 100ms voor vloeiende animatie
//       progressElement.style.width = `${(remainingTime / totalDuration) * 100}%`;
//       progressText.textContent = `${Math.ceil(remainingTime)}/${totalDuration}`; // afronding naar boven

//       if (remainingTime <= 0) {
//         clearInterval(interval);
//         progressElement.style.width = '0%';
//         progressText.textContent = `0/${totalDuration}`;
//       }
//     }, 100);
//   }
// }

function getTimerSetting(timerId) {
  if (currentSettings['TimeOuts'] === undefined) {
    getSettings();
  }

  switch (timerId) {
    case 'AP_Draaien':
      return currentSettings['TimeOuts']['APTime'] / 1000;
    case 'AP_Kantelen':
      return currentSettings['TimeOuts']['APTime'] / 1000;
    case 'Draaien_TO':
      return currentSettings['TimeOuts']['maxMoveTime'] / 1000;
    case 'Kantelen_TO':
      return currentSettings['TimeOuts']['maxMoveTime'] / 1000;
    case 'LogbookTimer':
      return currentSettings['TimeOuts']['logBook_delay'] / 1000;
    case 'Retry_Timer':
      return currentSettings['TimeOuts']['retryTime'] / 1000;
    case 'Settings_Unlock':
      return 1200; // 20 minuten
    case 'goToPosition':
      return 3600; // 1 uur

    default:
      return 0;
  }
}

function setTimer(timerId, remainingTime) {
  const totalDuration = getTimerSetting(timerId);
  if (totalDuration <= 0) return; // Geen negatieve of nul timers

  if (!timers[timerId]) {
    // Start een nieuwe timer als deze nog niet bestaat
    timers[timerId] = {
      remainingTime,
      interval: setInterval(() => updateTimer(timerId, totalDuration), 100),
    };
  } else {
    // Update de bestaande timer
    timers[timerId].remainingTime = remainingTime;
  }
  updateTimer(timerId, totalDuration);
}

function updateTimer(timerId, totalDuration) {
  const progressElement = document.getElementById(`${timerId}Progress`);
  const progressText = document.getElementById(`${timerId}ProgressText`);

  if (!timers[timerId]) return; // Timer bestaat niet

  timers[timerId].remainingTime -= 0.1; // update elke 100ms voor vloeiende animatie

  progressElement.style.width = `${
    (timers[timerId].remainingTime / totalDuration) * 100
  }%`;
  progressText.textContent = `${Math.ceil(
    timers[timerId].remainingTime
  )}/${totalDuration}`;

  if (timers[timerId].remainingTime <= 0) {
    clearInterval(timers[timerId].interval);
    delete timers[timerId];
    progressElement.style.width = '0%';
    progressText.textContent = `0/${totalDuration}`;
  }
}

function validatePositionInput(input) {
  const value = parseFloat(input.replace(',', '.'));
  if (isNaN(value)) {
    alert('Ongeldige invoer. Voer een geldig getal in.');
    return null;
  }
  const rangedValue = Math.min(Math.max(value, 0), 100);
  const roundedValue = Math.round(rangedValue * 100) / 100;
  return roundedValue * 100;
}
