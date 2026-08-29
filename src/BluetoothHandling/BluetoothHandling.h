#pragma once
#ifndef BLUETOOTHANDLING_H
#define BLUETOOTHHANDLING_H

#include <Arduino.h>
#include <vector>
#include "BluetoothA2DPSource.h"
#include "Audio/AudioProcessor.h"
#include "Player/PlayerManager.h"

struct BluetoothDevice{
    String name;
    esp_bd_addr_t address;
    int connectionQuality;
};


enum class BluetoothStatus{
    CONNECTED,
    ERRORCONNECTING,
    DISCONNECTED,
    CONNECTING,
    DISCONNECTING,
    DISCOVERING,
    STOPPEDDISCOVERING
};

class BluetoothManager {
    public:
        // BluetoothManager(AudioProcessor* audioProcessor,PlayerManager* player);
        BluetoothManager();

        void init(const String& localName);
        void startDiscovering();
        void stopDiscovering();
        
        bool connect();
        void disconnect();

        void volumeUp();
        void volumeDown();
        void setVolume();

        static bool searchSSID(const char* ssid, esp_bd_addr_t address, int rrsi);
        static void handleConnectionChanged(esp_a2d_connection_state_t state, void *ptr);
        static void handleDiscoveryStateChanged(esp_bt_gap_discovery_state_t state);
        static int32_t audioDataCallback(uint8_t *data, int32_t bytes);
        static void buttonCommands(uint8_t key, bool isReleased);


        static std::vector<BluetoothDevice> avaibleDevices;
        static BluetoothStatus status;
        uint8_t currentVolume;

        void increaseIndex();
        void decreaseIndex();
        int index=0;


    private:
        static BluetoothManager* instance;

        

        BluetoothA2DPSource atdpSource;
        // BluetoothDevice connectedDevice;
        // BluetoothDevice connectingDevice;

        AudioProcessor* audioProcessor;
        PlayerManager* player;
        
        
        
};

#endif

