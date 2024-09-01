#include "LocationsDisplayModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"
#include "modules/DisplayMessagesModule.h"

LocationsDisplayModule *locationsDisplayModule;

ProcessMessage LocationsDisplayModule::handleReceived(const meshtastic_MeshPacket &mp) {
    // TODO: figure out how to handle location data
    requestFocus();
    e.action = UIFrameEvent::Action::REGENERATE_FRAMESET;
    return ProcessMessage::CONTINUE;
}

int DisplayeMessagesModule::handleStatusUpdate(const meshtastic::Status *arg) {
    if (arg->getNumTotal() != neighborHistory.size()) {
        // TODO: rewrite for this module
        neighborHistory[nodeDB->meshNodes->back().num] = std::move(new std::vector());
    }
    return 0;
}

void LocationDisplayModule::updatePosition(NodeNum nodeNum) {
    // TODO
}

bool LocationsDisplayModule::shouldDraw() { return shouldDisplay; }


void LocationsDisplayModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // TODO: half compass with friend locations as letters?
    // List of friend distances
    display->drawString(16, 16, "HelloWorld")

    // For Each node in DB
    // Get the relative location
    
    // Then, get the current heading per the compass (new module...)
    // Then, do the math to actually put these on the compass
    // Consider the difficult of displaying timestamps...

    // Proposed plan: pre-compute 31 vectors, and selectively write them as activated
    // Proposed plan: only display the end, then put a count or name below them?
    // Curiosity: how to select the right vector? Get heading, then pick the closest
 }

void LocationsDisplayModule::setFocus() {
    shouldDisplay = true;
}

int LocationsDisplayModule::handleInputEvent(const InputEvent *event) {
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            displayMessagesModule->setFocus();
        }
    }
 }