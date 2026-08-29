#include "Context.h"

void Context::init(){

    this->state =new ConnectionState();
    Serial.println("initiliazed context from Context init()");
}

void Context::currentStateAction(){
    // Serial.println("----context handling action currrentStateAction()");
    this->state->handleAction();
}

