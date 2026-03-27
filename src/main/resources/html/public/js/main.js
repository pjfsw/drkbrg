import { getWsConfig, createSocket } from './connection.js';
import { updatePlayerStats, drawTile } from './renderer.js';

async function start() {
    const config = await getWsConfig();

    const socket = createSocket(`ws://${window.location.hostname}:${config.wsPort}`, (data) => {
        // When server sends data, decide what to do
        if (data.type === "STATS_UPDATE") {
            updatePlayerStats(data.payload);
        } else if (data.type === "NEW_TILE") {
            drawTile(data.x, data.y, data.tileType);
        }
    });


    socket.onopen = () => {
        const statusDiv = document.getElementById('status');
        statusDiv.textContent = 'Status: Connected';
        statusDiv.className = 'connected';

        // Optional: Log to the UI if you kept the #log div
        const logDiv = document.getElementById('log');
        if (logDiv) {
            const msg = document.createElement('div');
            msg.textContent = "[System]: Connected to Drakborgen Server";
            logDiv.appendChild(msg);
        }
    };


    // Handle button clicks
    document.getElementById('moveBtn').onclick = () => {
        socket.send(JSON.stringify({ type: "MOVE", dir: "NORTH" }));
    };
}

// At the bottom of main.js
window.addEventListener('DOMContentLoaded', () => {
    start();
});
