#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

// TODO:
//  --> rewrite "PositionModule" to save state + display, scrap this. Or no,  just crib from it.
//  --> get location data aggregated
//  --> get messages
//  --> call request focus
//  --> write "draw frame"
//  --> handle input (add a new input module)
//  --> pass off to message display UI
//  --> prevent defaults from displaying
//  --> notification

// message display UI TODOs:
// --> start the class
// --> display the last message + the location
// --> add an input broker
// --> scroll message support
// --> scroll users support
// --> transition back to compass or to send message
// --> write draw frame

// send messsage UI TODOs:
// --> start the class
// --> hold a list
// --> add an input broker
// --> scroll messages
// --> send messages
// --> cancel (back to other module)

// Simple: Compass display with fake data. Then view messages with fake data. Then send messages with fake data.
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