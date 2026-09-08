
#include "DirectoryNavigater.h"

DirectoryNavigater::DirectoryNavigater(const String& root): root(root)
{
    selected ={true,0};
    currentDirectory = CurrentDirectory{};
    
}




void DirectoryNavigater::openDirectory(const String dirname){
    Serial.println(dirname);
    File root = SD.open("/"+dirname);
    if (!root || !root.isDirectory()) return;
    
    setDirectoryPath();
    selected.index=0;
    currentDirectory.reset();
    currentDirectory->folder= root;

    currentDirectory->files.clear();

    getDirectoryFiles();
} 

void DirectoryNavigater:: openNextDirectory(){
    // Serial.println(String(parentDirectory.c_str()));
    Serial.println("current selected: "+String(currentDirectory->files[selected.index].name()));
    setDirectoryPath();

    // if (String(currentDirectory->name) !=String(root.c_str()) &&  String(parentDirectory.c_str()) == String(root.c_str()) ){
    //     Serial.println("sub");
    //     openDirectory(String(currentDirectory->name)+"/"+String(currentDirectory->files[selected.index].name()));
    //     return;
    // }
    // else if (String(parentDirectory.c_str()) != String(root.c_str())){
    //     Serial.println("subsub");
    //     openDirectory(String(parentDirectory.c_str())+"/"+String(currentDirectory->name)+"/"+String(currentDirectory->files[selected.index].name()));
    //     return;

    // }

    openDirectory(currentDirectory->path);


    

}


void DirectoryNavigater::exitDirectory(){

    currentDirectory->folder.close();

    if (String((currentDirectory->folder.name()).c_str()) == String(root.c_str())) return;
    
    openDirectory(String(parentDirectory.c_str()));

}
void DirectoryNavigater::getDirectoryFiles(){

    File file = currentDirectory->folder.openNextFile();
    while (file) {
        
        // if (file.isDirectory()) {
        //     // Serial.printf(" folder name: [%s]\n ", file.name());
            
        // } 
        // else {
        //     // Serial.printf("file name,size : %s (%d)\n", file.name(), file.size());
        // }
        if (String(file.name())!="System Volume Information")  currentDirectory->files.push_back(file);
    
        file = currentDirectory->folder.openNextFile();
    }

}

void DirectoryNavigater:: setDirectoryPath(){

    int lastSlash = currentDirectory->path.lastIndexOf('/');
    String lastPath= currentDirectory->path.substring(lastSlash,currentDirectory->path.length());
    Serial.println(lastPath);

    if (lastPath!= currentDirectory->folder.name()){
        currentDirectory->path=currentDirectory->path.concat(currentDirectory->folder.name());
    }
   

    
    // if (String(currentDirectory->name.c_str())==String(root)){
    //     Serial.println("root");
    //     parentDirectory.assign(root.c_str());//root
    //     return;
    // }
    // parentDirectory.assign(currentDirectory->name.c_str());
    // Serial.println("parent: "+String(parentDirectory.c_str()));
}

void DirectoryNavigater::increaseSelected(){
    if (selected.index >= currentDirectory->files.size()) selected.index=0;
    else selected.index +=1;

}
void DirectoryNavigater::decreaseSelected(){
    if (selected.index <= 0) selected.index=currentDirectory->files.size();
    else selected.index -=1;

}

String DirectoryNavigater:: returnPath(){

    // int currentIndex= selected.index;
    // String parent =String(parentDirectory.c_str());
    // String filePath;
    // if (dontHaveAlbum()) 
    //     filePath = root+ currentDirectory->name +"/"+ currentDirectory->files[currentIndex].name();
    // else
    //     filePath = root + parent  +"/"+ currentDirectory->name +"/"+ currentDirectory->files[currentIndex].name();// root + autor directory+ album directory+ current selected song
    
    return currentDirectory->path;
}

bool DirectoryNavigater:: dontHaveAlbum(){
    if (String(parentDirectory.c_str()) == root){
        Serial.println("parent diretory of selected file is root - isnt in album");
        return true;
    }
    return false;

}