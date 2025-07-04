// Description: This file contains the functions that are used to set the data on the page.

function getElementById(id) {
  const el = document.getElementById(id);
  if (!el) {
    console.warn(`Element with id ${id} not found.`);
  }
  return el;
}

function setGreenSquare(id, isOk) {
  const el = getElementById(id);
  if (!el)    return;
  el.classList.toggle('green-square', isOk);
}

function setStatusColorGreenRed(id, isOk) {
  const el = getElementById(id);
  if (!el)    return;
  
  el.classList.toggle('green', isOk);
  el.classList.toggle('red', !isOk);
}

function setStatusColorRed(id, isError) {
  const el = getElementById(id);
  if (!el)    return;
  el.classList.toggle('red', isError);
}

function setStatusColorGreen(id, isOk) {
  const el = getElementById(id);
  if (!el)    return;
  el.classList.toggle('green', isOk);
}

function setFlags(pageData) {
  setGreenSquare('colorSquare', !!pageData.Flags.SettingsUnlocked);
  setStatusColorGreenRed('Wifi_connected', !!pageData.Flags.Wifi_connected);
  setStatusColorGreenRed('Mqtt_connected', !!pageData.Flags.Mqtt_connected);
  setStatusColorRed('isNight', !!pageData.Flags.isNight);
  setStatusColorRed('Draaien_too_long', !!pageData.Flags.Draaien_too_long);
  setStatusColorRed('LEFT_Alarm', !!pageData.Flags.Draaien_too_long);
  setStatusColorRed('RIGHT_Alarm', !!pageData.Flags.Draaien_too_long);
  setStatusColorRed('Kantelen_too_long', !!pageData.Flags.Kantelen_too_long);
  setStatusColorRed('OUT_Alarm', !!pageData.Flags.Kantelen_too_long);
  setStatusColorRed('IN_Alarm', !!pageData.Flags.Kantelen_too_long);
  setStatusColorRed('reset', !!pageData.Flags.Reset);
  setStatusColorRed('TEST_MODE', !!pageData.Flags.testMode);
  setStatusColorRed('SAFE_MODE', !!pageData.Flags.safeMode);
  setStatusColorRed('STOP_MODE', !!pageData.Flags.stopMode);
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
  
  const percentage = pageData.Turn.percentage / 100;
  const string = `Draaien - ${percentage}%`;
  document.getElementById('Draaien-Positie_Percentage').innerHTML = string;

  // - goToPosition

  //LEFT
  setStatusColorRed('LEFT_Antipendel', pageData.Turn.antiPendel);
  setStatusColorGreen('LEFT_Active', pageData.Left.moving);
  setStatusColorGreen('LEFT_Sensors', pageData.Left.sensors);
  setStatusColorRed('LEFT_End', pageData.Left.eindeloop);
  setStatusColorGreen('LEFT_Force', pageData.Left.force);

  //RIGHT
  setStatusColorRed('RIGHT_Antipendel', pageData.Turn.antiPendel);
  setStatusColorGreen('RIGHT_Active', pageData.Right.moving);
  setStatusColorGreen('RIGHT_Sensors', pageData.Right.sensors);
  setStatusColorRed('RIGHT_End', pageData.Right.eindeloop);
  setStatusColorGreen('RIGHT_Force', pageData.Right.force);

}

function setTiltMovement(pageData) {
  const percentage = pageData.Tilt.percentage / 100;
  const string = `Kantelen - ${percentage}%`;
  document.getElementById('Kantelen-Positie_Percentage').innerHTML = string;

  // EXTEND
  setStatusColorRed('OUT_Antipendel', pageData.Tilt.antiPendel);
  setStatusColorGreen('OUT_Active', pageData.Extend.moving);
  setStatusColorGreen('OUT_Sensors', pageData.Extend.sensors);
  setStatusColorRed('OUT_End', pageData.Extend.eindeloop);
  setStatusColorGreen('OUT_Force', pageData.Extend.force);

  // RETRACT
  setStatusColorRed('IN_Antipendel', pageData.Tilt.antiPendel);
  setStatusColorGreen('IN_Active', pageData.Retract.moving);
  setStatusColorGreen('IN_Sensors', pageData.Retract.sensors);
  setStatusColorRed('IN_End', pageData.Retract.eindeloop);
  setStatusColorGreen('IN_Force', pageData.Retract.force);
}

function setTimeRemaining(pageData) {
  setTimer('draaien_AP', pageData.TimeRem.draaien_AP / 1000);
  setTimer('kantelen_AP', pageData.TimeRem.kantelen_AP / 1000);
  setTimer('Draaien_TO', pageData.TimeRem.draaien_TO / 1000);
  setTimer('Kantelen_TO', pageData.TimeRem.kantelen_TO / 1000);
  setTimer('LogbookTime', pageData.TimeRem.logBookTime / 1000);
  setTimer('Retry_Timer', pageData.TimeRem.periodicalTimer / 1000);
  setTimer('Settings_Unlock', pageData.TimeRem.setUnlockTimer / 1000);
  setTimer('goToPosition', pageData.TimeRem.gotoPosTimer / 1000);
}


async function setTimer(timerId, remainingTime) {
  const totalDuration = await getTimeoutSetting(timerId);
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