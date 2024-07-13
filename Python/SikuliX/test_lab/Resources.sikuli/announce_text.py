import sys
import os
import time
from gtts import gTTS
import pygame
import common

if sys.argv[2] == 'on':
    text_to_announce = sys.argv[1]
    audio = gTTS(text=text_to_announce.replace('_', ' '), lang='en', slow=False)
    name = text_to_announce
    audio.save(common.TEST_DIR+'output\\'+name+'.mp3')
    pygame.mixer.init()
    #pygame.mixer.music.load(common.TEST_DIR+'output\\'+name+'.mp3')
    audio = pygame.mixer.Sound(common.TEST_DIR+'output\\'+name+'.mp3')
    audio.play()
    audio.set_volume(float(sys.argv[3]))
    time.sleep(audio.get_length())