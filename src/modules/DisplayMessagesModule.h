#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

class DisplayMessagesModule : public SinglePortModule, public Observable<const UIFrameEvent *>, private concurrency::OSThread {
    DisplayMessagesModule() : SinglePortModule("display_messages_module", meshtastic_PortNum_TEXT_MESSAGE_APP) {}
    bool shouldDraw();
    protected:
        virtual bool wantUIFrame() override { return this->shouldDraw(); }
        virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
    private:
        bool shouldDisplay = true;
}

extern DisplayMessagesModule *displaymessageModule;