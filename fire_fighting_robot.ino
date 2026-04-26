#include <Servo.h>

// ================= PINS =================

// Flame Sensors - ANALOG (For Precision Tracking & Distance)
const int Ls = A0;
const int Cs = A1;
const int Rs = A2;

// Flame Sensors - DIGITAL (For Hardware Noise-Filtering)
const int LD = 4;
const int CD = 3;
const int RD = 2;

// Drive Motors (L298N #1)
const int ENA = 5;  
const int ENB = 6;  
const int IN1 = 7;
const int IN2 = 8;
const int IN3 = 9;  
const int IN4 = 10; 

// Water Pump (L298N #2)
const int PUMP_EN  = 11; 
const int PUMP_IN1 = 12;
const int PUMP_IN2 = 13;

// Servo Motor
const int servoPin = A3;
Servo waterTurret;

// ================= SETTINGS =================


int stopThreshold = 100;  

// Speeds
int baseSpeed = 100;
int scanSpeed = 75;
int maxSpeed  = 180;

// True PID Tuning
float Kp = 0.08; 
float Kd = 0.30; 
int lastError = 0;

// ================= SETUP =================

void setup() {
  Serial.begin(9600);

  // Digital Sensor Pins (Inputs)
  pinMode(LD, INPUT);
  pinMode(CD, INPUT);
  pinMode(RD, INPUT);

  // Drive Pins
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // Pump Pins
  pinMode(PUMP_EN, OUTPUT);
  pinMode(PUMP_IN1, OUTPUT);
  pinMode(PUMP_IN2, OUTPUT);

  // Servo Setup
  waterTurret.attach(servoPin);
  waterTurret.write(90); 

  stopMotors();
  stopPump();

  Serial.println("🔥 HYBRID SYSTEM ");
  delay(1000);
}

// ================= MAIN LOOP =================

void loop() {
  // Read Analog (For Steering & Distance)
  int aL = analogRead(Ls);
  int aC = analogRead(Cs);
  int aR = analogRead(Rs);

  // Read Digital (For Wake-up state). Most sensors go LOW (0) when fire is seen.
  bool fireL = (digitalRead(LD) == LOW);
  bool fireC = (digitalRead(CD) == LOW);
  bool fireR = (digitalRead(RD) == LOW);

  // Does ANY digital sensor see a fire?
  bool fireDetected = (fireL || fireC || fireR);

  // Debug stream
  Serial.print("D["); Serial.print(fireL); Serial.print(fireC); Serial.print(fireR); Serial.print("] | ");
  Serial.print("A[L:"); Serial.print(aL); Serial.print(" C:"); Serial.print(aC); Serial.print(" R:"); Serial.print(aR); Serial.print("]");

  // ================= 1. EXTINGUISH MODE =================
  // We still use Analog here because we need to know if the fire is point-blank
  if (aL < stopThreshold || aC < stopThreshold || aR < stopThreshold) {
    Serial.println(" -> [EXTINGUISH]");
    stopMotors();
    delay(200); 
    
    startPump(200); 

    // High-Speed Sweep
    for (int angle = 90; angle >= 50; angle -= 4) { waterTurret.write(angle); delay(10); }
    for (int angle = 50; angle <= 130; angle += 4) { waterTurret.write(angle); delay(10); }
    for (int angle = 130; angle >= 90; angle -= 4) { waterTurret.write(angle); delay(10); }

    stopPump();
    delay(1000); 
    return;      
  }

  // ================= 2. SCAN MODE =================
  // If the hardware digital pins say "No Fire", ignore the analog noise and just spin!
  if (!fireDetected) {
    Serial.println(" -> [SCANNING]");
    setMotor(scanSpeed, -scanSpeed); 
    lastError = 0; 
    return;
  }

  // ================= 3. PID TRACKING =================
  // The digital pins woke us up. Now we use the Analog math to steer smoothly.
  int error = aL - aR; 

  int derivative = error - lastError;
  int correction = (Kp * error) + (Kd * derivative);
  lastError = error;

  int speedDrop = abs(error) / 15; 
  int currentBase = constrain(baseSpeed - speedDrop, 60, baseSpeed);

  int leftSpeed  = currentBase + correction;
  int rightSpeed = currentBase - correction;

  leftSpeed  = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  Serial.print(" | Err:"); Serial.print(error);
  Serial.print(" | L_Spd:"); Serial.print(leftSpeed);
  Serial.print(" R_Spd:");   Serial.println(rightSpeed);

  setMotor(leftSpeed, rightSpeed);
}

// ================= MOTOR CONTROL =================
void setMotor(int left, int right) {
  if (left >= 0) {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW); analogWrite(ENA, left);
  } else {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); analogWrite(ENA, -left);
  }
  if (right >= 0) {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW); analogWrite(ENB, right);
  } else {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH); analogWrite(ENB, -right);
  }
}

void stopMotors() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// ================= PUMP CONTROL =================
void startPump(int pressure) {
  digitalWrite(PUMP_IN1, HIGH);
  digitalWrite(PUMP_IN2, LOW);
  pressure = constrain(pressure, 0, 170);
  analogWrite(PUMP_EN, pressure); 
}

void stopPump() {
  digitalWrite(PUMP_IN1, LOW);
  digitalWrite(PUMP_IN2, LOW);
  analogWrite(PUMP_EN, 0);
}