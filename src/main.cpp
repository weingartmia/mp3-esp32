#include <Arduino.h>
#include "StateMachine/Context.h"

Context* context; 

void setup() {
  Serial.begin(115200);
  context= new Context();
  context->init();
  Serial.print("init Context from main setup");
}
void loop() {
  context->currentStateAction();
  context->handleConnectionChange();
}

