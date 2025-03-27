int codigo; // Clave ingresada
bool alarma_activa = false;
float temperatura = 0.0;
int buzzer = 13;
int ledV = 11;
int ledR = 12; 

void setup() {
  pinMode(ledV, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  Serial.println("Sistema de alarma de incendios iniciado.");
}

void loop() {
  temperatura = TempMedida();
  Serial.print("Temperatura actual: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  // Se activa alarma si la temperatura supera los 40°C
  if (temperatura >= 40 && !alarma_activa) {
    activarAlarma();
  }

  // Controla los LEDs según la temperatura
  controlarLeds(temperatura);
  delay(1000); // Evitar lecturas muy seguidas
}

void activarAlarma() {
  alarma_activa = true;
  Serial.println("¡ADVERTENCIA! Posible incendio detectado.");

  // Sonar varias veces
  while (alarma_activa) {
    Activacion_Sonido();
    Desactivacion_Sonido();
  }
}

void Activacion_Sonido() {
  digitalWrite(ledR, HIGH); //Encendido
  tone(buzzer, 1000); // Sonido a 1000 Hz
  delay(400);

  digitalWrite(ledR, LOW); //Apagado
  noTone(buzzer);
  delay(400);
}

void Desactivacion_Sonido() {
  if (Serial.available()) {
    Serial.println("Ingrese la clave para desactivar la alarma:");
    while (!Serial.available()) {}
    codigo = Serial.parseInt();
    if (codigo == 1234) {
      desactivarAlarma();
    }
  }
}

void desactivarAlarma() {
  alarma_activa = false;
  Serial.println("ALARMA DESACTIVADA.");
  digitalWrite(ledV, HIGH); //Se enciende
  digitalWrite(ledR, LOW); //Se apaga
  noTone(buzzer); // Desactivado

  Serial.println("Esperando a que la temperatura baje...");
  while (TempMedida() >= 40) {
    Serial.print("Temperatura actual: ");
    Serial.println(TempMedida());
    delay(1000);
  }
  Serial.println("Temperatura normal. Sistema en espera.");
}

float TempMedida() {
  float suma = 0;
  for (int i = 0; i < 10; i++) {
    suma += (analogRead(A0) * (5.0 / 1023.0)) * 100.0; //Temp a grados C
    delay(10);
  }
  return suma / 10; // Promedio de las lecturas
}

void controlarLeds(float temp) {
  if (temp < 40) {
    digitalWrite(ledV, HIGH); // Encendido
    digitalWrite(ledR, LOW); // Apagado
  } else {
    digitalWrite(ledV, LOW); // Apagado
    digitalWrite(ledR, HIGH); // Encendido
  }
}

