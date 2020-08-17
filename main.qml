import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    visible: true
    width: 640
    height: 480
    maximumHeight: height
    maximumWidth: width
    title: qsTr("Blowoff Controller")

    Frame {
        id: settingsFrame
        x: 9
        y: 6
        width: 622
        height: 102

        ComboBox {
            id: serialBox
            x: 0
            y: 42
            width: 256
            height: 40
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            model: bovController.portList
        }

        Button {
            id: serialConnect
            x: 262
            y: 42
            text: qsTr("Connect")
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                bovController.serialConnect(serialBox.currentText)
            }
        }
    }

    Frame {
        id: manualControlFrame
        x: 9
        y: 114
        width: 622
        height: 162

        Button {
            id: startButton
            x: 162
            y: 49
            text: qsTr("Start BOV")
            onClicked: {
                bovController.blowoffStart()
            }
        }

        Button {
            id: endButton
            x: 268
            y: 49
            text: qsTr("End BOV")
            onClicked: {
                bovController.blowoffEnd()
            }
        }

        Button {
            id: flutterButton
            x: 374
            y: 49
            text: qsTr("Flutter BOV")
            onClicked: {
                bovController.blowoffFlutter()
            }
        }
    }

    Frame {
        id: integrationFrame
        x: 9
        y: 282
        width: 622
        height: 186

        TextField {
            id: oauthField
            x: 107
            y: 61
            echoMode: TextInput.Password
            placeholderText: qsTr("")
            onTextChanged: {
                twController.oathToken = oauthField.text
            }
        }

        Text {
            id: element
            x: 2
            y: 72
            text: qsTr("OAuth Token:")
            font.pixelSize: 15
        }

        Button {
            id: twConnectButton
            x: 498
            y: 122
            text: qsTr("Connect")
            onClicked: {
                twController.startConnect()
            }
        }

        Text {
            id: channelText
            x: 2
            y: 18
            text: qsTr("Channel Id:")
            font.pixelSize: 15
        }

        TextField {
            id: channelField
            x: 107
            y: 7
            placeholderText: qsTr("")
            onTextChanged: {
                twController.channelId = channelField.text
            }
        }
    }
}
