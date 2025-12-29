#include <Arduino.h>

// Pin del LED integrado del ESP32 (GPIO 2)
#define LED_PIN 2

// Variables globales
unsigned long startTime = 0;
bool ledState = false;

// Función para formatear el tiempo de actividad
String formatUptime(unsigned long milliseconds) {
    unsigned long seconds = milliseconds / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;
    
    seconds = seconds % 60;
    minutes = minutes % 60;
    hours = hours % 24;
    
    String uptime = "";
    if (days > 0) {
        uptime += String(days) + "d ";
    }
    if (hours > 0 || days > 0) {
        uptime += String(hours) + "h ";
    }
    if (minutes > 0 || hours > 0 || days > 0) {
        uptime += String(minutes) + "m ";
    }
    uptime += String(seconds) + "s";
    
    return uptime;
}

// Función para procesar comandos recibidos por serial
void processCommand(String command) {
    command.trim();
    command.toUpperCase();
    
    if (command == "LED_ON") {
        digitalWrite(LED_PIN, HIGH);
        ledState = true;
        Serial.println("OK: LED encendido");
    }
    else if (command == "LED_OFF") {
        digitalWrite(LED_PIN, LOW);
        ledState = false;
        Serial.println("OK: LED apagado");
    }
    else if (command == "GET_UPTIME") {
        unsigned long uptime = millis() - startTime;
        Serial.print("UPTIME: ");
        Serial.println(formatUptime(uptime));
    }
    else if (command == "STATUS") {
        Serial.print("LED: ");
        Serial.println(ledState ? "ON" : "OFF");
        unsigned long uptime = millis() - startTime;
        Serial.print("UPTIME: ");
        Serial.println(formatUptime(uptime));
    }
    else if (command.length() > 0) {
        Serial.print("ERROR: Comando desconocido: ");
        Serial.println(command);
    }
}

void setup() {
    // Inicializar comunicación serial
    Serial.begin(115200);
    delay(1000);
    
    // Configurar pin del LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledState = false;
    
    // Guardar tiempo de inicio
    startTime = millis();
    
    // Mensaje de inicio
    Serial.println("\n=== ESP32 LED Controller ===");
    Serial.println("Comandos disponibles:");
    Serial.println("  LED_ON      - Encender LED");
    Serial.println("  LED_OFF     - Apagar LED");
    Serial.println("  GET_UPTIME  - Obtener tiempo de actividad");
    Serial.println("  STATUS      - Estado del sistema");
    Serial.println("============================\n");
    
    // Enviar tiempo de actividad inicial
    Serial.print("UPTIME: ");
    Serial.println(formatUptime(0));
}

void loop() {
    // Leer comandos del puerto serial
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        processCommand(command);
    }
    
    // Enviar tiempo de actividad cada 5 segundos automáticamente
    static unsigned long lastUptimeUpdate = 0;
    if (millis() - lastUptimeUpdate >= 5000) {
        unsigned long uptime = millis() - startTime;
        Serial.print("UPTIME: ");
        Serial.println(formatUptime(uptime));
        lastUptimeUpdate = millis();
    }
    
    delay(10); // Pequeño delay para evitar saturar el CPU
}
