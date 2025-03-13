window.onload = function () {
  registerClient();
  setInterval(registerClient, 25000); // 25000 milliseconden = 25 seconden
};

let forceLeftButton = false;
let forceRightButton = false;
let forceInButton = false;
let forceOutButton = false;

const baseUrl = 'http://pieserver.myds.me:90';

// MQTT //
const client = mqtt.connect('ws://pieserver.myds.me:9001');
const topics = {
  FLAGS: 'flags',
  TILT: 'tiltMovementData',
  TURN: 'turnMovementData',
  FORCE: 'forceMovement',
  TIME: 'timeRemaining',
  SENSORS: 'sensorData',
};

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
    //setForceMovement(JSON.parse(message));
  }
});

async function registerClient() {
  const response = await fetch(`${baseUrl}/API/CLIENTCONNECTED`);
  console.log(response.status);
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
      document.getElementById('unlockForm').value = responseText;
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

function startTimer(timerId) {
  const timeoutInput = document.getElementById(`${timerId}Timeout`);
  const progressElement = document.getElementById(`${timerId}Progress`);
  const progressText = document.getElementById(`${timerId}ProgressText`);
  const totalDuration = parseInt(timeoutInput.value) || 0;
  let remainingTime = totalDuration;

  if (totalDuration > 0) {
    progressElement.style.width = '100%';
    progressText.textContent = `${remainingTime}/${totalDuration}`;
    const interval = setInterval(() => {
      remainingTime -= 0.1; // update elke 100ms voor vloeiende animatie
      progressElement.style.width = `${(remainingTime / totalDuration) * 100}%`;
      progressText.textContent = `${Math.ceil(remainingTime)}/${totalDuration}`; // afronding naar boven

      if (remainingTime <= 0) {
        clearInterval(interval);
        progressElement.style.width = '0%';
        progressText.textContent = `0/${totalDuration}`;
      }
    }, 100);
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
