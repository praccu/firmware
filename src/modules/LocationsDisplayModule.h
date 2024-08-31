#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

// TODO LocationDisplayModule:
// --> draws a half-compass.
// --> draws friends' locations on the half-compass.
// --> holds a database of users and their positions.
// --> holds real heading from compass.

// TODO DiplayMessagesModule:
// --> holds a list of neighbors and message histories
// --> displays focused neighbor
// --> allows scrolling Left+Right through history
// --> "Select" transitions to SendMessageModule to focused user
// --> left returns to compass display at latest message??
// --> Up+Down scrolling through users.


// TODO SendMessageModule:
// --> Up+Down scroll through message options
// --> Select sends and then back to location display
// --> left goes back to friend list
// --> right goes back to compass

class LocationsDisplayModule : public SinglePortModule, public Observable<const UIFrameEvent *>, private concurrency::OSThread {
    CallbackObserver<LocationsDisplayModule, const InputEvent *> inputObserver =
        CallbackObserver<LocationsDisplayModule, const InputEvent *>(this, &LocationsDisplayModule::handleInputEvent);

    public:
        LocationsDisplayModule() : SinglePortModule("locations_display_module", meshtastic_PortNum_TEXT_MESSAGE_APP) {}
            bool shouldDraw();
    protected:
        virtual bool wantUIFrame() override { return this->shouldDraw(); }
        virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
    private:
        bool shouldDisplay = true;
};

extern LocationsDisplayModule *locationsDisplayModule;