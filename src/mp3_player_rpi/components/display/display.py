from PIL import Image,ImageDraw,ImageFont
from mp3_player_rpi.components.display.display_settings.SH1106 import SH1106

class Display(SH1106):
    def __init__(self,navigater,selected,player):
        super().__init__(navigater,selected,player)
        self.navigater=navigater
        self.selected=selected
        self.player=player
        self.font20 =None
        self.font13 =None
        self.draw=None

    def prepare_display(self):
        self.Init()
        self.clear()

        image = Image.new('1', (self.width, self.height), "WHITE")
        self.draw = ImageDraw.Draw(image)

        self.font20 = ImageFont.truetype('Font.ttf', 20)
        self.font13 = ImageFont.truetype('Font.ttf',13)

    def draw_directory(self):
        height=5
        for index,entry in enumerate(self.navigater.current_directory):
            print(entry)

            if (index== self.selected.index):
                self.draw.text((30,height*index), '|>', font = self.font13, fill = 1)

                self.draw.text((35,height*index), entry, font = self.font13, fill = 0)
            else:
                self.draw.text((25,height*index), '>', font = self.font13, fill = 0)
                
                self.draw.text((30,height*index), entry, font = self.font13, fill = 0)

    def draw_playing(self):
        self.draw.text((30,10), self.player.music[self.selected.index], font = self.font13, fill = 1)
        
        


