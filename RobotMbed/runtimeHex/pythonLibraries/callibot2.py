from calliopemini import i2c, pin_A0_SCL, pin_A0_SDA
from time import sleep

class Callibot2(object):
    """Klasse für den Callibot des Calliope Mini, gesteuert von einem ESP32 über I2C-Bus."""

    def __init__(self):
        # i2c-Byte-Adressen des Chassis
        self.i2cAdrMotoren = 32
        self.i2cAdrAdapter = 33
        # Byte-Adressen des Motors
        self.motorLinks    = 0
        self.motorRechts   = 2
        # Geschwindigkeiten
        self.left_speed     = 0
        self.right_speed    = 0
        # LED
        self.ledrl         = 0                          # rote LED vorn links
        self.ledrr         = 0                          # rote LED vorn rechts
        # i2c Bus Initialisierung
        i2c.init(freq=400000, scl=pin_A0_SCL, sda=pin_A0_SDA)   # i2c-Bus Initialisierung
        # Speicherpuffer für i2c write-Anweisung
        self.buf = bytearray()

    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # I2C-Funktionen

    def i2cStop(self):
        """Setzt den I2C-Bus zurück und stoppt ihn."""

        i2c.init(freq=400000, scl=pin_A0_SCL, sda=pin_A0_SDA)   # i2c-Bus Initialisierung

    def i2cSetMotor(self, buf):
        """Schreibt eine bestimmte Anzahl Bytes (buf) zum Motorbaustein über I2C."""

        try:
            i2c.write(self.i2cAdrMotoren, buf)
        except OSError:                            # Betriebssystem Fehler ignorieren
            None                                  # bei schlechter Verbindung (timeout)

    def i2cReadAdapter(self, value):
        """Liest eine bestimmte Anzahl Bytes aus dem Adapter über I2C."""

        return i2c.read(self.i2cAdrAdapter, value)     # Anzahl Bytes lesen

    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # Motorfunktionen

    def get_speed_motor_l(self):
        '''Gibt die Geschwindigkeit des linken Motors zurück.
 
        Rückgabe: speed als Integerwert
        '''
        return self.left_speed

    def get_speed_motor_r(self):
        '''Gibt die Geschwindigkeit des rechten Motors zurück.
 
        Rückgabe: speed als Integerwert
        '''
        return self.right_speed

    def set_speed_motor_l(self, speed):          # Motor links wird mit speed gesetzt
        speed = int(speed / 100 * 255)
        speed = max(-255, min(255, speed))
        self.left_speed = speed
        if speed == 0:
            self.stop_motor_l()
        elif speed > 0:
            self.buf = bytearray([self.motorLinks, 0, speed])
            self.i2cSetMotor(self.buf)
        elif speed < 0:
            self.buf = bytearray([self.motorLinks, 1, speed])
            self.i2cSetMotor(self.buf)

    def set_speed_motor_r(self, speed):          # Motor rechts wird mit speed gesetzt
        speed = int(speed / 100 * 255)
        speed = max(-255, min(255, speed))
        self.right_speed = speed
        if speed == 0:
            self.stop_motor_r()
        elif speed > 0:
            self.buf = bytearray([self.motorRechts, 0, speed])
            self.i2cSetMotor(self.buf)
        elif speed < 0:
            self.buf = bytearray([self.motorRechts, 1, speed])
            self.i2cSetMotor(self.buf)

    def set_speed_motor(self, motor, speed):
        if motor == 1:
            self.set_speed_motor_l(speed)
        else:
            self.set_speed_motor_r(speed)


    def set_speed(self, left_speed, right_speed):          # beide Motoren werden mit speed gesetzt
        self.set_speed_motor_l(left_speed)
        self.set_speed_motor_r(right_speed)

    def stop_motor_l(self):
        """Stoppt den linken Motor."""

        self.left_speed = 0
        self.buf = bytearray([self.motorLinks, 0, 0])
        self.i2cSetMotor(self.buf)

    def stop_motor_r(self):
        """Stoppt den rechten Motor."""
        self.right_Speed = 0
        self.buf = bytearray([self.motorRechts, 0, 0])
        self.i2cSetMotor(self.buf)

    def stop_motor(self, motor):
        if motor == 1:
            self.stop_motor_l()
        else:
            self.stop_motor_r()
    def stop(self):
        """Stoppt beide Motoren."""

        self.stop_motor_l()
        self.stop_motor_r()

    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # vordere rote LED-Funktionen

    def get_red_led(self):
        """Ermittelt den Zustand der roten LED's aus Variablenwerten (nicht vom Adapter)."""

        return [self.ledrl, self.ledrr>>1]

    def set_red_led_on(self, led):
        if led == 1:
            self.set_red_led_left_on()
        else:
            self.set_red_led_right_on()
    def set_red_led_off(self, led):
        if led == 1:
            self.set_red_led_left_off()
        else:
            self.set_red_led_right_off()
    def set_red_led_left_on(self):
        """Schaltet linke rote LED ein."""

        self.ledrl = 1
        self.buf = bytearray([0, self.ledrl + self.ledrr])
        i2c.write(self.i2cAdrAdapter, self.buf)

    def set_red_led_left_off(self):
        """Schaltet linke rote LED aus."""

        self.ledrl = 0
        self.buf = bytearray([0, self.ledrl + self.ledrr])
        i2c.write(self.i2cAdrAdapter, self.buf)

    def set_red_led_right_on(self):
        """Schaltet rechte rote LED ein."""

        self.ledrr = 2
        self.buf = bytearray([0, self.ledrl + self.ledrr])
        i2c.write(self.i2cAdrAdapter, self.buf)

    def set_red_led_right_off(self):
        """Schaltet rechte rote LED aus."""

        self.ledrr = 0
        self.buf = bytearray([0, self.ledrl + self.ledrr])
        i2c.write(self.i2cAdrAdapter, self.buf)

    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # vier RGB-LED unter dem Chassi Funktionen

    def set_rgb_led(self, led, rgb):
        buffer = bytearray(5)

        buffer[0] = 0x03
        buffer[1] = led
        buffer[2] = int(rgb[0] / 255 * 16)
        buffer[3] = int(rgb[1] / 255 * 16)
        buffer[4] = int(rgb[2] / 255 * 16)
        i2c.write(34, buffer)

    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # Ultraschall Sensor Abfrage

    def get_ultrasonic_sensor(self):
        """Methode liefert den 16-Bit Bytewert der Entfernung in mm."""
        buffer = bytearray(i2c.read(self.i2cAdrAdapter, 3))
        return (256 * buffer[1] + buffer[2]) / 10


    # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    # Linien Sensor Abfrage

    def get_line_sensor(self, sensor):
        buffer = bytearray(i2c.read(self.i2cAdrAdapter, 1))
        if sensor == 1:
            buffer[0] &= 0x02
        elif sensor == 2:
            buffer[0] &= 0x01
        return buffer[0] != 0

    def get_bumper_sensor(self, sensor):
        buffer = bytearray(i2c.read(self.i2cAdrAdapter, 1))  # Read 1 byte from the device at address 0x21
        if sensor == 1:  #left
            buffer[0] &= 0x08
        elif sensor == 2:  # right
            buffer[0] &= 0x04
        return buffer[0] != 0