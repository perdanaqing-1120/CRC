/*
 * Proyek: Arduino Transporter Robot (Bluetooth Control)
 * Driver: L293D Motor Shield
 * Fitur: 2 Motor DC, 2 Servo (Lift & Grip), Bluetooth HC-05
 * Author: Akhsan Rian Anugrah, S.Pd., Gr.
 */

#include <AFMotor.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// --- Definisi Motor DC ---
// Asumsi motor kiri di port 1, motor kanan di port 2
AF_DCMotor motorKiri(1);
AF_DCMotor motorKanan(2);

// --- Definisi Servo ---
Servo servoLift;  // Servo Pengangkat
Servo servoGrip;  // Servo Pencapit

// --- Definisi Bluetooth (SoftwareSerial) ---
// Kita pakai A0 sebagai RX dan A1 sebagai TX karena pin digital D0-D12
// (kecuali D2 & D13) dipakai oleh Motor Shield.
SoftwareSerial btSerial(A0, A1);  // RX, TX

// --- Variabel Posisi Servo ---
int posLift = 140;        // Posisi awal pengangkat (tengah)
int posGrip = 90;         // Posisi awal pencapit (terbuka/tengah)
const int stepServo = 1;  // Kecepatan gerak servo (semakin besar semakin cepat)

// const int SERVO1_TOLERANCE = 46;  // dead zone ±3 derajat (servoGrip)

// --- Variabel Data ---
char command;
int speedVal = 155;  // Kecepatan maksimal (0-255)
int servoVal1 = 90;
int servoVal2 = 155;

void setup() {
  Serial.begin(9600);    // Untuk debugging di Serial Monitor
  btSerial.begin(9600);  // Default baud rate HC-05

  // Setup Motor
  motorKiri.setSpeed(speedVal);
  motorKanan.setSpeed(speedVal);
  motorKiri.run(RELEASE);
  motorKanan.run(RELEASE);

  // Setup Servo (Pin 9 dan 10 adalah standar pin Servo di Shield L293D)
  servoGrip.attach(10);  // Biasanya label 'SER1' di shield
  servoLift.attach(9);   // Biasanya label 'SER2' di shield

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
      // ===== MOTOR =====
      case 'F': maju(); break;
      case 'B': mundur(); break;
      case 'L': belokKiri(); break;
      case 'R': belokKanan(); break;
      case 'S': berhenti(); break;
      case 'Z': throwBox(); break;
      // ===== SERVO GRIP =====
      case '1':  // buka
        servoVal1 = 90;
        setServo1();
        break;

      case '2':  // tutup / kembali
        servoVal1 = 135;
        setServo1();
        break;

      case '3':  // tidakangkat
        servoVal2 = 180;
        setServo2();
        break;

      case 'Y':  // SETENGAH ANGKAT
        servoVal2 = 70;
        setServo2();
        break;

      case '4':  // tutup / kembali
        servoVal2 = 140;
        setServo2();
        break;
    }
  }
}


// --- Fungsi-fungsi Pembantu ---

void setServo1() {
  servoGrip.write(servoVal1);
}

void setServo2() {
  servoLift.write(servoVal2);
}

void throwBox() {
  servoVal1 = 90;
  servoVal2 = 140;
  setServo2();
  delay(80);
  setServo1();
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
  motorKiri.run(BACKWARD);  // Tank turn (putar di tempat)
  motorKanan.run(FORWARD);
}

void belokKanan() {
  motorKiri.run(FORWARD);
  motorKanan.run(BACKWARD);  // Tank turn
}

void berhenti() {
  motorKiri.run(RELEASE);
  motorKanan.run(RELEASE);
}
