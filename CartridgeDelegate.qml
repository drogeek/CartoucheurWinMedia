import QtQuick 2.0
import QtQuick.Controls 2.1

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

    states:[
//        State {
//            name: "SELECTABLE"
//            when: grid.state == "MOVEMODE"
//            onCompleted: {
//                console.log("MODE SELECTABLE")
//            }
//        },
        State {
            name: "ITEMSELECTEDFORMOVE"
            onCompleted: {console.log("MODE ITEMSELECTEDFORMOVE")
                console.log(index)}
//            when: mouseArea.pressed && grid.state == "MOVEMODE"
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

    Drag.active: mouseArea.drag.active
    Drag.source: mouseArea
    Drag.hotSpot.x: backgroundCell.width/2
    Drag.hotSpot.y: backgroundCell.height/2
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        onPressed:{
            console.log(backgroundCell.state)
            if(grid.state == "MOVEMODE"){
                grid.currentIndex = index
                grid.currentItem.state = "ITEMSELECTEDFORMOVE"
            }
        }
        onReleased: {
            if(backgroundCell.state == "ITEMSELECTEDFORMOVE"){
                parent = backgroundCell
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
    }

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
