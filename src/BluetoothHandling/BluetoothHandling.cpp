#include "BluetoothHandling.h"


BluetoothManager::BluetoothManager( ):
    connectedDevice{},
    connectingDevice{}
{

}
BluetoothStatus BluetoothManager::status= BluetoothStatus::DISCONNECTED;

void BluetoothManager::init(const String& localName){

    atdpSource.set_local_name(localName.c_str());
    atdpSource.set_on_connection_state_changed(handleConnectionChanged);
    atdpSource.set_ssid_callback(searchSSID);
    atdpSource.set_data_callback(audioDataCallback);
    atdpSource.set_discovery_mode_callback(handleDiscoveryStateChanged);
    
}
bool BluetoothManager ::searchSSID(const char* ssid, esp_bd_addr_t address, int rrsi){

    Serial.printf("New device found: %s, RSSI: %d\n", ssid, rrsi);
    BluetoothDevice device;
    device.name= ssid;
    device.connectionQuality = rrsi;
    memcpy(device.address, address, ESP_BD_ADDR_LEN);
    avaibleDevices.push_back(device);

}

void BluetoothManager ::startDiscovering(){

    if (status==BluetoothStatus::CONNECTED|| status==BluetoothStatus::CONNECTING|| status==BluetoothStatus::DISCOVERING ) 
        return;
    Serial.println("Started discovering avaible devices...");
    avaibleDevices.clear();
    atdpSource.start("Mp3 esp32");

}

void BluetoothManager::stopDiscovering(){

    if (status==BluetoothStatus::DISCOVERING)
        return;
        Serial.println("Stopping Bluetooth device discovery...");
    if (esp_bt_gap_cancel_discovery() != ESP_OK) {
        Serial.println("Failed to cancel discovery");
    }
}
void BluetoothManager:: handleConnectionChanged(esp_a2d_connection_state_t state, void *ptr){
  
    switch(state){
    case ESP_A2D_CONNECTION_STATE_DISCONNECTED:
        Serial.println("****disconnected*****");
        status= BluetoothStatus::DISCONNECTED;
        break;
    case ESP_A2D_CONNECTION_STATE_CONNECTING:
        Serial.println("****connecting*****");
        status =BluetoothStatus::CONNECTING;
        break;
    case ESP_A2D_CONNECTION_STATE_CONNECTED:
        Serial.println("****connected****");
        status= BluetoothStatus::CONNECTED;
        break;
    case ESP_A2D_CONNECTION_STATE_DISCONNECTING:
        Serial.println("****disconecting****");
        status =BluetoothStatus::DISCONNECTING;
        break;
  }

}
void BluetoothManager::handleDiscoveryStateChanged(esp_bt_gap_discovery_state_t state){
    switch(state){
        case ESP_BT_GAP_DISCOVERY_STOPPED:
            Serial.println("bluetooth stopped discovering");
            status= BluetoothStatus::STOPPEDDISCOVERING;
            break;
        default:
            Serial.println("bluetooth started discovering");
            status=BluetoothStatus::DISCOVERING;
            break;
    }

}
void BluetoothManager::connect(const BluetoothDevice& device){

    connectingDevice = device;
    status= BluetoothStatus::CONNECTING;
    atdpSource.connect_to(const_cast<uint8_t*>(device.address));
}

void BluetoothManager::disconnect(){

    connectedDevice={};
    status=BluetoothStatus::DISCONNECTED;
    atdpSource.set_connected(false);
}

int32_t BluetoothManager::audioDataCallback(uint8_t *data, int32_t bytes){

    if (!data || bytes<=0) {
        Serial.println("data or bytes arent valid");
        return 0;
    }

    // uin32_t result = audioProcesor.readAudioData(data,bytes);
    int32_t result =0;
    if (result==0){
        memset(data,0,bytes);
        return bytes;
    }
    return result;

}