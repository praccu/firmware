#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

// TODO: add the node DB, keep a message history here...

class DisplayMessagesModule : public SinglePortModule, public Observable<const UIFrameEvent *>, private concurrency::OSThread {
    public:
        DisplayMessagesModule() : SinglePortModule("display_messages_module", meshtastic_PortNum_TEXT_MESSAGE_APP) {}
        bool shouldDraw();
        void setFocus();
    protected:
        virtual bool wantUIFrame() override { return this->shouldDraw(); }
        virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
    private:
        bool shouldDisplay = false;
        NodeNum targetNode;
        std::unordered_map<NodeNum, std::vector<char[]>> neighborHistory;
        // TODO: add a friend data base and a displayedIndex.
}

extern DisplayMessagesModule *displaymessageModule;