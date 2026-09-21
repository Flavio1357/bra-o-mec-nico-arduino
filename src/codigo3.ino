#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// =========================
// SERVOS
// =========================
Servo servoBase;
Servo servoOmbro;
Servo servoCotovelo;
Servo servoGarra;

// =========================
// LCD I2C
// =========================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// =========================
// PINOS
// =========================
const int trigPin = 10;
const int echoPin = 11;
const int botao = 12;

// =========================
// POSIÇÕES INICIAIS
// =========================
int anguloBase = 90;
int anguloOmbro = 90;
int anguloCotovelo = 90;
int anguloGarra = 90;

// =========================
// CONTROLE DO SISTEMA
// =========================
bool estavaEmFalha = false;
bool sistemaLiberado = false;

// =========================
// MEDIR DISTÂNCIA
// =========================
float medirDistancia() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duracao = pulseIn(echoPin, HIGH);

  float distancia = duracao * 0.0343 / 2;

  return distancia;
}

// =========================
// TESTE SERVO BASE
// =========================
void testarServoBase() {

  Serial.println("BASE TESTANDO...");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTANDO BASE");

  servoBase.write(60);
  delay(500);

  servoBase.write(120);
  delay(500);

  servoBase.write(90);
  delay(500);

  Serial.println("BASE OK");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("BASE");
  lcd.setCursor(0, 1);
  lcd.print("OK");

  delay(700);
}

// =========================
// TESTE SERVO OMBRO
// =========================
void testarServoOmbro() {

  Serial.println("OMBRO TESTANDO...");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTANDO OMBRO");

  servoOmbro.write(60);
  delay(500);

  servoOmbro.write(120);
  delay(500);

  servoOmbro.write(90);
  delay(500);

  Serial.println("OMBRO OK");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OMBRO");
  lcd.setCursor(0, 1);
  lcd.print("OK");

  delay(700);
}

// =========================
// TESTE SERVO COTOVELO
// =========================
void testarServoCotovelo() {

  Serial.println("COTOVELO TESTANDO...");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTANDO");
  lcd.setCursor(0, 1);
  lcd.print("COTOVELO");

  servoCotovelo.write(60);
  delay(500);

  servoCotovelo.write(120);
  delay(500);

  servoCotovelo.write(90);
  delay(500);

  Serial.println("COTOVELO OK");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("COTOVELO");
  lcd.setCursor(0, 1);
  lcd.print("OK");

  delay(700);
}

// =========================
// TESTE SERVO GARRA
// =========================
void testarServoGarra() {

  Serial.println("GARRA TESTANDO...");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TESTANDO GARRA");

  servoGarra.write(60);
  delay(500);

  servoGarra.write(120);
  delay(500);

  servoGarra.write(90);
  delay(500);

  Serial.println("GARRA OK");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GARRA");
  lcd.setCursor(0, 1);
  lcd.print("OK");

  delay(700);
}

// =========================
// AUTOTESTE
// =========================
void autoteste() {

  Serial.println("==========================");
  Serial.println("   INICIANDO SISTEMA");
  Serial.println("==========================");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INICIANDO");
  lcd.setCursor(0, 1);
  lcd.print("SISTEMA...");

  delay(1000);

  // Teste dos servos
  testarServoBase();
  testarServoOmbro();
  testarServoCotovelo();
  testarServoGarra();

  // Sistema liberado
  sistemaLiberado = true;

  Serial.println("==========================");
  Serial.println("      SISTEMA OK");
  Serial.println("     BRACO LIBERADO");
  Serial.println("==========================");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SISTEMA OK");
  lcd.setCursor(0, 1);
  lcd.print("BRACO LIBERADO");

  delay(2000);
}

// =========================
// SETUP
// =========================
void setup() {

  Serial.begin(9600);

  // LCD
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("INICIANDO...");
  delay(1000);

  // Servos
  servoBase.attach(3);
  servoOmbro.attach(5);
  servoCotovelo.attach(6);
  servoGarra.attach(9);

  // Ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Botão failsafe
  pinMode(botao, INPUT_PULLUP);

  delay(500);

  // Executa autoteste
  autoteste();

  // Posição inicial
  servoBase.write(anguloBase);
  servoOmbro.write(anguloOmbro);
  servoCotovelo.write(anguloCotovelo);
  servoGarra.write(anguloGarra);
}

// =========================
// LOOP
// =========================
void loop() {

  // =========================
  // SISTEMA BLOQUEADO
  // =========================
  if (!sistemaLiberado) {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SISTEMA");
    lcd.setCursor(0, 1);
    lcd.print("BLOQUEADO");

    delay(100);

    return;
  }

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

    // LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FALHA DETECTADA");
    lcd.setCursor(0, 1);
    lcd.print("POSICAO SEGURA");

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

  // Conversão para ângulo
  int alvoBase = map(valorBase, 0, 1023, 0, 180);
  int alvoOmbro = map(valorOmbro, 0, 1023, 0, 180);
  int alvoCotovelo = map(valorCotovelo, 0, 1023, 0, 180);
  int alvoGarra = map(valorGarra, 0, 1023, 0, 180);

  // =========================
  // RETORNO DO FAILSAFE
  // =========================
  if (estavaEmFalha) {

    estavaEmFalha = false;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAILSAFE OFF");
    lcd.setCursor(0, 1);
    lcd.print("CONTROLE OK");

    Serial.println("FAILSAFE DESATIVADO - CONTROLE RETOMADO");

    delay(500);
  }

  // =========================
  // SENSOR ULTRASSONICO
  // =========================
  float distancia = medirDistancia();

  // =========================
  // MOVIMENTO SUAVE - BASE
  // =========================
  if (anguloBase < alvoBase) {

    anguloBase++;

  } else if (anguloBase > alvoBase) {

    anguloBase--;
  }

  // =========================
  // MOVIMENTO SUAVE - OMBRO
  // =========================
  if (anguloOmbro < alvoOmbro) {

    anguloOmbro++;

  } else if (anguloOmbro > alvoOmbro) {

    anguloOmbro--;
  }

  // =========================
  // MOVIMENTO SUAVE - COTOVELO
  // =========================
  if (anguloCotovelo < alvoCotovelo) {

    anguloCotovelo++;

  } else if (anguloCotovelo > alvoCotovelo) {

    anguloCotovelo--;
  }

  // =========================
  // GARRA + ANTI-ESMAGAMENTO
  // =========================
  if (anguloGarra < alvoGarra) {

    // Só fecha se não houver objeto próximo
    if (distancia > 10) {

      anguloGarra++;
    }

  } else if (anguloGarra > alvoGarra) {

    anguloGarra--;
  }

  // =========================
  // ENVIA POSIÇÕES AOS SERVOS
  // =========================
  servoBase.write(anguloBase);
  servoOmbro.write(anguloOmbro);
  servoCotovelo.write(anguloCotovelo);

  // =========================
  // MONITOR SERIAL
  // =========================
  Serial.print("Distancia: ");
  Serial.print(distancia);

  Serial.print(" cm | Base: ");
  Serial.print(anguloBase);

  Serial.print(" | Ombro: ");
  Serial.print(anguloOmbro);

  Serial.print(" | Cotovelo: ");
  Serial.print(anguloCotovelo);

  Serial.print(" | Garra: ");
  Serial.println(anguloGarra);

  // Pequeno intervalo para movimento suave
  delay(15);
}