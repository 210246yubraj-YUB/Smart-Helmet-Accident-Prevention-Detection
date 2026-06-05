#include <Wire.h>
#include <MPU6050.h>
#include <esp_now.h>
#include <WiFi.h>
#include <math.h>

// ================= MPU6050 =================
MPU6050 mpu(0x68);

// ================= PIN DEFINITIONS =================
#define IR_PIN           27
#define MQ3_ANALOG_PIN   34   // A0
#define MQ3_DIGITAL_PIN  14   // D0
#define BUZZER_PIN 26
#define ALCOHOL_LED 25

// ================= THRESHOLDS =================
#define ACC_THRESHOLD    30000
#define GYRO_THRESHOLD   20000
#define ALCOHOL_LIMIT    2500
#define NO_MOVEMENT_TIME 5000

// ================= ESP-NOW STRUCT =================
typedef struct struct_message {
  bool helmetWorn;
  bool alcoholDetected;
  bool accidentDetected;
} struct_message;

struct_message helmetData;

// Replace with RX ESP MAC address
uint8_t receiverMac[] = {0xCC, 0xDB, 0xA7, 0x9D, 0xC5, 0x54};

// ================= VARIABLES =================
unsigned long impactTime = 0;
bool impactDetected = false;

// ================= ESP-NOW SEND CALLBACK (ESP32 CORE 3.x FIX) =================
void onDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("ESP-NOW Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success ✅" : "Fail ❌");
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  // ================= IR & MQ3 INIT =================
  pinMode(IR_PIN, INPUT);
  pinMode(MQ3_DIGITAL_PIN, INPUT);

  // ================= I2C INIT =================
  Wire.begin(21, 22);

  // ================= LED AND BUZZER=================
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ALCOHOL_LED, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(ALCOHOL_LED, LOW);


  // ================= MPU INIT =================
  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 NOT connected ❌");
    while (1);
  }
  Serial.println("MPU6050 connected ✅");

  // ================= WIFI INIT =================
  WiFi.mode(WIFI_STA);

  // ================= ESP-NOW INIT =================
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed ❌");
    return;
  }

  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer ❌");
    return;
  }

  Serial.println("ESP-NOW Initialized ✅");
  Serial.println("Warming MQ-3 Sensor...");
  delay(30000);   // MQ-3 warm-up
  
}

void loop() {

  // ================= IR SENSOR =================
  int irValue = digitalRead(IR_PIN);
  helmetData.helmetWorn = (irValue == LOW);

  // ================= MQ-3 SENSOR =================
  int mq3Analog = analogRead(MQ3_ANALOG_PIN);
  //int mq3Digital = digitalRead(MQ3_DIGITAL_PIN);
  helmetData.alcoholDetected = (mq3Analog > ALCOHOL_LIMIT);

  // ================= ALCOHOL LED =================
  if (helmetData.alcoholDetected) {
  digitalWrite(ALCOHOL_LED, HIGH);   // LED ON
  }
  else{
  digitalWrite(ALCOHOL_LED, LOW);    // LED OFF
  }

  // ================= MPU6050 =================
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  long accMagnitude = sqrt((long)ax * ax +
                           (long)ay * ay +
                           (long)az * az);

  // Impact detection
  if (accMagnitude > ACC_THRESHOLD ||
      abs(gx) > GYRO_THRESHOLD ||
      abs(gy) > GYRO_THRESHOLD ||
      abs(gz) > GYRO_THRESHOLD) {

    impactTime = millis();
    impactDetected = true;
  }

  if (impactDetected && (millis() - impactTime > NO_MOVEMENT_TIME)) {
    helmetData.accidentDetected = true;
    impactDetected = false;
  } else {
    helmetData.accidentDetected = false;
  }

  // ================= BUZZER =================
  if (helmetData.accidentDetected) {
  digitalWrite(BUZZER_PIN, HIGH);   // BUZZER ON
  }
   else {
  digitalWrite(BUZZER_PIN, LOW);    // BUZZER OFF
  }


  // ================= SERIAL OUTPUT =================
  Serial.println("------------------------------------------------");
  Serial.print("IR Helmet Status: ");
  Serial.println(helmetData.helmetWorn ? "  WORN ✅" : "NOT WORN ❌  ");

  Serial.print("MQ3 Analog: ");
  Serial.print(mq3Analog);
  //Serial.print(" | Digital: ");
  //Serial.print(mq3Digital);
  Serial.println(helmetData.alcoholDetected ? " 🚨 ALCOHOL DETECTED  " : "  ALCOHOL STATUS-> OK✅  ");

  Serial.print("Accel Mag: ");
  Serial.print(accMagnitude);
  Serial.print(" | Gyro: ");
  Serial.print(abs(gx));
  Serial.print(" ");
  Serial.print(abs(gy));
  Serial.print(" ");
  Serial.println(abs(gz));

  Serial.print("ACCIDENT STATUS: ");
  Serial.println(helmetData.accidentDetected ? "🚨 YES " : " NO ");

  // ================= SEND DATA =================
  esp_now_send(receiverMac, (uint8_t *)&helmetData, sizeof(helmetData));

  delay(2000);
}