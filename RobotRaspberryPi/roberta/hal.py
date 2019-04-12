# Import
import time
from gpiozero import LED, RGBLED, Button
import sys
from signal import pause


class Hal(object):
    def __init__(self):
        
        self.led1_w = LED(17)
        self.led2_w = LED(11)
        self.sensor = Button(24)
        self.button = Button(23)
        self.button.when_pressed = self.__exit
        self.timers = {}
        self.timers[1] = time.time()
        self.leds = {}
        
        self.leds[1] = {'o': RGBLED(2, 3, 4, pwm=True, active_high=False), 'c': (1, 1, 1), 'b': 1.}
        self.leds[2] = {'o': RGBLED(10, 9, 25, pwm=True, active_high=False), 'c': (1, 1, 1), 'b': 1.}

    def __exit(self):
        self.button.close()
        self.sensor.close()
        self.leds[1]['o'].close()
        self.leds[2]['o'].close()
        time.sleep(1)
        sys.exit()


    def get_sensor_status(self):
        return self.sensor.is_pressed

    def set_color(self, x, color):
        rgb = self.hex_to_rgb(color)
        rgb_normalized = self.rgb_to_range(rgb)
        z = self.leds[x]['b']
        c = tuple(c * z for c in rgb_normalized)
        self.leds[x]['o'].color = c
        self.leds[x]['c'] = color

    def light_off(self, x):
        self.leds[x]['o'].off()
        
    def set_brightness(self, x, z):
        color = self.leds[x]['c']
        rgb = self.hex_to_rgb(color)
        rgb_normalized = self.rgb_to_range(rgb)
        z = z / 100.
        c = tuple(c * z for c in rgb_normalized)
        self.leds[x]['o'].color = c
        self.leds[x]['b'] = z

    def get_brightness(self, x):
        return self.leds[x]['b']*100.

    def blink(self, x, frequency, time):
        f = frequency*2.
        color = self.leds[x]['c']
        rgb = self.hex_to_rgb(color)
        rgb_normalized = self.rgb_to_range(rgb)
        z = self.leds[x]['b']
        c = tuple(c * z for c in rgb_normalized)
        self.leds[x]['o'].blink(1/f, 1/f, n=int(time*frequency), background=False, on_color=c)

    def dim(self, x, z1, z2, time):
        color = self.leds[x]['c']
        rgb = self.hex_to_rgb(color)
        rgb_normalized = self.rgb_to_range(rgb)
        z1 /= 100.
        z2 /= 100.
        z1_color = tuple(c * z1 for c in rgb_normalized)
        z2_color = tuple(c * z2 for c in rgb_normalized)
        if z1 < z2:
            self.leds[x]['o'].pulse(fade_in_time=time, fade_out_time=0, on_color=z2_color,
                                 off_color=z1_color, n=1, background=False)
        else:
            self.leds[x]['o'].pulse(fade_in_time=0, fade_out_time=time, on_color=z1_color,
                                 off_color=z2_color, n=1, background=False)

    def wait(self, timeSeconds):
        time.sleep(timeSeconds)

    def clear(self):
        pass

    def getTimerValue(self, timer):
        if timer in self.timers:
            return int((time.time() - self.timers[timer]))
        else:
            self.timers[timer] = time.time()
            return 0

    def resetTimer(self, timer):
        self.timers[timer] = time.time()

    def hex_to_rgb(self, hex):
        hex = hex.lstrip('#')
        hlen = len(hex)
        return tuple(int(hex[i:i+hlen/3], 16) for i in range(0, hlen, hlen/3))

    def rgb_to_range(self, rgb):
        return tuple(c / 255. for c in rgb)
