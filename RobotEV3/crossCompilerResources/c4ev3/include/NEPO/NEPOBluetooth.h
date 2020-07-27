#ifndef NEPOBLUETOOTH
#define NEPOBLUETOOTH


#define BLUETOOTH_STRING_MESSAGE_MAX_LENGTH 128
#define BLUETOOTH_CONNECTION_ATTEMPTS 10

void NotifyBluetoothNameNotFoundError();

BluetoothConnectionHandle NEPOConnectTo(std::string name) {
    NEPOInitEV3(); // connection may be assigned to a variable before main
    BluetoothConnectionHandle handle = ConnectTo((char *) name.c_str(), BLUETOOTH_CONNECTION_ATTEMPTS);
    if (handle == -1) {
        NEPOError("Unknown bluetooth name", "Please pair the robots using the EV3 menu");
    }
    return handle;
}

void NotifyBluetoothNameNotFoundError () {

    DrawString("ERROR", 8, 2);
    DrawString("Unknown bluetooth name", 0, 4);
    DrawString("Please pair the robots using the EV3 menu", 0, 6);
    DrawString("CENTER + DOWN to exit", 1, 11);
    PlaySystemSound(SOUND_LOW_BEEP);
    while(true) {
        Wait(1000);
    }
}

BluetoothConnectionHandle NEPOWaitConnection() {
    NEPOInitEV3(); // connection may be assigned to a variable before main
    return WaitConnection();
}

void NEPOSendStringTo(BluetoothConnectionHandle to, std::string string) {
    SendStringTo(to, (char *) string.c_str());
}

std::string NEPOReceiveStringFrom(BluetoothConnectionHandle from) {
    char buffer[BLUETOOTH_STRING_MESSAGE_MAX_LENGTH];
    ReceiveStringFrom(from, buffer, BLUETOOTH_STRING_MESSAGE_MAX_LENGTH);
    return buffer;
}

#endif