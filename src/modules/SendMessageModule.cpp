#include "SendMessageModule.h"

#include "graphics/Screen.h"
#include "main.h"

SendMessageModule *sendMessageModule;

ProcessMessage SendMessageModule::handleReceived(const meshtastic_MeshPacket &mp) {
    requestFocus();
    e.action = UIFrameEvent::Action::REGENERATE_FRAMESET;
    return ProcessMessage::CONTINUE;
}

bool SendMessageModule::shouldDraw() { return shouldDisplay; }


void SendMessageModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    display->drawString(16, 16, "HelloWorld")
 }

void SendMessageModule::setFocus(NodeNum dest) {
    shouldDisplay = true;
    targetNode = dest;
}

void SendMessageModule::changeMessage(const bool up) {
    if (up) {
        if (messageIndex == 0) { 
            messageIndex = maxMessageIndex; 
        } else {
            messageIndex--;
        }
    } else {
        if (messageIndex == maxMessageIndex) {
            messageIndex = 0;
        } else {
            messageIndex++;
        }
    }
}
void SendMessageModule::sendText(NodeNum dest, ChannelIndex channel, const char *message)
{
    meshtastic_MeshPacket *p = allocDataPacket();
    p->to = dest;
    p->channel = channel;
    p->want_ack = true;
    p->decoded.payload.size = strlen(message);
    memcpy(p->decoded.payload.bytes, message, p->decoded.payload.size);

    service->sendToMesh(
        p, RX_SRC_LOCAL,
        true);
}

int SendMessageModule::handleInputEvent(const InputEvent *event) {
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            locationsDisplayModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            displayMessagesModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_SELECT)) {
            sendMessage(targetNode, targetChannel, precannedMessages[messageIndex]);
            shouldDisplay = false;
            locationsDisplayModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_UP)) {
            changeMessage(true);
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_DOWN)) {
            changeMessage(false);
        }

    }
 }
