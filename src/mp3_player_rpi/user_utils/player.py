import subprocess
import os
import glob
from index import selected
import time
import signal


class Player():
    def __init__(self,selected,navigater):
        
        self.music = sorted(glob.glob("music/**/*.mp3"))
        print(self.music)
        self.selected=selected
        self.navigater = navigater

        self._start_time=0
        self.total_time =0

        self._is_paused=False
        self._paused_start=0
        self._paused_time=0

        self.volume =0

    def terminate_current(self):
        if self.player:
            self.player.terminate()
    
    def play_current(self) ->None:
        self.terminate_current()     
        self._start_time = time.monotonic()

        try:
            # self.player = subprocess.Popen(
            #     ["mpg123","-t",self.music[self.selected.index]],
            #     text=True,
            #     capture_output=True,
            #     bufsize=1,
            #     )
            self.player = subprocess.Popen(
                ["mpg123", "-R"],
                stdin=subprocess.PIPE,
                tdout=subprocess.PIPE,
                stderr=subprocess.DEVNULL,
                text=True,
                bufsize=1
            )
            self.player.stdin.write(f"LOAD {self.music[self.selected.index]}\n")
            self.player.stdin.flush()

            self.total_time = self.player.stdout
            print(self.total_time)
            print(f"Playing: {os.path.basename(self.music[self.selected.index])}")

        except subprocess.CalledProcessError as e:
            print(f"unable to play music {e}")

    def handle_pause_resume(self):

        if self._is_paused:
            self._paused_time = time.monotonic() - self._paused_start
            self.resume()
            self._is_paused=False
        else:
            self._paused_start=time.monotonic()
            self.pause()
            self._is_paused=True


    def pause(self):
        os.kill(self.player.pid, signal.SIGSTOP)
    def resume(self):
        os.kill(self.player.pid, signal.SIGCONT)

    def get_current_time(self):

        return  time.monotonic() -(self._start_time + self._paused_time)

    def playing_music(self) -> bool:#maybe useless 
        if self.player.poll() is None:
            return True
    

    def play_next(self):
    
        if self.selected.check_incresable(len(self.navigater.current_directory)):
            self.selected.increase_index()
        self.play_current()

    def play_past(self):
   
        if self.selected.check_decreasable():
            self.selected.decrease_index()
        self.play_current()

    

    def volume_up(self):
        if self.volume<=100:
            self.volume+=1
            subprocess.run(["wpctl","set-volume","@DEFAULT_AUDIO_SINK@",
    "5%+"])
 

    def volume_down(self):
        if self.volume>=0:
            self.volume-=1
            subprocess.run(["wpctl","set-volume","@DEFAULT_AUDIO_SINK@",
    "5%-"])
            
