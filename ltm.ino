#include <Arduino.h>

// ==== Motores ====
#define motor1A 13 
#define motor2A 14
#define motor1B 26
#define motor2B 25

// ==== Sensores ====
const int rightAvoidPin = 18; 
const int leftAvoidPin = 19; 
const int linePin = 33; // central

// ==== PWM ====
const int freq = 500; 
const int resolution = 8; 

// ==== Variables de estado ====
enum TurnDirection { LEFT, RIGHT, NONE };
TurnDirection lastTurn = NONE;

void setup() {
  Serial.begin(115200);

  // Configuración PWM
  ledcAttach(motor1A, freq, resolution);
  ledcAttach(motor2A, freq, resolution);
  ledcAttach(motor1B, freq, resolution);
  ledcAttach(motor2B, freq, resolution);

  // Sensores
  pinMode(rightAvoidPin, INPUT);
  pinMode(leftAvoidPin, INPUT);
  pinMode(linePin, INPUT);
}

void loop() {
  // Sensores típicos: activo en LOW → invertimos
  int leftSensor   = !digitalRead(leftAvoidPin);
  int centerSensor = !digitalRead(linePin);
  int rightSensor  = !digitalRead(rightAvoidPin);

  // Línea delgada ⇒ normalmente solo 1 sensor activo
  int state = (leftSensor << 2) | (centerSensor << 1) | rightSensor;

  //        L C R
  switch (state) {
    case 0b010: // 0 1 0  → solo CENTRO: avanzar recto rápido
      forward(255);
      lastTurn = NONE;
      break;

    case 0b100: // 1 0 0  → solo IZQUIERDA: corrige a la izquierda (suave)
      slightLeft();
      lastTurn = LEFT;
      break;

    case 0b001: // 0 0 1  → solo DERECHA: corrige a la derecha (suave)
      slightRight();
      lastTurn = RIGHT;
      break;

    case 0b110: // 1 1 0  → centro+izq (raro con línea delgada): sigue corrigiendo izq
      slightLeft();
      lastTurn = LEFT;
      break;

    case 0b011: // 0 1 1  → centro+der: sigue corrigiendo der
      slightRight();
      lastTurn = RIGHT;
      break;

    case 0b111: // 1 1 1  → los tres sobre negro (línea ancha/intersección): avanza
      forward(255);
      lastTurn = NONE;
      break;

    case 0b000: // 0 0 0  → ninguno ve línea: BÚSQUEDA según último giro
    default:
      if (lastTurn == LEFT) {
        turnLeft();   // pivota hacia donde la vio por última vez
      } else {
        turnRight();
        lastTurn = RIGHT;
      }
      break;
  }

  delay(5); // respuesta rápida
}

// ==== Funciones de movimiento ====
void forward(int speed) {
  ledcWrite(motor1A, 0);
  ledcWrite(motor2A, speed);
  ledcWrite(motor1B, 0);
  ledcWrite(motor2B, speed);
}

void turnLeft() {
  ledcWrite(motor1A, 0);
  ledcWrite(motor2A, 0);     
  ledcWrite(motor1B, 0);
  ledcWrite(motor2B, 255); 
}

void slightLeft() {
  ledcWrite(motor1A, 0);
  ledcWrite(motor2A, 200);  // mínimo
  ledcWrite(motor1B, 0);
  ledcWrite(motor2B, 255);  // más rápido
}

void turnRight() {
  ledcWrite(motor1A, 0);
  ledcWrite(motor2A, 255); 
  ledcWrite(motor1B, 0);
  ledcWrite(motor2B, 0);
}

void slightRight() {
  ledcWrite(motor1A, 0);
  ledcWrite(motor2A, 255);  // más rápido
  ledcWrite(motor1B, 0);
  ledcWrite(motor2B, 200);  // mínimo
}
