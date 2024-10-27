#include "FiveWayButton.h"

#define POLL_INTERVAL 250
// TODO: re-order correctly...
#define LEFT_PIN 1
#define RIGHT_PIN 2
#define UP_PIN 3
#define DOWN_PIN 19
#define PUSH_PIN 20

static const char inputSourceName[] = "FiveWayButton"; // should match "allow input source" string

FiveWayButton::FiveWayButton() : concurrency::OSThread(inputSourceName) {
    inputBroker->registerSource(this);
}

int32_t FiveWayButton::runOnce() {

    // Consider normally high vs normally low...
    if (digitalRead(LEFT_PIN) == HIGH) {
        sendKey(LEFT);

    } else if (digitalRead(RIGHT_PIN) == HIGH) {
        sendKey(RIGHT);
    } else if (digitalRead(DOWN_PIN) == HIGH) {
        sendKey(DOWN);
    } else if (digitalRead(UP_PIN) == HIGH) {
        sendKey(UP);
    } else if (digitalRead(PUSH_PIN) == HIGH) {
        sendKey(OK);
    }
    return POLL_INTERVAL;
}


void FiveWayButton::sendKey(KeyType key)
{
    InputEvent e;
    e.source = inputSourceName;
    e.inputEvent = key;
    notifyObservers(&e);
}
