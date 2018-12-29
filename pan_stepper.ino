#include <Stepper.h>

// Number of steps per revolution of INTERNAL motor in 4-step mode
#define STEPS_PER_MOTOR_REVOLUTION 32   

// Declare 'small_stepper' variable
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);
int steps_por_volta = 2048; // 32 steps por volta * 64 da caixa de reducao

double angulo = 90; // em graus
double tempo = 10; // em segundos
int sentido = 1; // 1 == CCW; -1 == CW
int mode = 0; // 0 = movimento continuo; 1 = parar ao fim do movimento; 2 = vai e volta
bool parado = true;

double total_de_steps = double(angulo / 360.0) * steps_por_volta; // total de steps
double delay_entre_steps = (tempo / total_de_steps) * 1000.0;
//double delay_entre_steps = 30000.0;
int contador_de_steps = 0;

void setup() {
  Serial.begin(9600);
  Serial.println(String(total_de_steps));
  Serial.println(delay_entre_steps);
  parado = false;
  small_stepper.setSpeed(500);
}

void loop() {
  if (parado == false) {
    small_stepper.step(sentido);
    delay(delay_entre_steps);
  }
  contador_de_steps++;

  if (contador_de_steps >= total_de_steps) {
    if (mode == 0) { // movimento continuo
      contador_de_steps = 0;
    }
    else if (mode == 1) { // parar ao fim do movimento
      parado = true;
    }
    else if (mode == 2) { // sweeping
      sentido = sentido * -1;
      contador_de_steps = 0;
    }
  }
}
