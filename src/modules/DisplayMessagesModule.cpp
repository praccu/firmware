#include "DisplayMessagesModule.h"

#include "LocationsDisplayModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"




ProcessMessage DisplayMessagesModule::handleReceived(const meshtastic_MeshPacket &mp) {

    e.action = UIFrameEvent::Action::REGENERATE_FRAMESET;
    return ProcessMessage::CONTINUE;
}

int DisplayeMessagesModule::handleStatusUpdate(const meshtastic::Status *arg) {
    // TODO: figure out how to update our message history object cleanly...
    // TODO: also fix the history and neighbor indexes as needed...
}

bool DisplayMessagesModule::shouldDraw() { return shouldDisplay; }

void DisplayMessagesModule::setFocus() { shouldDisplay = true; }


void DisplayMessagesModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    if (displayedNeighborIndex >= neighborHistory.size() || historyIndex >= neighborHistory[displayedNeighborIndex].second.size()) {
        display->drawString(16, 16, "No messages.");
    } else {
        display->drawString(16, 16, neighborHistory[displayedNeighborIndex].second[historyIndex]);
    }
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
            if (historyIndex == 0) {
                shouldDisplay = false;
                locationsDisplayModule->setFocus();
            } else {
                historyIndex++;
            }
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) { 
            if (historyIndex < neighborHistory[displayedNeighborIndex].size() - 1) {
                historyIndex++;
            }
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_SELECT)) {
            if (displayedNeighborIndex < neighborHistory.size()) {
                shouldDisplay = false;
                sendMessageModule->setFocus(neighborHistory[displayedNeighborIndex].first);\
            }
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_UP)) {
            historyIndex = 0;
            if (displayedNeighborIndex == 0) {
                displayedNeighborIndex = neighborHistory.size() - 1;
            } else {
                displayedNeighborIndex--;
            }
        } 
        else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_DOWN)) {
            historyIndex = 0;
            if (displayedNeighborIndex == neighborHistory.size() - 1) { 
                displayedNeighborIndex = 0;
            } else {
                displayedNeighborIndex++;
            }

        } 
    }
    return 0;
 }
