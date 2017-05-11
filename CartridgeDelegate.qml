import QtQuick 2.0
import QtQuick.Controls 2.1


MouseArea{
    property var itemId: id ? id : -1
    property var currentIndex: index
    property var backgroundCellAlias: backgroundCell
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
        //TODO: playerstate necessary?
        var nextState = mouseArea.state === "PLAY" ? false : true
        Notifier.sendRami((index)%gridModel.heightModel + 1,Math.floor((index)/gridModel.heightModel) + 1,nextState)
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
            opacity: 0.6
            color: parent.backgroundcolorINDICATOR
            radius: parent.radius
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height-2
            width: parent.width-(parent.width*timeDisplay.currentDuration)/(stop-start)
            Behavior on width{
                NumberAnimation{
                    easing.type: Easing.Linear
                    easing.amplitude: 2
                    duration: stretch ? 1000/stretch : 0
                }
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
                gridModel.swap(
                            dropArea.drag.source.currentIndex,
                            index,
                            dropArea.drag.source.itemId,
                            itemId
                            )
            }
        }

        Column{
            width: parent.width

            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                elide: Text.ElideRight
                fontSizeMode: Text.HorizontalFit
                text: performer ? "<b>"+performer+"</b>" : ""
            }

            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width
                elide: Text.ElideRight
                fontSizeMode: Text.HorizontalFit
                text: title ? title : ""
            }

            Text{
                id: timeDisplay
                width: parent.width
                fontSizeMode: Text.HorizontalFit

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

                function getCurrentDuration(){
                    if(StateKeeper.contains(id)){
                        backgroundCell.state = "PLAY"
                        return StateKeeper.get(id)
                    }
                    return stop-start;
                }

                property var currentDuration : getCurrentDuration();
                property var formatedHour : formateHour(currentDuration)
                font.family: "Helvetica"
                font.pointSize: 11
                text: stop ? "<b><i>"+ formatedHour.hour + ":" + formatedHour.min + ":" + formatedHour.sec + "</i></b>" : ""

                Timer{
                    triggeredOnStart: true
                    running: backgroundCell.state == "PLAY"
                    onTriggered: {
                        var newDuration = timeDisplay.currentDuration-1000
                        if(newDuration > 0){
                            timeDisplay.currentDuration=newDuration
                            StateKeeper.insert(id,timeDisplay.currentDuration)
                        }
                        else
                            timeDisplay.currentDuration=0
                    }
                    interval: 1000/stretch
                    repeat: true
                }
            }


        }

//        Row{
//            id: controlRow
            Rectangle{
                id: numberIndicator
                opacity: 0.6
                anchors.bottom: backgroundCell.bottom
                color: Qt.darker(backgroundCell.color)
                width: Math.min(4 + grid.cellWidth/8,20)
                height: width
                radius: 5
                Text{
                    width: parent.width
                    height: parent.height
                    fontSizeMode: Text.Fit
                    text: (index)%gridModel.heightModel + 1
                }
            }

//            states:State{
//                when: grid.state === "MOVEMODE"
//                PropertyChanges {
//                    target: controlRow
//                    enabled: false
//                }
//            }
//        }

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
                    from: backgroundCell.backgroundcolorNORMAL
                    to: backgroundCell.backgroundcolorPLAY
                    duration: 1000/stretch
                    easing.type: Easing.InOutBack
                }
            }
        ]
    }

}
