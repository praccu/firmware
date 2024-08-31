#include "FiveWayButton.h"

#define POLL_INTERVAL 250;
// TODO: get the correct pins!!!
#define LEFT_PIN 0;
#define RIGHT_PIN 1;
#define UP_PIN 2;
#define DOWN_PIN 3;
#define PUSH_PIN 4;

static const char inputSourceName[] = "FiveWayButton"; // should match "allow input source" string

FiveWayButton::FiveWayButton() : concurrency::OSThread(inputSourceName) {
    inputBroker->registerSource(this);
}

int32_t FiveWayButton::runOnce() {
    // TODO: convert to action
    // TODO: condition action on screen? Or just let the screens pick it up.

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
        sendKey(SELECT);
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
