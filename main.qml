import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

import "qrc:/LogicUI.js" as LogicUI
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

    toolBar: TabBar{
        id: tab
        anchors.fill: parent
        onCurrentIndexChanged: {
            console.log("tab n°"+currentIndex+" selected")
            grid.model.changePanel(currentIndex+1)
            grid.state = ""
        }

        Repeater{
            id: panelItem
            model: PanelModel{}
            TabButton{
                text: name
                width: 100
                MouseArea{
                    propagateComposedEvents: true
                    onHoveredChanged: console.log(id)
                    onClicked: {
                        parent.clicked = true
                        console.log("test de merde")
                    }
                }
            }
        }
    }

    GridView{
        id:grid
        anchors.fill: parent
        anchors.margins: 10
        clip: true
        model: CartridgeModel{}
        cellWidth: width/model.widthModel
        cellHeight: height/model.heightModel
        onCellWidthChanged: console.log("cellWidth changed:"+cellWidth)
        flow: GridView.FlowTopToBottom
        delegate: cartridgeDelegate
        onDimensionChanged: console.log("dimension changed:("+xIndex+","+yIndex+")")
//        onWidthChanged: LogicUI.computeDimension()

        signal dimensionChanged(int xIndex, int yIndex)
        Connections{
            onDimensionChanged: grid.model.fitToDimension(xIndex,yIndex)
        }

        states: [
            State{
                name: "MOVEMODE"
                onCompleted: console.log("STATE MOVEMODE")
            }
        ]

        Component.onCompleted: {
//            LogicUI.computeDimension()
        }

//        add: Transition {
//            NumberAnimation{ property: "scale"; from: 0; to: 1.0; duration: 400 }
//        }

        displaced: Transition{
            NumberAnimation { property:"scale"; to:1 }
        }

    }

    Component{
        id: cartridgeDelegate
        Rectangle {
            id: backgroundCell
            height: grid.cellHeight-2
            width: grid.cellWidth-2
            radius: 5

//            property color gradient1: "#AAAAAA"
//            property color gradient2: "#DDDDDD"
//            gradient: Gradient{
//                GradientStop{position: 0.0; color: gradient1}
//                GradientStop{position: 1.0; color: gradient2}
//            }

//            Gradient{
//                id: selectableGradient
//                GradientStop{position: 0.0; color: Qt.darker(gradient1)}
//                GradientStop{position: 1.0; color: Qt.darker(gradient2)}
//            }

//            border.color: Qt.darker(color)
            color: "#FAFAFA"
            border.color: "#CCCCCC"

            SequentialAnimation{
                loops: Animation.Infinite
                running: grid.state == "MOVEMODE" || grid.state == "ITEMSELECTEDFORMOVE"
                NumberAnimation{
                    target: backgroundCell
                    property: "scale"; to: 0.95; duration: 300
                }
                NumberAnimation{
                    target: backgroundCell
                    property: "scale"; to: 1; duration: 1000; easing.type: Easing.OutBounce
                }
            }

            states: [
                State {
                    name: "SELECTABLE"
                    when: grid.state == "MOVEMODE"
                    onCompleted: console.log("MODE SELECTABLE")
//                    PropertyChanges {
//                        target: backgroundCell
//                        gradient: selectableGradient
//                    }
                },
                State {
                    name: "ITEMSELECTEDFORMOVE"
                    onCompleted: console.log("MODE ITEMSELECTEDFORMOVE")
                    PropertyChanges{
                        target: grid.currentItem
                        border.width: 3
                    }
                },
                State{
                    onCompleted: console.log("MODE VALIDATIONMODE")
                    name: "VALIDATIONMOVE"
                }

            ]

            PropertyAnimation{
                running: grid.state == ""
                target: backgroundCell
                property: "scale"; to: 1; duration: 300
            }

            MouseArea{
                anchors.fill: parent
                onClicked:{
                    console.log(backgroundCell.state)
                    if(backgroundCell.state == "SELECTABLE"){
                        grid.currentIndex = index
                        grid.currentItem.state = "ITEMSELECTEDFORMOVE"
                    }
                }
                onPressAndHold:{
                    if(grid.state == ""){
                        grid.state = "MOVEMODE"
                    }
                    else if(grid.state == "MOVEMODE"){
                        grid.state = ""
                    }
                }
            }

            Column{
                width: parent.width
                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: performer ? "<b>"+performer+"</b>" : ""
                }

                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: title ? title : ""
                }

                Text{
                    property var min: Math.ceil(duration/3600)
                    font.family: "Helvetica"
                    font.pointSize: 18
                    text: min ? "<b><i>"+ min + "</b></i><i>min</i> " : ""
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

                ToolButton{
                    text: "►"
                    onClicked: {
                        console.log(grid.model.widthModel)
                        grid.model.widthModel++;
                    }
                }
                ToolButton{
                    text: "◼"
                    onClicked: {
                        console.log(grid.model.heightModel)
                        grid.model.heightModel++;
                    }
                }
                ToolButton{
                    text: "↺"
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
