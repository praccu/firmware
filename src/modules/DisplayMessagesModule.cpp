#include "DisplayMessagesModule.h"

#include "LocationsDisplayModule.h"
#include "SendMessageModule.h"
#include "NodeDB.h"
#include "gps/RTC.h"
#include "graphics/Screen.h"
#include "main.h"
#include "modules/NeighborInfoModule.h"
#include "MeshService.h"
#include <utility>

#define MAX_MESSAGES 12
#define DMM_BUFFER_SIZE 237

bool DisplayMessagesModule::wantPacket(const meshtastic_MeshPacket *p)
{
    return MeshService::isTextPayload(p);
}

ProcessMessage DisplayMessagesModule::handleReceived(const meshtastic_MeshPacket &mp) {
    // TODO: handle timestamping.
    char dmmTempBuf[DMM_BUFFER_SIZE];
    snprintf(dmmTempBuf, DMM_BUFFER_SIZE, "%s", mp.decoded.payload.bytes);
    const meshtastic_NodeInfoLite *node = nodeDB->getMeshNode(getFrom(&mp));
    const NodeNum nodeNum = node->num;
    for (std::pair<NodeNum, std::vector<String>>& neighbor : neighborHistory) {
        if (neighbor.first == nodeNum) {
            if (neighbor.second.size() == MAX_MESSAGES) {
                neighbor.second.pop_back();
            }
            // TODO: shuffling a pointer vector every insert isn't cheap...
            // Home brew a ring buffer?
            neighbor.second.insert(neighbor.second.begin(), dmmTempBuf);
            break;
        }
    }
}

int DisplayMessagesModule::handleStatusUpdate(const meshtastic::Status *arg) {
    if (arg->getStatusType() == STATUS_TYPE_NODE && nodeDB->getNumMeshNodes() != this->neighborHistory.size()) {
        std::vector<String> emptyMessages;
        neighborHistory.push_back(
            std::make_pair(
                nodeDB->meshNodes->back().num, 
                std::move(emptyMessages)
                )
            );
    }
    return 0;
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

int DisplayMessagesModule::handleInputEvent(const InputEvent *event) {
    if (shouldDisplay) {
        if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_LEFT)) {
            if (historyIndex == 0) {
                shouldDisplay = false;
                locationsDisplayModule->requestFocus();
            } else {
                historyIndex++;
            }
        } else if (event->inputEvent == static_cast<char>(meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT)) { 
            if (historyIndex < neighborHistory[displayedNeighborIndex].second.size() - 1) {
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
