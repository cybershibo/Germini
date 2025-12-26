#ifndef CONFIG_H
#define CONFIG_H

// ==================== CONFIGURACIÓN WIFI ====================
#define WIFI_SSID "TU_SSID"
#define WIFI_PASSWORD "TU_PASSWORD"

// ==================== CONFIGURACIÓN INFLUXDB ====================
#define INFLUXDB_URL "http://tu-servidor:8086"
#define INFLUXDB_TOKEN "tu-token"
#define INFLUXDB_ORG "tu-org"
#define INFLUXDB_BUCKET "tu-bucket"

// ==================== CONFIGURACIÓN DE PINES ====================
// Sensores DHT22
#define DHT_TYPE DHT22
#define PIN_SENSOR_DHT0 4  // Pin GPIO4 para el sensor DHT22
#define PIN_SENSOR_DHT1 0  // Pin GPIO5 para otro sensor DHT22 (si es necesario)

// Sensores LDR (ADC)
#define LDR_PIN_1 34  // ADC1_CH6
#define LDR_PIN_2 35  // ADC1_CH7

// ==================== CONFIGURACIÓN DE TAREAS ====================
// Prioridades (mayor número = mayor prioridad)
#define SENSOR_TASK_PRIORITY 3
#define Serial_TASK_PRIORITY 2
#define INFLUXDB_TASK_PRIORITY 2
#define DATAFLOW_TASK_PRIORITY 1

// Tamaños de stack (en bytes)
#define SENSOR_TASK_STACK 4096
#define Serial_TASK_STACK 4096
#define INFLUXDB_TASK_STACK 8192
#define DATAFLOW_TASK_STACK 2048

// ==================== CONFIGURACIÓN DE INTERVALOS ====================
// Intervalos en milisegundos
#define SENSOR_READ_INTERVAL 2000      // Leer sensores cada 2 segundos
#define INFLUXDB_SEND_INTERVAL 10000   // Enviar a InfluxDB cada 10 segundos
#define Serial_REPORT_INTERVAL 5000    // Reportar por Serial cada 5 segundos
#define DATAFLOW_CHECK_INTERVAL 5000   // Verificar flujo cada 5 segundos
#define STATUS_REPORT_INTERVAL 30000   // Reporte de estado cada 30 segundos

// ==================== CONFIGURACIÓN DE COLAS ====================
#define SENSOR_QUEUE_SIZE 10
#define Serial_QUEUE_SIZE 20

// ==================== CONFIGURACIÓN DE TIMEOUTS ====================
// Tiempos máximos sin actividad antes de generar alerta (milisegundos)
#define MAX_SENSOR_READ_TIMEOUT 10000      // 10 segundos
#define MAX_INFLUXDB_SEND_TIMEOUT 30000    // 30 segundos

// ==================== ASIGNACIÓN DE NÚCLEOS ====================
// Core 0: Tareas de E/S y comunicación
#define Serial_TASK_CORE 0
#define INFLUXDB_TASK_CORE 0

// Core 1: Tareas de procesamiento y monitoreo
#define SENSOR_TASK_CORE 1
#define DATAFLOW_TASK_CORE 1

#endif // CONFIG_H

