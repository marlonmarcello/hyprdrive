qt_add_library(hyprdrive_ui STATIC)

qt6_add_qml_module(hyprdrive_ui
    URI hyprdrive.ui
    VERSION 1.0
    QML_FILES
        main.qml
    RESOURCE_PREFIX "/"
)
