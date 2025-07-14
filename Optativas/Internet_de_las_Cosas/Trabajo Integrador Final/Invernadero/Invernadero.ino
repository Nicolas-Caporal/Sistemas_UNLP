#include <WiFi.h>
#include <PubSubClient.h>

#define LED_RIEGO 2  // LED integrado

// --- WiFi y MQTT ---
const char* ssid = "*WiFi 2.4GHz";
const char* password = "01234";
const char* mqttServer = "192.";
const int   mqttPort   = 1883;
const char* topicHum   = "invernadero/humedad";
const char* topicNivel = "invernadero/nivel";

WiFiClient espClient;
PubSubClient client(espClient);

// --- Parámetros de simulación ---
const unsigned long simInterval = 5000;  // cada 5s
unsigned long lastSim = 0;

const float umbralHumedad = 30.0;  // %
const float humBase       = 50.0;  // valor medio de la senoide
const float humAmp        = 30.0;  // amplitud
const unsigned long periodoHum = 90000; // 1 min para un ciclo completo

float humedad    = humBase;
float nivelTanque = 100.0;  // %

bool esperandoRecarga = false;
unsigned long tiempoVacio = 0;
const unsigned long demoraRecarga = 30000;  // 2 minutos


// --- Funciones de simulación ---
float simularHumedad(unsigned long t){
  // seno entre humBase±humAmp
  float angle = 2.0 * PI * (t % periodoHum) / (float)periodoHum;
  return humBase + humAmp * sin(angle);
}

void bajarNivelTanque(float consumo){
  nivelTanque -= consumo;
  if(nivelTanque <= 0){
    nivelTanque = 0;
    Serial.println("!! Tanque vacío !!");
  }
}

void llenarTanque(){
  nivelTanque = 100.0;
  Serial.println("Tanque recargado al 100%");
}

// --- Publicar por MQTT ---
void publicar(const char* topic, float valor){
  char buf[16];
  dtostrf(valor, 5, 2, buf);
  client.publish(topic, buf);
  Serial.printf("MQTT ► %s = %s\n", topic, buf);
}

// --- Setup y Loop ---
void setup(){
  Serial.begin(115200);
  delay(500);
  pinMode(LED_RIEGO, OUTPUT);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK");

  // Conexión MQTT
  client.setServer(mqttServer, mqttPort);
  while (!client.connected()) {
    Serial.print("Conectando MQTT a ");
Serial.print(mqttServer);
Serial.print(":");
Serial.println(mqttPort);

    if (client.connect("ESP32-simulador")) {
      Serial.println("MQTT OK");
    } else {
      Serial.print("MQTT Err: ");
      Serial.print(client.state());
      Serial.println(" – reintento en 2s");
      delay(2000);
    }
  }
}


void loop(){
  unsigned long now = millis();

  // reconexión MQTT si hace falta
  if(!client.connected()) client.connect("ESP32-simulador");
  client.loop();

  // simulación periódica
  if(now - lastSim >= simInterval){
    lastSim = now;

    // 1) calculo humedad
    humedad = simularHumedad(now);
    Serial.printf("Humedad: %.2f %%\n", humedad);

    // 2) si bajo umbral, riego y bajo nivel
    if(humedad < umbralHumedad){
      digitalWrite(LED_RIEGO, HIGH);
      float consumo = 10.0;        // p.ej. 5% cada riego
      bajarNivelTanque(consumo);
    } else {
      digitalWrite(LED_RIEGO, LOW);
    }

    // 3) gestionar recarga
  if (nivelTanque <= 0 && !esperandoRecarga) {
    esperandoRecarga = true;
    tiempoVacio = now;
    Serial.println("⚠️ Tanque vacío. Esperando recarga...");
  }

  if (esperandoRecarga && now - tiempoVacio >= demoraRecarga) {
    llenarTanque();
    esperandoRecarga = false;
  }


    Serial.printf("Nivel tanque: %.2f %%\n\n", nivelTanque);

    // 4) enviar por MQTT
    publicar(topicHum, humedad);
    publicar(topicNivel, nivelTanque);
  }
}
