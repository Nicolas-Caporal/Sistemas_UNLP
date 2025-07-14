#include <WiFi.h>
#include <PubSubClient.h>

#define LED_RIEGO 2  // LED integrado

// — WiFi y MQTT —
const char* ssid       = "WiFi 2.4GHz";
const char* password   = "01234";
const char* mqttServer = "192.";
const int   mqttPort   = 1883;
const char* topicHum   = "invernadero/humedad";
const char* topicNivel = "invernadero/nivel";

WiFiClient espClient;
PubSubClient client(espClient);

// — Parámetros de simulación —
const unsigned long simInterval      = 5000;   // cada 5s
const float           umbralHumedad  = 30.0;   // %
const float           humBase        = 50.0;   // valor medio
const float           humAmp         = 30.0;   // amplitud
const unsigned long   periodoHum     = 90000;  // 90 s
const float           consumoRiego   = 10.0;   // % por riego
const unsigned long   demoraRecarga  = 30000;  // 30 s de espera

// — Estado —
float    humedad       = humBase;
float    nivelTanque   = 100.0;
bool     esperandoRecarga = false;
unsigned long tiempoVacio  = 0;
unsigned long lastSim       = 0;


// — Conexiones —
void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" OK");
}

void conectarMQTT() {
  client.setServer(mqttServer, mqttPort);
  Serial.printf("Conectando MQTT a %s:%d", mqttServer, mqttPort);
  while (!client.connected()) {
    if (client.connect("ESP32-simulador")) {
      Serial.println(" ✓");
    } else {
      Serial.print(" Err ");
      Serial.print(client.state());
      Serial.println(" – reintento 2s");
      delay(2000);
    }
  }
}


// — Simulación — 
float leerHumedadVirtual(unsigned long t) {
  // Esto se reemplazará por sensor real
  float angle = 2 * PI * (t % periodoHum) / (float)periodoHum;
  return humBase + humAmp * sin(angle);
}

void procesarRiego(float h) {
  if (h < umbralHumedad) {
    digitalWrite(LED_RIEGO, HIGH);
    nivelTanque -= consumoRiego;
    if (nivelTanque <= 0) {
      nivelTanque = 0;
      Serial.println("!! Tanque vacío !!");
    }
  } else {
    digitalWrite(LED_RIEGO, LOW);
  }
}

void procesarRecarga(unsigned long now) {
  if (nivelTanque <= 0 && !esperandoRecarga) {
    esperandoRecarga = true;
    tiempoVacio = now;
    Serial.println("⚠️ Tanque vacío. Esperando recarga...");
  }
  if (esperandoRecarga && (now - tiempoVacio >= demoraRecarga)) {
    nivelTanque = 100.0;
    esperandoRecarga = false;
    Serial.println("🔄 Tanque recargado al 100%");
  }
}


// — Publicación MQTT —
void publicarDato(const char* topic, float valor) {
  char buf[16];
  dtostrf(valor, 5, 2, buf);
  client.publish(topic, buf);
  Serial.printf("MQTT ► %s = %s\n", topic, buf);
}

void publicarHumedadYNivel() {
  publicarDato(topicHum, humedad);
  publicarDato(topicNivel, nivelTanque);
}


// — Setup & Loop —
void setup() {
  Serial.begin(115200);
  delay(500);
  pinMode(LED_RIEGO, OUTPUT);

  conectarWiFi();
  conectarMQTT();
}

void loop() {
  unsigned long now = millis();

  if (!client.connected()) conectarMQTT();
  client.loop();

  if (now - lastSim >= simInterval) {
    lastSim = now;

    humedad = leerHumedadVirtual(now);
    Serial.printf("Humedad: %.2f %%\n", humedad);

    procesarRiego(humedad);
    procesarRecarga(now);

    Serial.printf("Nivel tanque: %.2f %%\n\n", nivelTanque);

    publicarHumedadYNivel();
  }
}
