package drkbrg;

import com.google.gson.Gson;
import drkbrg.response.DataType;
import drkbrg.response.GameMessage;
import drkbrg.response.NewTile;
import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

import java.net.InetSocketAddress;

public class PlayerServer extends WebSocketServer {
    private static final Gson GSON = new Gson();
    public PlayerServer(InetSocketAddress address) {
        super(address);
    }

    @Override
    public void onOpen(WebSocket conn, ClientHandshake handshake) {
        System.out.println("New player connected: " + conn.getRemoteSocketAddress());
        int width = 8;
        int height = 8;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                send(conn, new GameMessage(DataType.NEW_TILE, new NewTile(x,y,0)));
            }
        }
        // conn.send("{\"type\": \"WELCOME\", \"msg\": \"Connected to Drakborgen\"}");
    }

    @Override
    public void onClose(WebSocket conn, int code, String reason, boolean remote) {
        System.out.println("Player disconnected: " + conn.getRemoteSocketAddress());
    }

    @Override
    public void onMessage(WebSocket conn, String message) {
        System.out.println("Received action: " + message);
        // Parse JSON, update game state, broadcast changes
        // broadcast("{\"type\": \"UPDATE\", \"data\": ...}");
    }

    @Override
    public void onStart() {
        System.out.println("Drkbrg Server started successfully!");
    }

    @Override
    public void onError(WebSocket conn, Exception ex) {
        ex.printStackTrace();
    }

    private void send(WebSocket conn, GameMessage message) {
        String json = GSON.toJson(message);
        conn.send(json);
    }
}
