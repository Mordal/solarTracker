window.onload = async function () {
  getTimeoutSettings();
  getFlags()
  registerClient();
  setInterval(registerClient, 25000); // 25000 milliseconden = 25 seconden
};

let timeoutSettings = {};
let timers = {};

let forceLeftButton = false;
let forceRightButton = false;
let forceInButton = false;
let forceOutButton = false;

const baseUrl = 'http://pieserver.myds.me:90';


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
