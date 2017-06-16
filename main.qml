import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 2.1 as Quick
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQml.Models 2.2

import org.winmedia.guiennet 1.0

ApplicationWindow {
    property alias gridModel: delegateModel.model
    id: root
    visible: true
    height: 685
    width: 384
    title: qsTr("Cartridge")

    header:
    Row{
        width: parent.width
        ToolBar{
            z : 3
            width:40
            ToolButton{
                contentItem: Rectangle{
                    color: "white"
                    anchors.fill: parent
                    Image {
                        anchors.centerIn: parent
                        fillMode: Image.Pad
                        horizontalAlignment: Image.AlignHCenter
                        verticalAlignment: Image.AlignVCenter
                        source: "images/menu.png"
                    }
                }
                onClicked: {
                    optionDialog.refreshLocalIps()
                    optionsMenu.open()
                }
            }

            Quick.Dialog{
                id: optionsMenu
                title: qsTr("settings")
                x: Math.round((root.width - width) / 2)
                y: Math.round((root.height - height) / 2)
                width: Math.round(Math.min(root.width, root.height) / 3 * 2)
                modal: true
                focus: true
                standardButtons: Dialog.Ok | Dialog.Cancel
                onAccepted: {
                    Options.port = serverportinput.value
                    console.log(localIpsList.currentItem.text)
                    Options.ip = localIpsList.currentItem.text
                    Options.persistConfig();
                    optionsMenu.close()
                }

                contentItem:
                ColumnLayout{
                    id: optionDialog
                    function refreshLocalIps(){
                        localIps.clear()
                        Options.getLocalIps().forEach(function(elem){
                                localIps.append({cell: elem})
                            })
                    }

                    ListModel{
                        id:localIps
                    }

                    Component{
                        id:localIpsDelegate
                        MouseArea{
                            property string text : localIpsWrapperText.text
                            id: localIpsWrapper
                            height: 30
                            width: 200
                            Text{
                                id: localIpsWrapperText
                                anchors.centerIn: parent
                                color: localIpsWrapper.ListView.isCurrentItem ? "#fff" : "#000"
                                font.bold: localIpsWrapper.ListView.isCurrentItem ? true : false
                                text: cell
                            }
                            onClicked: localIpsList.currentIndex = index
                        }
                    }

                    Label{
                        id: localIpsLabel
                        text: qsTr("Local ip adresses")
                    }
                    Button{
                        id: localIpsRefreshBtn
                        text: qsTr("Refresh")
                        onClicked: optionDialog.refreshLocalIps()
                    }

                    ListView{
                        id: localIpsList
                        model: localIps
                        height: 100
                        delegate: localIpsDelegate
                        highlight: Rectangle {
                            color: "lightgray"
                            radius: 2
                        }
                    }



                    Label{
                        text: qsTr("Server port")
                    }

                    SpinBox{
                        id: serverportinput
                        editable: true
                        from: 0
                        to: 65535
                        value: Options.port
                    }
                }
            }
        }

        TabBar{
            id: tab
            z : 2
            width: root.width-40
            onCurrentIndexChanged: {
                console.log("tab n°"+currentIndex+" selected")
                console.log("id:"+panelItem.itemAt(currentIndex).idTab)
                StateKeeper.clear()
                gridModel.changePanel(panelItem.itemAt(currentIndex).idTab)
                grid.state = ""
            }

            Repeater{
                id: panelItem
                model: PanelModel{
                    notifier: Notifier
                }
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
        color:Qt.darker("lightgray",1.1)

        GridView{
            enabled: Notifier.connected
            id:grid
            anchors.fill: parent
            anchors.margins: 10
            model: DelegateModel{
                id: delegateModel
                model: CartridgeModel{
                    notifier: Notifier
                }
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

            displaced: Transition{
                NumberAnimation { property:"scale"; to:1 }
            }

            Connections{
                target: Notifier
                onCommandReceived: {
                    console.log(params.row)
                    console.log(params.column)
                    console.log(params.state)
                    grid.currentIndex=(params.column-1)*gridModel.heightModel+params.row-1
                    var id = grid.currentItem.itemId
                    if(params.state){
                        grid.currentItem.backgroundCellAlias.state = "PLAY"
                        StateKeeper.insert(id,grid.currentItem.timeDisplayAlias.currentTime)
                    }
                    else{
                        grid.currentItem.backgroundCellAlias.state = ""
                        StateKeeper.remove(id)
                    }
                 }
                onNewError:{
                    textErr.text = err
                }

                /*
                onDisconnected: {
                    grid.enabled = false;
                    disconnectionPopup.opacity = 1
//                    grid.state = "DISABLED"
                }
                onConnected: {
                    grid.enabled = true;
                    disconnectionPopup.opacity = 0
                }
                */
            }
        }
        Rectangle{
            id: disconnectionPopup
            opacity: grid.enabled ? 0 : 1
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
    footer: Text{
        id: textErr
        onTextChanged: {
            if(textErr.text != "")
                timerErr.running = true
        }
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        color: "red"
        text: ""
    }
    Timer{
        id: timerErr
        running: false
        onTriggered: {
            textErr.text = ""
            timerErr.running = false
        }
        repeat: false
        interval: 3000
    }

}
