#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H
#include "States.h"

class Context{

    private:
        State* state;

    public:
        void init();
        void currentStateAction();
        
};

#endif