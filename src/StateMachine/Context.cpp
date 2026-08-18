#include "Context.h"

void Context::init(){
    this->state->setState(new ConnectionState());
}

void Context::currentStateAction(){
    this->state->handleAction();
}

