/**
 * Drakborgen Client
 * Pure JS - No Frameworks
 */

let socket = null;

async function initializeGame() {
    const statusDiv = document.getElementById('status');
    const logDiv = document.getElementById('log');

    try {
        // 1. Fetch configuration from the Java HTTP server
        const response = await fetch('/config');
        if (!response.ok) throw new Error('Could not fetch server config');

        const config = await response.json();

        // 2. Establish WebSocket connection using the dynamic port
        const wsUrl = `ws://${window.location.hostname}:${config.wsPort}`;
        socket = new WebSocket(wsUrl);

        // Connection opened
        socket.onopen = () => {
            statusDiv.textContent = 'Status: Connected to Castle';
            statusDiv.className = 'connected';
            appendLog('System', 'Connected to the game server.');
        };

        // Listen for messages
        socket.onmessage = (event) => {
            const data = JSON.parse(event.data);
            appendLog('Server', JSON.stringify(data));
        };

        // Handle closure
        socket.onclose = () => {
            statusDiv.textContent = 'Status: Disconnected';
            statusDiv.className = 'disconnected';
            appendLog('System', 'Connection lost.');
        };

        socket.onerror = (error) => {
            appendLog('Error', 'WebSocket error occurred.');
            console.error('WebSocket Error:', error);
        };

        // 3. Setup UI Controls
        setupControls();

    } catch (err) {
        statusDiv.textContent = 'Status: Config Error';
        statusDiv.className = 'disconnected';
        console.error('Initialization failed:', err);
    }
}

function setupControls() {
    const moveBtn = document.getElementById('moveBtn');

    moveBtn.onclick = () => {
        if (socket && socket.readyState === WebSocket.OPEN) {
            const directions = ['NORTH', 'SOUTH', 'EAST', 'WEST'];
            const randomDir = directions[Math.floor(Math.random() * directions.length)];

            const action = {
                type: "MOVE_REQUEST",
                payload: {
                    heroId: "riddar_rohan",
                    direction: randomDir,
                    timestamp: Date.now()
                }
            };

            socket.send(JSON.stringify(action));
        } else {
            alert('Not connected to server.');
        }
    };
}

function appendLog(source, message) {
    const logDiv = document.getElementById('log');
    const entry = document.createElement('div');
    entry.style.marginBottom = '5px';
    entry.innerHTML = `<span style="color: #888;">[${source}]</span> ${message}`;
    logDiv.appendChild(entry);
    logDiv.scrollTop = logDiv.scrollHeight;
}

// Start the client
initializeGame();