setInterval(updateValues, 5000);
let forceLeftButton = false;
let baseUrl = 'http://192.168.0.111';

async function updateValues() {
  //   const data = await getSensors();
  //   setSensors(data);
}

async function getFlags() {
  console.log('getFlags');
  const response = await fetch(`${baseUrl}/API/FLAGS`, {
    method: 'GET',
  });
  console.log(response.status);
  const data = await response.json();
  console.log(data);
  setFlags(data);
}

async function getSensors() {
  const response = await fetch(`${baseUrl}/API/LIGHTSENSORS`, {
    method: 'GET',
  });

  // Controleer of de respons succesvol is
  if (!response.ok) {
    throw new Error(`HTTP-fout! Status: ${response.status}`);
  }

  const data = await response.json();
  console.log(data);
  return data;
}

function setFlags(data) {
  if (data.Flags.SettingsUnlocked) {
    document.getElementById('colorSquare').classList.add('green-square');
  } else {
    document.getElementById('colorSquare').classList.remove('green-square');
  }
}

function setSensors(data) {
  document.getElementById('lb').innerHTML = data.Sensors.lichtSensor_LB;
  document.getElementById('rb').innerHTML = data.Sensors.lichtSensor_RB;
  document.getElementById('lo').innerHTML = data.Sensors.lichtSensor_LO;
  document.getElementById('ro').innerHTML = data.Sensors.lichtSensor_RO;

  //   const l = data.lichtSensors.Links_Boven + data.lichtSensors.Links_Onder;
  //   const r = data.lichtSensors.Rechts_Boven + data.lichtSensors.Rechts_Onder;
  document.getElementById('l').innerHTML = data.Sensors.links;
  document.getElementById('r').innerHTML = data.Sensors.rechts;
  //   document.getElementById('l').innerHTML = l;
  //   document.getElementById('r').innerHTML = r;
  //   const b = data.lichtSensors.Links_Boven + data.lichtSensors.Rechts_Boven;
  //   const o = data.lichtSensors.Links_Onder + data.lichtSensors.Rechts_Onder;
  document.getElementById('b').innerHTML = data.Sensors.boven;
  document.getElementById('o').innerHTML = data.Sensors.onder;
  //   document.getElementById('b').innerHTML = b;
  //   document.getElementById('o').innerHTML = o;
}

function setFlags_Left(data) {
  // myObject["TURN_LEFT"]["Moving"] = false;
  // myObject["TURN_LEFT"]["Sensors"] = true;
  // myObject["TURN_LEFT"]["Einde_Loop"] = false;
  // myObject["TURN_LEFT"]["AntiPendel"] = false;
  // myObject["Flags"]["Draaien_too_long"] = false;
  // myObject["ForceMovement"]["LEFT_Force"]
  // ---> DEZE HEB IK NOG NIET OP MEN PAGINA --->
  // myObject["TURN_LEFT"]["licht_links"] = lichtLinks;
  // myObject["TURN_LEFT"]["licht_rechts"] = lichtRechts + licht_marge;

  document.getElementById('LEFT_Active').innerHTML = data.TURN_LEFT.Moving;
  document.getElementById('LEFT_Sensors').innerHTML = data.TURN_LEFT.Sensors;
  document.getElementById('LEFT_End').innerHTML = data.TURN_LEFT.Einde_Loop;
  document.getElementById('LEFT_Antipendel').innerHTML = data.TURN_LEFT.AntiPendel;
  document.getElementById('LEFT_Alarm').innerHTML = data.Flags.Draaien_too_long;
  if (!forceLeftButton) {
    document.getElementById('LEFT_Force').innerHTML = data.ForceMovement.LEFT_Force;
  }
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

function forceMovement_changeState(id) {
  if (id === 'LEFT_Force') {
    forceLeftButton = !forceLeftButton;
  }

  var statusCell = document.getElementById(id);
  if (statusCell.innerHTML === 'false') {
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
  if (statusCell.innerHTML == 'false') {
    statusCell.innerHTML = 'true';
  } else {
    statusCell.innerHTML = 'false';
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

document.getElementById('unlockForm').addEventListener('submit', async function (event) {
  // Voorkom dat de pagina herlaadt
  event.preventDefault();

  // Verkrijg de waarde van het inputveld
  const unlockCode = document.querySelector('input[name="unlock"]').value;
  document.querySelector('input[name="unlock"]').value = '';

  // Gebruik Fetch API om een POST-verzoek te sturen naar /UNLOCK
  fetch(`${baseUrl}/UNLOCK`, {
    method: 'POST',
    // mode: 'cors',
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
      document.querySelector('input[name="unlock"]').value = responseText;
      getFlags();
    })
    .catch((error) => {
      console.error('Fout:', error);
    });
});
