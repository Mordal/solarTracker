// Description: This file contains the functions that are used to set the data on the page.
function setFlags(pageData) {
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
    document.getElementById('OUT_Alarm').classList.add('red');
    document.getElementById('IN_Alarm').classList.add('red');
  } else {
    document.getElementById('Kantelen_too_long').classList.remove('red');
    document.getElementById('OUT_Alarm').classList.remove('red');
    document.getElementById('IN_Alarm').classList.remove('red');
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

function setSensors(pageData) {
  document.getElementById('lb').innerHTML = pageData.Sensors.lichtSensor_LB;
  document.getElementById('rb').innerHTML = pageData.Sensors.lichtSensor_RB;
  document.getElementById('lo').innerHTML = pageData.Sensors.lichtSensor_LO;
  document.getElementById('ro').innerHTML = pageData.Sensors.lichtSensor_RO;

  document.getElementById('l').innerHTML = pageData.Sensors.links;
  document.getElementById('r').innerHTML = pageData.Sensors.rechts;

  document.getElementById('b').innerHTML = pageData.Sensors.boven;
  document.getElementById('o').innerHTML = pageData.Sensors.onder;
}

function setTurnMovement(pageData) {
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

  const percentage = pageData.Turn.percentage / 100;
  const string = `Draaien - ${percentage}%`;
  document.getElementById('Draaien-Positie_Percentage').innerHTML = string;

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

function setTiltMovement(pageData) {
  // pageData["Tilt"]["percentage"] = (float)currentTiltPercentage / 100.0;
  // pageData["Tilt"]["antiPendel"] = antiPendel_Kantelen;
  // pageData["Tilt"]["goToPosition"] = gotoTiltPosition;

  // pageData["Retract"]["sensors"] = inschuiven_Sensors;
  // pageData["Retract"]["moving"] = inschuiven;
  // pageData["Retract"]["eindeloop"] = einde_Inschuiven;
  // pageData["Retract"]["force"] = inschuiven_FORCE;

  // UP and DOWN

  if (pageData.Tilt.antiPendel) {
    document.getElementById('OUT_Antipendel').classList.add('red');
    document.getElementById('IN_Antipendel').classList.add('red');
  } else {
    document.getElementById('OUT_Antipendel').classList.remove('red');
    document.getElementById('IN_Antipendel').classList.remove('red');
  }

  // TODO
  // - Percentage
  const percentage = pageData.Tilt.percentage / 100;
  const string = `Kantelen - ${percentage}%`;
  document.getElementById('Kantelen-Positie_Percentage').innerHTML = string;

  // - goToPosition

  // EXTEND
  if (pageData.Extend.moving) {
    document.getElementById('OUT_Active').classList.add('green');
  } else {
    document.getElementById('OUT_Active').classList.remove('green');
  }

  if (pageData.Extend.sensors) {
    document.getElementById('OUT_Sensors').classList.add('green');
  } else {
    document.getElementById('OUT_Sensors').classList.remove('green');
  }

  if (pageData.Extend.eindeloop) {
    document.getElementById('OUT_End').classList.add('red');
  } else {
    document.getElementById('OUT_End').classList.remove('red');
  }

  if (pageData.Extend.force) {
    document.getElementById('OUT_Force').classList.add('green');
  } else {
    document.getElementById('OUT_Force').classList.remove('green');
  }

  // RETRACT
  if (pageData.Retract.moving) {
    document.getElementById('IN_Active').classList.add('green');
  } else {
    document.getElementById('IN_Active').classList.remove('green');
  }

  if (pageData.Retract.sensors) {
    document.getElementById('IN_Sensors').classList.add('green');
  } else {
    document.getElementById('IN_Sensors').classList.remove('green');
  }

  if (pageData.Retract.eindeloop) {
    document.getElementById('IN_End').classList.add('red');
  } else {
    document.getElementById('IN_End').classList.remove('red');
  }

  if (pageData.Retract.force) {
    document.getElementById('IN_Force').classList.add('green');
  } else {
    document.getElementById('IN_Force').classList.remove('green');
  }
}

function setTimeRemaining(pageData) {
  // JSONVar remainingTimeObject = undefined;
  // remainingTimeObject["TimeRem"]["AP_Draaien_Timer"] =
  //     antiPendel_Draaien_Timer_Remaining;
  // remainingTimeObject["TimeRem"]["AP_Kantelen_Timer"] =
  //     antiPendel_Kantelen_Timer_Remaining;
  // remainingTimeObject["TimeRem"]["draaien_TO"] = draaien_TimeOut_Remaining;
  // remainingTimeObject["TimeRem"]["kantelen_TO"] = kantelen_TimeOut_Remaining;
  // remainingTimeObject["TimeRem"]["logBook_Timer"] = logBook_Timer_Remaining;
  // remainingTimeObject["TimeRem"]["clientTimer"] =
  //     clientConnectedTimer_Remaining;
  // remainingTimeObject["TimeRem"]["periodicalTimer"] = periodicalTimer_Remaining;
  // remainingTimeObject["TimeRem"]["setUnlockTimer"] =
  //     settingsUnlockedTimer_Remaining;
  // remainingTimeObject["TimeRem"]["gotoPosTimer"] =
  //     gotoPosition_Timer_Remaining;
  // return remainingTimeObject;
  // }
  //  }

  // Draaien
  const draaienTime = pageData.TimeRem.AP_Draaien_Timer / 1000;
  const draaienID = `AP_Draaien`;
  setTimer(draaienID, draaienTime);

  // Kantelen
  const kantelenTime = pageData.TimeRem.AP_Kantelen_Timer / 1000;
  const kantelenID = `AP_Kantelen`;
  setTimer(kantelenID, kantelenTime);

  // Draaien TimeOut
  const draaienTimeOut = pageData.TimeRem.draaien_TO / 1000;
  const draaienTimeOutID = `Draaien_TO`;
  setTimer(draaienTimeOutID, draaienTimeOut);

  // Kantelen TimeOut
  const kantelenTimeOut = pageData.TimeRem.kantelen_TO / 1000;
  const kantelenTimeOutID = `Kantelen_TO`;
  setTimer(kantelenTimeOutID, kantelenTimeOut);

  // Logbook timer
  const logBookTime = pageData.TimeRem.logBook_Timer / 1000;
  const logBookID = `LogbookTimer`;
  setTimer(logBookID, logBookTime);

  //periodicalTimer
  const periodicalTime = pageData.TimeRem.periodicalTimer / 1000;
  const retryID = `Retry_Timer`;
  setTimer(retryID, periodicalTime);

  //Settings Unlocked Timer
  const unlockTime = pageData.TimeRem.setUnlockTimer / 1000;
  const unlockID = `Settings_Unlock`;
  setTimer(unlockID, unlockTime);

  //goToPosition Timer
  const gotoPosTime = pageData.TimeRem.gotoPosTimer / 1000;
  const gotoPosID = `goToPosition`;
  setTimer(gotoPosID, gotoPosTime);
}
