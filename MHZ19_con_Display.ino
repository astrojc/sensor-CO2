#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Anchura en pixeles del display OLED
#define SCREEN_HEIGHT 64 // Altura en pixeles del display 

// Para el display SSD1306 display conectado al bus I2C (SDA, SCL)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MH-Z19B Sensor
#define pwmPin 10               // Pin donde está conectado la salida PWM del sensor
int t_precalentamiento = 120;   // Tiempo de precalentamiento en segundos
int ValorPrev = LOW;            // Estado del precalentamiento
long T_high, T_low, tth, ttl;   // Para el cálculo de la concentración de CO2
long ppm = 0;                   // Concentración de CO2 

// Función de interrupción. Se ejecuta cada vez que se detecta un cambio en el pin PWM
// Se pone ICACHE_RAM_ATTR para correr la interrupción en RAM (necesario en NodeMCU)
ICACHE_RAM_ATTR void PWM_ISR() {
  long tt = millis();                // Número de milisegundos transcurridos desde el inicio del programa
  int  valor = digitalRead(pwmPin);  // Leemos valor PWM
  
    if (valor == HIGH) {
        if (valor != ValorPrev) {
            tth = tt;
            T_low = tth - ttl;
            ValorPrev = valor;
        }
    }  
    else 
    { 
        if (valor != ValorPrev) {
            ttl = tt;
            T_high = ttl - tth;
            ValorPrev = valor;
            ppm = 5000 * (T_high - 2) / (T_high + T_low - 4);
        }
    }

    // Basado en la fórmula
    // C ppm = 5000 * (T_high - 2 ms) / (T_high + T_low - 4ms)
    // Que se encuentra en las especificaciones del sensor, apartado 7 
    // https://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf
}

void setup()   {
  // Para debug
  Serial.begin(115200);
  Serial.println("Inicializando");

  // Pin PWM como entrada 
  pinMode(pwmPin, INPUT);

  // Fijamos la interrupción para los cambios detectados en el pin PWM
  attachInterrupt(digitalPinToInterrupt(pwmPin), PWM_ISR, CHANGE);

  // Inicializamos display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {    // Cuidado con la dirección: 0x3C para 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Visualiza mensaje de inicio en el display
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Medirdor CO2 V1");
  display.display();
  delay(1000);
}

void display_PreCal(int segundos) {
  display.setTextSize(1);
  display.println("    PRECALENTANDO");
  display.println();
  display.setTextSize(5);
  display.print(" ");
  display.print(segundos);
  display.display();
}

void display_PPM(long ppm) {
  display.setTextSize(2);
  display.println("CO2    PPM");
  display.setTextSize(1);
  display.println();
  display.setTextSize(5);
  if (ppm < 1000) {
    display.print(" ");
  }
  display.print(ppm);
  display.display();
  Serial.println(ppm);
}

void loop() {
  display.clearDisplay();
  display.setCursor(0, 0);

  if (t_precalentamiento > 0) {
    // Si no se ha precalentado -> esperamos cuenta atrás
    display_PreCal(t_precalentamiento);
    t_precalentamiento--;
  }
  else {
    // Si se ha precalentado -> medimos
    display_PPM(ppm);
  }

  // Retardo entre medidas
  delay(1000);
}
