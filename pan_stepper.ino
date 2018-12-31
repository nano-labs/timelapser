#include <Stepper.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Number of steps per revolution of INTERNAL motor in 4-step mode
#define STEPS_PER_MOTOR_REVOLUTION 32   

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

static const unsigned char PROGMEM ca_logo[128 * 32 / 8] =
{
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000001, B11111000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00011111, B11111111, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00111111, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B01111111, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B01111111, B11111111, B11100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B01100000, B00000000, B00000000,
B00000001, B11110111, B11111110, B01111000, B00001100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B01100000, B00000000, B00000000,
B00000001, B11100000, B00000000, B01111000, B00001100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B01100000, B00000000, B00000000,
B00000011, B11100000, B00000000, B01111100, B00011000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B01100000, B00000000, B00000000,
B00000011, B11100000, B00000000, B01111100, B00011000, B11011110, B00001111, B00000110, B11110000, B00111100, B00000000, B00110000, B11110000, B01101111, B00001111, B10000000,
B00000011, B11100000, B00000000, B01111100, B00011000, B11111110, B00011111, B10000111, B11110000, B11111111, B00000000, B00110001, B11111000, B01111111, B10011111, B11000000,
B00000011, B11000000, B00000000, B00111100, B00111000, B11100111, B00010001, B11000111, B00111000, B11000011, B00000000, B00110001, B00011100, B01110001, B11011000, B01000000,
B00000011, B11000000, B00000000, B00111100, B00110000, B11000011, B00000000, B11000110, B00011001, B10000001, B10000000, B00110000, B00001100, B01100000, B11011100, B00000000,
B00000011, B11000000, B00000000, B00111100, B00110000, B11000011, B00001111, B11000110, B00011001, B10000001, B10111110, B00110000, B11111100, B01100000, B11001111, B10000000,
B00000011, B11000000, B00000000, B00111100, B00110000, B11000011, B00111111, B11000110, B00011001, B10000001, B10111110, B00110011, B11111100, B01100000, B11000011, B11000000,
B00000011, B11100000, B00000000, B00111100, B01110000, B11000011, B00110000, B11000110, B00011001, B10000001, B10000000, B00110011, B00001100, B01100000, B11000000, B11000000,
B00000011, B11100000, B00000000, B01111100, B01100000, B11000011, B00110001, B11000110, B00011000, B11000011, B00000000, B00110011, B00011100, B01110001, B11010000, B11000000,
B00000011, B11111000, B00000001, B11111100, B01100000, B11000011, B00111111, B11000110, B00011000, B11111111, B00000000, B00110011, B11111100, B01111111, B10011111, B11000000,
B00000011, B10111100, B00000011, B11111100, B01100000, B11000011, B00011110, B11000110, B00011000, B00111100, B00000000, B00110001, B11101100, B01101111, B00001111, B00000000,
B00000001, B10011111, B00001111, B11111000, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B11100110, B00000111, B11110000, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B01100000, B00000111, B11100000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B01111110, B00000111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00111110, B00000111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000110, B00000110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};



// Declare 'small_stepper' variable
Stepper small_stepper(STEPS_PER_MOTOR_REVOLUTION, 8, 10, 9, 11);
int steps_por_volta = 2048; // 32 steps por volta * 64 da caixa de reducao

const int b_esquerda_pin = 12;
const int b_direita_pin = 7;
const int b_modo_pin = 4;
const int b_start_pin = 2;

double angulo = 180; // em graus
double tempo = 300; // em segundos
int sentido = -1; // 1 == CCW; -1 == CW
int mode = 0; // 0 = movimento continuo; 1 = parar ao fim do movimento; 2 = vai e volta
bool parado = true;

double total_de_steps = 0;//angulo / 360.0 * steps_por_volta; // total de steps
double delay_entre_steps = 0;//(tempo / total_de_steps) * 1000.0;
//double delay_entre_steps = 10.0;
int contador_de_steps = 0;

unsigned long proximo_passo = 0;
int b_esquerda_trigged = 0;
int b_esquerda_pushed = 0;
int b_direita_trigged = 0;
int b_direita_pushed = 0;
int b_modo_trigged = 0;
int b_modo_pushed = 0;
int b_start_pushed = 0;
int b_start_trigged = 0;
int tela_modo = 0; // 0 = speed; 1 = angle; 2 = mode

void setup() {
  Serial.begin(9600);
  Serial.println(String(total_de_steps));
  Serial.println(delay_entre_steps);
  small_stepper.setSpeed(1000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.drawBitmap(0, 0, ca_logo, 128, 32, WHITE);
  display.display();
  delay(1000);
  calcular_passos();
  draw_time();

  pinMode(b_esquerda_pin, INPUT);
  pinMode(b_direita_pin, INPUT);
  pinMode(b_modo_pin, INPUT);
  pinMode(b_start_pin, INPUT);
//  pinMode(b_start_pin, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(b_start_pin), start_stop, CHANGE);
}

void calcular_passos() {
  total_de_steps = angulo / 360.0 * steps_por_volta; // total de steps
  delay_entre_steps = (tempo / total_de_steps) * 1000.0;
}
void draw_time() {
  display.clearDisplay();
  display.setTextSize(1);
//  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print("Time");
  display.setTextColor(WHITE);
  display.println("   Angle   Mode");
  display.setTextSize(2);
  display.setCursor(0,10);
  if (tempo < 300) {
    display.println(String(int(tempo)) + " seconds");
  } else if (tempo >= 6000){
    display.println(String(int(tempo / 60)) + " mins");
  } else {
    display.println(String(int(tempo / 60)) + " minutes");
  }
  display.setTextSize(1);
  display.setCursor(0,25);
  display.println(String(delay_entre_steps) + "ms " + String(1000.0/delay_entre_steps) + "fps");
  display.display();
}
void draw_angle() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Time   ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print("Angle");
  display.setTextColor(WHITE);
  display.println("   Mode");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.println(int(angulo));
  display.setTextSize(1);
  display.setCursor(0,25);
  display.println(String(delay_entre_steps) + "ms " + String(1000.0/delay_entre_steps) + "fps");
  display.display();
}
void draw_mode() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Time   Angle   ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println("Mode");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,10);
  String sentido_text;
  if (sentido == 1) {
    sentido_text = "CCW";
  } else {
    sentido_text = "CW";
  }
  if (mode == 0) {
    display.println("Loop " + sentido_text);  
  }
  else if (mode == 1) {
    display.println("Stop " + sentido_text);  
  }
  else if (mode == 2) {
    display.println("Sweep " + sentido_text);  
  }
  display.setTextSize(1);
  display.setCursor(0,25);
  display.println(String(delay_entre_steps) + "ms " + String(1000.0/delay_entre_steps) + "fps");
  display.display();
}
void draw_running() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setCursor(0,0);
  String mode_text;
  String time_text;
  String sentido_text;
  if (tempo < 300) {
    time_text = String(int(tempo)) + "s";
  } else {
    time_text = String(int(tempo / 60)) + "m";
  }
  if (mode == 0) {
    mode_text = "Loop";  
  }
  else if (mode == 1) {
    mode_text = "Stop";  
  }
  else if (mode == 2) {
    mode_text = "Sweep";  
  }
  if (sentido == 1) {
    sentido_text = "CCW";
  } else {
    sentido_text = "CW";
  }
  display.print(time_text);
  display.setTextColor(WHITE);
  display.print(" ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(String(int(angulo)) + "<");
  display.setTextColor(WHITE);
  display.print(" ");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(mode_text + " " + sentido_text);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0,10);
  display.println("MOVING " + String(int(contador_de_steps / total_de_steps * 100)) + "%");
  display.setTextSize(1);
  display.setCursor(0,25);
  display.println(String(delay_entre_steps) + "ms " + String(1000.0/delay_entre_steps) + "fps");
  display.display();
}
void loop() {
  if (parado == false) {
    if (proximo_passo <= millis()) {
      small_stepper.step(sentido);
      contador_de_steps++;
      proximo_passo = millis() + delay_entre_steps - 20; // 20 miliseconds to run one loop iteraction
      draw_running();
    }
  }

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

  if (digitalRead(b_esquerda_pin) == HIGH) {
    b_esquerda_trigged = 1;
  } else {
    if (b_esquerda_trigged == 1) {
      b_esquerda_pushed = 1;
    }
  }
  if (digitalRead(b_direita_pin) == HIGH) {
    b_direita_trigged = 1;
  } else {
    if (b_direita_trigged == 1) {
      b_direita_pushed = 1;
    }
  }
  if (digitalRead(b_modo_pin) == HIGH) {
    b_modo_trigged = 1;
  } else {
    if (b_modo_trigged == 1) {
      b_modo_pushed = 1;
    }
  }
  if (digitalRead(b_start_pin) == HIGH) {
    b_start_trigged = 1;
  } else {
    if (b_start_trigged == 1) {
      b_start_pushed = 1;
    }
  }

  if (b_esquerda_pushed == 1 || b_direita_pushed == 1 || b_modo_pushed == 1 || b_start_pushed == 1) {
    delay(200);
  }
  
  if (b_esquerda_pushed == 1) {
    Serial.println("esquerda");
    if (tela_modo == 0) { // tempo
      if (tempo <= 300) {
        tempo = tempo - 30;
      } else {
        tempo = tempo - 300;
      }
      if (tempo < 30) {
        tempo = 30;
      }
      calcular_passos();
      draw_time();
    }
    else if (tela_modo == 1) { // angulo
      angulo = angulo - 45.0;
      if (angulo < 45) {
        angulo = 45.0;
      }
      calcular_passos();
      draw_angle();
    }
    else if (tela_modo == 2) { // modo
      if (mode == 0) {
        sentido = sentido * -1;
        mode = 2;
      } else {
        mode = mode - 1;
      }
      draw_mode();
    }
    b_esquerda_pushed = 0;
    b_esquerda_trigged = 0;
  };
    if (b_direita_pushed == 1) {
    Serial.println("direita");
    if (tela_modo == 0) { // tempo
      if (tempo < 300) {
        tempo = tempo + 30;
      } else {
        tempo = tempo + 300;
      }
      calcular_passos();
      draw_time();
    }
    else if (tela_modo == 1) { // angulo
      angulo = angulo + 45;
      if (angulo > 360) {
        angulo = 360.0;
      }
      calcular_passos();
      draw_angle();
    }
    else if (tela_modo == 2) { // modo
      if (mode == 2) {
        sentido = sentido * -1;
        mode = 0;
      } else {
        mode = mode + 1;
      }
      draw_mode();
    }
    b_direita_pushed = 0;
    b_direita_trigged = 0;
  };
  if (b_modo_pushed == 1) {
    Serial.println("modo");
    if (tela_modo == 0) {
      draw_angle();
      tela_modo = 1;
    }
    else if (tela_modo == 1) {
      draw_mode();
      tela_modo = 2;
    }
    else {
      draw_time();
      tela_modo = 0;
    }
    b_modo_pushed = 0;
    b_modo_trigged = 0;
  };
  if (b_start_pushed == 1) {
    if (parado == true) {
      parado = false;
      draw_running();
      calcular_passos();
      Serial.println(angulo);
      Serial.println(tempo);
      Serial.println(delay_entre_steps);
    } else {
      parado = true;
      contador_de_steps = 0;
      proximo_passo = 0;
      draw_time();
      tela_modo = 0;
    }
    Serial.println("start");
    b_start_pushed = 0;
    b_start_trigged = 0;
  };
}
