from components.buttons_actions import handle_buttons
from user_utils.index import Selected
from mp3_player_rpi.user_utils.directory_navigation import DirectoryNavigater
from mp3_player_rpi.user_utils.player import Player

from signal import pause
ROOT='/home/pi/music'

selected=Selected()
player=Player(selected)
navigater = DirectoryNavigater(ROOT,selected)



while True:
    try:
        handle_buttons(player,navigater)
    except IOError as e:
        print(e)
    except KeyboardInterrupt:
        print('keyboard interrupt')