import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

import java.net.InetSocketAddress;

public class Server extends WebSocketServer {
    public Server(InetSocketAddress address) {
        super(address);
    }

    @Override
    public void onOpen(WebSocket conn, ClientHandshake handshake) {
        System.out.println("New player connected: " + conn.getRemoteSocketAddress());
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
        System.out.println("Drakborgen Server started successfully!");
    }

    @Override
    public void onError(WebSocket conn, Exception ex) {
        ex.printStackTrace();
    }
}
