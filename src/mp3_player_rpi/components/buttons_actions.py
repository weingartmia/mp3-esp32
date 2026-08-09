from gpiozero import Button





up = Button(6,pull_up=True)
down = Button(19,pull_up=True)
left = Button(5,pull_up=True)
right = Button(26,pull_up=True)
press = Button(13,pull_up=True)

key1 = Button(21,pull_up=True)
key2 =Button(20,pull_up=True)
key3 = Button(16,pull_up=True)

def deactivated():
    print("deactivated")
    pass

activated_map :dict={'up': True,
                     'down': True,
                     'left':True,
                     'right':True,
                     'press':False,

                     'key2':False
                    }

def check_action(name,active_callback,passive_callback):
    print(f"button was pressed : {name}")
    if activated_map.get(name):
        active_callback()
    else: passive_callback()



def handle_buttons(player,navigater):

    up.when_activated = lambda: check_action('up',navigater.decrease_selected,deactivated)
    down.when_activated = lambda: check_action('down',navigater.increase_selected,deactivated)
    left.when_pressed= lambda: check_action('left',navigater.set_directory_path,player.play_past)
    right.when_pressed= lambda:check_action('right',navigater.set_directory_path,player.play_next)

    press.when_pressed =lambda:check_action('press', player.play_current,player.handle_pause_resume)
 

    key1.when_pressed = player.volume_up
    key2.when_pressed = lambda:check_action('key2',player.terminate_current,deactivated )
    key3.when_pressed = player.volume_down


def reverse_callbacks():
    activated_map :dict={'up': not activated_map.get('up'),
                     'down':not activated_map.get('down'),
                     'left':not activated_map.get('left'),
                     'right':not activated_map.get('right'),
                     'press':not activated_map.get('press'),

                     'key2':not activated_map.get('key2')
                    }

# btn_prev.when_pressed = play_past
# btn_play.when_pressed = play_current