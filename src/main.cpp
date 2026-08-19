#include <Arduino.h>
#include "StateMachine/Context.h"

Context context; 

void setup() {
  Serial.begin(115200);
  context.init();
}
void loop() {
 context.currentStateAction();
}

