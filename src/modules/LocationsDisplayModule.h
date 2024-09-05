#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

// TODO LocationDisplayModule:
// --> ?? test??

// TODO DiplayMessagesModule:
// --> store timestamps
// --> implement a ring buffer?
// --> ask for new render when message comes in?


// TODO SendMessageModule:
// --> ?? test?

class LocationsDisplayModule : public Observable<const UIFrameEvent *>, private concurrency::OSThread {
   

    public:
        LocationsDisplayModule() : concurrency::OSThread("location_display_module")  {}
        bool shouldDraw();
        void requestFocus();
        CallbackObserver<LocationsDisplayModule, const InputEvent *> inputObserver =
            CallbackObserver<LocationsDisplayModule, const InputEvent *>(this, &LocationsDisplayModule::handleInputEvent);
        int handleStatusUpdate(const meshtastic::Status *arg);
    protected:
        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
        virtual int32_t runOnce() override;
    private:
        bool shouldDisplay = true;
};

extern LocationsDisplayModule *locationsDisplayModule;