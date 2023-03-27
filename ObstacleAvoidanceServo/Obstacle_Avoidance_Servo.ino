----------------------------------------------------------------------
* Programmname...............:  Hinderniserkennung                     
* Autor......................:  Ahmed Celen                                                                               
* Aufgabe....................:  Hinderniserkennung mit einem Ultraschallsensor und Ausweichmanöver                                                                       
* Erstellt am................:  10.12.2022                                                                                                                                                                         
----------------------------------------------------------------------

#include <Ultrasonic.h>
#include <Servo.h>

Ultrasonic ultrasonic(12, 9);  // (Trig Pin, Echo Pin)
int speed = 80; // Motordrehzahl

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

  motorServo.attach(13);
  Serial.begin(9600);
}

void loop() {
  
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
