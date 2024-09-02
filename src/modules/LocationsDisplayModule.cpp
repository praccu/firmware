#include "LocationsDisplayModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"
#include "modules/DisplayMessagesModule.h"

LocationsDisplayModule *locationsDisplayModule;

int DisplayeMessagesModule::handleStatusUpdate(const meshtastic::Status *arg) {
    if (arg->getNumTotal() != neighborHistory.size()) {
        // TODO: rewrite for this module
       this->neighborHistory[nodeDB->meshNodes->back().num] = std::move(new std::vector());
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
    display->drawString(16, 16, "HelloWorld");

    uint32_t now = getTime();

    Point baring(-0.04f, 0.65f); // TODO: proper values

    const auto& myNode = nodeDB->getMeshNode(nodeDB->getNodeNum());
    const auto& myPosition = myNode->position;
    const auto& myBearing = compassModule->getMyBearing();

    for (const meshtastic_NodeInfoLite& node : nodeDB->meshNodes) {
        if ((int)(now-node.last_heard) < 120) {

            // Calculates the bearing to friend, and then makes it relative to my bearing
            const float bearingToFriend = GeoCoord::bearing(
                DegD(myPosition.latitude_i), DegD(myPosition.longitude_i), 
                DegD(node.position.latitude_i), DegD(node.position.longitude_i)
                ) - myBearing;

            // TODO: rotate the bearing point based on bearingToFriend.
            // TODO: draw a friend rep at that point.
        }
    }


    // Consider the difficult of displaying timestamps...
    // Consider how to add a notification if a new message has arrived.

 }

void LocationsDisplayModule::setFocus() {
    this->shouldDisplay = true;
}

int LocationsDisplayModule::handleInputEvent(const InputEvent *event) {
    if (this->shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) {
            this->shouldDisplay = false;
            displayMessagesModule->setFocus();
        }
    }
 }

 int32_t LocationsDisplayModule::runOnce() {
    if (this->shouldDisplay) {
        this->requestFocus();
        UIFrameEvent e;
        e.action = UIFrameEvent::Action::REGENERATE_FRAMESET;
        this->notifyObservers(&e);
    }
    return 300;
 }