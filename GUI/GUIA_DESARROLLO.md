# Guía de Desarrollo: Interfaz Gráfica Qt para Control de ESP32

## Tabla de Contenidos
1. [Introducción](#introducción)
2. [Conceptos Fundamentales de Qt](#conceptos-fundamentales-de-qt)
3. [Estructura del Proyecto](#estructura-del-proyecto)
4. [Diseño de la Interfaz con Qt Designer](#diseño-de-la-interfaz-con-qt-designer)
5. [Lógica de Programación](#lógica-de-programación)
6. [Comunicación Serial](#comunicación-serial)
7. [Interacción con el Firmware](#interacción-con-el-firmware)
8. [Flujo de Datos Completo](#flujo-de-datos-completo)
9. [Conceptos Avanzados](#conceptos-avanzados)
10. [Mejores Prácticas](#mejores-prácticas)

---

## Introducción

Este documento explica el desarrollo completo de una interfaz gráfica en C++ usando Qt Framework para controlar un ESP32 mediante comunicación serial. La aplicación permite:

- Conectar/desconectar al ESP32 vía puerto serial
- Controlar un LED integrado (encender/apagar)
- Visualizar el tiempo de actividad (uptime) del microcontrolador
- Monitorear la comunicación serial en tiempo real

### Tecnologías Utilizadas

- **Qt 6**: Framework de desarrollo multiplataforma
- **Qt Designer**: Herramienta visual para diseñar interfaces
- **C++**: Lenguaje de programación
- **QSerialPort**: Módulo de Qt para comunicación serial
- **CMake**: Sistema de construcción

---

## Conceptos Fundamentales de Qt

### ¿Qué es Qt?

Qt es un framework multiplataforma que permite desarrollar aplicaciones con interfaces gráficas usando C++. Proporciona:

- **Widgets**: Componentes de interfaz (botones, etiquetas, cuadros de texto, etc.)
- **Señales y Slots**: Sistema de comunicación entre objetos (patrón Observer)
- **MOC (Meta-Object Compiler)**: Preprocesador que extiende C++ con capacidades de reflexión

### Señales y Slots

El sistema de **Señales y Slots** es fundamental en Qt:

- **Señal (Signal)**: Evento emitido por un objeto cuando ocurre algo (ej: `clicked()` de un botón)
- **Slot**: Función que se ejecuta cuando se recibe una señal

```cpp
// Ejemplo: Conectar la señal clicked() de un botón con un slot
connect(ui->pushButton_Connect, &QPushButton::clicked, 
        this, &MainWindow::on_pushButton_Connect_clicked);
```

**Ventajas:**
- Desacoplamiento: El objeto que emite no conoce al receptor
- Type-safe: Verificación de tipos en tiempo de compilación
- Flexible: Una señal puede conectarse a múltiples slots

### Widgets Principales Utilizados

1. **QMainWindow**: Ventana principal de la aplicación
2. **QPushButton**: Botones interactivos
3. **QComboBox**: Lista desplegable (selector de puertos)
4. **QTextEdit**: Área de texto multilínea (solo lectura o editable)
5. **QLabel**: Etiqueta de texto
6. **QGroupBox**: Contenedor con título para agrupar widgets
7. **QSplitter**: Divisor ajustable para organizar widgets
8. **QTimer**: Temporizador para ejecutar código periódicamente

---

## Estructura del Proyecto

### Archivos del Proyecto

```
Germini-GUI/
├── CMakeLists.txt          # Configuración de compilación
├── main.cpp                # Punto de entrada de la aplicación
├── mainwindow.h            # Declaración de la clase MainWindow
├── mainwindow.cpp          # Implementación de la clase MainWindow
├── mainwindow.ui           # Archivo XML de diseño (Qt Designer)
└── build/                  # Directorio de compilación
```

### CMakeLists.txt

Este archivo configura cómo se compila el proyecto:

```cmake
cmake_minimum_required(VERSION 3.19)
project(Germini-GUI LANGUAGES CXX)

# Buscar Qt6 con los módulos necesarios
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# Buscar SerialPort (opcional)
find_package(Qt6 QUIET COMPONENTS SerialPort)

# Configuración estándar de Qt
qt_standard_project_setup()

# Crear ejecutable
qt_add_executable(Germini-GUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Enlazar bibliotecas
target_link_libraries(Germini-GUI PRIVATE Qt::Core Qt::Widgets)
if(Qt6SerialPort_FOUND)
    target_link_libraries(Germini-GUI PRIVATE Qt::SerialPort)
endif()
```

**Explicación:**
- `find_package`: Busca las bibliotecas de Qt necesarias
- `qt_add_executable`: Crea el ejecutable con soporte para archivos .ui
- `target_link_libraries`: Enlaza las bibliotecas necesarias

### main.cpp

Punto de entrada de la aplicación:

```cpp
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Crea la aplicación Qt
    MainWindow w;                // Crea la ventana principal
    w.show();                    // Muestra la ventana
    return a.exec();             // Inicia el loop de eventos
}
```

**Flujo:**
1. `QApplication`: Gestiona el ciclo de vida de la aplicación
2. `MainWindow`: Instancia la ventana principal
3. `show()`: Hace visible la ventana
4. `exec()`: Inicia el loop de eventos (espera señales, procesa eventos, etc.)

---

## Diseño de la Interfaz con Qt Designer

### ¿Qué es Qt Designer?

Qt Designer es una herramienta visual que permite diseñar interfaces arrastrando y soltando widgets. Genera un archivo `.ui` en formato XML que describe la interfaz.

### Archivo mainwindow.ui

El archivo `.ui` es XML que describe la estructura de la interfaz:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
  <class>MainWindow</class>
  <widget class="QMainWindow" name="MainWindow">
    <!-- Propiedades de la ventana -->
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>500</width>
        <height>400</height>
      </rect>
    </property>
    <!-- Widgets de la interfaz -->
  </widget>
</ui>
```

### Estructura de la Interfaz

La interfaz se organiza en tres secciones principales usando layouts:

#### 1. Grupo de Conexión Serial

```xml
<widget class="QGroupBox" name="groupBox_Connection">
  <property name="title">
    <string>Conexión Serial</string>
  </property>
  <layout class="QHBoxLayout">
    <!-- QComboBox para seleccionar puerto -->
    <!-- QPushButton para actualizar puertos -->
    <!-- QPushButton para conectar -->
    <!-- QPushButton para desconectar -->
    <!-- QLabel para mostrar estado -->
  </layout>
</widget>
```

**Widgets utilizados:**
- `QComboBox`: Lista desplegable de puertos COM disponibles
- `QPushButton`: Botones de acción
- `QLabel`: Indicador de estado (Conectado/Desconectado)

#### 2. Grupo de Control LED

```xml
<widget class="QGroupBox" name="groupBox_Control">
  <property name="title">
    <string>Control LED</string>
  </property>
  <layout class="QHBoxLayout">
    <widget class="QPushButton" name="pushButton_LED_ON">
      <property name="text">
        <string>Encender LED</string>
      </property>
      <property name="styleSheet">
        <string>background-color: #4CAF50; color: white;</string>
      </property>
    </widget>
    <!-- Botón para apagar LED -->
  </layout>
</widget>
```

**Características:**
- Botones con colores personalizados usando `styleSheet`
- Habilitados/deshabilitados según el estado de conexión

#### 3. Área de Información

```xml
<widget class="QSplitter" name="splitter">
  <property name="orientation">
    <enum>Qt::Orientation::Vertical</enum>
  </property>
  <!-- Área de tiempo de actividad -->
  <!-- Área de debug/comunicación -->
</widget>
```

**QSplitter**: Permite al usuario ajustar el tamaño de las áreas arrastrando el divisor.

### Nombres de Widgets

**Importante**: Los nombres de los widgets en Qt Designer deben seguir una convención para que el sistema de señales y slots automático funcione:

- Botones: `pushButton_NombreAccion`
- Slots automáticos: `on_pushButton_NombreAccion_clicked()`

Qt conecta automáticamente la señal `clicked()` con el slot que tenga el nombre correcto.

### Layouts (Diseño)

Los layouts organizan los widgets:

- **QVBoxLayout**: Organiza widgets verticalmente
- **QHBoxLayout**: Organiza widgets horizontalmente
- **QGridLayout**: Organiza widgets en una cuadrícula
- **QSplitter**: Permite redimensionar áreas

---

## Lógica de Programación

### Estructura de la Clase MainWindow

#### mainwindow.h

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#ifdef QT_SERIALPORT_AVAILABLE
#include <QSerialPort>
#include <QSerialPortInfo>
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;  // Declaración forward (definida en ui_mainwindow.h)
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT  // Necesario para señales y slots

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:  // Slots que responden a señales
    void on_pushButton_RefreshPorts_clicked();
    void on_pushButton_Connect_clicked();
    void on_pushButton_Disconnect_clicked();
    void on_pushButton_LED_ON_clicked();
    void on_pushButton_LED_OFF_clicked();
    void on_pushButton_ClearDebug_clicked();
    void serialDataReceived();
    void updateUptime();

private:
    Ui::MainWindow *ui;  // Puntero a la interfaz generada desde .ui
    
#ifdef QT_SERIALPORT_AVAILABLE
    QSerialPort *serialPort;  // Objeto para comunicación serial
#endif
    
    QTimer *uptimeTimer;  // Timer para solicitar uptime periódicamente
    
    // Métodos auxiliares
    void updateSerialPorts();
    void updateUI(bool connected);
    void sendCommand(const QString &command);
    void appendDebug(const QString &text, const QString &type = "INFO");
};

#endif // MAINWINDOW_H
```

**Conceptos clave:**

1. **Q_OBJECT**: Macro que activa el sistema de señales/slots y MOC
2. **private slots**: Funciones que pueden ser conectadas a señales
3. **Ui::MainWindow**: Clase generada automáticamente desde el archivo .ui
4. **Compilación condicional**: `#ifdef QT_SERIALPORT_AVAILABLE` permite compilar sin SerialPort

#### Constructor (mainwindow.cpp)

```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , serialPort(nullptr)
    , uptimeTimer(nullptr)
{
    ui->setupUi(this);  // Carga la interfaz desde .ui
    
    // Configurar Serial
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, 
            this, &MainWindow::serialDataReceived);
    
    // Configurar Timer
    uptimeTimer = new QTimer(this);
    connect(uptimeTimer, &QTimer::timeout, 
            this, &MainWindow::updateUptime);
    
    // Inicialización
    updateSerialPorts();
    updateUI(false);
}
```

**Explicación:**

1. **Lista de inicialización**: Inicializa miembros antes del cuerpo del constructor
2. **ui->setupUi(this)**: Carga y crea todos los widgets definidos en .ui
3. **new QSerialPort(this)**: Crea el objeto serial con `this` como padre (se destruye automáticamente)
4. **connect()**: Conecta señales con slots
   - `readyRead`: Se emite cuando hay datos disponibles para leer
   - `timeout`: Se emite cuando el timer expira

### Gestión de Conexión Serial

#### Actualizar Lista de Puertos

```cpp
void MainWindow::updateSerialPorts()
{
    ui->comboBox_Port->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    
    for (const QSerialPortInfo &port : ports) {
        QString portString = port.portName();
        if (!port.description().isEmpty()) {
            portString += " - " + port.description();
        }
        ui->comboBox_Port->addItem(portString, port.portName());
    }
}
```

**Lógica:**
1. Limpia el combo box
2. Obtiene lista de puertos disponibles con `QSerialPortInfo::availablePorts()`
3. Para cada puerto, crea una cadena descriptiva
4. Agrega el item al combo box con el nombre del puerto como dato

#### Conectar al Puerto Serial

```cpp
void MainWindow::on_pushButton_Connect_clicked()
{
    QString portName = ui->comboBox_Port->currentData().toString();
    
    serialPort->setPortName(portName);
    serialPort->setBaudRate(115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    
    if (serialPort->open(QIODevice::ReadWrite)) {
        serialPort->clear();  // Limpiar buffers
        updateUI(true);
        // Esperar 500ms antes de enviar comandos
        QTimer::singleShot(500, this, [this]() {
            updateUptime();
            uptimeTimer->start(2000);
        });
    }
}
```

**Configuración Serial:**
- **Baud Rate**: 115200 (velocidad de transmisión)
- **Data Bits**: 8 bits por carácter
- **Parity**: Sin paridad
- **Stop Bits**: 1 bit de parada
- **Flow Control**: Sin control de flujo

**QTimer::singleShot**: Ejecuta una función después de un delay (500ms en este caso).

### Envío de Comandos

```cpp
void MainWindow::sendCommand(const QString &command)
{
    if (serialPort && serialPort->isOpen()) {
        QString commandWithNewline = command + "\n";
        QByteArray data = commandWithNewline.toUtf8();
        qint64 bytesWritten = serialPort->write(data);
        
        if (bytesWritten != -1) {
            serialPort->flush();  // Forzar envío inmediato
            appendDebug(QString("Enviado: %1").arg(command), "TX");
        }
    }
}
```

**Proceso:**
1. Verifica que el puerto esté abierto
2. Agrega salto de línea (`\n`) al comando
3. Convierte a `QByteArray` (UTF-8)
4. Escribe al puerto serial
5. `flush()`: Fuerza el envío inmediato (no espera al buffer)
6. Registra en el área de debug

### Recepción de Datos

```cpp
void MainWindow::serialDataReceived()
{
    QByteArray data = serialPort->readAll();
    QString text = QString::fromUtf8(data);
    
    // Mostrar en área de debug
    appendDebug(text, "RX");
    
    // Procesar líneas
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        if (trimmedLine.startsWith("UPTIME:")) {
            QString uptime = trimmedLine.mid(7).trimmed();
            ui->textEdit_Uptime->setPlainText(uptime);
        }
    }
}
```

**Lógica:**
1. `readAll()`: Lee todos los datos disponibles
2. Convierte a QString para procesamiento de texto
3. Muestra en área de debug
4. Divide por líneas (`\n`)
5. Busca líneas que empiecen con "UPTIME:"
6. Extrae y muestra el tiempo de actividad

### Sistema de Debug

```cpp
void MainWindow::appendDebug(const QString &text, const QString &type)
{
    QString timestamp = QTime::currentTime().toString("hh:mm:ss.zzz");
    QString prefix;
    QString color;
    
    if (type == "TX") {
        prefix = "→";
        color = "blue";
    } else if (type == "RX") {
        prefix = "←";
        color = "green";
    } else if (type == "ERROR") {
        prefix = "✗";
        color = "red";
    }
    
    QString formatted = QString("[%1] <span style='color: %2;'>%3</span> %4")
                       .arg(timestamp, color, prefix, text);
    
    ui->textEdit_Debug->append(formatted);
    
    // Auto-scroll
    QTextCursor cursor = ui->textEdit_Debug->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_Debug->setTextCursor(cursor);
}
```

**Características:**
- Timestamps con milisegundos
- Colores según tipo de mensaje
- HTML para formateo
- Auto-scroll al final

### Actualización Periódica

```cpp
void MainWindow::updateUptime()
{
    if (serialPort && serialPort->isOpen()) {
        sendCommand("GET_UPTIME");
    }
}
```

El timer se configura para llamar a este método cada 2 segundos:

```cpp
uptimeTimer->start(2000);  // 2000 milisegundos = 2 segundos
```

---

## Comunicación Serial

### Protocolo de Comunicación

La comunicación entre la aplicación Qt y el ESP32 sigue un protocolo simple basado en texto:

#### Comandos Enviados (Qt → ESP32)

| Comando | Descripción |
|---------|-------------|
| `LED_ON\n` | Enciende el LED integrado |
| `LED_OFF\n` | Apaga el LED integrado |
| `GET_UPTIME\n` | Solicita el tiempo de actividad |
| `STATUS\n` | Solicita estado completo del sistema |

**Formato:** Todos los comandos terminan con `\n` (salto de línea)

#### Respuestas Recibidas (ESP32 → Qt)

| Respuesta | Formato | Ejemplo |
|-----------|---------|---------|
| Confirmación LED | `OK: LED encendido\n` o `OK: LED apagado\n` | - |
| Tiempo de actividad | `UPTIME: 1d 2h 30m 45s\n` | - |
| Error | `ERROR: Comando desconocido: XXX\n` | - |
| Estado | `LED: ON\nUPTIME: ...\n` | - |

### Flujo de Comunicación

```
┌─────────────┐                    ┌─────────────┐
│   Qt App    │                    │    ESP32    │
└──────┬──────┘                    └──────┬──────┘
       │                                  │
       │  LED_ON\n                        │
       ├─────────────────────────────────>│
       │                                  │ Procesa comando
       │                                  │ Enciende LED
       │  OK: LED encendido\n             │
       │<─────────────────────────────────┤
       │                                  │
       │  GET_UPTIME\n                    │
       ├─────────────────────────────────>│
       │                                  │ Calcula uptime
       │  UPTIME: 1h 30m 15s\n           │
       │<─────────────────────────────────┤
       │                                  │
```

### Manejo de Buffers

**Problema común:** Los datos pueden llegar en fragmentos.

**Solución:** Leer todo lo disponible y procesar línea por línea:

```cpp
QByteArray data = serialPort->readAll();  // Lee todo disponible
QStringList lines = text.split('\n');      // Divide por líneas
```

### Sincronización

**Problema:** El ESP32 puede no estar listo inmediatamente después de conectar.

**Solución:** Esperar antes de enviar el primer comando:

```cpp
QTimer::singleShot(500, this, [this]() {
    updateUptime();
    uptimeTimer->start(2000);
});
```

---

## Interacción con el Firmware

### Firmware del ESP32

El firmware está desarrollado en PlatformIO usando el framework Arduino para ESP32.

#### Estructura del Firmware

```cpp
// Pin del LED integrado (GPIO 2 en ESP32)
#define LED_PIN 2

// Variables globales
unsigned long startTime = 0;
bool ledState = false;

void setup() {
    Serial.begin(115200);        // Inicializar serial a 115200 baud
    pinMode(LED_PIN, OUTPUT);     // Configurar pin como salida
    digitalWrite(LED_PIN, LOW);   // LED apagado inicialmente
    startTime = millis();         // Guardar tiempo de inicio
}

void loop() {
    // Leer comandos del puerto serial
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        processCommand(command);
    }
    
    // Enviar uptime automáticamente cada 5 segundos
    static unsigned long lastUptimeUpdate = 0;
    if (millis() - lastUptimeUpdate >= 5000) {
        unsigned long uptime = millis() - startTime;
        Serial.print("UPTIME: ");
        Serial.println(formatUptime(uptime));
        lastUptimeUpdate = millis();
    }
}
```

#### Procesamiento de Comandos

```cpp
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
}
```

**Características:**
- `trim()`: Elimina espacios en blanco
- `toUpperCase()`: Convierte a mayúsculas (case-insensitive)
- Respuestas inmediatas con `Serial.println()`

### Formato de Tiempo de Actividad

```cpp
String formatUptime(unsigned long milliseconds) {
    unsigned long seconds = milliseconds / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;
    unsigned long days = hours / 24;
    
    seconds = seconds % 60;
    minutes = minutes % 60;
    hours = hours % 24;
    
    String uptime = "";
    if (days > 0) uptime += String(days) + "d ";
    if (hours > 0 || days > 0) uptime += String(hours) + "h ";
    if (minutes > 0 || hours > 0 || days > 0) uptime += String(minutes) + "m ";
    uptime += String(seconds) + "s";
    
    return uptime;
}
```

**Ejemplo de salida:** `1d 2h 30m 45s`

---

## Flujo de Datos Completo

### 1. Inicialización

```
Usuario abre aplicación
    ↓
QApplication se crea
    ↓
MainWindow se crea
    ↓
ui->setupUi() carga interfaz desde .ui
    ↓
updateSerialPorts() busca puertos COM
    ↓
Interfaz lista, esperando acción del usuario
```

### 2. Conexión

```
Usuario selecciona puerto y hace clic en "Conectar"
    ↓
on_pushButton_Connect_clicked()
    ↓
Configura QSerialPort (baud rate, bits, etc.)
    ↓
serialPort->open(QIODevice::ReadWrite)
    ↓
Si éxito:
    - Limpia buffers
    - updateUI(true) habilita controles
    - Espera 500ms
    - Envía GET_UPTIME
    - Inicia timer para solicitar uptime cada 2s
```

### 3. Control del LED

```
Usuario hace clic en "Encender LED"
    ↓
on_pushButton_LED_ON_clicked()
    ↓
sendCommand("LED_ON")
    ↓
Convierte a QByteArray: "LED_ON\n"
    ↓
serialPort->write(data)
    ↓
serialPort->flush() fuerza envío
    ↓
ESP32 recibe comando
    ↓
processCommand("LED_ON")
    ↓
digitalWrite(LED_PIN, HIGH)
    ↓
Serial.println("OK: LED encendido")
    ↓
Qt recibe respuesta
    ↓
serialDataReceived()
    ↓
appendDebug() muestra en área de debug
```

### 4. Recepción de Uptime

```
Timer expira (cada 2 segundos)
    ↓
updateUptime()
    ↓
sendCommand("GET_UPTIME")
    ↓
ESP32 recibe comando
    ↓
Calcula uptime desde startTime
    ↓
Serial.print("UPTIME: 1h 30m 15s\n")
    ↓
Qt recibe datos
    ↓
serialDataReceived()
    ↓
Busca línea que empiece con "UPTIME:"
    ↓
Extrae tiempo: "1h 30m 15s"
    ↓
ui->textEdit_Uptime->setPlainText(uptime)
    ↓
Usuario ve tiempo actualizado
```

---

## Conceptos Avanzados

### 1. Compilación Condicional

Para permitir que el proyecto compile sin Qt SerialPort:

```cpp
#ifdef QT_SERIALPORT_AVAILABLE
    // Código que usa SerialPort
    QSerialPort *serialPort;
#else
    // Código alternativo o deshabilitar funcionalidad
#endif
```

**Ventaja:** El proyecto compila incluso si SerialPort no está instalado.

### 2. Lambda Functions

Usado para ejecutar código después de un delay:

```cpp
QTimer::singleShot(500, this, [this]() {
    updateUptime();
    uptimeTimer->start(2000);
});
```

**Sintaxis:** `[captura](parámetros) { código }`
- `[this]`: Captura el puntero `this`

### 3. Smart Pointers (Futuro)

Aunque no se usa en este proyecto, Qt soporta smart pointers:

```cpp
std::unique_ptr<QSerialPort> serialPort;
```

### 4. Threading

Para operaciones que bloquean, usar QThread:

```cpp
class SerialThread : public QThread {
    void run() override {
        // Código que bloquea
    }
};
```

**Nota:** En este proyecto no es necesario porque `readAll()` no bloquea.

### 5. Estilos con QSS (Qt Style Sheets)

Similar a CSS, permite personalizar la apariencia:

```cpp
button->setStyleSheet("background-color: #4CAF50; color: white;");
```

---

## Mejores Prácticas

### 1. Nombres de Widgets

- Usar nombres descriptivos: `pushButton_Connect` en lugar de `pushButton1`
- Seguir convención: `tipoWidget_Accion` (ej: `pushButton_LED_ON`)

### 2. Manejo de Errores

Siempre verificar operaciones:

```cpp
if (serialPort->open(QIODevice::ReadWrite)) {
    // Éxito
} else {
    // Mostrar error al usuario
    QMessageBox::critical(this, "Error", serialPort->errorString());
}
```

### 3. Limpieza de Recursos

Qt maneja automáticamente la destrucción si se usa `parent`:

```cpp
serialPort = new QSerialPort(this);  // Se destruye automáticamente
```

### 4. Documentación

Documentar funciones complejas:

```cpp
/**
 * Envía un comando al ESP32 vía serial
 * @param command Comando a enviar (sin salto de línea)
 */
void sendCommand(const QString &command);
```

### 5. Separación de Responsabilidades

- UI: Solo presentación
- Lógica: En métodos privados
- Comunicación: En clases separadas (si crece)

### 6. Testing

Para proyectos más grandes, considerar:
- Unit tests con Qt Test
- Integration tests para comunicación serial

---

## Resumen de Conceptos Clave

1. **Qt Designer**: Diseña interfaces visualmente, genera archivo .ui
2. **Señales y Slots**: Sistema de comunicación entre objetos
3. **QSerialPort**: Módulo para comunicación serial
4. **Layouts**: Organizan widgets en la interfaz
5. **Timers**: Ejecutan código periódicamente
6. **Protocolo de Comunicación**: Texto simple con comandos y respuestas
7. **Manejo de Datos**: Leer todo disponible, procesar línea por línea
8. **Debug**: Área de texto para monitorear comunicación

---

## Recursos Adicionales

- **Documentación Qt**: https://doc.qt.io/
- **Qt Serial Port**: https://doc.qt.io/qt-6/qtserialport-index.html
- **Qt Designer Manual**: https://doc.qt.io/qt-6/qtdesigner-manual.html
- **CMake Documentation**: https://cmake.org/documentation/

---

## Conclusión

Esta guía cubre el desarrollo completo de una interfaz gráfica Qt para controlar un ESP32. Los conceptos aprendidos son aplicables a cualquier proyecto Qt:

- Diseño de interfaces con Qt Designer
- Programación orientada a objetos con Qt
- Comunicación serial
- Manejo de eventos y señales/slots
- Temporizadores y actualizaciones periódicas

Con esta base, puedes desarrollar interfaces más complejas y agregar nuevas funcionalidades.

---

**Autor**: Documentación técnica del proyecto Germini-GUI  
**Fecha**: 2024  
**Versión**: 1.0

