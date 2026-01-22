// Директиви включення файлів конфігурації розумного будинку (Directives for Including Smart Home Configuration Files)
#include "sh_devices.hpp"
#include "sh_sendhtml.hpp"

#include "sh_ssid.h" //Окремий файл який містить: (A separate file containing:)
//#define SH_WIFI_SSID "******"
//#define SH_WIFI_PASS "******"

//Налаштування WiFi
const char* ssid = SH_WIFI_SSID;
const char* password = SH_WIFI_PASS;

ESP8266WebServer server(80); //Створення вебсервера на порту 80 (Creating a web server on port 80)

TM1637 tm(CLK, DIO); //Створення об'єкту (Creating an object) TM1637 (Clk, Dio)
String display;

// Масив пінів крокового двигуна для зручності у програмуванні
// Array of Pins for the Stepper Motor for Programming Convenience
const short step_pin[4]{IN1, IN2, IN3, IN4};

//Статус бар'єру розумного будинку(Smart home barrier status)
enum sh_Status {B_CLOSED, B_BLOCK, B_TO_OPEN, B_TO_CLOSE, B_OPEN};
const char* messages[] = {
                          "the barrier is closed", 
                          "the barrier is blocked", 
                          "the barrier is opening now...", 
                          "the barrier is closing now...", 
                          "the barrier is open"};
sh_Status sh_status = B_CLOSED;

//Time
unsigned long sh_time,
              sensor_time;

//Counter
short sh_counter;

//web message
String web_message;

//WiFi
//Підключення до WiFi у режимі STA (Connecting to WiFi in STA mode)
void WiFi_connecting(void) {
  pinMode(BLDLED_BL, OUTPUT);//Встановлення режиму виводу для вбулованого світлодіоду(Setting the output mode for the built-in LED)
  WiFi.begin(ssid, password); //Підключення до WiFi у режимі STA (Connecting to WiFi in STA mode)
  Serial.println();
  Serial.print("Connecting to WiFi ...");
  //Очікуння підключення до мережі з мерехтінням світлодіоду (Waiting for network connection)
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BLDLED_BL, HIGH);
    delay(200);
    digitalWrite(BLDLED_BL, LOW);
    delay(600);
  }
  digitalWrite(BLDLED_BL, HIGH);//Indication after successful connection to WiFi network
}

//Stepper Motor functions
//Функція ініціалізації портів крокового двигуна(Stepper motor ports initialization function)
void stepper_init(void) {
  for (short k = 0; k < 4; k++)   // Налаштування пінів другого порта для крокового мотора через цикл та масив
  {                               // Setting up the pins of the port 2 for a stepper motor using a "for" and an array
    pinMode(step_pin[k], OUTPUT); // Налаштування пінів на режим "вихід" (Setting Pins to Output Mode)
    digitalWrite(step_pin[k], LOW); 
    // Вимикання пінів для базового положення шлагбаума (Turning Off Pins for the Barrier Gate to set base position)
  }
  sh_status = B_CLOSED;
}
// Функція оберту крокового двигуна на 90 градусів за годинниковою стрілкою
// Function for Rotating the Stepper Motor 90 Degrees Clockwise
void stepper_forward(void) {
// Для оберту на 90 градусів кроковий двигун має пройти 32 кроків
// For a 90-degree rotation, the stepper motor needs to complete 32 steps.
  for (short k = 0; k < 32; k++) 
  { // Один крок для двигуна - подача сигналу на кожен пін на 3мс по черзі
    // One step for the motor is the 3ms activation of each pin by setting it to HIGH in sequence.
    for (short i = 0; i <= 3; i++) {
      digitalWrite(step_pin[i], HIGH);
      delay(10);
      digitalWrite(step_pin[i], LOW);
    }
    if (k % 5 == 0) traffic_light_blinking();//Функція мигання світлофору (Traffic light blinking function
    //Блимаємо рідше - щоп'ятого разу(Blink less often - every fifth time)
  }
}

// Функція оберту крокового двигуна на 90 градусів проти годинникової стрілки
// Function for a 90-Degree Counterclockwise Rotation of the Stepper Motor
void stepper_backward(void) {
// Для оберту на 90 градусів кроковий двигун має пройти 32 кроків
// For a 90-degree rotation, the stepper motor needs to complete 32 steps.
  for (short k = 0; k < 32; k++) 
  { // Один крок для двигуна - подача сигналу на кожен пін на 3мс по черзі
    // One step for the motor is the 3ms activation of each pin by setting it to HIGH in sequence.
    for (short i = 3; i >= 0; i--) {
      digitalWrite(step_pin[i], HIGH);
      delay(10);
      digitalWrite(step_pin[i], LOW);
    }
    if (k % 5 == 0) traffic_light_blinking();////Функція мигання світлофору (Traffic light blinking function
    //Блимаємо рідше - щоп'ятого разу(Blink less often - every fifth time)
  }
}
// Функція для блокування шлагбаума (Function for Barrier Gate Locking)
void stepper_lock(void) {for (short k = 0; k < 4; k++) { digitalWrite(step_pin[k], LOW); }}
// Функція керування шлагбаумом (Barrier control function)
void stepper(void) {
  if(sh_status == B_TO_OPEN){
    stepper_backward(); //Відкриємо шлагбаум (Opening the barrier)
    sh_status = B_OPEN;
    sh_timer_start(); //Функція запуску таймера (Timer start function)
  }
  else if(sh_status == B_TO_CLOSE){
    stepper_forward(); //Закриваємо шлагбаум (We close the barrier)
    sh_status = B_CLOSED;   
  }
}


//Distance Sensor functions
//Функція ініціалізації датчика відстані HC-SR04 (HC-SR04 Distance Sensor initialization function)
void sensor_init(void){ 
  sensor_time = 0; //Обнулюємо час (Resetting time)
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  digitalWrite(TRIGPIN, LOW); //Встановлення значення LOW для триггера
  delay(1000); //Затримка для стабілізації датчика відстані(Delay for stabilization of the distance sensor)
}
//Функція контролю дистанції (Distance control function)
void sensor_distance(void){
  if(sh_status == B_CLOSED or sh_status == B_OPEN){
    if(millis()- sensor_time > 500) { //контроль не частіше 500 мс
      sensor_time = millis(); //Фіксуємо час контролю відстані
      digitalWrite(TRIGPIN, HIGH);  //Відправка імпульсу на тригер датчика
      delayMicroseconds(10);        //Затримка 10 мікросекунд
      digitalWrite(TRIGPIN, LOW);   // Зупинка відправки імпульсу
      //Зчитування часу відбиття звуку і перетворення його у ввідстань в см
      int cm = pulseIn(ECHOPIN, HIGH) / 58;
      if(cm <= OPENING_DISTANCE){
        if (sh_status == B_CLOSED) sh_status = B_TO_OPEN; //Переходимо у статус відкриття (We are moving to the opening status)
        if (sh_status == B_OPEN) sh_timer_start();//Функція перезапуску таймера (Timer restart function)
      }
    }
  }
}

//TM display
//Функція ініціалізації ТМ1637 (ТМ1637 initialization function)
void tmdisplay_init(void){
  tm.begin(); //Ініціалізація дісплея
  tm.setBrightness(4); //Налаштування яскравості дисплєя (4)
  display = ""; //
  tm.clearScreen(); //Очищення дисплея
}
//Функція Функція відображення інформації ТМ1637 (ТМ1637 information display function)
void tmdisplay(void){
  String display_new;
  switch (sh_status) { // Аналізуємо статус для відображення (Analyzing the status for display)
    case B_CLOSED: //
      display_new = "CLSD"; //Відображення тексту CLSD на дісплеі
      break;
    case B_BLOCK: //
      display_new = "BLOC"; //Відображення тексту BLOC на дісплеі
      break;
    case B_TO_OPEN: //
      display_new ="----"; //Відображення тексту ---- на дісплеі
      break;
    case B_TO_CLOSE: //
      display_new ="----"; //Відображення тексту ---- на дісплеі
      break;
    case B_OPEN: //
      if(sh_counter == WAITING_TIME) display_new ="OPEN"; //Відображення тексту OPEN на дісплеі
      else display_new =String(sh_counter); //Відображення sh_counter на дісплеі
      break;      
  }
  if(display != display_new){
    display = display_new; //Оновлюємо display (Updating the display string)
    tm.clearScreen(); //Очищення дисплея
    tm.display(display); //Відображення тексту
  }
}

//Traffic light
//Функція ініціалізації світлофору (Traffic light initialization function)
void traffic_light_init(void){
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GRN, OUTPUT);
  digitalWrite(RGB_RED, LOW); // Вимкнути червоний
  digitalWrite(RGB_GRN, LOW); // Вимкнути зелений
}
//Функція світлофору (Traffic light function)
void traffic_light(void){
  // Аналізуємо статус для відображення на світлофорі (Analyzing the status for display on the Traffic light)
  if(sh_status == B_OPEN) {
    digitalWrite(RGB_RED, LOW); // Вимкнути червоний
    digitalWrite(RGB_GRN, HIGH); // Увімкнути зелений
  }
  else if(sh_status == B_TO_OPEN or sh_status == B_TO_CLOSE){
    digitalWrite(RGB_RED, HIGH); // Увімкнути червоний
    digitalWrite(RGB_GRN, HIGH); // Увімкнути зелений  
  }
  else{
    digitalWrite(RGB_RED, HIGH); // Увімкнути червоний
    digitalWrite(RGB_GRN, LOW); // Вимкнути зелений      
  }
}
//Функція мигання світлофору (Traffic light blinking function)
void traffic_light_blinking(void){
  static bool blinking = true;
  if(blinking){
    digitalWrite(RGB_RED, HIGH); // Увімкнути червоний
    digitalWrite(RGB_GRN, HIGH); // Увімкнути зелений  
  }
  else{
    digitalWrite(RGB_RED, LOW); // Вимкнути червоний
    digitalWrite(RGB_GRN, LOW); // Вимкнути зелений
  }
  blinking = !blinking;
}

//Timer
//Функція запуску таймера (Timer start function)
void sh_timer_start(void){
  sh_counter = WAITING_TIME;   //Заносимо в лічильник час очікування (We enter the waiting time into the counter)
  sh_time = 0;
}
//Функція керування за часом (Time control function)
void sh_timer(void){
  if(sh_status == B_OPEN){
    if(sh_counter > 0 and sh_time == 0) sh_time = millis();
    else if(sh_counter > 0 and millis()-sh_time > 1000){
      sh_counter--; //Зворотній відлік(Countdown)
      sh_time = millis();
    }
    if(sh_counter == 0){ //Зупеняємо таймер(Stop the timer)
      sh_time = 0;
      sh_status = B_TO_CLOSE; //Переходимо у статус закриття (We are moving to the closing status)
    }
  }
}


//Web
//Запуск веб-сервера(Starting the web server)
void Web_starting(void){
  //Налаштування маршрутів для веб-сервера
  server.on("/", handle_OnConnect); //Головна сторінка
  server.on("/open", handle_open);  //Дії при натисканні Open (Actions when pressing Open)
  server.on("/close", handle_close); //Дії при натисканні Close (Actions when pressing Close)
  server.on("/block", handle_block); //Дії при натисканні Block (Actions when pressing Block)
  server.onNotFound(handle_NotFound); //Обробка невідомих запитів
  server.begin(); //Запуск веб-сервера 
}
//Обробка кліентських запитів(Processing client requests)
void handle_Client(void){
  server.handleClient(); 
}
//Обробник головної сторінки
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(messages[sh_status])); //Відправлення HTML-сторінки
}
//Дії при натисканні Open (Actions when pressing Open)
void handle_open(void) {
  if(sh_status == B_CLOSED or sh_status == B_BLOCK) {
    sh_status = B_TO_OPEN; //Переходимо у статус відкриття (We are moving to the opening status)
  }
  server.sendHeader("Location", "/"); //Перенаправлення на головну сторінку
  server.send(303);
}
//Дії при натисканні Close (Actions when pressing Close)
void handle_close(void) {
  if(sh_status == B_OPEN) {
    sh_counter == 0; sh_time = 0; //Зупеняємо таймер(Stop the timer)
    sh_status = B_TO_CLOSE; //Переходимо у статус закриття (We are moving to the closing status)  
  }
  server.sendHeader("Location", "/"); //Перенаправлення на головну сторінку
  server.send(303);
}
//Дії при натисканні Open (Actions when pressing Open)
void handle_block() {
  if(sh_status == B_CLOSED) {
    sh_status = B_BLOCK; //Переходимо в статус блокування бар'єру (We are entering the barrier blocking status)
  }
  server.sendHeader("Location", "/"); //Перенаправлення на головну сторінку
  server.send(303);
}
//Обробник 404 (коли запитаний маршрут не знайдено)
void handle_NotFound() {
    server.send(404, "text/html", "Not Founf"); //Відправка відповіді 404
}
