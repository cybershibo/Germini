cmake_minimum_required(VERSION 3.19)
project(Germini-GUI LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# Intentar encontrar SerialPort (puede no estar instalado)
find_package(Qt6 QUIET COMPONENTS SerialPort)
if(NOT Qt6SerialPort_FOUND)
    message(WARNING "Qt6SerialPort no encontrado. La funcionalidad serial no estará disponible.")
    message(STATUS "Para instalar Qt6SerialPort, usa Qt Maintenance Tool y agrega el componente 'Qt Serial Port'")
endif()

qt_standard_project_setup()

qt_add_executable(Germini-GUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_link_libraries(Germini-GUI
    PRIVATE
        Qt::Core
        Qt::Widgets
)

# Enlazar SerialPort solo si está disponible
if(Qt6SerialPort_FOUND)
    target_link_libraries(Germini-GUI PRIVATE Qt::SerialPort)
    target_compile_definitions(Germini-GUI PRIVATE QT_SERIALPORT_AVAILABLE)
endif()

include(GNUInstallDirs)

install(TARGETS Germini-GUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Germini-GUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
