import com.sun.net.httpserver.HttpServer;
import org.java_websocket.server.WebSocketServer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;

public class Drkbrg {
    static void createWebServer(int httpPort, int wsPort) throws IOException {
        HttpServer httpServer = HttpServer.create(new InetSocketAddress(httpPort), 0);

        // Map the root context to our resource handler
        httpServer.createContext("/", exchange -> {
            String path = exchange.getRequestURI().getPath();

            // Default to index.html if root is requested
            if (path.equals("/")) {
                path = "/index.html";
            }

            // Look into src/main/resources/html/public/
            String resourcePath = "/html/public" + path;

            try (InputStream is = Drkbrg.class.getResourceAsStream(resourcePath)) {
                if (is == null) {
                    exchange.sendResponseHeaders(404, -1);
                } else {
                    byte[] bytes = is.readAllBytes();
                    // Set Content-Type based on extension if you want to be thorough
                    exchange.sendResponseHeaders(200, bytes.length);
                    try (OutputStream os = exchange.getResponseBody()) {
                        os.write(bytes);
                    }
                }
            }
        });
        // Inside your main method where you setup the HttpServer
        httpServer.createContext("/config", exchange -> {
            // The JSON payload telling the client where to look
            String json = "{\"wsPort\": "+wsPort+"}";
            byte[] response = json.getBytes();

            exchange.getResponseHeaders().set("Content-Type", "application/json");
            exchange.sendResponseHeaders(200, response.length);

            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response);
            }
        });
        httpServer.start();
        System.out.println("HTTP Server running at http://localhost:"+httpPort);
    }

    public static void main(String[] args) {
        String host = "localhost";
        int httpPort = 8000;
        int wsPort = 8080;

        try {
            createWebServer(httpPort,wsPort);
            WebSocketServer webSocketServer = new Server(new InetSocketAddress(host, wsPort));
            webSocketServer.start();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }


    }
}

