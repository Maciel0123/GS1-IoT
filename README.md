🚀 Projeto IoT com ESP32 – Monitoramento de Motocicletas via MQTT
Este projeto tem como objetivo a implementação de um sistema de monitoramento veicular utilizando uma placa ESP32, com conectividade Wi-Fi e comunicação via MQTT, enviando dados estruturados em JSON para um broker. A solução simula a telemetria de uma motocicleta, capturando e transmitindo dados de identificação e rede.

📡 Visão Geral da Solução
A placa ESP32 representa uma motocicleta conectada que:

Conecta-se automaticamente à rede Wi-Fi.

Estabelece conexão com um broker MQTT (Mosquitto).

Envia periodicamente um pacote JSON contendo:

Identificador único do dispositivo;

Modelo e placa da moto;

Endereço IP local.

Pisca o LED onboard como feedback visual de envio de mensagem.

⚙️ Tecnologias e Componentes Utilizados
Tecnologia	Descrição
ESP32	Microcontrolador com suporte Wi-Fi integrado
Wi-Fi	Comunicação com a internet via rede local
MQTT	Protocolo leve de mensagens pub/sub (Mosquitto)
ArduinoJson	Biblioteca para manipulação de objetos JSON
PubSubClient	Cliente MQTT para ESP32/Arduino

📦 Dependências
ArduinoJson

PubSubClient

Instaláveis via Gerenciador de Bibliotecas da IDE Arduino.

🧱 Estrutura do Código
Identificadores da Moto
```
const char* ID     = "01";
const char* modelo = "Honda 160";
const char* placa  = "ASD1234";
```
Conectividade
Wi-Fi:

```
const char* SSID = "Wokwi-GUEST";
const char* PASSWORD = "";
MQTT Broker:
```
```
const char* BROKER_MQTT = "191.232.37.240"; // IP da VM
const int BROKER_PORT = 1883;
const char* mqttUser = "iot"; //seu usario e senha mosquito broker
const char* mqttPassword = "Fiap@2tdsvms";
#define TOPICO_PUBLISH  "Teste/Henrique"
```
Fluxo de Execução
Inicializa Wi-Fi e conecta à rede.

Conecta-se ao broker MQTT com autenticação.

Envia a cada 10 segundos uma mensagem JSON no tópico definido.

Pisca o LED onboard como indicador visual.

{
  "ID": "01",
  "Modelo": "Honda 160",
  "Placa": "ASD1234",
  "IP": "192.168.x.x"
}

📈 Monitoramento
As mensagens publicadas podem ser visualizadas em um cliente MQTT (como MQTT Explorer ou Node-RED), conectando-se ao broker e se inscrevendo no tópico Teste/Henrique.

🔄 Loop Principal
Verifica constantemente a conectividade com Wi-Fi e o broker MQTT.

Publica dados de identificação e rede no formato JSON.

Mantém o envio periódico a cada 10 segundos.

💡 Observações
O LED onboard do ESP32 é usado para indicar o envio bem-sucedido da mensagem.

A simulação foi testada em ambiente virtual Wokwi e VM na Azure.

Ideal para projetos de telemetria, rastreamento ou gestão de frotas conectadas.

Integrantes:
  - Henrique Maciel RM556480
  - Gabriela Moguinho Gonçalves RM556143
  - Mariana Christina Rodrigues Fernandes RM554773
