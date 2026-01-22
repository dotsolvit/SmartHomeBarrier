#include <ESP8266WiFi.h> //Бібліотека для роботи з WiFi на ESP8266 (WiFi library)
#include <ESP8266WebServer.h> //Бібліотека для створення веб-сервера на ESP8266 (Library for creating a web server)
#include <TM1637.h> //Бібліотека для драйвера TM1637 (Driver library)

//Дистанція для відкривання бар'єру(Distance for opening the barrier)
#define OPENING_DISTANCE 5
//Час очікування(Waiting time)
#define WAITING_TIME 6 //Час затримки до закриття бар'єру(Delay time before closing the barrier) 

//Піни для крокового двигуна (Pins for the Stepper Motor)
#define IN1 5    // Макрос піна 1 для крокового двигуна (Macro for Pin 1 of the Stepper Motor)
#define IN2 4    // Макрос піна 2 для крокового двигуна (Macro for Pin 2 of the Stepper Motor)
#define IN3 14    // Макрос піна 3 для крокового двигуна (Macro for Pin 3 of the Stepper Motor)
#define IN4 16    // Макрос піна 4 для крокового двигуна (Macro for Pin 4 of the Stepper Motor)

//Піни для датчика відстані HC-SR04 (Pins for the HC-SR04 Distance Sensor)
#define TRIGPIN 0  //Пін TRIG для датчика відстані HC-SR04 (TRIG Pin for HC-SR04 Distance Sensor)
#define ECHOPIN 2  //Пін ECHO для датчика відстані HC-SR04 (ECHO Pin for HC-SR04 Distance Sensor)

//Піни для драйвера TM1637 (Pins for the TM1637 Driver)
#define CLK 1  // Пін CLK для драйвера TM1637 (CLK Pin for TM1637 Driver)
#define DIO 3  // Пін DIO для драйвера TM1637 (DIO Pin for TM1637 Driver)

// Піни RGB світлофора (Pins of an RGB Traffic Light)
#define RGB_RED 15 // Пін червоного кольору RGB світлофора (Pin for the Red Color of the RGB Traffic Light)
#define RGB_GRN 12 // Пін зеленого кольору RGB світлофора  (Pin for the Green Color of the RGB Traffic Light)
#define BLDLED_BL 13 //Пін вбудованого LED Blue (Built-in Blue LED pin)

//WiFi
//Підключення до WiFi у режимі STA (Connecting to WiFi in STA mode)
void WiFi_connecting(void);
//Stepper Motor functions
//Функція ініціалізації портів крокового двигуна(Stepper motor ports initialization function)
void stepper_init(void);
// Функція оберту крокового двигуна на 90 градусів за годинниковою стрілкою
// Function for Rotating the Stepper Motor 90 Degrees Clockwise
void stepper_forward(void);
// Функція оберту крокового двигуна на 90 градусів проти годинникової стрілки
// Function for a 90-Degree Counterclockwise Rotation of the Stepper Motor
void stepper_backward(void);
// Функція для блокування шлагбаума (Function for Barrier Gate Locking)
void stepper_lock(void);
// Функція керування шлагбаумом (Barrier control function)
void stepper(void);

//Distance Sensor functions
//Функція ініціалізації датчика відстані HC-SR04 (HC-SR04 Distance Sensor initialization function)
void sensor_init(void);
//Функція контролю дистанції (Distance control function)
void sensor_distance(void);

//TM display
//Функція ініціалізації ТМ1637 (ТМ1637 initialization function)
void tmdisplay_init(void);
//Функція Функція відображення інформації ТМ1637 (ТМ1637 information display function)
void tmdisplay(void);

//Traffic light
//Функція ініціалізації світлофору (Traffic light initialization function)
void traffic_light_init(void);
//Функція світлофору (Traffic light function)
void traffic_light(void);
//Функція мигання світлофору (Traffic light blinking function)
void traffic_light_blinking(void);

//Timer
void sh_timer_start(void); //Функція запуску таймера (Timer start function)
void sh_timer(void); //Функція керування за часом (Time control function)

//Web
//Запуск веб-сервера(Starting the web server)
void Web_starting(void);
//Обробка кліентських запитів(Processing client requests)
void handle_Client(void);
//Обробник головної сторінки
void handle_OnConnect(void);
//Дії при натисканні Open (Actions when pressing Open)
void handle_open(void);
//Дії при натисканні Close (Actions when pressing Close)
void handle_close(void);
//Дії при натисканні Open (Actions when pressing Open)
void handle_block(void);
//Обробник 404 (коли запитаний маршрут не знайдено)
void handle_NotFound(void);

