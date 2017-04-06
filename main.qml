import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 2.1 as Quick
import QtQuick.Layouts 1.0
import QtQml.Models 2.2

import org.winmedia.guiennet 1.0

ApplicationWindow {
    property alias gridModel: delegateModel.model
    id: root
    visible: true
    width: 1360
    height: 768
    title: qsTr("Cartridge")

    signal playerCommand(int row, int column, bool state);

    header:
    RowLayout{
        ToolBar{
//            width: parent.width
            ToolButton{
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/menu.png"
                }
                onClicked: optionsMenu.open()
                Menu{
                    id:optionsMenu
                    title: "File"
                    MenuItem{
                        text: "Settings"
                        onTriggered: console.log("Propreties")
                    }
                }
            }
        }

        TabBar{
            id: tab
//            width: parent.width
            onCurrentIndexChanged: {
                console.log("tab n°"+currentIndex+" selected")
                console.log("id:"+panelItem.itemAt(currentIndex).idTab)
                gridModel.changePanel(panelItem.itemAt(currentIndex).idTab)
                grid.state = ""
            }

            Repeater{
                id: panelItem
                model: PanelModel{}
                TabButton{
                    property var idTab: id
                    text: name
                    width: 100
                }
            }
        }
    }

    Rectangle{
        id: gridRect
        anchors.fill: parent

        GridView{
            enabled: false
            id:grid
            anchors.fill: parent
            anchors.margins: 10
            model: DelegateModel{
                id: delegateModel
                model: CartridgeModel{}
                delegate: CartridgeDelegate{}
            }
            delegate: CartridgeDelegate{}
            cellWidth: width/gridModel.widthModel
            cellHeight: height/gridModel.heightModel
            flow: GridView.FlowTopToBottom
            interactive: false

            states: [
                State{
                    name: "MOVEMODE"
                    onCompleted: console.log("STATE MOVEMODE")
                }
            ]

            displaced: Transition{
                NumberAnimation { property:"scale"; to:1 }
            }

            Connections{
                target: Connection
                onCommandReceived: {
                    console.log(params.row)
                    console.log(params.column)
                    console.log(params.state)
                    grid.currentIndex=(params.column-1)*gridModel.heightModel+params.row-1
                    grid.currentItem.backgroundCellAlias.state= params.state ? "PLAY" : ""
                 }
                onDisconnected: {
                    grid.enabled = false;
                    disconnectionPopup.opacity = 1
//                    grid.state = "DISABLED"
                }
                onConnected: {
                    grid.enabled = true;
                    disconnectionPopup.opacity = 0
                }
            }
        }
        Rectangle{
            id: disconnectionPopup
            opacity: 1
            anchors.centerIn: parent
            width: 300
            height: 100
            radius: 4
            border.color: "grey"
            border.width: 2
            Text {
                anchors.centerIn: parent
                text: qsTr("<b>Disconnected</b>")
            }
            Behavior on opacity {
                NumberAnimation{
                    duration: 2000
                    easing.type: Easing.OutQuint
                }
            }
        }
    }

}
