#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>


#define boardLED 2 


#define LED_VERDE 13
#define LED_AMARELO 12
#define LED_VERMELHO 14

// Wi-Fi
const char* SSID      = "Wokwi-GUEST";
const char* PASSWORD  = "";

// MQTT Broker
const char* BROKER_MQTT  = "4.201.157.200"; // IP da VM
const int   BROKER_PORT  = 1883;
const char* mqttUser     = "iot"; // Usuário MQTT
const char* mqttPassword = "Fiap@2tdsvms"; // Senha MQTT

// Tópico MQTT
#define TOPICO_PUBLISH  "Teste/Henrique"

// Variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
char buffer[512]; // Buffer maior para o JSON serializado

// Variáveis para armazenar dados climáticos
float temperatura = 0.0;
String descricao = "";

// Chave da API OpenWeather
const String apiKey = "Chave API";

// Coordenadas de Latitude e Longitude
const float lat = -23.5505;  // Exemplo: Latitude de São Paulo
const float lon = -46.6333;  // Exemplo: Longitude de São Paulo

//----------------------------------------------------------

// Conexão Wi-Fi
void initWiFi() {
    Serial.print("Conectando ao Wi-Fi");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}

void reconectaWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Reconectando Wi-Fi...");
        initWiFi();
    }
}

//----------------------------------------------------------

// Conexão MQTT
void initMQTT() {
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    while (!MQTT.connected()) {
        Serial.println("Conectando ao Broker MQTT...");
        if (MQTT.connect("ESP32Client", mqttUser, mqttPassword)) {
            Serial.println("Conectado ao Broker!");
        } else {
            Serial.print("Falha na conexão. Estado: ");
            Serial.println(MQTT.state());  // Mostrar o código do erro
            delay(2000);
        }
    }
}

void verificaConexoesWiFiEMQTT() {
    reconectaWiFi();
    if (!MQTT.connected()) {
        initMQTT();
    }
    MQTT.loop();
}

//----------------------------------------------------------

// Coleta dos dados climáticos da API OpenWeather
void obterDadosClimaticos() {
    HTTPClient http;
    String url = "https://api.openweathermap.org/data/2.5/weather?lat=" + String(lat) + "&lon=" + String(lon) + "&appid=" + apiKey + "&units=metric&lang=pt_br";
    http.begin(url);
    int httpCode = http.GET();
    
    if (httpCode > 0) {
        String payload = http.getString();
        JsonDocument doc; 
        deserializeJson(doc, payload);
        
        // Dados climáticos
        temperatura = doc["main"]["temp"];
        float vento = doc["wind"]["speed"];
        float umidade = doc["main"]["humidity"];
        float pressao = doc["main"]["pressure"];
        descricao = doc["weather"][0]["description"].as<String>();  // Converte corretamente para String
        
        // Monta o JSON com os dados climáticos
        JsonDocument dataDoc;  
        dataDoc["Temperatura"] = temperatura;
        dataDoc["Vento"] = vento;
        dataDoc["Umidade"] = umidade;
        dataDoc["Pressao"] = pressao;
        dataDoc["Descricao"] = descricao;
        dataDoc["IP"] = WiFi.localIP();

        // Serializa JSON para string
        serializeJson(dataDoc, buffer);

        // Exibe no monitor serial
        Serial.println("Dados Climáticos:");
        Serial.println(buffer);

        // Envia via MQTT
        Serial.println("Publicando dados no MQTT...");
        MQTT.publish(TOPICO_PUBLISH, buffer);
        Serial.println("Dados publicados com sucesso!");
    } else {
        Serial.println("Erro na requisição HTTP");
    }
    http.end();
}

//----------------------------------------------------------

// Envio de dados via MQTT
void enviaEstadoOutputMQTT() {
    MQTT.publish(TOPICO_PUBLISH, buffer);
    Serial.println("Mensagem publicada com sucesso!");
}

//----------------------------------------------------------

// Controle de LEDs (acende o LED conforme a situação)
void acendeLED(int ledPin) {
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
}

//----------------------------------------------------------

// Função de piscar o LED
void piscaLed() {
    digitalWrite(boardLED, LOW);
    delay(100);
    digitalWrite(boardLED, HIGH);
}

//----------------------------------------------------------

// Setup inicial
void setup() {
    Serial.begin(115200);
    pinMode(boardLED, OUTPUT);
    pinMode(LED_VERDE, OUTPUT);
    pinMode(LED_AMARELO, OUTPUT);
    pinMode(LED_VERMELHO, OUTPUT);

    digitalWrite(boardLED, LOW);
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    
    initWiFi();
    initMQTT();
}

//----------------------------------------------------------

// Loop principal
void loop() {
    // Verifica e mantém conexões ativas com Wi-Fi e MQTT
    verificaConexoesWiFiEMQTT();

    // Coleta dados climáticos da API e envia via MQTT
    obterDadosClimaticos();

    // Controle dos LEDs com base nos dados climáticos
    if (temperatura > 30) {  // Exemplo de temperatura elevada
        acendeLED(LED_AMARELO);  // Acende LED amarelo se a temperatura estiver alta
    } else if (descricao.indexOf("rain") >= 0) {  // Se estiver chovendo
        acendeLED(LED_VERMELHO);  // Acende LED vermelho se houver risco (chovendo)
    } else {
        acendeLED(LED_VERDE);  // Caso contrário, acende o LED verde
    }

    // Feedback visual
    piscaLed();

    // Intervalo de envio
    delay(10000); // Envia os dados a cada 10 segundos
}
