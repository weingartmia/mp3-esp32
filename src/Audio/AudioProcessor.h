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

struct Metadata{
    String album;
    String artist;
    String title;
};

struct Header{
    uint32_t sampleRate;
    uint32_t frames;
    uint32_t samplesPerFrame;



};

class AudioProcessor{
    public:
        AudioProcessor(const uint8_t csSDPin);
        bool init();
        bool openFile(const String& filepath);
        void closeCurrentFile();
        void pauseCurrentFile();
        void playCurrentFile();
        double getCurrentTime();
        double getTotalTime();
        void processFrame();

        // void printMetaData(MetaDataType type, const char* str, int len);
        
        bool songHasEnded();
        // void getMetaData(String path);

        int32_t readAudio(uint8_t* buffer, int32_t len);
        Metadata metadata ={};
        static AudioProcessor*instance;
        

    private:
        uint32_t getXingOffset(int8_t version,uint32_t channelMode);
        const uint8_t _csSDPin;
        File _currentFile;

        MP3DecoderHelix mp3;
        EncodedAudioStream decoder;
        MetaDataOutput outMeta;
        MultiOutput out;
        StreamCopy metaCopier;
        // MetaDataID3 i3d;


        uint64_t playedFrames = 0;

        Header _header ={};

        bool _paused =false;

        int sampleRate = 44100;
        int channels = 2;



};

#endif