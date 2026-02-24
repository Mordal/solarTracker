function validatePositionInput(input) {
  const value = parseFloat(String(input).replace(',', '.'));
  if (Number.isNaN(value)) {
    alert('Ongeldige invoer. Voer een geldig getal in.');
    return null;
  }

  const rangedValue = Math.min(Math.max(value, 0), 100);
  return Math.round(rangedValue * 100) / 100 * 100;
}

function readNumericInput(inputId) {
  const raw = document.getElementById(inputId)?.value;
  const number = Number(raw);
  if (Number.isNaN(number)) return null;
  return number;
}

function readSettingValue(inputId, type) {
  const el = document.getElementById(inputId);
  if (!el) return { ok: false, error: 'Input niet gevonden.' };

  if (type === 'boolean') {
    return { ok: true, value: !!el.checked };
  }

  if (type === 'json') {
    const raw = el.value?.trim();
    if (!raw) return { ok: false, error: 'JSON invoer is leeg.' };

    try {
      return { ok: true, value: JSON.parse(raw) };
    } catch {
      return { ok: false, error: 'Ongeldige JSON invoer.' };
    }
  }

  const number = Number(el.value);
  if (Number.isNaN(number)) return { ok: false, error: 'Voer een geldig nummer in.' };
  return { ok: true, value: number };
}

function bindStopButton() {
  const button = document.getElementById('StopButton');
  if (!button) return;

  button.addEventListener('click', async () => {
    const isCurrentlyStop = button.textContent === 'STOP';

    try {
      if (isCurrentlyStop) {
        await setStopMode(true);
        button.textContent = 'START';
        button.classList.add('running');
      } else {
        await setStopMode(false);
        button.textContent = 'STOP';
        button.classList.remove('running');
      }
    } catch (error) {
      console.error('Stop mode update failed:', error.message);
    }
  });
}

function bindUnlock() {
  const unlockBtn = document.getElementById('unlockBtn');
  const unlockInput = document.getElementById('unlockInput');
  if (!unlockBtn || !unlockInput) return;

  unlockBtn.addEventListener('click', async (event) => {
    event.preventDefault();
    const code = unlockInput.value.trim();
    unlockInput.value = '';

    if (!code) return;

    try {
      const responseText = await unlockSettings(code);
      unlockInput.value = responseText;
      refreshFallbackData();
    } catch (error) {
      unlockInput.value = 'Unlock failed';
      console.error('Unlock failed:', error.message);
    }
  });
}

function bindGoToButtons() {
  const draaienBtn = document.getElementById('Draaien-Positie_Btn');
  const kantelenBtn = document.getElementById('Kantelen-Positie_Btn');
  const draaienInput = document.getElementById('Draaien-Positie_Input');
  const kantelenInput = document.getElementById('Kantelen-Positie_Input');

  draaienBtn?.addEventListener('click', async (event) => {
    event.preventDefault();
    const input = document.getElementById('Draaien-Positie_Input').value;
    const value = validatePositionInput(input);
    if (value === null) return;
    await goToPosition('TURN_Position', value);
  });

  kantelenBtn?.addEventListener('click', async (event) => {
    event.preventDefault();
    const input = document.getElementById('Kantelen-Positie_Input').value;
    const value = validatePositionInput(input);
    if (value === null) return;
    await goToPosition('TILT_Position', value);
  });

  draaienInput?.addEventListener('keydown', (event) => {
    if (event.key !== 'Enter') return;
    event.preventDefault();
    draaienBtn?.click();
  });

  kantelenInput?.addEventListener('keydown', (event) => {
    if (event.key !== 'Enter') return;
    event.preventDefault();
    kantelenBtn?.click();
  });

  const presetRows = document.querySelectorAll('.preset-row');
  presetRows.forEach((row) => {
    const targetInputId = row.getAttribute('data-target-input');
    const targetBtnId = row.getAttribute('data-target-btn');

    row.querySelectorAll('[data-preset]').forEach((presetButton) => {
      presetButton.addEventListener('click', () => {
        const presetValue = presetButton.getAttribute('data-preset');
        const targetInput = document.getElementById(targetInputId);
        const targetBtn = document.getElementById(targetBtnId);
        if (!targetInput || !targetBtn) return;

        targetInput.value = presetValue;
        targetBtn.click();
      });
    });
  });
}

function bindForceButtons() {
  const forceButtons = document.querySelectorAll('[data-force-id]');

  forceButtons.forEach((button) => {
    const forceId = button.getAttribute('data-force-id');

    button.addEventListener('mousedown', async () => {
      appState.forceHold[forceId] = true;
      button.classList.add('hold');
      try {
        await setForceMovement(forceId, true);
      } catch (error) {
        console.error(`Force activate failed for ${forceId}:`, error.message);
      }
    });

    button.addEventListener('touchstart', async (event) => {
      event.preventDefault();
      appState.forceHold[forceId] = true;
      button.classList.add('hold');
      try {
        await setForceMovement(forceId, true);
      } catch (error) {
        console.error(`Force activate failed for ${forceId}:`, error.message);
      }
    }, { passive: false });
  });

  const releaseAllForces = async () => {
    const activeIds = Object.keys(appState.forceHold).filter((id) => appState.forceHold[id]);
    if (!activeIds.length) return;

    activeIds.forEach((id) => {
      appState.forceHold[id] = false;
      const buttons = document.querySelectorAll(`[data-force-id="${id}"]`);
      buttons.forEach((button) => button.classList.remove('hold'));
    });

    await Promise.all(activeIds.map((id) => setForceMovement(id, false).catch(() => null)));
  };

  document.addEventListener('mouseup', releaseAllForces);
  document.addEventListener('touchend', releaseAllForces);
  document.addEventListener('mouseleave', releaseAllForces);
}

function bindSettingsButtons() {
  const buttons = document.querySelectorAll('button[data-setting-key]');

  buttons.forEach((button) => {
    button.addEventListener('click', async (event) => {
      event.preventDefault();

      const key = button.getAttribute('data-setting-key');
      const customInputId = button.getAttribute('data-input-id');
      const inputId = customInputId || key;
      const valueType = button.getAttribute('data-setting-type') || 'number';
      const parsed = readSettingValue(inputId, valueType);

      if (!parsed.ok) {
        alert(parsed.error);
        return;
      }

      try {
        await saveSetting(key, parsed.value);
        button.classList.remove('save-error');
        button.classList.add('save-success');
        button.textContent = 'Opgeslagen';
        setTimeout(() => {
          button.classList.remove('save-success');
          button.textContent = 'Opslaan';
        }, 1200);
      } catch (error) {
        button.classList.remove('save-success');
        button.classList.add('save-error');
        button.textContent = 'Mislukt';
        setTimeout(() => {
          button.classList.remove('save-error');
          button.textContent = 'Opslaan';
        }, 1200);
        console.error(`Saving ${key} failed:`, error.message);
      }
    });
  });
}

function bindSettingToggles() {
  const toggles = document.querySelectorAll('input[type="checkbox"][data-setting-key][data-setting-type="boolean"]');

  toggles.forEach((toggle) => {
    toggle.addEventListener('change', async () => {
      const key = toggle.getAttribute('data-setting-key');
      const previous = !toggle.checked;

      try {
        await saveSetting(key, !!toggle.checked);
      } catch (error) {
        toggle.checked = previous;
        console.error(`Saving ${key} failed:`, error.message);
      }
    });
  });
}

function bindAlarmActions() {
  const resetBtn = document.getElementById('resetAlarmsBtn');
  const alarmLog = document.getElementById('alarmLog');
  if (!resetBtn || !alarmLog) return;

  const appendLog = (message) => {
    const now = new Date().toLocaleTimeString('nl-BE', {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
    });
    const line = `[${now}] ${message}`;
    alarmLog.textContent = `${alarmLog.textContent}\n${line}`.trim();
    alarmLog.scrollTop = alarmLog.scrollHeight;
  };

  resetBtn.addEventListener('click', async () => {
    resetBtn.textContent = 'Bezig...';
    resetBtn.disabled = true;

    try {
      await resetAlarms();
      appendLog('Alarms reset uitgevoerd.');
      refreshFallbackData();
    } catch (error) {
      appendLog('Reset mislukt (mogelijk locked).');
      console.error('Reset alarms failed:', error.message);
    } finally {
      resetBtn.textContent = 'Reset alarms';
      resetBtn.disabled = false;
    }
  });
}

async function bootstrap() {
  initTimerUi();
  bindStopButton();
  bindUnlock();
  bindGoToButtons();
  bindForceButtons();
  bindSettingsButtons();
  bindSettingToggles();
  bindAlarmActions();

  try {
    await fetchInitialData();
  } catch (error) {
    console.warn('Initial API preload failed:', error.message);
  }

  await registerClient();
  setInterval(registerClient, 25000);
  setInterval(updateDataFreshnessIndicator, 1000);

  const mqttClient = await startMqtt();
  if (!mqttClient) {
    setInterval(refreshFallbackData, 1500);
  }
}

window.addEventListener('load', bootstrap);
