#ifndef BLUETOOTHANDLING_H
#define BLUETOOTHHANDLING_H

#include <Arduino.h>
#include <vector>
#include "BluetoothA2DPSource.h"

struct BluetoothDevice{
    String name;
    esp_bd_addr_t address;
    int connectionQuality;
};

enum class BluetoothStatus{
    CONNECTED,
    DISCONNECTED,
    CONNECTING,
    DISCONNECTING,
    DISCOVERING,
    STOPPEDDISCOVERING
};

class BluetoothManager {
    public:
        BluetoothManager();
        
        void init(const String& localName);
        void startDiscovering();
        void stopDiscovering();
        static bool searchSSID(const char* ssid, esp_bd_addr_t address, int rrsi);
        void connect(const BluetoothDevice& device);
        void disconnect();
        static void handleConnectionChanged(esp_a2d_connection_state_t state, void *ptr);
        static void handleDiscoveryStateChanged(esp_bt_gap_discovery_state_t state);

        static int32_t audioDataCallback(uint8_t *data, int32_t bytes);

    private:
        static BluetoothStatus status;
        static std::vector<BluetoothDevice> avaibleDevices;
        BluetoothA2DPSource atdpSource;
        BluetoothDevice connectedDevice;
        BluetoothDevice connectingDevice;
        
        
        
};

#endif

