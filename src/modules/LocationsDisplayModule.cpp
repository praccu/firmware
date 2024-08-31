#include "LocationsDisplayModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"
#include "modules/DisplayMessagesModule.h"

LocationsDisplayModule *locationsDisplayModule;

ProcessMessage LocationsDisplayModule::handleReceived(const meshtastic_MeshPacket &mp) {
    requestFocus();
    e.action = UIFrameEvent::Action::REGENERATE_FRAMESET;
    return ProcessMessage::CONTINUE;
}

bool LocationsDisplayModule::shouldDraw() { return shouldDisplay; }


void LocationsDisplayModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // TODO: half compass with friend locations as letters?
    // List of friend distances
    display->drawString(16, 16, "HelloWorld")
 }

int LocationsDisplayModule::handleInputEvent(const InputEvent *event) {
    // TODO: if the event is right, go to the message display screen
    // TODO: add a new input handler, given it Special input event types.
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            shouldDisplay = false;
            displayMessagesModule->setFocus();
        }
    }
 }
