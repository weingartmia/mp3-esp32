#include <Arduino.h>
#include <SD.h>
#include <vector>
#include <string>

struct Selected{
   
    bool isDirectory;
    int index;
};

struct CurrentDirectory{
    File folder;
    String name;
    std::vector<File> files;

};

class DirectoryNavigater {
    public:
        DirectoryNavigater(const String& root);

        void openDirectory(const String& dirname);
        void setDirectoryParent();
        void exitDirectory();
        void getDirectoryFiles();

        void increaseSelected();
        void decreaseSelected();

        CurrentDirectory currentDirectory;
        Selected selected;

    private:
        const String& root;
        std::string parentDirectory;
        

};