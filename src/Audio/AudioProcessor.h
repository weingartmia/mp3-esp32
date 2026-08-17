
#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H
#include <Arduino.h>

#include <AudioTools.h>
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include <SD.h>

class AudioProcessor{
    public:
        AudioProcessor(const uint8_t csSDPin);
        void init();
        bool openFile(const String& filepath);
        void closeCurrentFile();
        int32_t readAudio(uint8_t* buffer, int32_t len);
    private:
        const uint8_t _csSDPin;
        File _currentFile;
        MP3DecoderHelix mp3;
        EncodedAudioStream decoder;


};

#endif