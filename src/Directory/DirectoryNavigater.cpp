
#include "DirectoryNavigater.h"

DirectoryNavigater::DirectoryNavigater(const String& root): root(root){
    selected ={true,0};
    openDirectory(root);
}



void DirectoryNavigater::openDirectory(const String& dirname){
    File root = SD.open(dirname);
    if (!root || !root.isDirectory()) return;

    setDirectoryParent();

    currentDirectory.folder= root;
    currentDirectory.name = dirname;
    currentDirectory.files.clear();

    getDirectoryFiles();
} 

void DirectoryNavigater:: openNextDirectory(){
    const char* nextFolder= ;
    openDirectory(currentDirectory.files[selected.index].name().c_str());

}


void DirectoryNavigater::exitDirectory(){

    currentDirectory.folder.close();

    if (currentDirectory.name.c_str() == root.c_str()) return;
    openDirectory(parentDirectory.c_str());

}
void DirectoryNavigater::getDirectoryFiles(){

    File file = currentDirectory.folder.openNextFile();
    while (file) {
        
        if (file.isDirectory()) {
            Serial.printf("[%s]\n", file.name());
            
        } 
        else {
            Serial.printf("%s (%d)\n", file.name(), file.size());
        }
        currentDirectory.files.push_back(file);
        file = currentDirectory.folder.openNextFile();
    }

}

void DirectoryNavigater:: setDirectoryParent(){
    if (currentDirectory.name==root){
        parentDirectory.assign(root.c_str());//root
        return;
    }
    parentDirectory= currentDirectory.name.c_str();
}

void DirectoryNavigater::increaseSelected(){
    if (selected.index >= currentDirectory.files.size()) selected.index=0;
    else selected.index +=1;

}
void DirectoryNavigater::decreaseSelected(){
    if (selected.index <= 0) selected.index=currentDirectory.files.size();
    else selected.index -=1;

}