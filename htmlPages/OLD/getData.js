//TIMERS
async function getTimeoutSettings() {
  const response = await fetch(`${baseUrl}/API/SETTINGSTIMEOUTS`);
  timeoutSettings = await response.json();
}

async function getTimeoutSetting(timerId) {
    while (!timeoutSettings['TimeOuts']) {
        // Wacht tot timeoutSettings is geladen
        await new Promise(resolve => setTimeout(resolve, 100));
    }

  switch (timerId) {
    case 'draaien_AP':
      return timeoutSettings['TimeOuts']['APTime'] / 1000;
    case 'kantelen_AP':
      return timeoutSettings['TimeOuts']['APTime'] / 1000;
    case 'Draaien_TO':
      return timeoutSettings['TimeOuts']['maxMoveTime'] / 1000;
    case 'Kantelen_TO':
      return timeoutSettings['TimeOuts']['maxMoveTime'] / 1000;
    case 'LogbookTime':
      return timeoutSettings['TimeOuts']['logbookTime'] / 1000;
    case 'Retry_Timer':
      return timeoutSettings['TimeOuts']['periodicalTime'] / 1000;
    case 'Settings_Unlock':
      return timeoutSettings['TimeOuts']['settingsUnlockTime'] / 1000;
    case 'goToPosition':
      return 3600; // 1 uur

    default:
      return 0;
  }
}

async function getFlags() {
  const response = await fetch(`${baseUrl}/API/FLAGS`);
  flags = await response.json();
  setFlags(flags);
}


