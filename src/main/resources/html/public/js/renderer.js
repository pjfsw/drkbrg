const canvas = document.getElementById('gameBoard');
const ctx = canvas.getContext('2d');
const TILE_SIZE = 50;

export function drawTile(x, y, tileType, rotation = 0) {
    // In a real version, you'd draw an Image here.
    // For now, let's draw a labeled square.
    const screenX = x * TILE_SIZE;
    const screenY = y * TILE_SIZE;

    ctx.save();
    ctx.translate(screenX + TILE_SIZE / 2, screenY + TILE_SIZE / 2);
    ctx.rotate((rotation * Math.PI) / 180); // Rotate based on Drakborgen rules

    // Draw Tile Background
    ctx.fillStyle = "#3e2723";
    ctx.fillRect(-TILE_SIZE / 2, -TILE_SIZE / 2, TILE_SIZE - 2, TILE_SIZE - 2);

    // Draw Tile Text/Type
    ctx.fillStyle = "#ffffff";
    ctx.font = "10px Arial";
    ctx.fillText(tileType, -15, 5);

    ctx.restore();
}

export function updatePlayerStats(stats) {
    document.getElementById('hp').textContent = `KP: ${stats.kp}`;
    document.getElementById('strength').textContent = `SF: ${stats.sf}`;
}

export function clearBoard() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
}
