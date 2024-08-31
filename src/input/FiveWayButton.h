#include "InputBroker.h"
#include "concurrency/OSThread.h"
#include "main.h"
#include "modules/CannedMessageModule.h"


class FiveWayButton : public Observable<const InputEvent *>, public concurrency::OSThread { 
    public:
        FiveWayButton();
    private:
        int32_t runOnce() override;
    typedef enum {
        UP = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_UP,
        DOWN = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_DOWN,
        LEFT = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_LEFT,
        RIGHT = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_RIGHT,
        OK = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_SELECT,
        CANCEL = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_CANCEL,
        NO_PRESS = meshtastic_ModuleConfig_CannedMessageConfig_InputEventChar_NONE
    } KeyType;

}

extern FiveWayButton *fiveWayButton;