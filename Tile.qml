import QtQuick 2.0

Rectangle {
    id: root

    property string displayText: ""
    width: parent.width / 4
    height: parent.height / 4
    radius: 10

    Image {
        id: background
        anchors.fill: parent
        source: "TileBackground.jpg"
    }

    border {
        color: "black"
        width: 1
    }

    Text {
        id: _firstText
        anchors.centerIn: root
        color: "white"
        font {

            bold: true
            pointSize: Math.min(root.width, root.height) / 2;
        }
        text: root.displayText
    }
}
