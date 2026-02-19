#include <AFMotor.h>
#include <Servo.h>
#include <SoftwareSerial.h>

AF_DCMotor motorKiri(1);
AF_DCMotor motorKanan(2);

Servo servoLift;  // Pengangkat
Servo servoGrip;  // Pencapit

SoftwareSerial btSerial(A0, A1);  // RX, TX

// Posisi Servo
int posLift = 140;        // Posisi awal pengangkat
int posGrip = 90;         // Posisi awal capit
const int stepServo = 1;  // Kecepatan gerak

// const int SERVO1_TOLERANCE = 46;  // dead zone

char command;
int speedVal = 155;  // Kecepatan maksimal (0-255)
int servoVal1 = 90;
int servoVal2 = 155;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

  // Setup Motor
  motorKiri.setSpeed(speedVal);
  motorKanan.setSpeed(speedVal);
  motorKiri.run(RELEASE);
  motorKanan.run(RELEASE);

  // Setup Servo
  servoGrip.attach(10);
  servoLift.attach(9);

  // Inisialisasi posisi awal
  servoLift.write(posLift);
  servoGrip.write(posGrip);

  setServo1();

  Serial.println("Sistem Transporter Siap!");
}

void loop() {
  if (btSerial.available() > 0) {
    command = btSerial.read();

    switch (command) {
      case 'F': maju(); break;
      case 'B': mundur(); break;
      case 'L': belokKiri(); break;
      case 'R': belokKanan(); break;
      case 'S': berhenti(); break;
      case 'Z': throwBox(); break;

      case '1':
        servoVal1 = 90;
        setServo1();
        break;

      case '2':
        servoVal1 = 135;
        setServo1();
        break;

      case '3':
        servoVal2 = 180;
        setServo2();
        break;

      case '4':
        // servoVal2 = 120;
        // setServo2();
        F4();
        break;

      case 'Y':
        servoVal2 = 50;
        setServo2();
        break;
    }
}

void F4() {
  servoVal1 = 135;
  setServo1();
  delay(500);
  servoVal2 = 180; // moveraa
  // servoVal2 = 50; // terminator
  setServo2();
}

void setServo1() {
  servoGrip.write(servoVal1);
}

void setServo2() {
  // servoVal2 = 50;
  // setServo2();
  servoLift.write(servoVal2);
}

void throwBox() {
  servoVal2 = 120;
  setServo2();
  delay(500);
  servoVal1 = 90;
  setServo1();
  delay(500);
  mundur();
  delay(100);
  berhenti();
  // break;
}

void maju() {
  motorKiri.run(FORWARD);
  motorKanan.run(FORWARD);
}

void mundur() {
  motorKiri.run(BACKWARD);
  motorKanan.run(BACKWARD);
}

void belokKiri() {
  motorKiri.run(BACKWARD);
  motorKanan.run(FORWARD);
}

void belokKanan() {
  motorKiri.run(FORWARD);
  motorKanan.run(BACKWARD);
}

void berhenti() {
  motorKiri.run(RELEASE);
  motorKanan.run(RELEASE);
}
