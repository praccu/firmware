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

void SendMessageModule::setFocus(const char[] targetName) {
    shouldDisplay = true;
    displayString = targetName;
}

void changeMessage(const bool up) {
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

int SendMessageModule::handleInputEvent(const InputEvent *event) {
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            locationsDisplayModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            displayMessagesModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_SELECT)) {
            // TODO: send message!!
            sendMessage();
            shouldDisplay = false;
            locationsDisplayModule->setFocus();
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_UP)) {
            changeMessage(true);
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_DOWN)) {
            changeMessage(false);
        }

    }
 }
