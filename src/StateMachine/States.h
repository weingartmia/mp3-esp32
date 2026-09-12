#pragma once 


#ifndef STATES_H
#define STATES_H

#include <Arduino.h>
class Context;

class State{
    public:
    

        virtual void handleAction() =0 ;
        virtual void handleInputs()=0;
        virtual void onConnected() =0;
        virtual void onDisconnected() = 0;

        void setContext(Context* newCon);

        // bool firstEnter= true;
        // bool onEnter();
        


        virtual ~State()=default;
    protected:
        Context* con= nullptr;
        
};
#endif
class ConnectionChangeState: public State{
    public:
        
        virtual void handleAction() =0 ;
        virtual void handleInputs()=0;
        virtual void onConnected() =0;
        virtual void onDisconnected() = 0;

        void passiveConnection();
        

        bool isConnected = false;
        ~ConnectionChangeState() override = default;     

};
class ConnectionState : public ConnectionChangeState{
    public:
        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;
        void onDisconnected() override;
        ~ConnectionState() override = default;


};
class ErrorState : public ConnectionChangeState{
    public:
        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;
        void onDisconnected() override;
        ~ErrorState() override = default;


};

class LoadingState:public State{
    public:
        void handleInputs() override ;
        void handleAction() override;
        void onConnected() override {}
        void onDisconnected() override{}
        ~LoadingState() override = default;

};

class SelectingState: public ConnectionChangeState{
    private:
        void openFile();
    public:

        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;
        void onDisconnected() override;
        ~SelectingState() override= default;


};

class PlayingState: public ConnectionChangeState{
    private:
        
        bool _onEnter =true;
        int _playedSongIndex;
        double _totalTime;

        String convertToMinutes(double time);
        void stopPlaying();

    public:
        void handleInputs() override;
        void handleAction() override;
        void onConnected() override;
        void onDisconnected() override;
        ~PlayingState() override= default;

};
