#include "BluetoothHandling.h"
#include "utils.h"

BluetoothManager* BluetoothManager::instance = nullptr;
std::vector<BluetoothDevice> BluetoothManager::avaibleDevices ={};

// BluetoothManager::BluetoothManager(AudioProcessor* audioProcessor,PlayerManager* player): 
//     audioProcessor(audioProcessor), 
//     player(player)
    
//     // connectedDevice{},
//     // connectingDevice{}
// {
//     instance = this;
// }
BluetoothManager::BluetoothManager()
{
    instance=this;
}
BluetoothStatus BluetoothManager::status= BluetoothStatus::DISCONNECTED;
// BluetoothManager* BluetoothManager :: instance= nullptr;

void BluetoothManager::init(const String& localName){
    
    atdpSource.set_local_name(localName.c_str());
    atdpSource.set_on_connection_state_changed(handleConnectionChanged);
    atdpSource.set_ssid_callback(searchSSID);
    atdpSource.set_data_callback(audioDataCallback);
    atdpSource.set_discovery_mode_callback(handleDiscoveryStateChanged);
    atdpSource.set_avrc_passthru_command_callback(buttonCommands);
    Serial.println("initiliazed bluetooth from BluetoothManager init");

    
}
bool BluetoothManager ::searchSSID(const char* ssid, esp_bd_addr_t address, int rrsi){

    Serial.printf("New device found: %s, RSSI: %d\n", ssid, rrsi);
        if (!instance || ssid == nullptr || strlen(ssid) == 0) {
        return false;
    }
    BluetoothDevice device;
    device.name= ssid;
    device.connectionQuality = rrsi;
    Serial.print(rrsi);
    memcpy(device.address, address, ESP_BD_ADDR_LEN);

    avaibleDevices.push_back(device);
    return true;

}

void BluetoothManager ::startDiscovering(){

    if (status==BluetoothStatus::CONNECTED|| status==BluetoothStatus::CONNECTING|| status==BluetoothStatus::DISCOVERING ) 
        return;
    Serial.println("Started discovering avaible devices...");
    avaibleDevices.clear();
    atdpSource.start("Mp3 esp32");

}

void BluetoothManager::stopDiscovering(){

    if (status!=BluetoothStatus::DISCOVERING)
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
bool BluetoothManager::connect(){// connect to selected device

    if (status==BluetoothStatus::DISCOVERING) stopDiscovering();
    Serial.print("connecting device with index");
    Serial.println(index);

    if (atdpSource.connect_to(const_cast<uint8_t*>(avaibleDevices[index].address))){
        Serial.println("Succesfully connected");
        return true;
    }
    status= BluetoothStatus::ERRORCONNECTING;
    return false;    
}

void BluetoothManager::disconnect(){// disconnect from current device

    atdpSource.set_connected(false);
}

int32_t BluetoothManager::audioDataCallback(uint8_t *data, int32_t bytes){// callback from a2dp source to process more audio

    if (!data || bytes<=0) {
        Serial.println("data or bytes arent valid");
        return 0;
    }

    int32_t result = instance->audioProcessor->readAudio(data, bytes);
    if (result==0){
        memset(data,0,bytes);
        return bytes;
    }
    return result;

}

void BluetoothManager:: buttonCommands(uint8_t key, bool isReleased){// handling external key pressed from connected device
    if (!isReleased) return;

    Serial.print("button was pressed from device.... ");
    switch (key) {
        case ESP_AVRC_PT_CMD_PLAY:     Serial.println("playing ");instance->player->play(); break;
        case ESP_AVRC_PT_CMD_PAUSE:    Serial.println("paused"); instance->player->pause(); break;
        case ESP_AVRC_PT_CMD_STOP:     Serial.println("stopped"); instance->player->stop(); break;
        case ESP_AVRC_PT_CMD_FORWARD:  Serial.println("next"); instance->player->next(); break;
        case ESP_AVRC_PT_CMD_BACKWARD: Serial.println("previous"); instance->player->previous(); break;
        case ESP_AVRC_PT_CMD_VOL_UP:   Serial.println("volume up"); instance->volumeUp(); break;
        case ESP_AVRC_PT_CMD_VOL_DOWN: Serial.println("volume down"); instance->volumeDown(); break;
        default: Serial.printf("Unknown: 0x%02X\n", key); break;
    }
}

void BluetoothManager:: volumeDown(){
    currentVolume -= VOLUME_STEP;

    if (currentVolume <= 0) currentVolume=0;
    setVolume();
}
void BluetoothManager::volumeUp(){
    currentVolume += VOLUME_STEP;

    if (currentVolume >= 127) currentVolume=127;
    setVolume();
}
void BluetoothManager::setVolume(){

    atdpSource.set_volume(currentVolume);
    if (status==BluetoothStatus::CONNECTED ) {
        esp_avrc_ct_send_set_absolute_volume_cmd(0, currentVolume);
    }

}

void BluetoothManager::increaseIndex(){
    if (index == avaibleDevices.size() -1 ) index=0;
    else index +=1;

}
void BluetoothManager::decreaseIndex(){
    if (index <= 0) index=avaibleDevices.size();
    else index -=1;

}