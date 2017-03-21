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
                }
            }
        }
    }

    Rectangle{
        id: gridRect
        anchors.fill: parent
        GridView{
            property int alias
            id:grid
            anchors.fill: parent
            anchors.margins: 10
            model: delegateModel
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

        DelegateModel{
            id: delegateModel
            model: CartridgeModel{}
            delegate: dragDelegate
            items.includeByDefault: false
            function sort(){
                while(unorderedItems.count > 0){
                    var item = unorderedItems.get(0)
                    var index = item.model.position

                }
            }

            groups: [
                DelegateModelGroup {
                    id: unorderedItems
                    name: "unordered"
                    includeByDefault: true
                    onChanged: {
                        delegateModel.sort()
                    }
                }

            ]
        }
    }

/* ********************START OF DEFINITION OF DELEGATE ****************************
   *********************************************************************************** */
    Component{
        id: dragDelegate
        MouseArea{
            id: mouseArea
            height: grid.cellHeight-2
            width: grid.cellWidth-2
            onPressed:{
                if(grid.state == "MOVEMODE"){
                    grid.currentIndex = index
                    grid.currentItem.state = "ITEMSELECTEDFORMOVE"
                }
            }
            onReleased: {
                backgroundCell.Drag.drop()
                if(mouseArea.state == "ITEMSELECTEDFORMOVE"){
                    backgroundCell.parent = mouseArea
                    grid.currentItem.state = ""
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

            states:[
                State {
                    name: "ITEMSELECTEDFORMOVE"
                    onCompleted: console.log("MODE ITEMSELECTEDFORMOVE")
                    PropertyChanges{
                        target: grid.currentItem
                        border.width: 3
                        z: 100
                    }
                    PropertyChanges{
                        target: mouseArea
                        drag.target: backgroundCell
                    }
                    PropertyChanges{
                        target: dropArea
                        enabled: false
                    }
                    ParentChange{
                        target: backgroundCell
                        parent: gridRect
                    }
                },
                State{
                    onCompleted: console.log("MODE DEFAULT")
                    name: ""
                    PropertyChanges{
                        target: backgroundCell
                        x:0
                        y:0
                    }
                }

            ]
            Rectangle {
                id: backgroundCell
                height: grid.cellHeight-2
                width: grid.cellWidth-2
                radius: 5
                color: "#FAFAFA"
                border.color: "#CCCCCC"

                SequentialAnimation{
                    loops: Animation.Infinite
                    running: grid.state == "MOVEMODE"
                    NumberAnimation{
                        target: backgroundCell
                        property: "scale"; to: 0.95; duration: 300
                    }
                    NumberAnimation{
                        target: backgroundCell
                        property: "scale"; to: 1; duration: 1000; easing.type: Easing.OutBounce
                    }
                }



                PropertyAnimation{
                    running: grid.state == ""
                    target: backgroundCell
                    property: "scale"; to: 1; duration: 300
                }

                Drag.active: mouseArea.drag.active
                Drag.source: mouseArea
                Drag.hotSpot.x: backgroundCell.width/2
                Drag.hotSpot.y: backgroundCell.height/2


                DropArea{
                    id: dropArea
                    anchors.fill: parent
                    states: State{
                        when: dropArea.containsDrag
                        PropertyChanges{
                            target: backgroundCell
                            color: "#AAAAAA"
                        }
                    }

                    onDropped: {
                        console.log("dropped")
                        console.log("origin:"+drag.source.DelegateModel.itemsIndex)
                        console.log("destination:"+mouseArea.DelegateModel.itemsIndex)
                        delegateModel.items.move(
                                drag.source.DelegateModel.itemsIndex,
                                mouseArea.DelegateModel.itemsIndex)
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
                    id: controlRow
                    anchors.bottom: backgroundCell.bottom
                    Rectangle{
                        color: Qt.darker(backgroundCell.color)
                        width: 20
                        height: 20
                        radius: 5
                        anchors.bottom: parent.bottom
                        Text{
                            anchors.centerIn: parent
                            text: index
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
                    states:State{
                        when: grid.state == "MOVEMODE"
                        PropertyChanges{
                            target: controlRow
                            enabled: false
                        }
                    }
                }

            }
        }
    }
/* ********************************END DELEGATE**********************************
  ******************************************************************************* */
}


