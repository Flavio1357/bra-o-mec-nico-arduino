#include <Servo.h>

Servo servoBase;
Servo servoOmbro;
Servo servoCotovelo;
Servo servoGarra;

const int trigPin = 10;
const int echoPin = 11;
const int botao = 12;

int anguloBase = 90;
int anguloOmbro = 90;
int anguloCotovelo = 90;
int anguloGarra = 90;

// Controle do failsafe
bool estavaEmFalha = false;

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);

  return duracao * 0.0343 / 2;
}

void setup() {

  servoBase.attach(3);
  servoOmbro.attach(5);
  servoCotovelo.attach(6);
  servoGarra.attach(9);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(botao, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {

  // =========================
  // FAILSAFE
  // =========================

  if (digitalRead(botao) == LOW) {

    estavaEmFalha = true;

    // Posição segura
    anguloBase = 90;
    anguloOmbro = 90;
    anguloCotovelo = 90;
    anguloGarra = 30;

    servoBase.write(anguloBase);
    servoOmbro.write(anguloOmbro);
    servoCotovelo.write(anguloCotovelo);
    servoGarra.write(anguloGarra);

    Serial.println("FALHA DETECTADA - POSICAO SEGURA");

    delay(20);
    return;
  }

  // =========================
  // LEITURA DOS POTENCIOMETROS
  // =========================

  int valorBase = analogRead(A0);
  int valorOmbro = analogRead(A1);
  int valorCotovelo = analogRead(A2);
  int valorGarra = analogRead(A3);

  int alvoBase = map(valorBase, 0, 1023, 0, 180);
  int alvoOmbro = map(valorOmbro, 0, 1023, 0, 180);
  int alvoCotovelo = map(valorCotovelo, 0, 1023, 0, 180);
  int alvoGarra = map(valorGarra, 0, 1023, 0, 180);

  // =========================
  // SAIDA DO FAILSAFE
  // =========================

  if (estavaEmFalha) {

    estavaEmFalha = false;

    Serial.println("FAILSAFE DESATIVADO - CONTROLE RETOMADO");
  }

  // =========================
  // ANTI-CRUSH
  // =========================

  float distancia = medirDistancia();

  // =========================
  // MOVIMENTO SUAVE - BASE
  // =========================

  if (anguloBase < alvoBase) {
    anguloBase++;
  }
  else if (anguloBase > alvoBase) {
    anguloBase--;
  }

  // =========================
  // MOVIMENTO SUAVE - OMBRO
  // =========================

  if (anguloOmbro < alvoOmbro) {
    anguloOmbro++;
  }
  else if (anguloOmbro > alvoOmbro) {
    anguloOmbro--;
  }

  // =========================
  // MOVIMENTO SUAVE - COTOVELO
  // =========================

  if (anguloCotovelo < alvoCotovelo) {
    anguloCotovelo++;
  }
  else if (anguloCotovelo > alvoCotovelo) {
    anguloCotovelo--;
  }

  // =========================
  // GARRA + ANTI-ESMAGAMENTO
  // =========================

  if (anguloGarra < alvoGarra) {

    // Só permite fechar se não houver objeto próximo
    if (distancia > 10) {
      anguloGarra++;
    }

  }
  else if (anguloGarra > alvoGarra) {

    anguloGarra--;
  }

  // =========================
  // ENVIA POSIÇÕES AOS SERVOS
  // =========================

  servoBase.write(anguloBase);
  servoOmbro.write(anguloOmbro);
  servoCotovelo.write(anguloCotovelo);
  servoGarra.write(anguloGarra);

  // =========================
  // MONITOR SERIAL
  // =========================

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm | ");

  Serial.print("Base: ");
  Serial.print(anguloBase);

  Serial.print(" | Ombro: ");
  Serial.print(anguloOmbro);

  Serial.print(" | Cotovelo: ");
  Serial.print(anguloCotovelo);

  Serial.print(" | Garra: ");
  Serial.println(anguloGarra);

  delay(15);
}