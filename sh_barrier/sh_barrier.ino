/*********************************************************************
 * @brief Smart Home Barrier
 * @author Dotsenko Oleksandr
 * @date 2026-01-08
 * https://github.com/dotsolvit/SmartHomeBarrier
**********************************************************************
Для компіляції проекту потрібно:
To compile the project you need:
1. встановити Arduiono IDE 
   install Arduino IDE
2. встановити менеджер плати ESP8266
   install board manager ESP8266
   File - Preferenses: 
   Additional boards manager URLs:
   http://arduino.esp8266.com/stable/package_esp8266com_index.json 
   Boards manager:
======================================================================
|          NAME           |             ATHORS               |VERSION|
======================================================================
|ESP8266                  |by ESP8266 Community              | 3.1.2 |
----------------------------------------------------------------------
   Tolls Board: "NodeMCU 1.0 (ESP-12E Module)"  
3. встановити додаткові бібліотеки:
   installing additional libraries:
======================================================================
|          NAME           |             ATHORS               |VERSION|
======================================================================
|TM1637 Driver            |by AKJ                            | 2.2.1 |
----------------------------------------------------------------------
**********************************************************************/


#include "sh_devices.hpp" 
// Директива включення файлів конфігурації розумного будинку (Directive for Including Smart Home Configuration Files)

void setup() // Стартова функція скетчу (Starting Function of the Sketch)
{
  WiFi_connecting();//Підключення до WiFi у режимі STA (Connecting to WiFi in STA mode)
  stepper_init(); //Функція ініціалізації портів крокового двигуна(Stepper motor ports initialization function)
  sensor_init(); //Функція ініціалізації датчика відстані HC-SR04 (HC-SR04 Distance Sensor initialization function)
  tmdisplay_init(); //Функція ініціалізації ТМ1637 (ТМ1637 initialization function)
  traffic_light_init(); //Функція ініціалізації світлофору (Traffic light initialization function)
  Web_starting(); //Запуск веб-сервера(Starting the web server)
}

void loop() // Основний цикл роботи алгоритму розумного будинку (Main Loop of the Smart Home Algorithm)
{
  handle_Client();//Обробка кліентських запитів(Processing client requests)
  sensor_distance();//Функція контролю дистанції (Distance control function)
  tmdisplay();//Функція Функція відображення інформації ТМ1637 (ТМ1637 information display function)
  traffic_light();//Функція світлофору (Traffic light function)
  stepper(); // Функція керування шлагбаумом (Barrier control function)
  sh_timer(); //Функція керування за часом (Time control function)
}
