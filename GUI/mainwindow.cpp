#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
#ifdef QT_SERIALPORT_AVAILABLE
    , serialPort(nullptr)
#endif
    , uptimeTimer(nullptr)
{
    ui->setupUi(this);
    
#ifdef QT_SERIALPORT_AVAILABLE
    // Configurar Serial
    serialPort = new QSerialPort(this);
    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::serialDataReceived);
    connect(serialPort, QOverload<QSerialPort::SerialPortError>::of(&QSerialPort::errorOccurred),
            this, &MainWindow::serialErrorOccurred);
    
    // Actualizar puertos disponibles
    updateSerialPorts();
#else
    // Si SerialPort no está disponible, deshabilitar controles
    ui->comboBox_Port->setEnabled(false);
    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_RefreshPorts->setEnabled(false);
    QMessageBox::warning(this, "Qt SerialPort no disponible", 
                        "El módulo Qt SerialPort no está instalado.\n"
                        "Por favor instálalo usando Qt Maintenance Tool.");
#endif
    
    // Configurar timer para solicitar uptime periódicamente
    uptimeTimer = new QTimer(this);
    connect(uptimeTimer, &QTimer::timeout, this, &MainWindow::updateUptime);
    
    // Estado inicial
    updateUI(false);
}

MainWindow::~MainWindow()
{
#ifdef QT_SERIALPORT_AVAILABLE
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
#endif
    delete ui;
}

void MainWindow::updateSerialPorts()
{
#ifdef QT_SERIALPORT_AVAILABLE
    ui->comboBox_Port->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        QString portString = port.portName();
        if (!port.description().isEmpty()) {
            portString += " - " + port.description();
        }
        ui->comboBox_Port->addItem(portString, port.portName());
    }
#endif
}

void MainWindow::on_pushButton_RefreshPorts_clicked()
{
    updateSerialPorts();
}

void MainWindow::on_pushButton_Connect_clicked()
{
#ifdef QT_SERIALPORT_AVAILABLE
    if (serialPort->isOpen()) {
        return;
    }
    
    QString portName = ui->comboBox_Port->currentData().toString();
    if (portName.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor selecciona un puerto serial.");
        return;
    }
    
    serialPort->setPortName(portName);
    serialPort->setBaudRate(115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    
    if (serialPort->open(QIODevice::ReadWrite)) {
        // Limpiar buffers
        serialPort->clear();
        
        appendDebug(QString("Conectado a %1 a 115200 baud").arg(portName), "SUCCESS");
        
        updateUI(true);
        
        // Esperar un poco para que el ESP32 esté listo
        QTimer::singleShot(500, this, [this]() {
            // Solicitar uptime inmediatamente y luego cada 2 segundos
            updateUptime();
            uptimeTimer->start(2000);
        });
    } else {
        QString error = serialPort->errorString();
        appendDebug(QString("Error al conectar: %1").arg(error), "ERROR");
        QMessageBox::critical(this, "Error", 
                            QString("No se pudo abrir el puerto serial:\n%1")
                            .arg(error));
    }
#else
    QMessageBox::warning(this, "Error", "Qt SerialPort no está disponible.");
#endif
}

void MainWindow::on_pushButton_Disconnect_clicked()
{
#ifdef QT_SERIALPORT_AVAILABLE
    if (serialPort->isOpen()) {
        uptimeTimer->stop();
        serialPort->close();
        appendDebug("Desconectado del puerto serial", "INFO");
        updateUI(false);
        ui->textEdit_Uptime->clear();
    }
#endif
}

void MainWindow::on_pushButton_LED_ON_clicked()
{
    sendCommand("LED_ON");
}

void MainWindow::on_pushButton_LED_OFF_clicked()
{
    sendCommand("LED_OFF");
}

void MainWindow::on_pushButton_ClearDebug_clicked()
{
    ui->textEdit_Debug->clear();
}

void MainWindow::sendCommand(const QString &command)
{
#ifdef QT_SERIALPORT_AVAILABLE
    if (serialPort && serialPort->isOpen()) {
        QString commandWithNewline = command + "\n";
        QByteArray data = commandWithNewline.toUtf8();
        qint64 bytesWritten = serialPort->write(data);
        
        if (bytesWritten == -1) {
            appendDebug(QString("Error al enviar comando: %1").arg(serialPort->errorString()), "ERROR");
        } else {
            // Forzar el envío inmediato
            serialPort->flush();
            appendDebug(QString("Enviado: %1 (%2 bytes)").arg(command).arg(bytesWritten), "TX");
        }
    } else {
        appendDebug("Error: Puerto serial no conectado", "ERROR");
        QMessageBox::warning(this, "Error", "El puerto serial no está conectado.");
    }
#else
    QMessageBox::warning(this, "Error", "Qt SerialPort no está disponible.");
#endif
}

void MainWindow::serialDataReceived()
{
#ifdef QT_SERIALPORT_AVAILABLE
    QByteArray data = serialPort->readAll();
    QString text = QString::fromUtf8(data);
    
    // Mostrar todos los datos recibidos en el área de debug
    appendDebug(text, "RX");
    
    // Procesar línea por línea
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);
    for (const QString &line : lines) {
        QString trimmedLine = line.trimmed();
        
        if (trimmedLine.startsWith("UPTIME:")) {
            // Extraer el tiempo de actividad
            QString uptime = trimmedLine.mid(7).trimmed(); // Remover "UPTIME:"
            ui->textEdit_Uptime->setPlainText(uptime);
        }
        else if (trimmedLine.startsWith("OK:")) {
            // Mostrar confirmaciones
            appendDebug(trimmedLine, "SUCCESS");
        }
        else if (trimmedLine.startsWith("ERROR:")) {
            // Mostrar errores
            appendDebug(trimmedLine, "ERROR");
        }
    }
#endif
}

#ifdef QT_SERIALPORT_AVAILABLE
void MainWindow::serialErrorOccurred(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, "Error Serial", 
                            QString("Error crítico en el puerto serial:\n%1")
                            .arg(serialPort->errorString()));
        if (serialPort->isOpen()) {
            serialPort->close();
            updateUI(false);
        }
    }
}
#endif

void MainWindow::updateUptime()
{
#ifdef QT_SERIALPORT_AVAILABLE
    if (serialPort && serialPort->isOpen()) {
        sendCommand("GET_UPTIME");
    }
#endif
}

void MainWindow::updateUI(bool connected)
{
    ui->pushButton_Connect->setEnabled(!connected);
    ui->pushButton_Disconnect->setEnabled(connected);
    ui->pushButton_LED_ON->setEnabled(connected);
    ui->pushButton_LED_OFF->setEnabled(connected);
    ui->comboBox_Port->setEnabled(!connected);
    
    if (connected) {
        ui->label_Status->setText("Conectado");
        ui->label_Status->setStyleSheet("color: green; font-weight: bold;");
    } else {
        ui->label_Status->setText("Desconectado");
        ui->label_Status->setStyleSheet("color: red; font-weight: bold;");
    }
}

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
    } else if (type == "SUCCESS") {
        prefix = "✓";
        color = "darkgreen";
    } else {
        prefix = "•";
        color = "gray";
    }
    
    QString formatted = QString("[%1] <span style='color: %2;'>%3</span> %4")
                       .arg(timestamp, color, prefix, text);
    
    ui->textEdit_Debug->append(formatted);
    
    // Auto-scroll
    QTextCursor cursor = ui->textEdit_Debug->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit_Debug->setTextCursor(cursor);
}
