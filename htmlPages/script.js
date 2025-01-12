setInterval(updateValues, 10000); // 5 seconden
let forceLeftButton = false;
let forceRightButton = false;
let forceInButton = false;
let forceOutButton = false;

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
      button = false;
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
  const unlockCode = document.querySelector('input[name="Unlock"]').value;
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
