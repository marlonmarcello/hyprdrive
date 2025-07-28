import QtQuick

// This is the root window of our application
Window {
    width: 1920
    height: 30
    visible: true
    title: "hyprdrive Panel"
    color: "#333333" // A simple background color

    Text {
        anchors.centerIn: parent
        text: "Hello, QML!"
        color: "white"
    }
}
