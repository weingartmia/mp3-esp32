
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
        void pauseCurrentFile();
        void playCurrentFile();
        double getCurrentTime();
        
        bool songHasEnded();
        double getMP3Duration(File& file);

        int32_t readAudio(uint8_t* buffer, int32_t len);
        

    private:

        

        const uint8_t _csSDPin;
        File _currentFile;
        MP3DecoderHelix mp3;
        EncodedAudioStream decoder;
        uint64_t playedFrames = 0;
        bool _paused =false;

        int sampleRate = 44100;
        int channels = 2;



};

#endif