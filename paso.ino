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

const int gradosMover[] = {15, 100, 25}; // Grados de movimiento para cada paso
const int tiempos[] = {100, 400, 300}; // Tiempos en milisegundos

int pasosMover[3]; // Array para almacenar los pasos calculados
int movimientoActual = 0; // Índice del movimiento actual

long totalSteps = 0; // Contador de pasos totales

void setup() {
  pinMode(stepPin, OUTPUT);   // Configurar pin de pasos como salida
  pinMode(dirPin, OUTPUT);    // Configurar pin de dirección como salida
  pinMode(enablePin, OUTPUT); // Configurar pin de habilitación como salida
  pinMode(buttonPin, INPUT_PULLUP); // Configurar pin del botón como entrada con resistencia pull-up

  digitalWrite(enablePin, LOW); // Habilitar el driver (LOW para habilitar, puede ser HIGH en otros drivers)
  Serial.begin(9600);           // Iniciar comunicación serial para depuración

  stepper.setMaxSpeed(800);     // Establecer la velocidad máxima
  stepper.setAcceleration(400);  // Establecer la aceleración

  // Calcula los pasos por cada grado de movimiento
  for (int i = 0; i < 3; i++) {
    pasosMover[i] = gradosMover[i] * pasosPorGrado;
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  // Verificar si el botón fue presionado
  if (buttonState == LOW && lastButtonState == HIGH && !movementInProgress) {
    movimientoActual = 0; // Reiniciar el movimiento
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
        movementInProgress = false; // Finalizar el ciclo de movimientos
        totalSteps += abs(stepper.currentPosition());
        stepper.setCurrentPosition(0); // Reset current position
        Serial.println(totalSteps);    // Enviar el número total de pasos a Python
      }
    } else {
      stepper.run();
    }
  }
}
