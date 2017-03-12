import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import fr.univtln.jguiennet299 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Cartridge")

    GridView{
        id: view
        anchors.fill: parent
        clip: true
        model: CartridgeModel{}
        delegate: Column{
            Text{
                text: model.performer ? model.performer : "…"
            }

            Text{
                text: model.title ? model.title : "…"
            }

            Text{
                property var min: Math.ceil(model.duration/3600)
                text: min ? min + "min " : "0min0sec"
            }
        }
    }

//    SwipeView {
//        id: swipeView
//        anchors.fill: parent
//        currentIndex: tabBar.currentIndex

//        Page1 {
//        }

//        Page {
//            Label {
//                text: qsTr("Second page")
//                anchors.centerIn: parent
//            }
//        }
//    }

//    footer: TabBar {
//        id: tabBar
//        currentIndex: swipeView.currentIndex
//        TabButton {
//            text: qsTr("First")
//        }
//        TabButton {
//            text: qsTr("Second")
//        }
//    }
}
