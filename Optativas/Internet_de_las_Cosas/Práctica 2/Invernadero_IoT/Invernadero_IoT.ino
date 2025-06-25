#include <WiFi.h>
#include <PubSubClient.h>
#include <InfluxDbClient.h>

#define LED_VENTANA 2  // LED integrado

// datos WiFi 
const char* ssid = "WiFi 2.4GHz";
const char* password = "01234";

// — broker MQTT —
const char* mqttServer = "192...";
const int mqttPort = 1883;
const char* mqttTopicTemp = "invernadero/temperatura";
const char* mqttTopicControl = "invernadero/ventana/control";  // topic para recibir comandos
const char* mqttTopicEstado = "invernadero/ventana/estado";    // topic para publicar estado

// Config InfluxDB 1.8
const char* INFLUXDB_HOST = "192...";  // Misma IP que MQTT
const int INFLUXDB_PORT = 8086;
const char* INFLUXDB_DB = "practica2";

// Intervalos de tiempo
const int intervaloTemperatura = 15000;    // Cada 15 segundos
const unsigned long duracionLED = 10000;             // LED encendido por 10 segundos
const unsigned long intervaloEstado = 180000; // Cada 3 minutos

// Clientes
WiFiClient espClient;
PubSubClient client(espClient);
InfluxDBClient influxClient(INFLUXDB_HOST, INFLUXDB_DB);

// Variables de estado
bool ventanaAbierta = false;
bool ledEncendido = false;

float temperatura;
unsigned long tiempoUltimaTemperatura = 0;
unsigned long tiempoInicioLED = 0;
unsigned long tiempoUltimoInformeEstado = 0;

// Control de parpadeo
unsigned long tiempoParpadeo = 0;
bool estadoLED = false;
int parpadeosRestantes = 0;


// ================== FUNCIONES ================== //

void enviarDatosInfluxDB() {
  Point sensorPoint("clima_invernadero");  // Nombre de la medición
  
  // Fields
  sensorPoint.addField("temperatura", temperatura);
  sensorPoint.addField("ventana_abierta", ventanaAbierta ? 1.0 : 0.0);

  if (!influxClient.writePoint(sensorPoint)) {
    Serial.print("Error InfluxDB: ");
    Serial.println(influxClient.getLastErrorMessage());
  } else {
    Serial.println("Datos enviados a InfluxDB (v1)");
  }
}


void conectarInfluxDB(){
  String influxUrl = "http://" + String(INFLUXDB_HOST) + ":" + String(INFLUXDB_PORT);
  influxClient.setConnectionParamsV1(influxUrl, INFLUXDB_DB);

  // Verificar conexión
  if (influxClient.validateConnection()) {
    Serial.print("Conectado a InfluxDB: ");
    Serial.print(influxClient.getServerUrl());
    Serial.println(" 💾");
    
  } else {
    Serial.print("Error conexión InfluxDB: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
}


void callbackMQTT(char* topic, byte* payload, unsigned int length) {
  // Convertir payload a String
  String mensaje;
  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }
  
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensaje);

  if (String(topic) == String(mqttTopicControl)) {
    int payloadIndex = mensaje.indexOf("\"payload\":\"");
    if (payloadIndex >= 0) {
      int start = payloadIndex + 11; // Posición después de "payload":"
      int end = mensaje.indexOf("\"", start);
      
      if (end > start) {
        String comando = mensaje.substring(start, end);
        Serial.print("Comando extraído: ");
        Serial.println(comando);
        
        if (comando == "abrir") {
          abrirVentana();
        } 
        else if (comando == "cerrar") {
          cerrarVentana();
        }
        return;
      }
    }
    Serial.println("Error: Formato JSON inválido");
  }
}


void conectarMQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callbackMQTT);
  
  while (!client.connected()) {
    if (client.connect("ESP32-Invernadero")) {
      Serial.println("MQTT conectado 🌐");
      client.subscribe(mqttTopicControl); 
      Serial.print("Suscripción a: ");
      Serial.print(mqttTopicControl);
      Serial.println(" 🦻");
    } else {
      Serial.print("Error MQTT, rc=");
      Serial.print(client.state());
      Serial.println(" - Reintentando en 2s...");
      delay(2000);
    }
  }
}


void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("¡WiFi conectado! 🛜");
}


void parpadearLed(int pin, int veces, int duracionMillis) {
  pinMode(pin, OUTPUT);
  for (int i = 0; i < veces; i++) {
    digitalWrite(pin, HIGH);
    delay(duracionMillis);
    digitalWrite(pin, LOW);
    delay(duracionMillis);
  }
}


void abrirVentana() {         
  digitalWrite(LED_VENTANA, HIGH);
  ventanaAbierta = true;
  ledEncendido = true;
  tiempoInicioLED = millis();
  Serial.println(tiempoInicioLED);
  Serial.println("ABRIENDO ventana por comando");
  publicarEstadoVentana();
}


void cerrarVentana() {       
  digitalWrite(LED_VENTANA, HIGH);
  ledEncendido = true;
  ventanaAbierta = false;
  tiempoInicioLED = millis();
  Serial.println("CERRANDO ventana por comando");
  publicarEstadoVentana();
}


void informarStatus() {
  tiempoUltimoInformeEstado = millis();
  String estado = ventanaAbierta ? "Abierta" : "Cerrada";
  
  Serial.println("\n===== ESTADO ACTUAL =====");
  Serial.print("Ventana: ");
  Serial.println(estado);
  Serial.print("Temperatura: ");
  Serial.print(temperatura, 2);
  Serial.println(" °C");
  Serial.println("=========================\n");
}


void publicarTemperatura() {
  char payload[16];
  dtostrf(temperatura, 4, 2, payload);
  
  if (client.publish(mqttTopicTemp, payload)) {
    Serial.print("[MQTT] Publicado: ");
    Serial.println(payload);
  } else {
    Serial.println("[MQTT] Error en publicación!");
  }
  enviarDatosInfluxDB(); 
}


void publicarEstadoVentana() {
  const char* estado = ventanaAbierta ? "abierta" : "cerrada";
  
  if (client.publish(mqttTopicEstado, estado)) {
    Serial.print("[MQTT] Estado ventana: ");
    Serial.println(estado);
  } else {
    Serial.println("[MQTT] Error publicando estado!");
  }
  enviarDatosInfluxDB(); 
}


// ================== SETUP & LOOP ================== //

void setup() {
  pinMode(LED_VENTANA, OUTPUT);
  digitalWrite(LED_VENTANA, LOW);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("INICIANDO SISTEMA INVERNADERO (Modo Remoto)");
  
  conectarWiFi();
  conectarMQTT();
  conectarInfluxDB();
  
  parpadearLed(LED_VENTANA, 3, 150);  // 3 veces, 150 ms prendido y 150 ms apagado


  randomSeed(analogRead(0));
  
  Serial.println("Sistema listo - Esperando comandos MQTT ✅");
}


void loop() {
  unsigned long tiempoActual = millis();

  

  // Publicar temperatura
  if (tiempoActual - tiempoUltimaTemperatura >= intervaloTemperatura) {
    tiempoUltimaTemperatura = tiempoActual;
    
    // Simular temperatura (15.00°C a 35.00°C)
    temperatura = random(1500, 3501) / 100.0;
    
    Serial.print("Temperatura: ");
    Serial.print(temperatura, 2);
    Serial.println(" °C");
    
    publicarTemperatura();
  }

  // Informar estado
  if (tiempoActual - tiempoUltimoInformeEstado >= intervaloEstado) {
    informarStatus();
    publicarEstadoVentana();
  }

  // Apagar LED después de tiempo de operación
  if (ledEncendido && (tiempoActual - tiempoInicioLED >= duracionLED)) {
    ledEncendido = false;
    digitalWrite(LED_VENTANA, LOW);
    Serial.println("LED apagado (fin operación ventana)");
  }

  // Mantener conexión MQTT
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

}