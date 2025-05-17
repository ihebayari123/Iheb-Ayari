// Marker.qml
import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

MapQuickItem {
    id: marker
    anchorPoint.x: image.width / 2
    anchorPoint.y: image.height

    property string title: ""

    sourceItem: Column {
        Image {
            id: image
            source: "qrc:/new/image/icons/minimal_map.png" // Your marker icon
        }
        Text {
            text: title
            font.bold: true
            color: "black"
            style: Text.Outline
            styleColor: "white"
        }
    }
}
