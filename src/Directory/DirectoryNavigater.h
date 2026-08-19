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

        void init();
        void openDirectory(const String& dirname);
        void openNextDirectory();
        

        void setDirectoryParent();
        void exitDirectory();
        void getDirectoryFiles();

        void increaseSelected();
        void decreaseSelected();

        bool dontHaveAlbum();

        String returnPath();

        CurrentDirectory currentDirectory;
        Selected selected;
        std::string parentDirectory;

    private:
        const String& root;
        
        

};