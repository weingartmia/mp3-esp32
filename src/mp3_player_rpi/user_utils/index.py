from components.buttons_actions import reverse_callbacks

class Selected:
    def __init__(self):
        self.index=0
        self.mode = 'select'

    def change_mode(self):
        reverse_callbacks()
        if self.mode=='select':
            self.mode='play'
        elif self.mode =='play':
            self.mode='select'

    def increase_index(self):
        self.index=+1

    def decrease_index(self):
        self.index=-1

    def check_incresable(self,list) -> bool:
        if (self.index -1)== len(list):
            return False
        else: 
            return True
        
    def check_decreasable(self) -> bool:
        if self.index== 0:
            return False
        else: 
            return True

