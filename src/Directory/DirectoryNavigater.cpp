
#include "DirectoryNavigater.h"

DirectoryNavigater::DirectoryNavigater(const String& root): root(root)
{
    selected ={true,0};
    currentDirectory = CurrentDirectory{};
    currentDirectory->path= "/";
    
}




void DirectoryNavigater::openDirectory(const String dirname){
    Serial.println(dirname);
    File root = SD.open(dirname);
    if (!root || !root.isDirectory()) return;

    if (!currentDirectory->folder || !currentDirectory->folder.isDirectory()) {parentDirectory= root.path();}
    

    Serial.println("directory path: "+ currentDirectory->path);
    selected.index=0;

    currentDirectory.reset();
    currentDirectory->folder= root;


    currentDirectory->paths.clear();
    currentDirectory->names.clear();

    getDirectoryFiles();
} 

void DirectoryNavigater:: openNextDirectory(){
    Serial.println("current selected folder: "+String(currentDirectory->paths[selected.index]));
    parentDirectory= currentDirectory->folder.path();
    setDirectoryPath();
    openDirectory(currentDirectory->path);  

}


void DirectoryNavigater::exitDirectory(){

    currentDirectory->folder.close();

    if (String(currentDirectory->folder.name()) == root) return;

    removeDirectoryPath();
    
    openDirectory(currentDirectory->path);

}
void DirectoryNavigater::getDirectoryFiles(){
    while (true) {
        File file = currentDirectory->folder.openNextFile();
        if (!file) {
            Serial.println("blocking form file");
            currentDirectory->folder.rewindDirectory();
            break;
        }

        if (String(file.name())!="System Volume Information")  {
            currentDirectory->paths.push_back(file.path());
            
            currentDirectory->names.push_back(file.name());
        }
        file.close();      
    }

}

void DirectoryNavigater:: setDirectoryPath(){
    currentDirectory->path= String(currentDirectory->paths[selected.index]);
    Serial.println("current directory path: "+ currentDirectory->path);
   
}

void DirectoryNavigater:: removeDirectoryPath(){
    Serial.println(parentDirectory);
    Serial.println("parent directory: " + String(parentDirectory));
    currentDirectory->path= String(parentDirectory);
    
}

void DirectoryNavigater::increaseSelected(){
    if (selected.index >= currentDirectory->paths.size()) selected.index=0;
    else selected.index +=1;

}
void DirectoryNavigater::decreaseSelected(){
    if (selected.index <= 0) selected.index=currentDirectory->paths.size();
    else selected.index -=1;

}

String DirectoryNavigater:: returnPath(){

    return currentDirectory->path;
}

bool DirectoryNavigater:: dontHaveAlbum(){
    // if (String(parentDirectory.c_str()) == root){
    //     Serial.println("parent diretory of selected file is root - isnt in album");
    //     return true;
    // }
    return false;

}