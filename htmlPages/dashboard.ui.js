function getEl(id) {
  return document.getElementById(id);
}

function setDot(id, type) {
  const el = getEl(id);
  if (!el) return;
  el.classList.remove('green', 'red', 'neutral');
  el.classList.add(type);
}

function setGreenRed(id, isOk) {
  setDot(id, isOk ? 'green' : 'red');
}

function setRedNeutral(id, isError) {
  setDot(id, isError ? 'red' : 'neutral');
}

function setGreenNeutral(id, isOn) {
  setDot(id, isOn ? 'green' : 'neutral');
}

function initTimerUi() {
  const container = getEl('timerGrid');
  if (!container) return;

  container.innerHTML = '';
  dashboardConfig.timers.forEach((timer) => {
    const wrapper = document.createElement('div');
    wrapper.className = 'timer-item';
    wrapper.innerHTML = `
      <div class="timer-title">${timer.label}</div>
      <div class="progress-track">
        <div class="progress-fill" id="${timer.id}Progress"></div>
        <div class="progress-text" id="${timer.id}ProgressText">0/0</div>
      </div>
    `;
    container.appendChild(wrapper);
  });
}

function setFlags(data) {
  if (!data?.Flags) return;

  setGreenRed('Wifi_connected', !!data.Flags.Wifi_con);
  setGreenRed('Mqtt_connected', !!data.Flags.Mqtt_con);

  setRedNeutral('Draaien_too_long', !!data.Flags.Draaien_TL);
  setRedNeutral('Kantelen_too_long', !!data.Flags.Kantelen_TL);

  setRedNeutral('LEFT_Alarm', !!data.Flags.Draaien_TL);
  setRedNeutral('RIGHT_Alarm', !!data.Flags.Draaien_TL);
  setRedNeutral('OUT_Alarm', !!data.Flags.Kantelen_TL);
  setRedNeutral('IN_Alarm', !!data.Flags.Kantelen_TL);

  setRedNeutral('NIGHT_MODE', !!data.Flags.NIGHT_MODE);
  setRedNeutral('STOP_MODE', !!data.Flags.STOP_MODE);
  setGreenNeutral('FORCE_MODE', !!data.Flags.FORCE_MODE);
  setRedNeutral('TEST_MODE', !!data.Flags.TEST_MODE);
  setRedNeutral('SAFE_MODE', !!data.Flags.SAFE_MODE);

  const stopButton = getEl('StopButton');
  if (stopButton) {
    const stopMode = !!data.Flags.STOP_MODE;
    stopButton.textContent = stopMode ? 'START' : 'STOP';
    stopButton.classList.toggle('running', stopMode);
  }
}

function setOtherData(data) {
  setGreenNeutral('reset', !!data?.Reset);
  const unlockSquare = getEl('colorSquare');
  if (!unlockSquare) return;
  unlockSquare.classList.toggle('green-square', !!data?.SettingsUnlock);
}

function setText(id, value) {
  const el = getEl(id);
  if (!el) return;
  el.textContent = value;
}

function setWidth(id, percent) {
  const el = getEl(id);
  if (!el) return;
  const clamped = Math.max(0, Math.min(100, percent));
  el.style.width = `${clamped}%`;
}

function setHeight(id, percent) {
  const el = getEl(id);
  if (!el) return;
  const clamped = Math.max(0, Math.min(100, percent));
  el.style.height = `${clamped}%`;
}

function setWaterLevelBubble(left, right, top, bottom) {
  const bubble = getEl('sensorLevelBubble');
  if (!bubble) return;

  const xDiff = right - left;
  const yDiff = bottom - top;

  const edgeAt = 2000;

  // normaliseer
  let xNorm = xDiff / edgeAt;
  let yNorm = yDiff / edgeAt;

  // bereken vector lengte
  const length = Math.sqrt(xNorm * xNorm + yNorm * yNorm);

  // als buiten de cirkel → schaal terug tot rand
  if (length > 1) {
    xNorm /= length;
    yNorm /= length;
  }

  const maxOffset = 66;
  const x = xNorm * maxOffset;
  const y = yNorm * maxOffset;

  bubble.style.transform =
    `translate(calc(-50% + ${x}px), calc(-50% + ${y}px))`;
}

function setSensors(data) {
  if (!data?.Sensors) return;

  const lb = Number(data.Sensors.lichtSensor_LB) || 0;
  const rb = Number(data.Sensors.lichtSensor_RB) || 0;
  const lo = Number(data.Sensors.lichtSensor_LO) || 0;
  const ro = Number(data.Sensors.lichtSensor_RO) || 0;

  setText('lb', lb);
  setText('rb', rb);
  setText('lo', lo);
  setText('ro', ro);

  const maxSensorValue = 2047;
  setWidth('lbLevel', (lb / maxSensorValue) * 100);
  setWidth('rbLevel', (rb / maxSensorValue) * 100);
  setWidth('loLevel', (lo / maxSensorValue) * 100);
  setWidth('roLevel', (ro / maxSensorValue) * 100);

  const left = lb + lo;
  const right = rb + ro;
  const top = lb + rb;
  const bottom = lo + ro;

  setWaterLevelBubble(left, right, top, bottom);

  setText('leftCombinedValue', left);
  setText('rightCombinedValue', right);
  setText('topCombinedValue', top);
  setText('bottomCombinedValue', bottom);

  const lrTotal = left + right;
  const leftPercent = lrTotal > 0 ? (left / lrTotal) * 100 : 50;
  const rightPercent = lrTotal > 0 ? (right / lrTotal) * 100 : 50;
  setWidth('leftBalanceFill', leftPercent);
  setWidth('rightBalanceFill', rightPercent);

  const udTotal = top + bottom;
  const topPercent = udTotal > 0 ? (top / udTotal) * 100 : 50;
  const bottomPercent = udTotal > 0 ? (bottom / udTotal) * 100 : 50;
  setHeight('topBalanceFill', topPercent);
  setHeight('bottomBalanceFill', bottomPercent);

  const lrDirection = getEl('lrDirection');
  if (lrDirection) {
    const lrDiff = right - left;
    const lrThreshold = 25;
    if (Math.abs(lrDiff) <= lrThreshold) {
      lrDirection.textContent = 'Rotatierichting: in balans ↔';
    } else if (lrDiff > 0) {
      lrDirection.textContent = 'Rotatierichting: draai naar rechts →';
    } else {
      lrDirection.textContent = 'Rotatierichting: draai naar links ←';
    }
  }

  const udDirection = getEl('udDirection');
  if (udDirection) {
    const udDiff = top - bottom;
    const udThreshold = 25;
    if (Math.abs(udDiff) <= udThreshold) {
      udDirection.textContent = 'Kantelrichting: in balans ↕';
    } else if (udDiff > 0) {
      udDirection.textContent = 'Kantelrichting: kantel omhoog ↑';
    } else {
      udDirection.textContent = 'Kantelrichting: kantel omlaag ↓';
    }
  }
}

function setTurnMovement(data) {
  if (!data?.Turn || !data?.Left || !data?.Right) return;

  const percentage = data.Turn.percentage / 100;
  getEl('Draaien-Positie_Percentage').textContent = `Draaien - ${percentage}%`;

  setRedNeutral('LEFT_Antipendel', !!data.Turn.antiPendel);
  setGreenNeutral('LEFT_Active', !!data.Left.moving);
  setGreenNeutral('LEFT_Sensors', !!data.Left.sensors);
  setRedNeutral('LEFT_End', !!data.Left.eindeloop);
  setGreenNeutral('LEFT_Force', !!data.Left.force);

  setRedNeutral('RIGHT_Antipendel', !!data.Turn.antiPendel);
  setGreenNeutral('RIGHT_Active', !!data.Right.moving);
  setGreenNeutral('RIGHT_Sensors', !!data.Right.sensors);
  setRedNeutral('RIGHT_End', !!data.Right.eindeloop);
  setGreenNeutral('RIGHT_Force', !!data.Right.force);
}

function setTiltMovement(data) {
  if (!data?.Tilt || !data?.Extend || !data?.Retract) return;

  const percentage = data.Tilt.percentage / 100;
  getEl('Kantelen-Positie_Percentage').textContent = `Kantelen - ${percentage}%`;

  setRedNeutral('OUT_Antipendel', !!data.Tilt.antiPendel);
  setGreenNeutral('OUT_Active', !!data.Extend.moving);
  setGreenNeutral('OUT_Sensors', !!data.Extend.sensors);
  setRedNeutral('OUT_End', !!data.Extend.eindeloop);
  setGreenNeutral('OUT_Force', !!data.Extend.force);

  setRedNeutral('IN_Antipendel', !!data.Tilt.antiPendel);
  setGreenNeutral('IN_Active', !!data.Retract.moving);
  setGreenNeutral('IN_Sensors', !!data.Retract.sensors);
  setRedNeutral('IN_End', !!data.Retract.eindeloop);
  setGreenNeutral('IN_Force', !!data.Retract.force);
}

function getTimeoutSetting(timerId) {
  const t = appState.timeoutSettings?.TimeOuts;
  if (!t) return 0;

  switch (timerId) {
    case 'draaien_AP':
    case 'kantelen_AP':
      return t.APTime / 1000;
    case 'Draaien_TO':
    case 'Kantelen_TO':
      return t.maxMoveTime / 1000;
    case 'LogbookTime':
      return t.logbookTime / 1000;
    case 'Retry_Timer':
      return t.periodicalTime / 1000;
    case 'Settings_Unlock':
      return t.settingsUnlockTime / 1000;
    case 'goToPosition':
      return 3600;
    default:
      return 0;
  }
}

function setTimeRemaining(data) {
  const time = data?.TimeRem;
  if (!time) return;

  setTimer('draaien_AP', time.draaien_AP / 1000);
  setTimer('kantelen_AP', time.kantelen_AP / 1000);
  setTimer('Draaien_TO', time.draaien_TO / 1000);
  setTimer('Kantelen_TO', time.kantelen_TO / 1000);
  setTimer('LogbookTime', time.logBookTime / 1000);
  setTimer('Retry_Timer', time.periodicalTimer / 1000);
  setTimer('Settings_Unlock', time.setUnlockTimer / 1000);
  setTimer('goToPosition', time.gotoPosTimer / 1000);
}

function setTimer(timerId, remainingTime) {
  const totalDuration = getTimeoutSetting(timerId);
  if (totalDuration <= 0) return;

  if (!appState.timers[timerId]) {
    appState.timers[timerId] = {
      remainingTime,
      interval: setInterval(() => updateTimer(timerId, totalDuration), 100),
    };
  } else {
    appState.timers[timerId].remainingTime = remainingTime;
  }

  updateTimer(timerId, totalDuration);
}

function updateTimer(timerId, totalDuration) {
  const timer = appState.timers[timerId];
  if (!timer) return;

  timer.remainingTime -= 0.1;
  if (timer.remainingTime < 0) timer.remainingTime = 0;

  const progress = getEl(`${timerId}Progress`);
  const text = getEl(`${timerId}ProgressText`);
  if (!progress || !text) return;

  const ratio = Math.max(0, Math.min(100, (timer.remainingTime / totalDuration) * 100));
  progress.style.width = `${ratio}%`;
  text.textContent = `${Math.ceil(timer.remainingTime)}/${totalDuration}`;

  if (timer.remainingTime <= 0) {
    clearInterval(timer.interval);
    delete appState.timers[timerId];
  }
}

function hydrateSettingsInputs(data) {
  const offsets = data?.Offsets;
  const sensors = data?.Sensors;
  if (!offsets || !sensors) return;

  getEl('LB_Offset').value = offsets.LB;
  getEl('RB_Offset').value = offsets.RB;
  getEl('LO_Offset').value = offsets.LO;
  getEl('RO_Offset').value = offsets.RO;
  getEl('lichtMarge').value = sensors.licht_marge;
}
