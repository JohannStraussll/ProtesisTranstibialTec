// Definición de Pines
const int PWM_PIN = 9;        // Pin usado para PWM y lectura
const int BUTTON_PIN = 2;     // Pin para el botón

// Variables para el control del ciclo
unsigned long startMillis;    // Tiempo de inicio del ciclo
bool cicloActivo = false;     // Indicador de si el ciclo está activo

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Configura el botón con resistencia interna de pull-up
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), iniciarCiclo, FALLING);  // Interrupción en el flanco de bajada
  Serial.begin(9600);
}

void loop() {
  if (cicloActivo) {
    unsigned long now = millis();
    double tiempoTranscurrido = (now - startMillis) / 1000.0; // Tiempo en segundos

    if (tiempoTranscurrido < 3) {
      // Configura el pin 9 como salida para enviar PWM
      pinMode(PWM_PIN, OUTPUT);
      int PWMValue = 1500 + 150 * sin(2 * PI * tiempoTranscurrido / 3 - PI / 2);
      digitalWrite(PWM_PIN, HIGH);
      delayMicroseconds(PWMValue);
      digitalWrite(PWM_PIN, LOW);
      delayMicroseconds(20000 - PWMValue);
      Serial.print("PWM enviado: "); Serial.println(PWMValue);

      // Cambia el pin a entrada para leer el estado
      pinMode(PWM_PIN, INPUT);
      int readValue = digitalRead(PWM_PIN);
      Serial.print("Estado del pin después de PWM: "); Serial.println(readValue);
    } else {
      // Finalizar el ciclo y mantener el motor en 1500
      pinMode(PWM_PIN, OUTPUT);
      digitalWrite(PWM_PIN, HIGH);
      delayMicroseconds(1500);
      digitalWrite(PWM_PIN, LOW);
      delayMicroseconds(20000 - 1500);
      Serial.println("Ciclo completado, motor en posición neutra.");
      cicloActivo = false;  // Resetea el estado del ciclo
    }
  } else {
    // Configura el pin 9 como entrada para leer el estado cuando no está activo
    pinMode(PWM_PIN, INPUT);
    int readValue = digitalRead(PWM_PIN);
    Serial.print("Leyendo del pin 9 en reposo: "); Serial.println(readValue);
  }
}

// Función para manejar la interrupción del botón
void iniciarCiclo() {
  if (!cicloActivo) {  // Verificar si un ciclo ya está en progreso
    cicloActivo = true;
    startMillis = millis();  // Reinicia el contador de tiempo para el ciclo
  }
}
