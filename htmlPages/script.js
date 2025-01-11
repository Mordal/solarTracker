setInterval(updateValues, 1000); // 5 seconden
let forceLeftButton = false;

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

function forceMovement_changeState(id) {
  if (id === 'LEFT_Force') {
    forceLeftButton = !forceLeftButton;
  }

  var statusCell = document.getElementById(id);
  if (statusCell.innerHTML === 'F') {
    changeState(statusCell);
    sendRequest(id, true);
  } else {
    changeState(statusCell);
    sendRequest(id, false);
  }
}

function changeState(statusCell) {
  changeStatusColor(statusCell);
  changeStatusText(statusCell);
}

function changeStatusColor(statusCell) {
  if (statusCell.className == 'green status') {
    statusCell.className = 'red status';
  } else {
    statusCell.className = 'green status';
  }
}

function changeStatusText(statusCell) {
  if (statusCell.innerHTML == 'F') {
    statusCell.innerHTML = 'T';
  } else {
    statusCell.innerHTML = 'F';
  }
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
