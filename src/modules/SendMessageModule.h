#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

class SendMessageModule : public SinglePortModule, Observable<const UIFrameEvent *>, private concurrency::OSThread {
        public:
        SendMessageModule() : SinglePortModule("send_message_module", meshtastic_PortNum_TEXT_MESSAGE_APP), OSThread("send_message_module") {}
        bool shouldDraw();
        void setFocus(NodeNum dest);
        CallbackObserver<SendMessageModule, const InputEvent *> inputObserver =
            CallbackObserver<SendMessageModule, const InputEvent *>(this, &SendMessageModule::handleInputEvent);
    protected:
        virtual bool wantUIFrame() override { return this->shouldDraw(); }
        virtual ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override;

        void drawFrame(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
        int handleInputEvent(const InputEvent *event);
    private:
        void sendText(NodeNum dest, ChannelIndex channel, const char *message);
        bool shouldDisplay = false;
        uint8_t messageIndex = 0;
        const targetChannel = 0; // TODO!!!! What?
        char[][] precannedMessages = {"Message0", "Message1", "Message2"};
        uint8_t maxMessageIndex = precannedMessages.size() - 1;
        NodeNum targetNode;
};

extern SendMessageModule *sendMessageModule;