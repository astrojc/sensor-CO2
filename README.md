# sensor-CO2
Medidor de concentración de CO2 basado en el MH-Z19B

Componentes:
- NodeMCU V3
- Sensor de CO2 MH-Z19B
- Display OLED 128x64 pixeles

El código fuente lo puedes descargar en: https://github.com/astrojc/sensor-CO2/blob/master/MHZ19_con_Display.ino

Se puede alimentar con una Powerbank directamente a través del puerto microUSB del NodeMCU.

El sensor requiere un "precalentamiento" para su correcto fucionamiento de 120 segundos. Hasta pasado este tiempo no ofrece medidas.

Para calibrar el sensor, una vez precalentado, debemos situarlo al aire libre (donde la concentración de CO2 ronda los 410ppm) y hacer un puente entre el pin "ZERO point calibration" del sensor MH-Z19B y GND durante > 7 segundos. Ver especificaciones del sensor en: https://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf

<img src="/MH-Z19B Sketch_bb.png" align="center">

