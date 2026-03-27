export async function getWsConfig() {
    const response = await fetch('/config');
    return await response.json();
}

export function createSocket(url, onMessage) {
    const socket = new WebSocket(url);
    socket.onmessage = (event) => onMessage(JSON.parse(event.data));
    return socket;
}