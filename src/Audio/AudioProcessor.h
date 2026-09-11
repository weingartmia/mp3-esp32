#pragma once

#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H
#include <Arduino.h>

#include <AudioTools.h>
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "AudioTools/CoreAudio/AudioMetaData.h"
#include "AudioTools/Disk/AudioSourceSD.h"
#include <SD.h>
#include <SPI.h>

class AudioProcessor{
    public:
        AudioProcessor(const uint8_t csSDPin);
        void init();
        bool openFile(const String& filepath);
        void closeCurrentFile();
        void pauseCurrentFile();
        void playCurrentFile();
        double getCurrentTime();

        // void printMetaData(MetaDataType type, const char* str, int len);
        
        bool songHasEnded();
        void getMetaData(String path);

        int32_t readAudio(uint8_t* buffer, int32_t len);
        std::string metadata;
        static AudioProcessor*instance;
        

    private:

        const uint8_t _csSDPin;
        File _currentFile;

        MP3DecoderHelix mp3;
        EncodedAudioStream decoder;
        MetaDataOutput out;
        // MetaDataID3 i3d;


        uint64_t playedFrames = 0;

        bool _paused =false;

        int sampleRate = 44100;
        int channels = 2;



};

#endif