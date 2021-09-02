// Constants

#ifndef INPUT_MAGENTACOLOR
#define INPUT_MAGENTACOLOR 7
#endif

#ifndef INPUT_ORANGECOLOR
#define INPUT_ORANGECOLOR 8
#endif

#ifndef INPUT_LIMECOLOR
#define INPUT_LIMECOLOR 9
#endif

#ifndef INPUT_VIOLETCOLOR
#define INPUT_VIOLETCOLOR 10
#endif

#ifndef INPUT_CRIMSONCOLOR
#define INPUT_CRIMSONCOLOR 11
#endif

#ifndef INPUT_PURPLECOLOR
#define INPUT_PURPLECOLOR 12
#endif

// Sensor variables

int _LIGHT_SENSOR_MODE = 0;  // 0 - non initialised, 1 - reflective, 2 - ambient

// Display

void BoolOut(int column, int row, bool boolean) {
    if (boolean) {
        TextOut(column, row, "true");
    } else {
        TextOut(column, row, "false");
    }
}

void ColorOut(int column, int row, int color) {
    switch (color) {
        case 1:
            TextOut(column, row, "Black");
            break;
        case 2:
            TextOut(column, row, "Blue");
            break;
        case 3:
            TextOut(column, row, "Green");
            break;
        case 4:
            TextOut(column, row, "Yellow");
            break;
        case 5:
            TextOut(column, row, "Red");
            break;
        case 6:
            TextOut(column, row, "White");
            break;
        case 7:
            TextOut(column, row, "Magenta");
            break;
        case 8:
            TextOut(column, row, "Orange");
            break;
        case 9:
            TextOut(column, row, "Lime");
            break;
        case 10:
            TextOut(column, row, "Violet");
            break;
        case 11:
            TextOut(column, row, "Crimson");
            break;
        case 12:
            TextOut(column, row, "Purple");
            break;
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            TextOut(column, row, "White");
            break;
        default:
            TextOut(column, row, "Grey");
            break;
    }
}

// Drive

void SteerDriveEx(unsigned char portLeft, unsigned char portRight, float powerLeft, float powerRight, bool forward, float distance) {
    if (!forward) {
        powerLeft = -powerLeft;
        powerRight = -powerRight;
    }
    byte ports[2];
    ports[0] = portRight;
    ports[1] = portLeft;
    float absPowerLeft = abs(powerLeft);
    float absPowerRight = abs(powerRight);
    if (powerLeft == powerRight) {
        long angle = distance * 360.0 / (M_PI * WHEELDIAMETER);
        RotateMotorEx(ports, powerLeft, angle, 0, true, true);
        Wait(1);
        return;
    }
    float outerCircumference;
    if (powerLeft + powerRight == 0) {
        outerCircumference = distance;
    } else if (absPowerLeft < absPowerRight) {
        outerCircumference = powerLeft / (powerRight + powerLeft) * 2.0 * distance;
    } else {
        outerCircumference = powerRight / (powerRight + powerLeft) * 2.0 * distance;
    }
    long angle = abs(outerCircumference * 360.0 / (M_PI * WHEELDIAMETER));
    if (absPowerLeft < absPowerRight) {
        OnFwdRegEx(portLeft, powerLeft, OUT_REGMODE_SPEED, RESET_ALL);
        OnFwdRegEx(portRight, powerRight, OUT_REGMODE_SPEED, RESET_ALL);
        Wait(1);
        if (powerRight > 0) {
            while (MotorRotationCount(portRight) < angle) {
            }
        } else {
            while (MotorRotationCount(portRight) > -angle) {
            }
        }
    } else {
        OnFwdRegEx(portRight, powerRight, OUT_REGMODE_SPEED, RESET_ALL);
        OnFwdRegEx(portLeft, powerLeft, OUT_REGMODE_SPEED, RESET_ALL);
        Wait(1);
        if (powerLeft > 0) {
            while (MotorRotationCount(portLeft) < angle) {
            }
        } else {
            while (MotorRotationCount(portLeft) > -angle) {
            }
        }
    }
    Off(ports);
    Wait(1);
}

void SteerDrive(unsigned char portLeft, unsigned char portRight, float powerLeft, float powerRight, bool forward) {
    if (!forward) {
        powerLeft = -powerLeft;
        powerRight = -powerRight;
    }
    if (powerLeft == powerRight) {
        byte ports[2];
        ports[0] = portRight;
        ports[1] = portLeft;
        OnFwdSync(ports, powerLeft, 0);
        return;
    }
    float absPowerLeft = abs(powerLeft);
    float absPowerRight = abs(powerRight);
    if (absPowerLeft < absPowerRight) {
        OnFwdReg(portLeft, powerLeft, OUT_REGMODE_SPEED);
        OnFwdReg(portRight, powerRight, OUT_REGMODE_SPEED);
    } else {
        OnFwdReg(portRight, powerRight, OUT_REGMODE_SPEED);
        OnFwdReg(portLeft, powerLeft, OUT_REGMODE_SPEED);
    }
}

// Sensors

int SensorLight(int port, string mode) {
    if (mode == "AMBIENTLIGHT") {
        SetSensorType(port, SENSOR_TYPE_LIGHT_INACTIVE);
    } else if (mode == "LIGHT") {
        SetSensorType(port, SENSOR_TYPE_LIGHT_ACTIVE);
    } else {
        SetSensorType(port, SENSOR_TYPE_LIGHT_ACTIVE);
    }
    ResetSensor(port);
    return Sensor(port);
}

int SensorColor(int port, string mode) {
    if (mode == "AMBIENTLIGHT") {
        SetSensor(port, SENSOR_COLORNONE);
    } else if (mode == "COLOUR") {
        SetSensor(port, SENSOR_COLORFULL);
    } else if (mode == "LIGHT") {
        SetSensor(port, SENSOR_COLORRED);
    } else {
        SetSensor(port, SENSOR_COLORFULL);
    }
    return Sensor(port);
}

int SensorHtColor(int port, string mode) {
    SetSensorLowspeed(port);
    unsigned int red, green, blue, white;
    int color;
    int newColor;
    if (mode == "AMBIENTLIGHT") {
        SetHTColor2Mode(port, HT_CMD_COLOR2_PASSIVE);
        ReadSensorHTNormalizedColor2Active(port, color, red, green, blue);
        return (red + green + blue) / 3 / 2.55;
    } else if (mode == "COLOUR") {
        SetHTColor2Mode(port, HT_CMD_COLOR2_LED_LOW);
        color = SensorHTColorNum(port);
        //rearrange numbers:
        switch (color) {
            case 0:            // regular color sensor = none
                newColor = 1;  //black
                break;
            case 1:             // regular color sensor = black
                newColor = 10;  //violet
                break;
            case 2:             // regular color sensor = blue
                newColor = 12;  //purple
                break;
            case 3:            // regular color sensor = green
                newColor = 2;  //blue
                break;
            case 4:            // regular color sensor = yellow
                newColor = 3;  //green
                break;
            case 5:            // regular color sensor = red
                newColor = 9;  //lime
                break;
            case 6:            // regular color sensor = white
                newColor = 4;  //yellow
                break;
            case 7:
                newColor = 8;  //orange
                break;
            case 8:
                newColor = 5;  //red
                break;
            case 9:
                newColor = 11;  //crimson
                break;
            case 10:
                newColor = 7;  //magenta
                break;
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:
            case 17:
                newColor = 6;  //white
                break;
        }
        return newColor;
    } else if (mode == "LIGHT") {
        SetHTColor2Mode(port, HT_CMD_COLOR2_ACTIVE);
        ReadSensorHTNormalizedColor2Active(port, color, red, green, blue);
        return (red + green + blue) / 3 / 2.55;
    }
    return Sensor(port);
}

int _readLightSensor(int port, int mode) {
    if (mode != _LIGHT_SENSOR_MODE) {
        if (mode == 1) {
            SetSensorLight(port, true);
        } else if (mode == 2) {
            SetSensorLight(port, false);
        }
        _LIGHT_SENSOR_MODE = mode;
    }
    return Sensor(port);
}

// Bluetooth

inline bool BTCheck(int conn) {
    if (!BluetoothStatus(conn) == NO_ERR) {
        TextOut(5, LCD_LINE2, "Error");
        Wait(1000);
        StopAllTasks();
        return false;
    } else {
        return true;
    }
}

inline float BluetoothGetNumber(int inbox) {
    int in;
    ReceiveRemoteNumber(inbox, true, in);
    return in;
}

inline string BluetoothGetString(int inbox) {
    string in;
    ReceiveRemoteString(inbox, true, in);
    return in;
}

inline bool BluetoothGetBoolean(int inbox) {
    bool in;
    ReceiveRemoteBool(inbox, true, in);
    return in;
}

// Lists (actually Arrays)

inline float ArraySum(float arr[]) {
    float sum = 0;
    for (int i = 0; i < ArrayLen(arr); i++) {
        sum += arr[i];
    }
    return sum;
}
inline float ArrayMin(float arr[]) {
    float min = arr[0];
    for (int i = 1; i < ArrayLen(arr); i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}
inline float ArrayMax(float arr[]) {
    float max = arr[0];
    for (int i = 1; i < ArrayLen(arr); i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

inline int idty(int id) {
    return id;
}

inline float ArrayMean(float arr[]) {
    float sum = 0;
    for (int i = 0; i < ArrayLen(arr); i++) {
        sum += arr[i];
    }
    return sum / idty(ArrayLen(arr));
}

inline void ArrayInsertionSort(float &arr[]) {
    for (int i = 1; i < ArrayLen(arr); i++) {
        int index = arr[i];
        int j = i;
        while (j > 0 && arr[j - 1] > index) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = index;
    }
}
inline float ArrayMedian(float arr[]) {
    int n = idty(ArrayLen(arr));
    if (n == 0) {
        return 0;
    }
    ArrayInsertionSort(arr);
    float median;
    if (n % 2 == 0) {
        median = (arr[n / 2] + arr[n / 2 - 1]) / 2;
    } else {
        median = arr[n / 2];
    }
    return median;
}

inline float ArrayMode(float arr[]) {
    ArrayInsertionSort(arr);
    float element = arr[0];
    float max_seen = element;
    int count = 1;
    int mode_count = 1;
    for (int i = 1; i < ArrayLen(arr); i++) {
        if (arr[i] == element) {
            count++;
            if (count > mode_count) {
                mode_count = count;
                max_seen = element;
            }
        } else {
            element = arr[i];
            count = 1;
        }
    }
    return max_seen;
}

inline int ArrFindFirstNum(float arr[], float item) {
    int i = 0;
    if (arr[0] == item) {
        return i;
    } else {
        do {
            i++;
        } while ((arr[i] != item) && (i != idty(ArrayLen(arr))));
        return i;
    }
}

inline int ArrFindLastNum(float arr[], float item) {
    int i = 0;
    if (arr[idty(ArrayLen(arr)) - 1] == item) {
        return idty(ArrayLen(arr)) - 1 - i;
    } else {
        do {
            i++;
        } while ((arr[idty(ArrayLen(arr)) - 1 - i] != item) && (i != 0));
        return idty(ArrayLen(arr)) - 1 - i;
    }
}

inline int ArrFindFirstStr(string arr[], string item) {
    int i = 0;
    if (arr[0] == item) {
        return i;
    } else {
        do {
            i++;
        } while ((arr[i] != item) && (i != idty(ArrayLen(arr))));
        return i;
    }
}

inline int ArrFindLastStr(string arr[], string item) {
    int i = 0;
    if (arr[idty(ArrayLen(arr)) - 1] == item) {
        return idty(ArrayLen(arr)) - 1 - i;
    } else {
        do {
            i++;
        } while ((arr[idty(ArrayLen(arr)) - 1 - i] != item) && (i != 0));
        return idty(ArrayLen(arr)) - 1 - i;
    }
}

inline int ArrFindFirstBool(bool arr[], bool item) {
    int i = 0;
    if (arr[0] == item) {
        return i;
    } else {
        do {
            i++;
        } while ((arr[i] != item) && (i != idty(ArrayLen(arr))));
        return i;
    }
}

inline int ArrFindLastBool(bool arr[], bool item) {
    int i = 0;
    if (arr[idty(ArrayLen(arr)) - 1] == item) {
        return idty(ArrayLen(arr)) - 1 - i;
    } else {
        do {
            i++;
        } while ((arr[idty(ArrayLen(arr)) - 1 - i] != item) && (i != 0));
        return idty(ArrayLen(arr)) - 1 - i;
    }
}

int sanitiseFromStart(int array_length, int index) {
    if (index >= 0 && index < array_length) {
        return index;
    } else {
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Index out of bounds: ");
        NumOut(0, (MAXLINES - 2) * MAXLINES, index);
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Press enter to continue.");
        while (true) {
            if (ButtonPressed(BTNCENTER, false) == true) {
                Stop(true);
            }
            Wait(15);
        }
    }
}

int sanitiseFromEnd(int array_length, int from_end) {
    int index = array_length - from_end;
    if (index >= 0 && index < array_length) {
        return index;
    } else {
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Index out of bounds: ");
        NumOut(0, (MAXLINES - 2) * MAXLINES, index);
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Press enter to continue.");
        while (true) {
            if (ButtonPressed(BTNCENTER, false) == true) {
                Stop(true);
            }
            Wait(15);
        }
    }
}
