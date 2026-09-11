
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

    std::vector<String> paths;
    std::vector<String> names;
    

};

class DirectoryNavigater {
    public:
        DirectoryNavigater(const String& root);

        // void init();
        void openDirectory(const String dirname);
        void openNextDirectory();
        
        

        void setDirectoryPath();
        void removeDirectoryPath();
        void exitDirectory();
        void getDirectoryFiles();

        void increaseSelected();
        void decreaseSelected();

        bool dontHaveAlbum();
        bool isCurrrentSelectedDIrectory();

        String returnPath();

        std::optional<CurrentDirectory> currentDirectory;
        Selected selected;
        String parentDirectory;

    private:
        const String& root;
        void setExitParent();
        
        

};
#endif