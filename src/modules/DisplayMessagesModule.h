#pragma once
#include "ProtobufModule.h"
#include "NodeDB.h"
#include "input/InputBroker.h"

// TODO: add the node DB, keep a message history here...

class DisplayMessagesModule : public SinglePortModule, public Observable<const UIFrameEvent *>, private concurrency::OSThread {
     CallbackObserver<DisplayMessagesModule, const meshtastic::Status *> nodeStatusObserver =
        CallbackObserver<DisplayMessagesModule, const meshtastic::Status *>(this, &DisplayMessagesModule::handleStatusUpdate);
    public:
        DisplayMessagesModule() : SinglePortModule("display_messages_module", meshtastic_PortNum_TEXT_MESSAGE_APP), concurrency::OSThread("display_messages_module") {
             nodeStatusObserver.observe(&nodeStatus->onNewStatus);
        }
        bool shouldDraw();
        void setFocus();
        virtual bool wantPacket(const meshtastic_MeshPacket *p) override;
    protected:
        virtual bool wantUIFrame() override { return this->shouldDraw(); }
        virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
        int handleStatusUpdate(const meshtastic::Status *arg);
    private:
        bool shouldDisplay = false;
        uint16_t displayedNeighborIndex = 0;
        uint16_t historyIndex = 0;
        std::vector<std::pair<NodeNum, std::vector<String>>> neighborHistory;
};

extern DisplayMessagesModule *displayMessagesModule;