#pragma once
#include "ProtobufModule.h"
#include "input/InputBroker.h"

#include <string>

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
        void changeMessage(const bool up);
        void sendText(NodeNum dest, ChannelIndex channel, const String* message);
        bool shouldDisplay = false;
        uint8_t messageIndex = 0;
        const int targetChannel = 0; // TODO!!!! What?
        uint8_t maxMessageIndex = 2;
        NodeNum targetNode;
        std::vector<String> precannedMessages = {"Message0", "Message1", "Message2"};
};

extern SendMessageModule *sendMessageModule;