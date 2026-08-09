from pathlib import Path
from index import selected

class DirectoryNavigater():
    def __init__(self,root,selected):
        self.current_directory = Path(root)
        self.selected= selected
        

    def set_directory_path(self,directory):
        if self.current_directory[self.selected.index].is_dir():
            self.current_directory = self.current_directory / directory
            self.selected.index=0
        

    def return_from_directory(self):
        if self.current_directory.parent != self.current_directory:
            self.current_directory =self.current_directory.parent
        self.selected.index=0

    def show_directory(self) ->list:
        return list(self.current_directory.iterdir())

    
    def increase_selected(self):
        if self.selected.check_incresable(list(self.current_directory.iterdir())):
            self.selected.increase_index()

    def decrease_selected(self):
        if self.selected.check_decreasable():
            self.selected.decrease_index()

