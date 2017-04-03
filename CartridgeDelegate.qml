import QtQuick 2.0
import QtQuick.Controls 2.1


MouseArea{
    property var currentIndex: index
    property var backgroundCellAlias: backgroundCell
    property var playerState: false
    id: mouseArea
    height: grid.cellHeight-2
    width: grid.cellWidth-2
    onPressed:{
        console.log(backgroundCell.state)
        if(grid.state === "MOVEMODE"){
            grid.currentIndex = index
            grid.currentItem.state = "ITEMSELECTEDFORMOVE"
        }
    }
    onReleased: {
        backgroundCell.Drag.drop()
        if(mouseArea.state === "ITEMSELECTEDFORMOVE"){
            backgroundCell.parent = mouseArea
            grid.currentItem.state = ""
            console.log(grid.currentItem.state)
        }
    }

    onClicked: {
        mouseArea.playerState = !mouseArea.playerState;
        console.log(mouseArea.playerState)
        root.playerCommand((index)%gridModel.heightModel + 1,Math.floor((index)/gridModel.heightModel) + 1,mouseArea.playerState)
    }

    onPressAndHold:{
        if(grid.state === ""){
            grid.state = "MOVEMODE"
        }
        else if(grid.state === "MOVEMODE"){
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
//            PropertyChanges{
//                target: backgroundCell
//                parent: gridRect
//            }
        },
        State{
            name: ""
            onCompleted: console.log("MODE DEFAULT")
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
        property var backgroundcolorNORMAL: "#FAFAFA"
        property var backgroundcolorDISABLED: "#AAAAAA"
        property var backgroundcolorINDICATOR: "#E65C00"
        property var backgroundcolorPLAY: "#FFB366"
        color: grid.enabled ? backgroundcolorNORMAL : backgroundcolorDISABLED
        border.color: "#CCCCCC"

        Rectangle{
            id: timeIndicator
            color: parent.backgroundcolorINDICATOR
            radius: parent.radius/2
            anchors.left: parent.left
            height: parent.height-1
            width: parent.width-(parent.width*timeDisplay.currentDuration)/(stop-start)
            Behavior on width{
                NumberAnimation{
                    easing.type: Easing.InOutElastic
                    duration: 1000/stretch
                }
            }
        }

        Behavior on color{

            ColorAnimation {
                duration: 2000
            }
        }

        SequentialAnimation{
            loops: Animation.Infinite
            running: grid.state === "MOVEMODE"
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
            running: grid.state === ""
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
                //TODO
                console.log("source:"+dropArea.drag.source.currentIndex)
                console.log("dest:"+index)
                gridModel.swap(dropArea.drag.source.currentIndex, index)
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
                id: timeDisplay

                function formateHour(s){
                    var milliToSec=Math.floor(s/1000)
                    var sec=milliToSec%60
                    var min=Math.floor(milliToSec/60)
                    var hour=Math.floor(min/60)
                    return {
                        sec: padding(sec),
                        min: padding(min),
                        hour: padding(hour)
                    }
                }

                function padding(x){
                    return ("00"+x).slice(-2)
                }

                property var currentDuration : stop-start;
                property var formatedHour : formateHour(currentDuration)
                font.family: "Helvetica"
                font.pointSize: 18
                text: stop ? "<b><i>"+ formatedHour.hour + ":" + formatedHour.min + ":" + formatedHour.sec + "</i></b>" : ""

                Timer{
                    running: backgroundCell.state == "PLAY"
                    onTriggered: {
                        var newDuration = timeDisplay.currentDuration-1000
                        if(newDuration > 0)
                            timeDisplay.currentDuration=newDuration
                        else
                            timeDisplay.currentDuration=0
                    }
                    interval: 1000/stretch
                    repeat: true
                }
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
                    text: (index)%gridModel.heightModel + 1
                }
            }

            states:State{
                when: grid.state === "MOVEMODE"
                PropertyChanges {
                    target: controlRow
                    enabled: false
                }
            }
        }

        states: [
            State{
                name: "PLAY"
                PropertyChanges{
                    target: backgroundCell
                    color: backgroundCell.backgroundcolorPLAY
                }
            },
            State{
                name: ""
                PropertyChanges{
                    target: timeDisplay
                    currentDuration: stop-start
                }
            }

        ]
        transitions: [
            Transition {
                from: ""
                to: "PLAY"
                ColorAnimation {
                    loops: Animation.Infinite
                    to: backgroundCell.backgroundcolorPLAY
                    duration: 1000/stretch
                    easing.type: Easing.InOutBack
                }
            }
        ]
    }

}
