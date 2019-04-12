# Import
import time
import serial


class Hal(object):
    def __init__(self):
        self.conn = serial.Serial(
            port='/dev/ttyACM0', baudrate=115200, timeout=1)
        self.exec_command('TestMode On')

    def __format_response(self, response):
        size = len(response)
        response = response[1:size - 2]
        return response

    def __convert_analog_sensors_to_dictionary(self, sensors):
        result = {}
        for sensor in sensors:
            sensor = sensor.split(",")
            result.update({sensor[0]: float(sensor[2])})
        return result

    def __convert_digital_sensors_to_dictionary(self, sensors):
        result = {}
        for sensor in sensors:
            sensor = sensor.split(",")
            result.update({sensor[0]: float(sensor[1].strip())})
        return result

    def __right_wheel_enable(self):
        self.exec_command('SetMotor RWheelEnable')

    def __left_wheel_enable(self):
        self.exec_command('SetMotor LWheelEnable')

    def __right_wheel_disable(self):
        self.exec_command('SetMotor RWheelDisable')

    def __left_wheel_disable(self):
        self.exec_command('SetMotor LWheelDisable')

    def __disable_wheels(self):
        self.__right_wheel_disable()
        self.__left_wheel_disable()

    def __enable_wheels(self):
        self.__right_wheel_enable()
        self.__left_wheel_enable()

    def stop_motors(self):
        self.exec_command('SetMotor LWheelDist 1 RWheelDist 1 Speed 100')

    def drive_distance(self, direction, speed, distance):
        distance *= 10
        speed *= 3.5
        if direction != 'foreward':
            distance *= -1
        if distance <= -10000:
            distance = -9999
        if distance >= 10000:
            distance = 9999
        self.exec_command(
            'SetMotor LWheelDist {0} RWheelDist {1} Speed {2}'.format(
                distance, distance, speed))

    def left_motor_on(self, speed, distance):
        distance *= 10
        speed *= 3.5
        if distance <= -10000:
            distance = -9999
        if distance >= 10000:
            distance = 9999
        self.exec_command(
            'SetMotor LWheelDist {0} RWheelDist 0 Speed {1}'.format(
                distance, speed))

    def right_motor_on(self, speed, distance):
        distance *= 10
        speed *= 3.5
        if distance <= -10000:
            distance = -9999
        if distance >= 10000:
            distance = 9999
        self.exec_command(
            'SetMotor LWheelDist 0 RWheelDist {0} Speed {1}'.format(
                distance, speed))

    def left_motor_stop(self):
        self.exec_command('SetMotor LWheelDist 1 Speed 100')

    def right_motor_stop(self):
        self.exec_command('SetMotor RWheelDist 1 Speed 100')

    def play_sound(self, sound_id):
        self.exec_command('PlaySound SoundID ' + str(sound_id))

    def brush_on(self, speed):
        if speed < 0:
            speed = 0
        elif speed > 100:
            speed = 100
        speed *= 100
        self.exec_command("SetMotor Brush RPM " + str(speed))

    def brush_off(self):
        self.exec_command("SetMotor Brush RPM 0")

    def vacuum_on(self, speed):
        if speed < 1:
            speed = 1
        elif speed > 100:
            speed = 100
        self.exec_command("SetMotor VacuumOn VacuumSpeed " + str(speed))

    def vacuum_off(self):
        self.exec_command("SetMotor VacuumOff")

    def side_brush_on(self):
        self.exec_command("SetMotor SideBrushOn")

    def side_brush_off(self):
        self.exec_command("SetMotor SideBrushOff")

    def exec_command(self, cmd):
        self.conn.write(cmd + '\r\n')
        time.sleep(.05)
        response = self.conn.readlines()
        response = self.__format_response(response)
        # for r in response:
        #     print r
        return response

    def sample_analog_sensors(self):
        response = self.exec_command('GetAnalogSensors')
        response = self.__convert_analog_sensors_to_dictionary(response[1:])
        return response

    def sample_digital_sensors(self):
        response = self.exec_command('GetDigitalSensors')
        response = self.__convert_digital_sensors_to_dictionary(response[1:])
        return response

    def sample_ultrasonic_sensor(self, port, slot):
        sensors = self.sample_analog_sensors()
        port = port.split('_')[0]
        key = 'UltraSound' + port.title() + slot.title()
        return sensors[key] / 10.

    def sample_touch_sensor(self, port, slot):
        sensors = self.sample_digital_sensors()
        key = port[0] + slot + 'bit'
        key = key.upper()
        return sensors[key]

    def sample_dropoff_sensor(self, port):
        sensors = self.sample_analog_sensors()
        key = 'DropSensor' + port.title()
        return sensors[key] / 10.

    def sample_wall_sensor(self):
        sensors = self.sample_analog_sensors()
        key = 'WallSensor'
        return sensors[key] / 10.

    def sample_accelerometer_sensor(self, port):
        sensors = self.sample_analog_sensors()
        key = 'Accelerometer' + port.upper()
        return sensors[key]

    def wait(self, timeMilliSeconds):
        timeSeconds = timeMilliSeconds / 1000
        time.sleep(timeSeconds)
