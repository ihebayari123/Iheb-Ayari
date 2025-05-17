// map.qml
import QtQuick 2.0
import QtLocation 5.6
import QtPositioning 5.6

Item {
    Plugin {
        id: mapPlugin
        name: "osm" // or "mapboxgl", "esri", etc.
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(48.8566, 2.3522) // Paris as default
        zoomLevel: 10

        // ... other map properties ...
    }

    function centerMap(latitude, longitude) {
        map.center = QtPositioning.coordinate(latitude, longitude);
    }

    function addMarker(latitude, longitude, title) {
        var marker = Qt.createQmlObject(`
            import QtQuick 2.0
            import QtLocation 5.6
            import QtPositioning 5.6

            MapQuickItem {
                anchorPoint.x: image.width / 2
                anchorPoint.y: image.height
                coordinate: QtPositioning.coordinate(${latitude}, ${longitude})

                sourceItem: Rectangle {
                    id: image
                    width: 20
                    height: 20
                    radius: width / 2
                    color: "red"
                    border.color: "white"
                    border.width: 2

                    Text {
                        text: "${title || ''}"
                        anchors.bottom: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: "black"
                        font.bold: true
                    }
                }
            }
        `, map);
        map.addMapItem(marker);
        return marker;
    }

    function clearMarkers() {
        // Remove all markers from the map
        for (var i = map.mapItems.length - 1; i >= 0; i--) {
            if (String(map.mapItems[i]).includes("MapQuickItem")) {
                map.removeMapItem(map.mapItems[i]);
            }
        }
    }
}
