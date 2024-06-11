#include <AccelStepper.h>

#define stepPin 2   // Pin para controlar los pasos (CLK+)
#define dirPin 3    // Pin para controlar la dirección (CW+)
#define enablePin 4 // Pin para habilitar el motor (EN+)
#define buttonPin 7 // Pin donde está conectado el botón

AccelStepper stepper(1, stepPin, dirPin); // 1 = DRIVER

bool buttonState = false;
bool lastButtonState = false;
bool movementInProgress = false; // Variable para controlar si el movimiento está en progreso

const int pasosPorRevolucion = 200; // Número de pasos por revolución del motor
const int pasosPorGrado = round(pasosPorRevolucion / 360.0); // Número de pasos por grado

int gradosMover[3]; // Grados de movimiento para cada paso
const int tiempos[] = {300, 400, 100}; // Tiempos en milisegundos

int pasosMover[3]; // Array para almacenar los pasos calculados
int movimientoActual = 0; // Índice del movimiento actual

int ciclos = 1; // Número de ciclos a realizar (puede ser cambiado)
int cicloActual = 0; // Contador del ciclo actual

void setup() {
  pinMode(stepPin, OUTPUT);   // Configurar pin de pasos como salida
  pinMode(dirPin, OUTPUT);    // Configurar pin de dirección como salida
  pinMode(enablePin, OUTPUT); // Configurar pin de habilitación como salida
  pinMode(buttonPin, INPUT_PULLUP); // Configurar pin del botón como entrada con resistencia pull-up

  digitalWrite(enablePin, LOW); // Habilitar el driver (LOW para habilitar, puede ser HIGH en otros drivers)
  Serial.begin(9600);           // Iniciar comunicación serial para depuración

  stepper.setMaxSpeed(900);     // Establecer la velocidad máxima
  stepper.setAcceleration(150);  // Establecer la aceleración

  randomSeed(analogRead(0));    // Inicializar la semilla de números aleatorios
  actualizarGradosMover();
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  // Verificar si el botón fue presionado
  if (buttonState == LOW && lastButtonState == HIGH && !movementInProgress) {
    movimientoActual = 0; // Reiniciar el movimiento
    cicloActual = 0; // Reiniciar el ciclo
    movementInProgress = true;
    stepper.moveTo(pasosMover[movimientoActual]); // Primer movimiento a la derecha
  }
  
  lastButtonState = buttonState;

  if (movementInProgress) {
    if (stepper.distanceToGo() == 0) {
      // Movimiento completado, actualizar el siguiente movimiento
      movimientoActual++;
      if (movimientoActual == 1) {
        stepper.moveTo(-pasosMover[movimientoActual]); // Movimiento a la izquierda
      } else if (movimientoActual == 2) {
        stepper.moveTo(pasosMover[movimientoActual]); // Movimiento a la derecha
      } else {
        movimientoActual = 0; // Reiniciar el índice de movimiento
        cicloActual++; // Incrementar el ciclo actual
        if (cicloActual >= ciclos) {
          movementInProgress = false; // Finalizar el ciclo de movimientos
          Serial.print("Ciclo completado: "); // Enviar el número de ciclos completados y grados a Python
          Serial.print(cicloActual);
          Serial.print(" Grados: ");
          Serial.print(gradosMover[0]);
          Serial.print(", ");
          Serial.print(gradosMover[1]);
          Serial.print(", ");
          Serial.println(gradosMover[2]);
        } else {
          delay(500); // Esperar 200 ms antes de iniciar el siguiente ciclo
          actualizarGradosMover(); // Actualizar los grados de movimiento para el siguiente ciclo
          Serial.print("Ciclo: ");
          Serial.print(cicloActual + 1);
          Serial.print(" Grados: ");
          Serial.print(gradosMover[0]);
          Serial.print(", ");
          Serial.print(gradosMover[1]);
          Serial.print(", ");
          Serial.println(gradosMover[2]);
          stepper.moveTo(pasosMover[movimientoActual]); // Reiniciar el ciclo
        }
      }
    } else {
      stepper.run();
    }
  }
}

void actualizarGradosMover() {
  // Generar valores aleatorios para gradosMover[0] entre 60 y 70, y gradosMover[2] entre 30 y 40
  gradosMover[0] = random(20, 30); // Valor entre 60 y 70
  gradosMover[2] = random(5, 10); // Valor entre 30 y 40
  gradosMover[1] = gradosMover[0]/2; // Suma de los dos valores aleatorios

  // Calcula los pasos por cada grado de movimiento
  for (int i = 0; i < 3; i++) {
    pasosMover[i] = gradosMover[i] * pasosPorGrado;
  }
}
