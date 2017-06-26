import QtQuick 2.0
import QtQuick.Controls 2.1
import QtGraphicalEffects 1.0


MouseArea{
    property var itemId: id ? id : -1
    property var currentIndex: index
    property var backgroundCellAlias: backgroundCell
    property var timeDisplayAlias: timeDisplay
    id: mouseArea
    height: grid.cellHeight-2
    width: grid.cellWidth-2
    onPressed:{
        console.log(backgroundCell.state)
        if(grid.state === "MOVEMODE" && backgroundCell.state !== "PLAY"){
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
        var nextState = backgroundCell.state === "PLAY" ? false : true
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
        function decimalToHexString(number){
            if (number < 0)
            {
                number = 0xFFFFFFFF + number + 1;
            }

            return "#"+number.toString(16).substring(2);
        }
        property var backgroundcolorNORMAL: backcolor != undefined ? decimalToHexString(backcolor) : "white"
        property var backgroundcolorDISABLED: "#AAAAAA"
        property var backgroundcolorINDICATOR: "#E65C00"
        property var backgroundcolorPLAY: "#FFB366"
        property var backgroundcolorPRESSED: "#eee"
        property var backgroundcolorBLINK: "orange"
        color: !grid.enabled ? backgroundcolorDISABLED : "lightgray"

        Rectangle{
            id: backgroundCellInner
            width: parent.width-6
            height: parent.height-6
            anchors.centerIn: parent
            color: "#eee"

            LinearGradient{
                id: buttonGradient
                opacity: backcolor ? 1 : 0
                width: parent.width
                height: parent.height
                gradient: Gradient{
                    GradientStop{
                        position: 1
                        color: Qt.darker(backgroundCell.backgroundcolorNORMAL,1.5)
                    }
                    GradientStop{
                        position: 0
                        color: Qt.lighter(backgroundCell.backgroundcolorNORMAL,1.5)
                    }

                }
            }

            LinearGradient{
                id: buttonGradientPlay
                opacity: backgroundCell.state == "PLAY" ? 1 : 0
                anchors.centerIn: parent
                width: parent.width
                height: parent.height
                gradient: Gradient{
                    GradientStop{
                        position: 1
                        color: Qt.lighter(backgroundCell.backgroundcolorNORMAL,1.5)
                    }
                    GradientStop{
                        position: 0
                        color: Qt.darker(backgroundCell.backgroundcolorNORMAL,1.5)
                    }

                }
                Rectangle{
                    id: timeIndicator
                    opacity: 0.4
                    color: "#222"
                    radius: 4
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height
                    width: parent.width-parent.width*timeDisplay.currentDuration/(stop-start)


                    Behavior on width{
                        NumberAnimation{
                            easing.type: Easing.Linear
                            easing.amplitude: 2
                            duration: stretch ? 1000/stretch : 1000
                        }
                    }
                }
                Rectangle{
                    id: blinkIndicator
                    opacity: 0
                    anchors.fill: parent
                    color: backcolor
                }
           }
            Column{
                id: textColumn
                width: parent.width
                property string textcolor: "#111"

                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: textColumn.textcolor
                    width: parent.width
                    elide: Text.ElideRight
                    fontSizeMode: Text.HorizontalFit
                    text: performer ? "<b>"+performer+"</b>" : ""
                }

                Text{
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: textColumn.textcolor
                    width: parent.width
                    elide: Text.ElideRight
                    fontSizeMode: Text.HorizontalFit
                    text: title ? title : ""
                }

                Text{
                    id: timeDisplay
                    color: textColumn.textcolor
                    x:15
                    width: parent.width-15
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
                            //TODO: try to do something less disgusting bellow…
                            return StateKeeper.get(id)
                        }
                        return stop-start;
                    }





                    property var currentDuration : getCurrentDuration();
                    property var formatedHour : formateHour(currentDuration)
                    font.family: "Times"
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
            Text{
                    function convertGenre(x){
                        if(x === 0)
                            return "Start"
                        else if(x === 1)
                            return "Loop"
                        else if(x === 2)
                            return "Intro"
                        else if(x === 3)
                            return "Hook"
                    }
                    function splitGenre(x){
                        if(x >= 8){
                            console.log("loop activated")
                            x-= 8
                            genreDisplay.isLoop = true
                        }
                        if(x >= 4){
                            console.log("lock activated")
                            x-=4
                            genreDisplay.isLock = true
                        }
                        console.log(x)
                        genreDisplay.genreRemaining = x
                    }

                    property bool isLoop: false
                    property bool isLock: false
                    property int genreRemaining: undefined
                    id: genreDisplay
                    font.family: "Times"
                    anchors.bottom: parent.bottom
                    x: (parent.width-width)/2
                    text: genre !== undefined ? "<b>"+convertGenre(genreRemaining)+"</b>" : ""
            }
            Image {
                id: imageLock
                anchors.bottom:  genreDisplay.bottom
                anchors.left: genreDisplay.right
                opacity: genreDisplay.isLock ? 1 : 0
                source: "lock.png"
            }
            Image {
                id: imageLoop
                anchors.bottom:  imageLock.bottom
                anchors.left: imageLock.right
                opacity: genreDisplay.isLoop ? 1 : 0
                source: "loop.png"
            }
            Component.onCompleted: genreDisplay.splitGenre(genre)

        }


        border.color: "black"
//        scale: state === "PLAY" ? 0.96 : 1.0
        transform: Scale{
            id: scaleButton
            origin.x: backgroundCell.width/2
            origin.y: backgroundCell.height/2
//            xScale: backgroundCell.state === "PLAY" ? 0.97 : 1.00
//            yScale: backgroundCell.state === "PLAY" ? 0.96 : 1.00
        }





        SequentialAnimation{
            loops: Animation.Infinite
            running: grid.state === "MOVEMODE" && backgroundCell.state !== "PLAY"

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
            running: grid.state === "" && backgroundCell.state !== "PLAY"
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
                when: dropArea.containsDrag && backgroundCell.state !== "PLAY"
                PropertyChanges{
                    target: backgroundCell
                    color: "#AAAAAA"
                }
            }

            onDropped: {
                console.log("source:"+dropArea.drag.source.currentIndex)
                console.log("dest:"+index)
                if(backgroundCell.state !== "PLAY"){
                    gridModel.swap(
                                dropArea.drag.source.currentIndex,
                                index,
                                dropArea.drag.source.itemId,
                                itemId
                                )
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
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
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
            },
            State{
                name: ""
                PropertyChanges{
                    target: timeDisplay
                    currentDuration: stop-start
                }
            }

        ]
        PropertyAnimation{
            target: blinkIndicator
            running: backgroundCell.state == "PLAY"
            properties:  "opacity"
            loops: Animation.Infinite
            from: 0.0
            to: 0.4
            duration: 1000 / stretch
            easing.type: Easing.InOutBack
        }
        transitions: [
            Transition {
                from: ""
                to: "PLAY"
                PropertyAnimation{
                    target: scaleButton
                    properties: "xScale"
                    from: 1.0
                    to: 0.98
                    duration: 300
                    easing.type: Easing.Linear
                }
                PropertyAnimation{
                    target: scaleButton
                    properties: "yScale"
                    from: 1.0
                    to: 0.96
                    duration: 300
                    easing.type: Easing.Linear
                }

                PropertyAnimation{
                    target: backgroundCell
                    properties: "border.color"
                    from: "black"
                    to: "#eee"
                    duration: 300
                    easing.type: Easing.Linear
                }
            },
            Transition {
                from: "PLAY"
                to: ""
                PropertyAnimation{
                    target: scaleButton
                    properties: "xScale"
                    from: 0.98
                    to: 1
                    duration: 300
                    easing.type: Easing.InOutBack
                }
                PropertyAnimation{
                    target: scaleButton
                    properties: "yScale"
                    from: 0.96
                    to: 1
                    duration: 300
                    easing.type: Easing.InOutBack
                }
                PropertyAnimation{
                    target: buttonGradientPlay
                    properties: "opacity"
                    from: 1
                    to: 0
                    duration: 300
                    easing.type: Easing.InOutBack
                }
                PropertyAnimation{
                    target: backgroundCell
                    properties: "border.color"
                    from: "#eee"
                    to: "black"
                    duration: 300
                    easing.type: Easing.InOutBack
                }

            }
        ]
    }

}
