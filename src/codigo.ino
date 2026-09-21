#include <Servo.h>

Servo servoBase;
Servo servoOmbro;
Servo servoCotovelo;
Servo servoGarra;

const int trigPin = 10;
const int echoPin = 11;

int anguloBase = 90;
int anguloOmbro = 90;
int anguloCotovelo = 90;
int anguloGarra = 90;

void setup() {
  servoBase.attach(3);
  servoOmbro.attach(5);
  servoCotovelo.attach(6);
  servoGarra.attach(9);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);

  return duracao * 0.0343 / 2;
}

void loop() {
  int valorBase = analogRead(A0);
  int valorOmbro = analogRead(A1);
  int valorCotovelo = analogRead(A2);
  int valorGarra = analogRead(A3);

  int alvoBase = map(valorBase, 0, 1023, 0, 180);
  int alvoOmbro = map(valorOmbro, 0, 1023, 0, 180);
  int alvoCotovelo = map(valorCotovelo, 0, 1023, 0, 180);
  int alvoGarra = map(valorGarra, 0, 1023, 0, 180);

  float distancia = medirDistancia();

  if (anguloBase < alvoBase) {
    anguloBase++;
  } else if (anguloBase > alvoBase) {
    anguloBase--;
  }

  if (anguloOmbro < alvoOmbro) {
    anguloOmbro++;
  } else if (anguloOmbro > alvoOmbro) {
    anguloOmbro--;
  }

  if (anguloCotovelo < alvoCotovelo) {
    anguloCotovelo++;
  } else if (anguloCotovelo > alvoCotovelo) {
    anguloCotovelo--;
  }

  if (anguloGarra < alvoGarra) {
    if (distancia > 10) {
      anguloGarra++;
    }
  } else if (anguloGarra > alvoGarra) {
    anguloGarra--;
  }

  servoBase.write(anguloBase);
  servoOmbro.write(anguloOmbro);
  servoCotovelo.write(anguloCotovelo);
  servoGarra.write(anguloGarra);

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm | Garra: ");
  Serial.println(anguloGarra);

  delay(15);
}