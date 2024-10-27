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
#define START_Y -0.65f

#define COMPASS_DIAM 40.0f

LocationsDisplayModule *locationsDisplayModule;

bool LocationsDisplayModule::shouldDraw() { return shouldDisplay; }


void LocationsDisplayModule::drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
    // TODO: half compass with friend locations as letters?
    // List of friend distances

    uint32_t now = getTime();

    Point bearing(START_X, START_Y);

    if (nodeDB == NULL) {
        display->drawString(16,16, "DB Null");
        return;
    }

    const auto* myNode = nodeDB->getMeshNode(nodeDB->getNodeNum());
    if (myNode == NULL) { display->drawString(16,16,"NULL"); return;}
    const auto& myPosition = myNode->position;
    if (compassModule == NULL) { display->drawString(16,16, "No compass"); return;}
    const auto& myBearing = compassModule->getBearing();
    float myFakeBearing = 0.0;


    if (nodeDB->meshNodes == NULL) {
        display->drawString(16,16, "No meshnodes"); return;
    }
    meshtastic_NodeInfoLite fakeNode;
    fakeNode.num = myNode->num + 1;
    fakeNode.last_heard = now;
    fakeNode.position.latitude_i = 423601000;
    fakeNode.position.longitude_i = 710589000;

    meshtastic_NodeInfoLite fakeNode2;
    fakeNode2.num = myNode->num + 2;
    fakeNode2.last_heard = now;
    fakeNode2.position.latitude_i = myPosition.latitude_i - 0.1;
    fakeNode2.position.longitude_i = myPosition.longitude_i - 0.1;

    meshtastic_NodeInfoLite fakeNodes[] = {fakeNode, fakeNode2};
    char name = 'a';
    int offset = 0;
    for (const meshtastic_NodeInfoLite& node : fakeNodes) {// *(nodeDB->meshNodes)) {
        // For friends seen recently
        if (node.num == myNode->num) { continue; }
        if ((int)(now - node.last_heard) < FRIEND_LIVENESS_TIME) {

            float bearingToFriend = GeoCoord::bearing(
                DegD(423601000), DegD(-710589000),
                //DegD(myPosition.latitude_i), DegD(myPosition.longitude_i), 
                DegD(node.position.latitude_i), DegD(node.position.longitude_i)
                );

            bearingToFriend -= myFakeBearing;

            // Find the appropriate point on the half compass
            bearing.rotate(-bearingToFriend);
            bearing.scale(COMPASS_DIAM);
            bearing.translate(0, 0);

            // Draw the first character of the friend's name.
            //display->drawString(bearing.x, bearing.y, String(name));//node.user.short_name[0]));
            display->drawString(4+offset, 4, String(bearing.x));
            display->drawString(4+offset, 12, String(bearing.y));
            display->drawString((int)bearing.x, (int)bearing.y, String(name));
            name++;
            offset += 30;

            
            // Reset the bearin for the next loop.
            bearing.x = START_X;
            bearing.y = START_Y;
        }
    }
    return;

    // Consider how to add a notification if a new message has arrived.

 }

void LocationsDisplayModule::requestFocus() {
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