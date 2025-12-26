#include <Arduino.h>
#include "config.h"
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//====================== VARIABLES GLOBALES =======================
float temp_sensor1 = 0.0;
float hum_sensor1 = 0.0;
float temp_sensor2 = 0.0;
float hum_sensor2 = 0.0;
float avg_temp = 0.0;
float avg_hum = 0.0;
float ldr1_value = 0;
float ldr2_value = 0;
float avg_ldr = 0.0;

//====================== ESTRUCTURA DE DATOS =======================
struct MCUData{
  float temperature_sen1;
  float temperature_sen2;
  float humidity_sen1;
  float humidity_sen2;
  int ldr1_value;
  int ldr2_value;
  float fan1_speed;
  float fan2_speed;
  float peltier_voltage;
  float setpoint_temp;
};

MCUData arrayData;

// ==================== PROTOTIPOS DE FUNCIONES ====================
void USBData(void* parameter);
void task2(void* parameter);
void task3(void* parameter);
void task4(void* parameter);
void task5(void* parameter);
void task6(void* parameter);

// ==================== SENSORES ====================
DHT dhtSensor(PIN_SENSOR_DHT0, DHT22); // Pin 4 para el sensor DHT22
DHT dhtSensor1(PIN_SENSOR_DHT1, DHT22); // Pin 5 para otro sensor DHT22

// ==================== SETUP ====================
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=== Iniciando sistema ===");
  Serial.print("Inicializando DHT22 #1 en pin ");
  Serial.println(PIN_SENSOR_DHT0);
  dhtSensor.begin();
  delay(500);
  
  Serial.print("Inicializando DHT22 #2 en pin ");
  Serial.println(PIN_SENSOR_DHT1);
  dhtSensor1.begin();
  delay(500);
  
  // Verificar que los pines estén configurados correctamente
  Serial.println("=== Configuración de pines ===");
  Serial.print("DHT22 Sensor 1 - Pin: ");
  Serial.println(PIN_SENSOR_DHT0);
  Serial.print("DHT22 Sensor 2 - Pin: ");
  Serial.println(PIN_SENSOR_DHT1);
  Serial.print("LDR 1 - Pin: ");
  Serial.println(LDR_PIN_1);
  Serial.print("LDR 2 - Pin: ");
  Serial.println(LDR_PIN_2);
  
  Serial.println("=== Iniciando 6 tareas RTOS ===");
  
  // Crear 6 tareas distribuidas entre los dos núcleos

  // Tarea 1 - Core 0 - Envio y recepcion de datos seriales
  xTaskCreatePinnedToCore(
    USBData,          // Función de la tarea
    "USBData",       // Nombre de la tarea (para depuración)
    8192,           // Tamaño del stack asignado
    NULL,           // Parámetro pasado a la tarea
    1,              // Prioridad de la tarea
    NULL,           // Handle de la tarea (no se usa aquí)
    0               // Núcleo donde se ejecuta la tarea (Core 0)
  );

  // Tarea 2 - Core 0
  xTaskCreatePinnedToCore(
    task2,          // Función de la tarea
    "SensorReading",       // Nombre de la tarea (para depuración)
    2048,           // Tamaño del stack asignado
    NULL,           // Parámetro pasado a la tarea
    2,              // Prioridad de la tarea
    NULL,           // Handle de la tarea (no se usa aquí)
    0               // Núcleo donde se ejecuta la tarea (Core 0)
  );

  // Tarea 3 - Core 0
  xTaskCreatePinnedToCore(
    task3,          // Función de la tarea
    "Tarea3",       // Nombre de la tarea (para depuración)
    2048,           // Tamaño del stack asignado
    NULL,           // Parámetro pasado a la tarea
    3,              // Prioridad de la tarea
    NULL,           // Handle de la tarea (no se usa aquí)
    0               // Núcleo donde se ejecuta la tarea (Core 0)
  );

  // Tarea 4 - Core 1
  xTaskCreatePinnedToCore(
    task4,          // Función de la tarea
    "Tarea4",       // Nombre de la tarea (para depuración)
    2048,           // Tamaño del stack asignado
    NULL,           // Parámetro pasado a la tarea
    4,              // Prioridad de la tarea
    NULL,           // Handle de la tarea (no se usa aquí)
    1               // Núcleo donde se ejecuta la tarea (Core 1)
  );

  // Tarea 5 - Core 1
  xTaskCreatePinnedToCore(
    task5,          // Función de la tarea
    "Tarea5",       // Nombre de la tarea (para depuración)
    2048,           // Tamaño del stack asignado
    NULL,           // Parámetro pasado a la tarea
    5,              // Prioridad de la tarea
    NULL,           // Handle de la tarea (no se usa aquí)
    1               // Núcleo donde se ejecuta la tarea (Core 1)
  );

  // Tarea 6 - Core 1
  xTaskCreatePinnedToCore(
    task6,          // Función de la tarea
    "Tarea6",       // Nombre de la tarea (para depuración)
    2048,           // Tamaño del stack asignado
    NULL,           // Parámetro pasado a la tarea
    6,              // Prioridad de la tarea
    NULL,           // Handle de la tarea (no se usa aquí)
    1               // Núcleo donde se ejecuta la tarea (Core 1)
  );

}

void loop() {
  // El loop principal queda vacío ya que las tareas se ejecutan en paralelo
}

// ==================== TAREA 1 ====================
void USBData(void* parameter) {
  Serial.println("[USBData] Tarea iniciada en Core 0");
  
  while (true) {
    Serial.println("\n=== Datos de sensores ===");
    
    // Sensor 1
    Serial.print("Temperatura Sensor 1: ");
    if (isnan(temp_sensor1)) {
      Serial.print("NaN (ERROR)");
    } else {
      Serial.print(temp_sensor1);
      Serial.print(" °C");
    }
    Serial.println();
    
    Serial.print("Humedad Sensor 1: ");
    if (isnan(hum_sensor1)) {
      Serial.print("NaN (ERROR)");
    } else {
      Serial.print(hum_sensor1);
      Serial.print(" %");
    }
    Serial.println();
    
    // Sensor 2
    Serial.print("Temperatura Sensor 2: ");
    if (isnan(temp_sensor2)) {
      Serial.print("NaN (ERROR) - Verificar conexión en pin ");
      Serial.print(PIN_SENSOR_DHT1);
    } else {
      Serial.print(temp_sensor2);
      Serial.print(" °C");
    }
    Serial.println();
    
    Serial.print("Humedad Sensor 2: ");
    if (isnan(hum_sensor2)) {
      Serial.print("NaN (ERROR) - Verificar conexión en pin ");
      Serial.print(PIN_SENSOR_DHT1);
    } else {
      Serial.print(hum_sensor2);
      Serial.print(" %");
    }
    Serial.println();
    
    Serial.print("LDR 1: ");
    Serial.println(ldr1_value);
    Serial.print("LDR 2: ");
    Serial.println(ldr2_value);
    Serial.print("Promedio Temp: ");
    Serial.print(avg_temp);
    Serial.println(" °C");
    Serial.print("Promedio Hum: ");
    Serial.print(avg_hum);
    Serial.println(" %");
    Serial.println("========================");
    
    vTaskDelay(pdMS_TO_TICKS(5000)); // Esperar 5 segundos
  }
}

// ==================== TAREA 2 ====================
void task2(void* parameter) {
  Serial.println("[SensorReading] Tarea iniciada en Core 0");
  
  // Esperar un poco después del setup para que los sensores se estabilicen
  vTaskDelay(pdMS_TO_TICKS(2000));
  
  while (true) {
    // Leer sensor DHT22 #1
    temp_sensor1 = dhtSensor.readTemperature();
    hum_sensor1 = dhtSensor.readHumidity();
    
    // Verificar si la lectura del sensor 1 es válida
    if (isnan(temp_sensor1) || isnan(hum_sensor1)) {
      Serial.println("[SensorReading] ERROR: Sensor DHT22 #1 no responde");
      temp_sensor1 = 0.0;
      hum_sensor1 = 0.0;
    }
    
    // Delay importante entre lecturas de sensores DHT22
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // Leer sensor DHT22 #2
    temp_sensor2 = dhtSensor1.readTemperature();
    hum_sensor2 = dhtSensor1.readHumidity();
    
    // Verificar si la lectura del sensor 2 es válida
    if (isnan(temp_sensor2) || isnan(hum_sensor2)) {
      Serial.print("[SensorReading] ERROR: Sensor DHT22 #2 no responde (Pin ");
      Serial.print(PIN_SENSOR_DHT1);
      Serial.println(")");
      temp_sensor2 = 0.0;
      hum_sensor2 = 0.0;
    } else {
      Serial.print("[SensorReading] Sensor 2 OK - Temp: ");
      Serial.print(temp_sensor2);
      Serial.print("°C, Hum: ");
      Serial.print(hum_sensor2);
      Serial.println("%");
    }
    
    // Leer sensores LDR
    ldr1_value = analogRead(LDR_PIN_1);
    ldr2_value = analogRead(LDR_PIN_2);
    
    // Calcular promedios solo si ambos sensores tienen valores válidos
    if (!isnan(temp_sensor1) && !isnan(temp_sensor2) && temp_sensor1 > 0 && temp_sensor2 > 0) {
      avg_temp = (temp_sensor1 + temp_sensor2) / 2.0;
    } else if (!isnan(temp_sensor1) && temp_sensor1 > 0) {
      avg_temp = temp_sensor1;
    } else if (!isnan(temp_sensor2) && temp_sensor2 > 0) {
      avg_temp = temp_sensor2;
    } else {
      avg_temp = 0.0;
    }
    
    if (!isnan(hum_sensor1) && !isnan(hum_sensor2) && hum_sensor1 > 0 && hum_sensor2 > 0) {
      avg_hum = (hum_sensor1 + hum_sensor2) / 2.0;
    } else if (!isnan(hum_sensor1) && hum_sensor1 > 0) {
      avg_hum = hum_sensor1;
    } else if (!isnan(hum_sensor2) && hum_sensor2 > 0) {
      avg_hum = hum_sensor2;
    } else {
      avg_hum = 0.0;
    }
    
    avg_ldr = (ldr1_value + ldr2_value) / 2.0;
    
    // Los sensores DHT22 necesitan al menos 2 segundos entre lecturas
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// ==================== TAREA 3 ====================
void task3(void* parameter) {
  Serial.println("[Tarea3] Tarea iniciada en Core 0");
  
  while (true) {
    // Aquí puedes agregar la funcionalidad de la tarea 3
    // Por ejemplo: control de ventiladores, procesamiento de datos, etc.
    
    vTaskDelay(pdMS_TO_TICKS(3000)); // Esperar 3 segundos
  }
}

// ==================== TAREA 4 ====================
void task4(void* parameter) {
  Serial.println("[Tarea4] Tarea iniciada en Core 1");
  
  while (true) {
    // Aquí puedes agregar la funcionalidad de la tarea 4
    // Por ejemplo: control de Peltier, procesamiento de datos, etc.
    
    vTaskDelay(pdMS_TO_TICKS(1500)); // Esperar 1.5 segundos
  }
}

// ==================== TAREA 5 ====================
void task5(void* parameter) {
  Serial.println("[Tarea5] Tarea iniciada en Core 1");
  
  while (true) {
    // Aquí puedes agregar la funcionalidad de la tarea 5
    // Por ejemplo: monitoreo de flujo de datos, validación, etc.
    
    vTaskDelay(pdMS_TO_TICKS(2500)); // Esperar 2.5 segundos
  }
}

// ==================== TAREA 6 ====================
void task6(void* parameter) {
  Serial.println("[Tarea6] Tarea iniciada en Core 1");
  
  while (true) {
    // Aquí puedes agregar la funcionalidad de la tarea 6
    // Por ejemplo: comunicación con InfluxDB, logging, etc.
    
    vTaskDelay(pdMS_TO_TICKS(3500)); // Esperar 3.5 segundos
  }
}
