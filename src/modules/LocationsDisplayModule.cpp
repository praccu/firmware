#include "LocationsDisplayModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"
#include "modules/DisplayMessagesModule.h"
#include "modules/CompassModule.h"

#define FRIEND_LIVENESS_TIME 120
// TODO: proper values.
#define START_X -0.04f
#define START_Y 0.65f

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

    Point bearing(START_X, START_Y);

    const auto& myNode = nodeDB->getMeshNode(nodeDB->getNodeNum());
    const auto& myPosition = myNode->position;
    const auto& myBearing = compassModule->getMyBearing();

    for (const meshtastic_NodeInfoLite& node : nodeDB->meshNodes) {
        // For friends seen recently
        if ((int)(now-node.last_heard) < FRIEND_LIVENESS_TIME) {
            float bearingToFriend = GeoCoord::bearing(
                DegD(myPosition.latitude_i), DegD(myPosition.longitude_i), 
                DegD(node.position.latitude_i), DegD(node.position.longitude_i)
                );

            bearingToFriend -= myBearing

            // Find the appropriate point on the half compass
            bearing.rotate(-bearingToFriend);
            bearing.scale(compassDiam);
            bearing.translate(compassX, compassY);

            // Draw the first character of the friend's name.
            display->drawString(bearing.x, bearing.y, string(node.user.short_name[0]));
            
            // Reset the bearin for the next loop.
            bearing.x = START_X;
            bearing.y = START_Y;
        }
    }


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