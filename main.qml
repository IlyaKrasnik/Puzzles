import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 400
    height: 400
    visible: true
    Image {
        anchors.fill: parent
        id: windowBackground
        source: "WindowBackground.jpg"
    }
    title: qsTr("Game")

    GameBoard {
        anchors.fill: parent
    }
}
