#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

class Context{

    private:
        State* state;

    public:
        void init();
        void currentStateAction();
        void setState(State* state);


};

#endif