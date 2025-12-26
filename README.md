# Germini Firmware - Sistema Multi-Tarea con FreeRTOS

Sistema de firmware para ESP32 que utiliza FreeRTOS para gestionar múltiples tareas distribuidas entre los dos núcleos del procesador.

## Características

- **Arquitectura Multi-Tarea**: 4 tareas principales distribuidas entre los núcleos del ESP32
- **Lectura de Sensores**: Soporte para 2 sensores DHT22 y 2 sensores LDR
- **Comunicación Serial**: Interfaz de comandos para control y monitoreo
- **Integración InfluxDB**: Envío automático de datos a base de datos de series temporales
- **Monitoreo de Flujo**: Verificación continua del estado del sistema

## Distribución de Tareas

### Núcleo 0 (Core 0) - E/S y Comunicación
- **SerialComm**: Maneja la comunicación Serial bidireccional
- **InfluxDB**: Gestiona el envío de datos a InfluxDB

### Núcleo 1 (Core 1) - Procesamiento y Monitoreo
- **SensorReader**: Lee los sensores DHT22 y LDR
- **DataFlowMonitor**: Verifica el flujo de datos y estado del sistema

## Configuración

### 1. Configurar WiFi e InfluxDB

Edita el archivo `include/config.h` y actualiza las siguientes constantes:

```cpp
#define WIFI_SSID "TU_SSID"
#define WIFI_PASSWORD "TU_PASSWORD"

#define INFLUXDB_URL "http://tu-servidor:8086"
#define INFLUXDB_TOKEN "tu-token"
#define INFLUXDB_ORG "tu-org"
#define INFLUXDB_BUCKET "tu-bucket"
```

### 2. Configurar Pines de Sensores

Por defecto, los pines están configurados así:

- **DHT22 #1**: Pin 4
- **DHT22 #2**: Pin 5
- **LDR #1**: Pin 34 (ADC1_CH6)
- **LDR #2**: Pin 35 (ADC1_CH7)

Puedes cambiar estos valores en `include/config.h`.

### 3. Ajustar Intervalos (Opcional)

En `include/config.h` puedes ajustar los intervalos de ejecución:

```cpp
#define SENSOR_READ_INTERVAL 2000      // Lectura de sensores cada 2 segundos
#define INFLUXDB_SEND_INTERVAL 10000   // Envío a InfluxDB cada 10 segundos
#define Serial_REPORT_INTERVAL 5000    // Reporte Serial cada 5 segundos
#define DATAFLOW_CHECK_INTERVAL 5000   // Verificación de flujo cada 5 segundos
```

## Conexión de Sensores

### DHT22
- **VCC** → 3.3V del ESP32
- **GND** → GND del ESP32
- **DATA** → Pin configurado (4 o 5)
- **Nota**: Se recomienda usar una resistencia pull-up de 10kΩ entre DATA y VCC

### LDR (Light Dependent Resistor)
- Conectar en divisor de voltaje con resistencia de 10kΩ
- **LDR** → Pin ADC configurado (34 o 35)
- **Resistencia 10kΩ** → Entre el pin ADC y GND
- **VCC** → 3.3V del ESP32

## Comandos Serial

El sistema acepta los siguientes comandos por el puerto Serial (115200 baudios):

- `status` o `estado`: Muestra el estado del sistema
- `sensors` o `sensores`: Muestra los últimos valores de los sensores
- `wifi`: Muestra el estado de la conexión WiFi
- `wifi reconnect` o `wifi reconectar`: Fuerza la reconexión WiFi
- `help` o `ayuda`: Muestra la lista de comandos disponibles

## Estructura del Proyecto

```
Germini-Firmware/
├── include/
│   ├── config.h          # Archivo de configuración
│   └── README
├── src/
│   └── main.cpp          # Código principal con todas las tareas
├── platformio.ini        # Configuración de PlatformIO
└── README.md            # Este archivo
```

## Compilación y Carga

1. Asegúrate de tener PlatformIO instalado
2. Conecta tu ESP32 al puerto USB
3. Compila y carga el firmware:
   ```bash
   pio run -t upload
   ```
4. Abre el monitor Serial:
   ```bash
   pio device monitor
   ```

## Funcionamiento

1. **Inicialización**: El sistema conecta a WiFi y configura InfluxDB
2. **Lectura de Sensores**: Cada 2 segundos se leen todos los sensores
3. **Almacenamiento**: Los datos se almacenan en una cola compartida
4. **Envío a InfluxDB**: Cada 10 segundos se envían los datos más recientes
5. **Monitoreo**: El sistema verifica continuamente el flujo de datos
6. **Comunicación Serial**: Responde a comandos y envía reportes periódicos

## Solución de Problemas

### Los sensores no leen correctamente
- Verifica las conexiones físicas
- Asegúrate de que los pines estén correctamente configurados
- Para DHT22, verifica que tenga la resistencia pull-up

### No se conecta a WiFi
- Verifica las credenciales en `config.h`
- Usa el comando `wifi reconnect` para forzar la reconexión
- Revisa que el ESP32 esté dentro del rango del router

### No se envían datos a InfluxDB
- Verifica la configuración de InfluxDB en `config.h`
- Asegúrate de que el servidor InfluxDB sea accesible desde la red
- Revisa los mensajes de error en el monitor Serial

### Cola de sensores llena
- Aumenta `SENSOR_QUEUE_SIZE` en `config.h`
- Reduce `SENSOR_READ_INTERVAL` o aumenta `INFLUXDB_SEND_INTERVAL`

## Personalización

### Agregar más sensores
1. Agrega los pines en `config.h`
2. Actualiza la estructura `SensorData` en `main.cpp`
3. Modifica la función `taskSensorReader()` para leer los nuevos sensores

### Cambiar prioridades de tareas
Ajusta las constantes en `config.h`:
```cpp
#define SENSOR_TASK_PRIORITY 3      // Mayor número = mayor prioridad
#define Serial_TASK_PRIORITY 2
#define INFLUXDB_TASK_PRIORITY 2
#define DATAFLOW_TASK_PRIORITY 1
```

## Licencia

Este proyecto está disponible para uso personal y educativo.

