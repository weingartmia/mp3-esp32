
#pragma once
#ifndef DIRECTORYNAVIGATER_H
#define DIRECTORYNAVIGATER_H
#include <Arduino.h>
#include <SD.h>
#include <vector>
#include <string>
#include <optional>

struct Selected{
   
    bool isDirectory;
    int index;
};

struct CurrentDirectory{
    File folder;
    String path;

    std::vector<File> files;

};

class DirectoryNavigater {
    public:
        DirectoryNavigater(const String& root);

        // void init();
        void openDirectory(const String dirname);
        void openNextDirectory();
        
        

        void setDirectoryPath();
        void exitDirectory();
        void getDirectoryFiles();

        void increaseSelected();
        void decreaseSelected();

        bool dontHaveAlbum();

        String returnPath();

        std::optional<CurrentDirectory> currentDirectory;
        Selected selected;
        std::string parentDirectory;

    private:
        const String& root;
        
        

};
#endif