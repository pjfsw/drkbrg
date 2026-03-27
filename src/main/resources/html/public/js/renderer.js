export function updatePlayerStats(stats) {
    document.getElementById('hp').textContent = `KP: ${stats.kp}`;
    document.getElementById('strength').textContent = `SF: ${stats.sf}`;
}

export function drawTile(x, y, type) {
    console.log(`Rendering tile ${type} at ${x},${y}`);
    // Logic to add a div or draw on Canvas goes here
}