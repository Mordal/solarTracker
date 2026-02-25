// function getApiBaseCandidates() {
//   if (appState.apiBaseUrl) {
//     return [appState.apiBaseUrl, ...dashboardConfig.apiBaseCandidates.filter((base) => base !== appState.apiBaseUrl)];
//   }
//   return [...dashboardConfig.apiBaseCandidates];
// }

async function apiRequest(path, options = {}) {
  // const candidates = getApiBaseCandidates();
  // let lastError = null;

  // for (const baseUrl of candidates) {
  const baseUrl = `https://${apiHost}:91`;
    try {
      const response = await fetch(`${baseUrl}${path}`, options);
      appState.apiBaseUrl = baseUrl;
      return response;
    } catch (error) {
      lastError = error;
    }
  // }

  throw new Error(`API request failed for ${path}: ${lastError?.message || 'no reachable base URL'}`);
}

async function apiGet(path) {
  const response = await apiRequest(path);
  if (!response.ok) {
    throw new Error(`GET ${path} failed: ${response.status}`);
  }
  return response.json();
}

async function apiPost(path, body, isJson = true) {
  const response = await apiRequest(path, {
    method: 'POST',
    body: isJson ? JSON.stringify(body) : body,
  });

  if (!response.ok) {
    const text = await response.text();
    throw new Error(`POST ${path} failed: ${response.status} ${text}`);
  }
  return response;
}

async function fetchCoreDashboardData() {
  const [flags, sensors, turn, tilt, otherData, timeRem] = await Promise.all([
    apiGet('/API/FLAGS'),
    apiGet('/API/LIGHTSENSORS'),
    apiGet('/API/TURNMOVEMENT'),
    apiGet('/API/TILTMOVEMENT'),
    apiGet('/API/OTHERDATA'),
    apiGet('/API/TIMERS'),
  ]);

  return { flags, sensors, turn, tilt, otherData, timeRem };
}

async function fetchInitialData() {
  const [timeouts, coreData, settingOffsets] = await Promise.all([
    apiGet('/API/SETTINGSTIMEOUTS'),
    fetchCoreDashboardData(),
    apiGet('/API/SETTINGSOFFSETS'),
  ]);

  appState.timeoutSettings = timeouts;
  applyCoreDashboardData(coreData);
  hydrateSettingsInputs(settingOffsets);
}


function applyCoreDashboardData({ flags, sensors, turn, tilt, otherData, timeRem }) {
  setFlags(flags);
  setSensors(sensors);
  setTurnMovement(turn);
  setTiltMovement(tilt);
  setOtherData(otherData);
  setTimeRemaining(timeRem);
  markDataFresh();
}


async function refreshFallbackData() {
  try {
    const coreData = await fetchCoreDashboardData();
    applyCoreDashboardData(coreData);
  } catch (error) {
    console.warn('Fallback refresh failed:', error.message);
  }
}

async function registerClient() {
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), 2000);

  try {
    await apiRequest('/API/CLIENTCONNECTED', {
      signal: controller.signal,
    });
    clearTimeout(timeout);
  } catch (error) {
    if (error.name === 'AbortError') {
      console.warn('Register client timed out');
    } else {
      console.warn('Register client failed:', error.message);
    }
  }
}

async function setStopMode(status) {
  await apiPost('/CONTROL', { STOP_MODE: status });
}

async function setForceMovement(id, state) {
  await apiPost('/CONTROL', { [id]: state });
}

async function goToPosition(key, value) {
  await apiPost('/CONTROL', { [key]: value });
}

async function saveSetting(key, value) {
  await apiPost('/SETTINGS', { [key]: value });
}

async function unlockSettings(code) {
  const response = await apiPost('/UNLOCK', code, false);
  return response.text();
}

async function resetAlarms() {
  const response = await apiRequest('/API/RESETALARM');
  if (!response.ok) {
    const text = await response.text();
    throw new Error(`GET /API/RESETALARM failed: ${response.status} ${text}`);
  }
  return response.text();
}
