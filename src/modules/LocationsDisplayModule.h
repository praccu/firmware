#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

// TODO LocationDisplayModule:
// --> draws a half-compass.
// --> draws friends' locations on the half-compass.
// --> holds a database of users and their positions.
// --> holds real heading from compass.

// TODO DiplayMessagesModule:
// --> store timestamps
// --> implement a ring buffer?
// --> ask for new render when message comes in?


// TODO SendMessageModule:
// --> ?? test?

// TODO magnetometer etc ???
class LocationsDisplayModule : public SinglePortModule, public Observable<const UIFrameEvent *>, private concurrency::OSThread {
   

    public:
        LocationsDisplayModule() : SinglePortModule("locations_display_module", meshtastic_PortNum_TEXT_MESSAGE_APP) {}
        bool shouldDraw();
        void setFocus();
        void updatePosition(NodeNum nodeNum);
        CallbackObserver<LocationsDisplayModule, const InputEvent *> inputObserver =
            CallbackObserver<LocationsDisplayModule, const InputEvent *>(this, &LocationsDisplayModule::handleInputEvent);
    protected:
        virtual bool wantUIFrame() override { return this->shouldDraw(); }
        virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
    private:
        bool shouldDisplay = true;
};

extern LocationsDisplayModule *locationsDisplayModule;