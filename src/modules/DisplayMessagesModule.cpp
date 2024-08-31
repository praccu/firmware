#include "DisplayMessagesModule.h"

#include "LocationsDisplayModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"




ProcessMessage DisplayMessagesModule::handleReceived(const meshtastic_MeshPacket &mp) {
    // TODO: handle appropriately...
    requestFocus();
    e.action = UIFrameEvent::Action::REGENERATE_FRAMESET;
    return ProcessMessage::CONTINUE;
}

bool DisplayMessagesModule::shouldDraw() { return shouldDisplay; }

void DisplayMessagesModule::setFocus() { shouldDisplay = true; }


void DisplayMessagesModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // TODO: Display a friend and their message and distance
    display->drawString(16, 16, "HelloWorld")
 }

void DisplayMessagesModule::setFocus() {
    shouldDisplay = true;
}

// TODO: when a new neighbor appears, add them here
// TODO: when a new message is received, add it to the history.
// TODO: cap history

int DisplayMessagesModule::handleInputEvent(const InputEvent *event) {
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_LEFT)) {
            shouldDisplay = false;
            locationsDisplayModule->setFocus();
            return 0;
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_SELECT)) {
            sendMessageModule->setFocus(targeNode);
        } // TODO: up + down

    }
 }
