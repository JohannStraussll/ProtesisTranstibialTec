#define PIN_PWM 9
#define BOTON1_PIN 2  // Pin para el botón que establece 1650
#define BOTON2_PIN 3  // Pin para el botón que establece 1350
#define VALOR_NEUTRO 1500  // Valor neutral cuando no se presiona ningún botón

void setup() {
  Serial.begin(9600);
  pinMode(PIN_PWM, OUTPUT);
  pinMode(BOTON1_PIN, INPUT_PULLUP);  // Configura el pin del botón con resistencia pull-up
  pinMode(BOTON2_PIN, INPUT_PULLUP);  // Configura el pin del botón con resistencia pull-up
}

void loop() {
  int tiempoAlto;

  if (digitalRead(BOTON1_PIN) == LOW) {  // Si se presiona el botón 1
    tiempoAlto = 1650;
  } else if (digitalRead(BOTON2_PIN) == LOW) {  // Si se presiona el botón 2
    tiempoAlto = 1350;
  } else {
    tiempoAlto = VALOR_NEUTRO;  // Valor neutral
  }

  digitalWrite(PIN_PWM, HIGH);
  delayMicroseconds(tiempoAlto);
  digitalWrite(PIN_PWM, LOW);
  delayMicroseconds(20000 - tiempoAlto);

  Serial.println(tiempoAlto);
}
