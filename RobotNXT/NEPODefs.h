//Constants

#ifndef E
#define E 2.718281828459045
#endif

#ifndef GOLDEN_RATIO
#define GOLDEN_RATIO 1.61803398875
#endif


#ifndef SQRT2
#define SQRT2 1.41421356237
#endif

#ifndef SQRT1_2
#define SQRT1_2 0.707106781187
#endif

#ifndef INFINITY
#define INFINITY 0x7f800000
#endif

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


int _LIGHT_SENSOR_MODE = 0; // 0 - non initialised, 1 - reflective, 2 - ambient


//sensor functions

void BoolOut(int column, int row, bool boolean) {
  if (boolean){
    TextOut(column, row, "true");
  } else {
    TextOut(column, row, "false");
  }
}

void ColorOut(int column, int row, int color){
  switch(color) {
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

int SensorLight(int port, string mode){
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

int SensorColor(int port, string mode){
  if (mode == "AMBIENTLIGHT"){
    SetSensor(port, SENSOR_COLORNONE);
  } else if (mode == "COLOUR"){
    SetSensor(port, SENSOR_COLORFULL);
  } else if (mode == "LIGHT") {
    SetSensor(port, SENSOR_COLORRED);
  } else {
    SetSensor(port, SENSOR_COLORFULL);
  }
  return Sensor(port);
}

int SensorHtColor(int port, string mode){
  SetSensorLowspeed(port);
  unsigned int red, green, blue, white;
  int color;
  int newColor;
  if (mode == "AMBIENTLIGHT"){
    SetHTColor2Mode(port, HT_CMD_COLOR2_PASSIVE);
    ReadSensorHTNormalizedColor2Active(port, color, red, green, blue);
    return (red + green + blue)/3/2.55;
  } else if (mode == "COLOUR"){
    SetHTColor2Mode(port, HT_CMD_COLOR2_LED_LOW);
    color = SensorHTColorNum(port);
    //rearrange numbers: 
    switch(color) {
	    case 0: // regular color sensor = none
	      newColor = 1; //black
	      break;
	    case 1: // regular color sensor = black
	      newColor = 10; //violet
	      break;
	    case 2: // regular color sensor = blue
          newColor = 12; //purple
	      break;
	    case 3: // regular color sensor = green
	      newColor = 2; //blue
	      break;
	    case 4: // regular color sensor = yellow
	      newColor = 3; //green
	      break;
	    case 5: // regular color sensor = red
	      newColor = 9; //lime
	      break;
	    case 6: // regular color sensor = white
	      newColor = 4; //yellow
	      break;
	    case 7:
	      newColor = 8; //orange
	      break;
	    case 8:
          newColor = 5; //red
	      break;
	    case 9: 
	      newColor = 11; //crimson
	      break;
	    case 10:
          newColor = 7; //magenta
	      break;
	    case 11:
	    case 12:
	    case 13:
	    case 14:
	    case 15:
	    case 16:
	    case 17:
	      newColor = 6; //white
	      break;
	  }
	  return newColor;
  } else if (mode == "LIGHT") {
    SetHTColor2Mode(port, HT_CMD_COLOR2_ACTIVE);
    ReadSensorHTNormalizedColor2Active(port, color, red, green, blue);
    return (red + green + blue)/3/2.55;
  }
  return Sensor(port);
}

float MotorDistance(int port, float diameter){
  float rad = diameter/2;
  int degreePi = 180;
  return (PI*MotorTachoCount(port)/degreePi)*rad;
}

void SetTimerValue(long& timer1){
  timer1 = CurrentTick();
}

inline long GetTimerValue(long timer1){
  return CurrentTick() - timer1;
}

void ResetTimerValue(long& timer1){
  timer1 = CurrentTick();
}

//Bluetooth functions and constants

#ifndef BT_SLAVE_CONN
#define BT_SLAVE_CONN 0
#endif


inline bool BTCheck(int conn){
  if (!BluetoothStatus(conn)==NO_ERR){
    TextOut(5,LCD_LINE2,"Error");
    Wait(1000);
    StopAllTasks();
    return false;
  }
  else{
    return true;
  }
}

inline float BluetoothGetNumber(int inbox){
  int in;
  ReceiveRemoteNumber(inbox,true,in);
  return in;
  //TextOut(0,LCD_LINE3,"Receiving");
  //TextOut(0,LCD_LINE4,"  ");
  //NumOut(5,LCD_LINE4,in);
  //return in;
}

//void bluetooth_send_number(int out, int connection, int inbox, int outbox){
  //TextOut(0,LCD_LINE1,"Sending");
  //TextOut(0,LCD_LINE2,"  ");
  //NumOut(5,LCD_LINE2,out);
  //SendRemoteNumber(connection,outbox,out);
//}

inline string BluetoothGetString(int inbox){
    string in;
    ReceiveRemoteString(inbox,true,in);
    //TextOut(0,LCD_LINE3,"Receiving");
    //TextOut(0,LCD_LINE4,"  ");
    //TextOut(5,LCD_LINE4,in);
    return in;
}

//void bluetooth_send_string(string out, int connection, int outbox){
  //TextOut(0,LCD_LINE1,"Sending");
  //TextOut(0,LCD_LINE2,"  ");
  //TextOut(5,LCD_LINE2,out);
  //SendRemoteString(connection,outbox,out);
//}

inline bool BluetoothGetBoolean(int inbox){
  bool in;
  ReceiveRemoteBool(inbox,true,in);
  return in;
  //TextOut(0,LCD_LINE3,"Receiving");
  //TextOut(0,LCD_LINE4,"  ");
  //if (in == true){
  //  TextOut(5,LCD_LINE2,"true");
  //}
  //else{
  //  TextOut(5,LCD_LINE2,"false");
  //}
}

//void bluetooth_send_boolean(bool out, int connection, int inbox, int outbox){
  //TextOut(0,LCD_LINE1,"Sending");
  //TextOut(0,LCD_LINE2,"  ");
  //if (out == true){
  //  TextOut(5,LCD_LINE2,"true");
  //}
  //else{
  //  TextOut(5,LCD_LINE2,"false");
  //}
  //SendRemoteBool(connection,outbox,out);
//}

//sensors' functions
void IsPressedAndReleased(int button){
  SetButtonReleaseCount(button, 0);
  NumOut (0,LCD_LINE6, ButtonReleaseCount(button));
  while((ButtonReleaseCount(button) == 0)||(ButtonPressCount(button) == 0)){
    TextOut(0,LCD_LINE1, "zero");
  }
  //SetButtonState(BTN1, BTNSTATE_PRESSED_EV);
  //SetButtonState(BTNLEFT, 0x10);
}

//math Functions

inline int MathFloor(float val) {
  int temp = val;
  return temp;
}

inline int MathRound(float val){
  return MathFloor(0.5 + val);
}

inline int MathRoundUp(float val){
  return (1 + MathFloor(val));
}

inline bool MathIsWhole(float val){
  int intPart = val;
  return ((val - intPart) == 0);
}

inline float MathPow(float firstValue, float secondValue) {
  float result = 1;
  for (int i = 0; i < abs(secondValue); i++) {
    result = result * firstValue;
  }
  if (secondValue < 0){
     if(firstValue == 0){
       return NULL;
     }
     else{
       return 1/result;
     }
  }
  else{
    return result;
  }
}

inline float MathMin(float firstValue, float secondValue) {
  if (firstValue < secondValue){
    return firstValue;
  }
  else{
    return secondValue;
  }
}

inline float MathMax(float firstValue, float secondValue) {
  if (firstValue > secondValue){
    return firstValue;
  }
  else{
    return secondValue;
  }
}

inline bool MathPrime(float number) {
    if ((number % 2 == 0) || (number == 1)) return false;
    //if not, then just check the odds
    for(int i = 3; i * i <= number; i += 2) {
        if(number % i == 0)
            return false;
    }
    return true;
}

inline float MathLn(float val) {
  if (val > 1){
    float values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 10000};
    float results[] = {0, 0.693147, 1.098612, 1.386294, 1.609438, 1.791759, 1.945910, 2.079442, 2.197225, 2.302585, 2.995732, 3.401197, 3.688879, 3.912023, 4.094345, 4.248495, 4.382027, 4.499810, 4.605170, 5.298317, 5.703782, 5.991465, 6.214608, 6.396930, 6.551080, 6.684612, 6.802395, 6.907755, 9.210340};
    int i = 1;
    while(values[i] < abs(val)){
      i++;
    }
    float result =  results[i - 1] + (abs(val) - values[i - 1]) * (results[i] - results[i - 1]) / (values[i] - values[i - 1]);
    if (val <= 100000){
      return result;
    }
    else{
      return 9.22;
    }
  } else if ((val > 0) && (val <= 1)) {
    float summ = 0;
    for (int n = 1; n < 10; n++){
      summ += MathPow(-1, (n + 1)) * MathPow((val - 1), n)/n;
    }
    return summ;
  }
  else{
    TextOut(0, LCD_LINE1, "invalid value");
    Wait(1000);
    ClearScreen();
    return NULL;
  }
}

inline float MathLog(float val) {
  return MathLn(val)/MathLn(2.71828);
}

inline float MathFactorial(float val) {
  float result = val;
  if (val == 0) {
    return 1;
  } else {
    for (int i = 1; i < val; i++) {
      result = result*(val - i);
    }
    return result;
  }
}

inline float MathSin(float val) {
  float angle = PI*val/180;
  float summ = 0;
  for (int n = 0; n < 10; n++) {
    summ += MathPow(-1, n) * MathPow(angle, (2 * n + 1)) / MathFactorial(2 * n + 1);
  }
  return summ;
}

inline float MathCos(float val) {
  float angle = PI*val/180;
  float summ = 0;
  for (float n = 0; n < 10; n++){
    summ += (MathPow(-1, n)/MathFactorial(2 * n)) * MathPow(angle, (2 * n));
  }
  return summ;
}

inline float MathTan(float val) {
  return MathSin(val) / MathCos(val);
}

inline float MathAsin(float val) {
  if (abs(val) > 1){
    TextOut(0, LCD_LINE1, "invalid value");
    Wait(1000);
    ClearScreen();
    return NULL;
  } else {
    float summ = 0;
    for (float n = 0; n < 15; n++){
      summ += MathFactorial(2 * n) * MathPow(val, (2 * n + 1)) / MathPow(4, n) / MathPow(MathFactorial(n), 2)/(2* n + 1);
    }
    return summ * 180 / PI;
  }
}


inline float MathAcos(float val) {
  if (abs(val) > 1) {
    TextOut(0, LCD_LINE1, "invalid value");
    Wait(1000);
    ClearScreen();
    return NULL;
  } else {
    return 90 - MathAsin(val);
  }
}

inline float MathAtan(float val) {
  if (abs(val) > 1) {
    float values[] = {1, sqrt(3), 2, 3, 0x7f800000};
    float results[] = {45, 60, 63.435, 71.565, 90};
    int i = 1;
    while(values[i] < abs(val)) {
      i++;
    }
    float result =  results[i - 1] + (abs(val) - values[i - 1]) * (results[i] - results[i - 1]) / (values[i] - values[i - 1]);
    if (val > 0) {
      return result;
    } else {
      return -result;
    }
  } else {
    float summ = 0;
    for (float n = 1; n < 15; n++){
      summ += MathPow(-1, (n - 1)) * MathPow(val, (2 * n - 1)) / (2 * n - 1);
    }
    return summ * 180 / PI;
  }
}

inline int RandomIntegerInRange(int val1, int val2) {
  return abs(val1 - val2) * Random(100) / 100 + MathMin(val1, val2);
}

inline float RandomFloat() {
  return Random(100) / 100;
}

inline float Constrain(float val, float min, float max){
	return MathMin(MathMax(val, min), max);
}

//numerical array functions

inline float ArrSum(float arr[]) {
  float sum = 0;
  for(int i = 0; i < ArrayLen(arr); i++) {
    sum += arr[i];
  }
  return sum;
}
inline float ArrMin(float arr[]) {
  float min = arr[0];
  for(int i = 1; i < ArrayLen(arr); i++) {
    if (arr[i] < min){
      min = arr[i];
    }
  }
  return min;
}
inline float ArrMax(float arr[]) {
  float max = arr[0];
  for(int i = 1; i < ArrayLen(arr); i++) {
    if (arr[i] > max){
      max = arr[i];
    }
  }
  return max;
}

inline int idty(int id) {
  return id;
}

inline float ArrMean(float arr[]) {
  float sum = 0;
  for(int i = 0; i < ArrayLen(arr); i++) {
    sum += arr[i];
  }
  return sum/idty(ArrayLen(arr));
}

inline void ArrInsertionSort(float &arr[]) {
  for (int i=1; i < ArrayLen(arr); i++) {
    int index = arr[i];
    int j = i;
    while (j > 0 && arr[j-1] > index) {
      arr[j] = arr[j-1];
      j--;
    }
    arr[j] = index;
  }
}
inline float ArrMedian(float arr[]) {
  int n = idty(ArrayLen(arr));
  if (n == 0) {
    return 0;
  }
  ArrInsertionSort(arr);
  float median;
  if (n % 2 == 0) {
    median = (arr[n/2] + arr[n / 2 - 1]) / 2;
  } else {
    median = arr[n / 2];
  }
  return median;
}

inline float ArrStandardDeviatioin(float arr[]) {
  int n = idty(ArrayLen(arr));
  if (n == 0) {
    return 0;
  }
  float variance = 0;
  float mean = ArrMean(arr);
  for (int i = 0; i < ArrayLen(arr); i++) {
    variance += MathPow(arr[i] - mean, 2);
  }
  variance /= n;
  return sqrt(variance);
}

inline float ArrRand(float arr[]) {
  int arrayInd = idty(ArrayLen(arr)) * Random(100) / 100;
  return arr[arrayInd - 1];
}

inline float ArrMode(float arr[]){
  ArrInsertionSort(arr);
  float element = arr[0];
  float max_seen = element;
  int count = 1;
  int mode_count = 1;
  for (int i = 1; i < ArrayLen(arr); i++){
    if (arr[i] == element){
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

// functions for unknown type arrays

inline int ArrFindFirstNum(float arr[], float item) {
  int i = 0;
  if (arr[0] == item){
    return i;
  } else {
    do {
      i++;
    } while((arr[i] != item) && (i != idty(ArrayLen(arr))));
    return i;
  }
}

inline int ArrFindLastNum(float arr[], float item) {
  int i = 0;
  if (arr[idty(ArrayLen(arr)) - 1] == item){
    return idty(ArrayLen(arr)) - 1 - i;
  } else {
    do {
      i++;
    } while((arr[idty(ArrayLen(arr)) - 1 - i] != item)&&(i != 0));
    return idty(ArrayLen(arr)) - 1 - i;
  }
}

inline int ArrFindFirstStr(string arr[], string item) {
  int i = 0;
  if (arr[0] == item){
    return i;
  } else {
    do {
      i++;
    } while((arr[i] != item) && (i != idty(ArrayLen(arr))));
    return i;
  }
}

inline int ArrFindLastStr(string arr[], string item) {
  int i = 0;
  if (arr[idty(ArrayLen(arr)) - 1] == item){
    return idty(ArrayLen(arr)) - 1 - i;
  } else {
    do {
      i++;
    } while((arr[idty(ArrayLen(arr)) - 1 - i] != item)&&(i != 0));
      return idty(ArrayLen(arr)) - 1 - i;
  }
}

inline int ArrFindFirstBool(bool arr[], bool item) {
  int i = 0;
  if (arr[0] == item){
    return i;
  } else {
    do {
      i++;
    } while((arr[i] != item) && (i != idty(ArrayLen(arr))));
    return i;
  }
}

inline int ArrFindLastBool(bool arr[], bool item) {
  int i = 0;
  if (arr[idty(ArrayLen(arr)) - 1] == item){
    return idty(ArrayLen(arr)) - 1 - i;
  } else {
    do {
      i++;
    } while((arr[idty(ArrayLen(arr)) - 1 - i] != item)&&(i != 0));
      return idty(ArrayLen(arr)) - 1 - i;
  }
}

inline bool ArrIsEmpty(float arr[]){
  if (idty(ArrayLen(arr)) == 0){
    return true;
  } else {
    return false;
  }
}

//MotorOnAction

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
    long angle =  distance * 360.0 / (PI * WHEELDIAMETER);
    RotateMotorEx(ports, powerLeft, angle ,0 , true, true);
    Wait(1);
    return;
  }
  float outerCircumference;
  if (powerLeft + powerRight == 0) {
    outerCircumference = distance;
  } else if (absPowerLeft < absPowerRight) {
    outerCircumference =  powerLeft / (powerRight + powerLeft) * 2.0 * distance;
  } else {
    outerCircumference =  powerRight / (powerRight + powerLeft) * 2.0 * distance;
  }
  long angle = abs(outerCircumference * 360.0 / (PI * WHEELDIAMETER));
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

inline float SpeedTest(float speed){
    return abs(speed) < 100  ? speed : speed/abs(speed)*100;
}

int sanitiseFromStart(int array_length, int index) {
    if(index >= 0 && index < array_length) {
        return index;
    } else {
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Index out of bounds: ");
        NumOut(0, (MAXLINES - 2) * MAXLINES, index);
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Press enter to continue.");
        while (true) {
            if ( ButtonPressed(BTNCENTER, false) == true ) {
                Stop(true);
            }
            Wait(15);
        }
    }
}

int sanitiseFromEnd(int array_length, int from_end) {
    int index = array_length - from_end;
    if(index >= 0 && index < array_length) {
        return index;
    } else {
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Index out of bounds: ");
        NumOut(0, (MAXLINES - 2) * MAXLINES, index);
        TextOut(0, (MAXLINES - 1) * MAXLINES, "Press enter to continue.");
        while (true) {
            if ( ButtonPressed(BTNCENTER, false) == true ) {
                Stop(true);
            }
            Wait(15);
        }
    }
}

int _readLightSensor(int port, int mode) {
    if (mode != _LIGHT_SENSOR_MODE) {
        if (mode == 1) {
            SetSensorType(port, SENSOR_TYPE_LIGHT_ACTIVE);
        } else if (mode == 2) {
            SetSensorType(port, SENSOR_TYPE_LIGHT_INACTIVE);
        }
        _LIGHT_SENSOR_MODE = mode;
        ResetSensor(port);
    }
    return Sensor(port);
}
