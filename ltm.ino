#include <Arduino.h>

// ==== Motores ====
// Motor Izquierdo -> IO13 (A), IO14 (B)
// Motor Derecho   -> IO26 (A), IO25 (B)
#define motor1A 26 
#define motor1B 25
#define motor2A 14
#define motor2B 13

// ==== Sensores ====
const int rightAvoidPin = 18; 
const int leftAvoidPin  = 19; 
const int linePin       = 33; // central

// ==== PWM ====
const int freq = 500; 
const int resolution = 8; 

// ==== Variables de estado ====
enum TurnDirection { LEFT, RIGHT, NONE };
TurnDirection lastTurn = NONE;

void setup() {
  Serial.begin(115200);

  // Configuración PWM simplificada (Sunfounder style)
  ledcAttach(motor1A, freq, resolution);
  ledcAttach(motor1B, freq, resolution);
  ledcAttach(motor2A, freq, resolution);
  ledcAttach(motor2B, freq, resolution);

  // Sensores
  pinMode(rightAvoidPin, INPUT);
  pinMode(leftAvoidPin, INPUT);
  pinMode(linePin, INPUT);
}

void loop() {
  int leftSensor   = digitalRead(leftAvoidPin);
  int centerSensor = digitalRead(linePin);
  int rightSensor  = digitalRead(rightAvoidPin);

  int state = (leftSensor << 2) | (centerSensor << 1) | rightSensor;

  Serial.print("L=");
  Serial.print(leftSensor);
  Serial.print(" C=");
  Serial.print(centerSensor);
  Serial.print(" R=");
  Serial.print(rightSensor);
  Serial.print(" | Acción: ");

  switch (state) {
    case 0b010: // solo CENTRO
      forward(180);
      Serial.println("Forward");
      lastTurn = NONE;
      break;

    case 0b100: // solo IZQUIERDA
    case 0b110: // centro+izq
      slightLeft();
      Serial.println("Slight Left");
      lastTurn = LEFT;
      break;

    case 0b001: // solo DERECHA
    case 0b011: // centro+der
      slightRight();
      Serial.println("Slight Right");
      lastTurn = RIGHT;
      break;

    case 0b000: // ninguno ve línea
    default:
      if (lastTurn == LEFT) {
        turnLeft();
        Serial.println("Lost line → Turn Left (search)");
      } else {
        turnRight();
        Serial.println("Lost line → Turn Right (search)");
        lastTurn = RIGHT;
      }
      break;
  }

  delay(50);
}

// ==== Funciones de movimiento ====
// Cada motor se controla con 2 pines (A y B)

void motorLeftForward(int speed) {
  ledcWrite(motor1A, speed);
  ledcWrite(motor1B, 0);
}

void motorRightForward(int speed) {
  ledcWrite(motor2A, speed);
  ledcWrite(motor2B, 0);
}

void motorLeftStop() {
  ledcWrite(motor1A, 0);
  ledcWrite(motor1B, 0);
}

void motorRightStop() {
  ledcWrite(motor2A, 0);
  ledcWrite(motor2B, 0);
}

void forward(int speed) {
  motorLeftForward(speed);
  motorRightForward(speed);
}

void turnLeft() {
  motorLeftStop();
  motorRightForward(200);
}

void slightLeft() {
  motorLeftForward(80);   // lento
  motorRightForward(200); // rápido
}

void turnRight() {
  motorLeftForward(200);
  motorRightStop();
}

void slightRight() {
  motorLeftForward(200); // rápido
  motorRightForward(80); // lento
}
