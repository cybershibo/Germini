/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_Connection;
    QHBoxLayout *horizontalLayout_Connection;
    QLabel *label_Port;
    QComboBox *comboBox_Port;
    QPushButton *pushButton_RefreshPorts;
    QPushButton *pushButton_Connect;
    QPushButton *pushButton_Disconnect;
    QLabel *label_Status;
    QGroupBox *groupBox_Control;
    QHBoxLayout *horizontalLayout_Control;
    QPushButton *pushButton_LED_ON;
    QPushButton *pushButton_LED_OFF;
    QSplitter *splitter;
    QWidget *widget_Uptime;
    QVBoxLayout *verticalLayout_Uptime;
    QLabel *label_Uptime;
    QTextEdit *textEdit_Uptime;
    QWidget *widget_Debug;
    QVBoxLayout *verticalLayout_Debug;
    QHBoxLayout *horizontalLayout_Debug;
    QLabel *label_Debug;
    QPushButton *pushButton_ClearDebug;
    QTextEdit *textEdit_Debug;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(500, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox_Connection = new QGroupBox(centralwidget);
        groupBox_Connection->setObjectName("groupBox_Connection");
        horizontalLayout_Connection = new QHBoxLayout(groupBox_Connection);
        horizontalLayout_Connection->setObjectName("horizontalLayout_Connection");
        label_Port = new QLabel(groupBox_Connection);
        label_Port->setObjectName("label_Port");

        horizontalLayout_Connection->addWidget(label_Port);

        comboBox_Port = new QComboBox(groupBox_Connection);
        comboBox_Port->setObjectName("comboBox_Port");
        comboBox_Port->setMinimumSize(QSize(150, 0));

        horizontalLayout_Connection->addWidget(comboBox_Port);

        pushButton_RefreshPorts = new QPushButton(groupBox_Connection);
        pushButton_RefreshPorts->setObjectName("pushButton_RefreshPorts");
        pushButton_RefreshPorts->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_Connection->addWidget(pushButton_RefreshPorts);

        pushButton_Connect = new QPushButton(groupBox_Connection);
        pushButton_Connect->setObjectName("pushButton_Connect");

        horizontalLayout_Connection->addWidget(pushButton_Connect);

        pushButton_Disconnect = new QPushButton(groupBox_Connection);
        pushButton_Disconnect->setObjectName("pushButton_Disconnect");
        pushButton_Disconnect->setEnabled(false);

        horizontalLayout_Connection->addWidget(pushButton_Disconnect);

        label_Status = new QLabel(groupBox_Connection);
        label_Status->setObjectName("label_Status");

        horizontalLayout_Connection->addWidget(label_Status);


        verticalLayout->addWidget(groupBox_Connection);

        groupBox_Control = new QGroupBox(centralwidget);
        groupBox_Control->setObjectName("groupBox_Control");
        horizontalLayout_Control = new QHBoxLayout(groupBox_Control);
        horizontalLayout_Control->setObjectName("horizontalLayout_Control");
        pushButton_LED_ON = new QPushButton(groupBox_Control);
        pushButton_LED_ON->setObjectName("pushButton_LED_ON");
        pushButton_LED_ON->setEnabled(false);

        horizontalLayout_Control->addWidget(pushButton_LED_ON);

        pushButton_LED_OFF = new QPushButton(groupBox_Control);
        pushButton_LED_OFF->setObjectName("pushButton_LED_OFF");
        pushButton_LED_OFF->setEnabled(false);

        horizontalLayout_Control->addWidget(pushButton_LED_OFF);


        verticalLayout->addWidget(groupBox_Control);

        splitter = new QSplitter(centralwidget);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Orientation::Vertical);
        widget_Uptime = new QWidget(splitter);
        widget_Uptime->setObjectName("widget_Uptime");
        verticalLayout_Uptime = new QVBoxLayout(widget_Uptime);
        verticalLayout_Uptime->setObjectName("verticalLayout_Uptime");
        label_Uptime = new QLabel(widget_Uptime);
        label_Uptime->setObjectName("label_Uptime");

        verticalLayout_Uptime->addWidget(label_Uptime);

        textEdit_Uptime = new QTextEdit(widget_Uptime);
        textEdit_Uptime->setObjectName("textEdit_Uptime");
        textEdit_Uptime->setMaximumSize(QSize(16777215, 60));
        textEdit_Uptime->setReadOnly(true);
        QFont font;
        font.setFamilies({QString::fromUtf8("Consolas")});
        font.setPointSize(12);
        textEdit_Uptime->setFont(font);

        verticalLayout_Uptime->addWidget(textEdit_Uptime);

        splitter->addWidget(widget_Uptime);
        widget_Debug = new QWidget(splitter);
        widget_Debug->setObjectName("widget_Debug");
        verticalLayout_Debug = new QVBoxLayout(widget_Debug);
        verticalLayout_Debug->setObjectName("verticalLayout_Debug");
        horizontalLayout_Debug = new QHBoxLayout();
        horizontalLayout_Debug->setObjectName("horizontalLayout_Debug");
        label_Debug = new QLabel(widget_Debug);
        label_Debug->setObjectName("label_Debug");

        horizontalLayout_Debug->addWidget(label_Debug);

        pushButton_ClearDebug = new QPushButton(widget_Debug);
        pushButton_ClearDebug->setObjectName("pushButton_ClearDebug");

        horizontalLayout_Debug->addWidget(pushButton_ClearDebug);


        verticalLayout_Debug->addLayout(horizontalLayout_Debug);

        textEdit_Debug = new QTextEdit(widget_Debug);
        textEdit_Debug->setObjectName("textEdit_Debug");
        textEdit_Debug->setReadOnly(true);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Consolas")});
        font1.setPointSize(9);
        textEdit_Debug->setFont(font1);

        verticalLayout_Debug->addWidget(textEdit_Debug);

        splitter->addWidget(widget_Debug);

        verticalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 500, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Control ESP32 - Germini", nullptr));
        groupBox_Connection->setTitle(QCoreApplication::translate("MainWindow", "Conexi\303\263n Serial", nullptr));
        label_Port->setText(QCoreApplication::translate("MainWindow", "Puerto:", nullptr));
        pushButton_RefreshPorts->setText(QCoreApplication::translate("MainWindow", "Actualizar", nullptr));
        pushButton_Connect->setText(QCoreApplication::translate("MainWindow", "Conectar", nullptr));
        pushButton_Disconnect->setText(QCoreApplication::translate("MainWindow", "Desconectar", nullptr));
        label_Status->setText(QCoreApplication::translate("MainWindow", "Desconectado", nullptr));
        label_Status->setStyleSheet(QCoreApplication::translate("MainWindow", "color: red; font-weight: bold;", nullptr));
        groupBox_Control->setTitle(QCoreApplication::translate("MainWindow", "Control LED", nullptr));
        pushButton_LED_ON->setText(QCoreApplication::translate("MainWindow", "Encender LED", nullptr));
        pushButton_LED_ON->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #4CAF50; color: white; font-weight: bold; padding: 10px;", nullptr));
        pushButton_LED_OFF->setText(QCoreApplication::translate("MainWindow", "Apagar LED", nullptr));
        pushButton_LED_OFF->setStyleSheet(QCoreApplication::translate("MainWindow", "background-color: #f44336; color: white; font-weight: bold; padding: 10px;", nullptr));
        label_Uptime->setText(QCoreApplication::translate("MainWindow", "Tiempo de Actividad:", nullptr));
        textEdit_Uptime->setPlaceholderText(QCoreApplication::translate("MainWindow", "El tiempo de actividad del ESP32 aparecer\303\241 aqu\303\255...", nullptr));
        label_Debug->setText(QCoreApplication::translate("MainWindow", "Comunicaci\303\263n Serial (Debug):", nullptr));
        pushButton_ClearDebug->setText(QCoreApplication::translate("MainWindow", "Limpiar", nullptr));
        textEdit_Debug->setPlaceholderText(QCoreApplication::translate("MainWindow", "Aqu\303\255 se mostrar\303\241 toda la comunicaci\303\263n serial...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
