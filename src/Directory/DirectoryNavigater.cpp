
#include "DirectoryNavigater.h"

DirectoryNavigater::DirectoryNavigater(const String& root): root(root){
    selected ={true,0};
    openDirectory(root);
}



void DirectoryNavigater::openDirectory(const String& dirname){
    File root = SD.open(dirname);
    if (!root || !root.isDirectory()) return;
    
    setDirectoryParent();
    selected.index=0;
    currentDirectory.folder= root;
    currentDirectory.name = dirname;
    currentDirectory.files.clear();

    getDirectoryFiles();
} 

void DirectoryNavigater:: openNextDirectory(){
    openDirectory(String(currentDirectory.files[selected.index].name()));

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

String DirectoryNavigater:: returnPath(){

    int currentIndex= selected.index;
    String parent =String(parentDirectory.c_str());
    String filePath;
    if (dontHaveAlbum) 
        filePath = root+ currentDirectory.name + currentDirectory.files[currentIndex].name();
    else
        filePath = root + parent  + currentDirectory.name + currentDirectory.files[currentIndex].name();// root + autor directory+ album directory+ current selected song
    Serial.println(filePath);
    return filePath;
}

bool DirectoryNavigater:: dontHaveAlbum(){
    if (String(parentDirectory.c_str()) == root){
        Serial.println("parent diretory of selected file is root - isnt in album");
        return true;
    }
    return false;

}