package drkbrg;

import com.sun.net.httpserver.HttpServer;
import org.java_websocket.server.WebSocketServer;

import java.io.IOException;
import java.net.InetSocketAddress;

public class Drkbrg {
    public static void main(String[] args) {
        String host = "localhost";
        int httpPort = 8000;
        int wsPort = 8080;

        try {
            HttpServer httpServer = WebServer.create(httpPort, wsPort);
            WebSocketServer webSocketServer = new PlayerServer(new InetSocketAddress(host, wsPort));
            webSocketServer.setReuseAddr(true);
            webSocketServer.start();
            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                System.out.println("Shutting down servers...");
                try {
                    httpServer.stop(0);
                    System.out.println("HTTP Server stopped.");
                    // Java-WebSocket stop() handles the handshake and socket closure
                    webSocketServer.stop(2000);
                    System.out.println("WebSocket Server stopped.");
                } catch (Exception e) {
                    System.err.println("Error during shutdown: " + e.getMessage());
                }
            }));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}

