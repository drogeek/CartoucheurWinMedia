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
    width: 480
    height: 640
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
            gridModel.changePanel(currentIndex+1)
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
        property int alias
        id:grid
        anchors.fill: parent
        anchors.margins: 10
        clip: true
        model: DelegateModel{
            id: delegateModel
            model: CartridgeModel{}
            delegate: CartridgeDelegate{}
        }
        cellWidth: width/gridModel.widthModel
        cellHeight: height/gridModel.heightModel
        flow: GridView.FlowTopToBottom
        interactive: false
//        onDimensionChanged: console.log("dimension changed:("+xIndex+","+yIndex+")")
//        onWidthChanged: LogicUI.computeDimension()

//        signal dimensionChanged(int xIndex, int yIndex)
//        Connections{
//            onDimensionChanged: grid.model.fitToDimension(xIndex,yIndex)
//        }

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
