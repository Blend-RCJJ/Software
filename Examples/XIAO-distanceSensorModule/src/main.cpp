#include <PCA95x5.h>
#include <VL53L0X.h>

const int numOfSensors = 12;
const char firstAddr   = 0x30;

VL53L0X distanceSensor[numOfSensors];

PCA9555 ioex;

const int xshutPin[] = {P02, P01, P00, P17, P16, P15,
                        P14, P13, P12, P11, P04, P03};

void deviceScanner(void);

void ioexInit(void);
void distanceSensorInit(void);

void setup() {
    Serial.begin(2000000);  // PCとの通信を開通
    Wire.begin();           // I2C通信を開通

    ioexInit();  // GPIOエキスパンダの初期化
    distanceSensorInit();
}

void loop() {
    for (int i = 0; i < numOfSensors; i++) {
        Serial.print(distanceSensor[i].readRangeContinuousMillimeters());
        Serial.print("\t");
    }

    Serial.println();
}

void deviceScanner(void) {  //デバイスの接続チェック
    char error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("I2C device found at address 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

            nDevices++;
        } else if (error == 4) {
            Serial.print("Unknown error at address 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");

    delay(1000);
}

void ioexInit(void) {
    ioex.attach(Wire);
    ioex.polarity(PCA95x5::Polarity::ORIGINAL_ALL);
    ioex.direction(PCA95x5::Direction::OUT_ALL);
    ioex.write(PCA95x5::Level::L_ALL);
}

void distanceSensorInit(void) {
    for (int i = 0; i < numOfSensors; i++) {
        ioex.write(xshutPin[i], PCA95x5::Level::H);  // VL53L0Xの電源をON
        distanceSensor[i].setTimeout(500);

        if (!distanceSensor[i].init()) {  // ERROR
            Serial.println("Failed to detect and initialize sensor!");
            while (1) {  // FIXME:無限ループは流石にまずいので修正すること
            }
        }

        distanceSensor[i].setAddress(firstAddr + i);  //順にアドレスを割り当て

        //測定距離を広げる
        distanceSensor[i].setSignalRateLimit(0.1);
        distanceSensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        distanceSensor[i].setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange,
                                              14);

        distanceSensor[i].setMeasurementTimingBudget(20000);  //高速化
        // distanceSensor[i].setMeasurementTimingBudget(200000);//精度重視

        distanceSensor[i].startContinuous();  //計測開始
    }
}