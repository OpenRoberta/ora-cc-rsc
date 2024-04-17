import os
import time
import threading
import signal
import types
import ftgui

display = ftgui.fttxt2_gui_connector("app")
display.open()

def display_monitoring():
    while display.is_open():
        time.sleep(1)
    os.kill(os.getpid(), signal.SIGTERM)
    exit()

threading.Thread(target=display_monitoring, daemon=True).start()

def __set_base64_image(self, cameraDisplayName, base64Img):
    width = str(self.get_attr(cameraDisplayName + ".width"))
    height = str(self.get_attr(cameraDisplayName + ".height"))
    imageTag = '<img src="' + str(base64Img) + '" width="' + width + '" height="' + height + '" />'
    self.set_attr(cameraDisplayName + ".text", imageTag)
    time.sleep(0.75)

display.set_base64_image = types.MethodType(__set_base64_image, display)

