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