#ifndef NEPOBLUETOOTH
#define NEPOBLUETOOTH

BluetoothConnectionHandle NEPOConnectTo(std::string name) {
    NEPOInitEV3(); // connection may be assigned to a variable before main
    return ConnectTo((char *) name.c_str());
}

BluetoothConnectionHandle NEPOWaitConnection() {
    NEPOInitEV3(); // connection may be assigned to a variable before main
    return WaitConnection();
}

void NEPOSendStringTo(BluetoothConnectionHandle to, std::string string) {
    SendStringTo(to, (char *) string.c_str());
}

std::string NEPOReceiveStringFrom(BluetoothConnectionHandle from) {
    char buffer[128];
    ReceiveStringFrom(from, buffer, 128);
    return buffer;
}

#endif