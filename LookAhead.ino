#include <SoftwareSerial.h>
#include <Servo.h>

//https://www.instructables.com/id/Arduino-Android-Bluetooth-Car/

Servo myservo;
SoftwareSerial myBlue(10, 11);

struct Ultrasonic {
  int trigPin;
  int echoPin;
  int distance;
  int grades;
  int id;
};

Ultrasonic usc[6];

long duration;
int distance;
int n = 0;

struct Ultrasonic newUsc(int trigPin, int echoPin, int grades, int id) {
  struct Ultrasonic usc1;
  usc1.trigPin = trigPin;
  usc1.echoPin = echoPin;
  usc1.grades = grades;
  usc1.id = id;
  return usc1;
}

void setup() {
  //               Trig Echo
  usc[0] = newUsc(24, 25, 155, 0);
  usc[1] = newUsc(27, 26, 100, 1);
  usc[2] = newUsc(22, 23, 55, 2);
  usc[3] = newUsc(2, 3, 20, 3);
  usc[4] = newUsc(28, 29, 155, 4);
  usc[5] = newUsc(30, 31, 100, 5);

  //Conexion de pines
  for (int x = 0; x < 6; x++) {
    pinMode(usc[x].trigPin, OUTPUT);
    pinMode(usc[x].echoPin, INPUT);
  }

  myservo.attach(9);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Listo");
  myBlue.begin(9600);
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void setPos(int x) {
  myservo.write(x);
  delay(50);
}

void restartPos() {
  setPos(0);
}

/**
   Obtiene el indice del ultrasonico con la menor distancia.
*/
int getMin() {
  int minimum = usc[0].distance, n1 = 0;

  for (int x = 0; x < 6; x++) {
    if (usc[x].distance < minimum) {
      minimum = usc[x].distance;
      n1 = x;
    }
  }
  return n1;
}

void loop()
{
  //  usc[2].distance = getDistance(usc[2].trigPin, usc[2].echoPin);

  // Indice del ultrasonico.
  

  //Serial.print("ultrasonic:");
  //Serial.println(usc[n].id);

//  Serial.print("Distance: ");
//  Serial.println(usc[n].distance);

//  if (Serial.available()) {
//    String miPC = Serial.readString();
//    myBlue.println(usc[n].distance);
//  }
  if (myBlue.available()) {
      //Actualiza las distancias.
    usc[0].distance = getDistance(usc[0].trigPin, usc[0].echoPin);
    usc[1].distance = getDistance(usc[1].trigPin, usc[1].echoPin);
    usc[2].distance = getDistance(usc[2].trigPin, usc[2].echoPin);
    usc[3].distance = getDistance(usc[3].trigPin, usc[3].echoPin);
    usc[4].distance = getDistance(usc[4].trigPin, usc[4].echoPin);
    usc[5].distance = getDistance(usc[5].trigPin, usc[5].echoPin);

    n = getMin();
    
    String msj = myBlue.readString();
    Serial.println(msj);
    myBlue.flush();

    setPos(usc[n].grades);
    msj= "{ Distancia: " + String(abs(usc[n].distance)) + ", id: " + String(usc[n].id) + "}";
    myBlue.println(msj);
  }
}
