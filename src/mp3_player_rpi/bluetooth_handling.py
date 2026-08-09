import subprocess

headphones=''

def make_connection():

    command=f'''
        power on
        agent on
        default-agent
        select {headphones}
        pair {headphones}
        connect {headphones}
        trust {headphones}

'''
    subprocess.Popen(['bluetoothctl'], input=command, text=True)

