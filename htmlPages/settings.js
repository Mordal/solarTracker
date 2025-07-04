async function setStopMode(status){
   // curl -X POST -d "{\"STOP_MODE\":true"} http://192.168.0.205:90/CONTROL
    fetch(`${baseUrl}/CONTROL`, {
        method: 'POST',
        body: JSON.stringify({ STOP_MODE: status }),

    });
}