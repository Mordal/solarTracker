let pageData = {};
let baseUrl = 'http://192.168.0.111';

async function updateValues() {
  if (!(await getAllPageData())) return;

  setFlags();
  setSensors();
  setTurnMovement();
}

async function getAllPageData() {
  let response;

  try {
    response = await fetch(`${baseUrl}/API/PAGEDATA`, {
      method: 'GET',
    });
  } catch (error) {
    console.error('Fout:', error);
    return false;
  }

  if (!response.ok) {
    console.log(response);
    return false;
  }

  pageData = await response.json();
  if (pageData.hasOwnProperty('Flags')) {
    return true;
  }
  return false;
}

function setFlags() {
  if (pageData.Flags.SettingsUnlocked) {
    document.getElementById('colorSquare').classList.add('green-square');
  } else {
    document.getElementById('colorSquare').classList.remove('green-square');
  }

  if (pageData.Flags.Wifi_connected) {
    document.getElementById('Wifi_connected').classList.remove('red');
    document.getElementById('Wifi_connected').classList.add('green');
  } else {
    document.getElementById('Wifi_connected').classList.remove('green');
    document.getElementById('Wifi_connected').classList.add('red');
  }

  if (pageData.Flags.Mqtt_connected) {
    document.getElementById('Mqtt_connected').classList.remove('red');
    document.getElementById('Mqtt_connected').classList.add('green');
  } else {
    document.getElementById('Mqtt_connected').classList.remove('green');
    document.getElementById('Mqtt_connected').classList.add('red');
  }

  if (pageData.Flags.isNight) {
    document.getElementById('isNight').classList.add('red');
  } else {
    document.getElementById('isNight').classList.remove('red');
  }

  if (pageData.Flags.Draaien_too_long) {
    document.getElementById('Draaien_too_long').classList.add('red');
    document.getElementById('LEFT_Alarm').classList.add('red');
    document.getElementById('RIGHT_Alarm').classList.add('red');
  } else {
    document.getElementById('Draaien_too_long').classList.remove('red');
    document.getElementById('LEFT_Alarm').classList.remove('red');
    document.getElementById('RIGHT_Alarm').classList.remove('red');
  }

  if (pageData.Flags.Kantelen_too_long) {
    document.getElementById('Kantelen_too_long').classList.add('red');
  } else {
    document.getElementById('Kantelen_too_long').classList.remove('red');
  }

  if (pageData.Flags.Reset) {
    document.getElementById('reset').classList.add('red');
  } else {
    document.getElementById('reset').classList.remove('red');
  }

  if (pageData.Flags.testMode) {
    document.getElementById('TEST_MODE').classList.add('red');
  } else {
    document.getElementById('TEST_MODE').classList.remove('red');
  }

  if (pageData.Flags.safeMode) {
    document.getElementById('SAFE_MODE').classList.add('red');
  } else {
    document.getElementById('SAFE_MODE').classList.remove('red');
  }

  if (pageData.Flags.stopMode) {
    document.getElementById('STOP_MODE').classList.add('red');
  } else {
    document.getElementById('STOP_MODE').classList.remove('red');
  }
}

function setSensors() {
  document.getElementById('lb').innerHTML = pageData.Sensors.lichtSensor_LB;
  document.getElementById('rb').innerHTML = pageData.Sensors.lichtSensor_RB;
  document.getElementById('lo').innerHTML = pageData.Sensors.lichtSensor_LO;
  document.getElementById('ro').innerHTML = pageData.Sensors.lichtSensor_RO;

  document.getElementById('l').innerHTML = pageData.Sensors.links;
  document.getElementById('r').innerHTML = pageData.Sensors.rechts;

  document.getElementById('b').innerHTML = pageData.Sensors.boven;
  document.getElementById('o').innerHTML = pageData.Sensors.onder;
}

function setTurnMovement() {
  // movementDataObject["Turn"]["percentage"] =
  //      (float)currentTurnPercentage / 100.0;
  // movementDataObject["Turn"]["goToPosition"] = gotoTurnPosition;

  // LEFT and RIGHT
  if (pageData.Turn.antiPendel) {
    document.getElementById('LEFT_Antipendel').classList.add('red');
    document.getElementById('RIGHT_Antipendel').classList.add('red');
  } else {
    document.getElementById('LEFT_Antipendel').classList.remove('red');
    document.getElementById('RIGHT_Antipendel').classList.remove('red');
  }
  // TODO
  // - Percentage
  // - goToPosition

  //LEFT
  if (pageData.Left.moving) {
    document.getElementById('LEFT_Active').classList.add('green');
  } else {
    document.getElementById('LEFT_Active').classList.remove('green');
  }

  if (pageData.Left.sensors) {
    document.getElementById('LEFT_Sensors').classList.add('green');
  } else {
    document.getElementById('LEFT_Sensors').classList.remove('green');
  }

  if (pageData.Left.eindeloop) {
    document.getElementById('LEFT_End').classList.add('red');
  } else {
    document.getElementById('LEFT_End').classList.remove('red');
  }

  if (pageData.Left.force) {
    document.getElementById('LEFT_Force').classList.add('green');
  } else {
    document.getElementById('LEFT_Force').classList.remove('green');
  }

  //RIGHT
  if (pageData.Right.moving) {
    document.getElementById('RIGHT_Active').classList.add('green');
  } else {
    document.getElementById('RIGHT_Active').classList.remove('green');
  }

  if (pageData.Right.sensors) {
    document.getElementById('RIGHT_Sensors').classList.add('green');
  } else {
    document.getElementById('RIGHT_Sensors').classList.remove('green');
  }

  if (pageData.Right.eindeloop) {
    document.getElementById('RIGHT_End').classList.add('red');
  } else {
    document.getElementById('RIGHT_End').classList.remove('red');
  }

  if (pageData.Right.force) {
    document.getElementById('RIGHT_Force').classList.add('green');
  } else {
    document.getElementById('RIGHT_Force').classList.remove('green');
  }
}
