// Inclusão das bibliotecas
#include <WiFi.h>
#include <AsyncWebSocket.h>

// Instanciação dos objetos da classe AsyncWebServer e AsyncWebSocket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws"); 
const int sensorPin = 34; // Exemplo de pino do sensor analógico
const char* ssid = "";
const char* password = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("WiFi connected");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Configurar a rota para o cliente estabelecer a conexão WebSocket
  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  
  // Disponibiliza o url "/" com o conteúdo da String abaixo
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //mostrar no html os valores...
    request->send(200, "text/html", "<html><body><script>var socket = new WebSocket('ws://' + window.location.host + '/ws');socket.onmessage = function(event) { document.getElementById('current').innerHTML = event.data; };</script>Current value: <span id='current'>...</span></body></html>");
  });
  // Servidor começa a ouvir os clientes
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  float current = analogRead(sensorPin); 
  String currentString = String(current);
  ws.textAll(currentString); // Enviar o valor da corrente a todos os clientes conectados via WebSocket
  //verifica se foi enviado o valor para o cliente ou nao
  Serial.print("passouaq");
  delay(2000); // Intervalo de envio (1 segundo). Você pode ajustar este valor conforme necessário.
  Serial.println(currentString);
  //verifica se tem cliente conectado 
    if (ws.count() > 0) {
        Serial.println("tem cliente conectado");
    }
    else {
        Serial.println("nao tem cliente conectado");
    }
}
 
// Função de tratamento de eventos do WebSocket
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  // Tratar eventos do WebSocket, se necessário (pode ser deixado em branco para este exemplo)
}
