import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import org.winmedia.guiennet 1.0

ApplicationWindow {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Cartridge")

    menuBar: MenuBar{
        Menu{
            title: "File"
            MenuItem{
                text: "Settings"
                onTriggered: console.log("Propreties")
            }
        }
    }

    statusBar: StatusBar{
        Label {
            text: qsTr("status")
        }
    }

    GridView{
        property var cpt: 0
        id:grid
        anchors.fill: parent
        anchors.margins: 10
        clip: true
        cellWidth: 150
        flow: (root.width < root.height) ? GridView.FlowTopToBottom : GridView.FlowLeftToRight
        model: CartridgeModel{}
        delegate: cartridgeDelegate
        states: State{
            name: "ITEM_SELECTED_FOR_MOVE"
            onCompleted: console.log("STATE ITEM_SELECTED_FOR_MOVE")
            PropertyChanges {
                target: grid.currentItem
                border.width: 2

            }
        }
    }

    Component{
        id: cartridgeDelegate
        Rectangle {
            id: backgroundCell
            color: "#888888"
            height: grid.cellHeight-2
            width: grid.cellWidth-2
            radius: 5
            gradient: Gradient{
                GradientStop{position: 0.0; color: "#AAAAAA"}
                GradientStop{position: 1.0; color: "#DDDDDD"}
            }
            border.color: Qt.darker(color)
//            states: [
//                State {
//                    name: "ZOOM"
//                    onCompleted: console.log("ZOOM state")
//                    PropertyChanges {
//                        target: backgroundCell
//                        z: 100
//                        height: grid.height
//                        width: grid.width
//                        color: "green"
//                    }
//                    PropertyChanges {
//                        target: backgroundCell.GridView.view
//                        explicit: true
//                        contentY: backgroundCell.y
//                        contentX: backgroundCell.x
//                        interactive: false
//                    }

//                },

//                State {
//                    name: "TARGET"
//                    PropertyChanges {
//                        target: backgroundCell
//                        color: "pink"
//                    }
//                }
//            ]

            MouseArea{
                anchors.fill: parent
//                onPressAndHold:{
//                    if (backgroundCell.state == ""){
//                        labelStatus.text=qsTr("status")
//                        backgroundCell.state = "ZOOM"
//                    }
//                }
//                onClicked:{
//                    if(backgroundCell.state == "ZOOM"){
//                        backgroundCell.state = ""
//                    }
                onClicked:{
                    grid.currentIndex = index
                    if(grid.state == ""){
                        grid.state = "ITEM_SELECTED_FOR_MOVE"
                    }
                    else if(grid.state == "ITEM_SELECTED_FOR_MOVE"){
                        grid.state = ""
                        //TODO: activate moving around
                    }

                }
            }

            Column{
                width: parent.width
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: performer ? performer : "…"
                }

                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: title ? title : "…"
                }

                Text{
                    property var min: Math.ceil(duration/3600)
                    text: min ? min + "min " : "…"
                }
            }

            Row{
                anchors.bottom: backgroundCell.bottom
                Rectangle{
                    color: Qt.darker(backgroundCell.color)
                    width: 20
                    height: 20
                    radius: 5
                    anchors.bottom: parent.bottom
                    Text{
                        anchors.centerIn: parent
                        text: index+1
                    }
                }

                Button{
                    text: ">"
                    onClicked: console.log(id)
                }
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
