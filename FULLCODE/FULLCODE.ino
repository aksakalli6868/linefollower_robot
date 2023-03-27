----------------------------------------------------------------------
* Programmname...............:  Hinderniserkennung                     
* Autor......................:  Ahmed Celen                                                                               
* Aufgabe....................:  Linienverfolgung und Hinderniserkennung [mit Ultraschallsensor und KY-033 Sensoren]                                                                 
* Erstellt am................:  12.02.2023                                                                                                                                                                         
----------------------------------------------------------------------

#include <Ultrasonic.h>
#include <Servo.h>

Ultrasonic ultrasonic(12, 9);  // (Trig Pin, Echo Pin)
int speed = 80;

// Diese Zeilen definieren die Pin-Nummern für die Sensoren und Motortreiberanschlüsse.
const int left1_Sensor = 2;
const int left2_Sensor = 3;
const int mid_Sensor = 4;
const int right2_Sensor = 7;
const int right1_Sensor = 8;

// Motor A Verbindungen
int motorPin1 = 11;
int motorPin2 = 10;
// Motor B Verbindungen
int motorPin3 = 6;
int motorPin4 = 5;

Servo motorServo;

// Diese Funktion wird einmal am Anfang des Programms ausgeführt
// und setzt die Pin-Modi für den Servotreiber und Motortreiber.
void setup() {
  Serial.begin(9600);          // Initialisierung serielle Ausgabe
  pinMode(motorPin1, OUTPUT);  // Legt den Pin-Modus für motorPin1 fest.
  pinMode(motorPin2, OUTPUT);  // Legt den Pin-Modus für motorPin2 fest.
  pinMode(motorPin3, OUTPUT);  // Legt den Pin-Modus für motorPin3 fest.
  pinMode(motorPin4, OUTPUT);  // Legt den Pin-Modus für motorPin4 fest.

  pinMode(left1_Sensor, INPUT);   //Legt den Pin-Modus für den Sensor ganz links fest.
  pinMode(left2_Sensor, INPUT);   // Legt den Pin-Modus für den zweiten Sensor links fest.
  pinMode(mid_Sensor, INPUT);     // Legt den Pin-Modus für den Sensor in der Mitte fest.
  pinMode(right2_Sensor, INPUT);  // Legt den Pin-Modus für den zweiten Sensor links fest.
  pinMode(right1_Sensor, INPUT);  // Legt den Pin-Modus für den Sensor ganz rechts fest.

  motorServo.attach(13);
  Serial.begin(9600);
}

void loop() {
  // Sensorwerte auslesen und speichern
  bool left1_Val = digitalRead(left1_Sensor);
  bool left2_Val = digitalRead(left2_Sensor);
  bool mid_Val = digitalRead(mid_Sensor);
  bool right2_Val = digitalRead(right2_Sensor);
  bool right1_Val = digitalRead(right1_Sensor);

  // Diese Reihe von if/else-Anweisungen bestimmt, was der Roboter basierend auf den Sensorwerten tun soll.
  if (left1_Val == LOW && left2_Val == LOW && mid_Val == HIGH && right2_Val == LOW && right1_Val == LOW) {
    // Wenn der mittlere Sensor die Linie erkennt, bewegen Sie den Roboter vorwärts.
    moveForward();
  } else if (left1_Val == HIGH && left2_Val == HIGH && mid_Val == LOW && right2_Val == LOW && right1_Val == LOW) {
    // Wenn die beiden linken Sensoren die Linie erkennen, bewegt sich der rechte Motor.
    rightMotorForward(motorSpeed_G3);
  } else if (left1_Val == HIGH && left2_Val == LOW && mid_Val == LOW && right2_Val == LOW && right1_Val == LOW) {
    // Wenn nur der linke Sensor die Linie erkennt, bewegt sich der rechte Motor.
    rightMotorForward(motorSpeed_G3);
  } else if (left1_Val == LOW && left2_Val == HIGH && mid_Val == LOW && right2_Val == LOW && right1_Val == LOW) {
    // Wenn der zweite linke Sensor die Linie erkennt, bewegt sich der rechte Motor.
    rightMotorForward(motorSpeed_G3);
  } else if (left1_Val == LOW && left2_Val == LOW && mid_Val == LOW && right2_Val == HIGH && right1_Val == HIGH) {
    // Wenn die beiden rechten Sensoren die Linie erkennen, bewegt sich der linke Motor.
    leftMotorForward(motorSpeed_G3);
  } else if (left1_Val == LOW && left2_Val == LOW && mid_Val == LOW && right2_Val == LOW && right1_Val == HIGH) {
    // Wenn nur der rechte Sensor die Linie erkennt, bewegt sich der linke Motor.
    leftMotorForward(motorSpeed_G3);
  } else if (left1_Val == LOW && left2_Val == LOW && mid_Val == LOW && right2_Val == HIGH && right1_Val == LOW) {
    // Wenn der zweite rechte Sensor die Linie erkennt, bewegt sich der linke Motor.
    leftMotorForward(motorSpeed_G3);
  } else if (left1_Val == LOW && left2_Val == LOW && mid_Val == LOW && right2_Val == LOW && right1_Val == LOW) {
    // Dieser Codeblock prüft, ob alle Sensoren eine schwarze Linie erkennen. Wenn dies der Fall ist, werden beide Motoren
    // für kurze Zeit (50 ms) mit einer bestimmten Geschwindigkeit (motorSpeed_G2) vorwärts bewegt und dann die Motoren gestoppt.
    // Dies kann als Strategie verwendet werden, um sich auf einer geraden schwarzen Linie vorwärts zu bewegen, ohne
    // vom Pfad abzuweichen.
    rightMotorForward(motorSpeed_G2);
    delay(50);
    stopMotors();
    leftMotorForward(motorSpeed_G2);
    delay(50);
    stopMotors();
  } else {
    // Bei einem unerwarteten Zustand stoppen die Motoren.
    stopMotors();
  }

  // Kontrolle von Hindernissen
  int distance = ultrasonic.read();
  Serial.println("Distanz (Vorne): ");
  Serial.println(distance);

  if (distance < 20) {
    // Kontrolle der Distanz auf der linken Seite
    stopMotors();
    motorServo.write(180);
    delay(500);
    int leftdistance = ultrasonic.read();

    Serial.println("Distanz (Links): ");
    Serial.println(leftdistance);

    if (leftdistance < 20) {
      stopMotors();
      // Kontrolle der Distanz auf der rechten Seite
      motorServo.write(0);
      delay(500);
      int rightdistance = ultrasonic.read();
      if (rightdistance < 20) {
        // Alle Wege besetzt, der Motor wird gestoppt
        motorServo.write(90);
        delay(500);
        stopMotors();
      } else {
        // Motor dreht sich nach rechts
        motorServo.write(0);
        delay(500);
        leftMotorForward();
      }
    } else {
      // Motor dreht sich nach links
      motorServo.write(180);
      delay(500);
      rightMotorForward();
    }
  } else {
    // Beide Motoren bewegen sich nach vorne
    motorServo.write(90);
    delay(500);
    rightMotorForward();
    leftMotorForward();
  }
}

// Motor Funktionen
void rightMotorForward() {
  analogWrite(motorPin1, speed);
  digitalWrite(motorPin2, speed);
}
void leftMotorForward() {
  analogWrite(motorPin3, 90);
  digitalWrite(motorPin4, LOW);
}
void stopMotors() {
  analogWrite(motorPin1, LOW);
  analogWrite(motorPin2, LOW);
  analogWrite(motorPin3, LOW);
  analogWrite(motorPin4, LOW);
}
