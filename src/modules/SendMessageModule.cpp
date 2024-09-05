#include "SendMessageModule.h"

#include "graphics/Screen.h"
#include "modules/LocationsDisplayModule.h"
#include "main.h"
#include "modules/DisplayMessagesModule.h"
#include "MeshService.h"

SendMessageModule *sendMessageModule;


bool SendMessageModule::shouldDraw() { return shouldDisplay; }


void SendMessageModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    display->drawString(16, 16, "HelloWorld");
 }

void SendMessageModule::setFocus(NodeNum dest) {
    shouldDisplay = true;
    targetNode = dest;
}

void SendMessageModule::changeMessage(const bool up) {
    if (up) {
        if (this->messageIndex == 0) { 
            this->messageIndex = this->maxMessageIndex; 
        } else {
            this->messageIndex--;
        }
    } else {
        if (this->messageIndex == this->maxMessageIndex) {
            this->messageIndex = 0;
        } else {
            this->messageIndex++;
        }
    }
}
void SendMessageModule::sendText(NodeNum dest, ChannelIndex channel, const String* message)
{
    meshtastic_MeshPacket *p = allocDataPacket();
    p->to = dest;
    p->channel = channel;
    p->want_ack = true;
    p->decoded.payload.size = strlen(message->c_str());
    memcpy(p->decoded.payload.bytes, message->c_str(), p->decoded.payload.size);

    service->sendToMesh(
        p, RX_SRC_LOCAL,
        true);
}

int SendMessageModule::handleInputEvent(const InputEvent *event) {
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            locationsDisplayModule->requestFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            displayMessagesModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_SELECT)) {
            sendText(targetNode, this->targetChannel, &this->precannedMessages[messageIndex]);
            shouldDisplay = false;
            locationsDisplayModule->requestFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_UP)) {
            this->changeMessage(true);
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_DOWN)) {
            changeMessage(false);
        }

    }
 }
