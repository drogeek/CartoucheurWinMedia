import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
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

    Rectangle{
        id: gridRect
        anchors.fill: parent
        GridView{
            id:grid
            anchors.fill: parent
            anchors.margins: 10
            model: DelegateModel{
                id: delegateModel
                model: CartridgeModel{}
                delegate: CartridgeDelegate{}
            }
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

            Component.onCompleted: {
            }


            displaced: Transition{
                NumberAnimation { property:"scale"; to:1 }
            }

        }
    }
}
