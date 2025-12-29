include("C:/Users/cybershibo/Desktop/QtProjects/Desktop/Germini-GUI/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/Germini-GUI-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtserialport")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/cybershibo/Desktop/QtProjects/Desktop/Germini-GUI/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/Germini-GUI.exe"
    GENERATE_QT_CONF
)
