[Читати мене Українською](README_uk.md)

<center>

# Educational project "Smart Home Barrier" based on ESP8266

</center>

![Real Photo](/images/sh_barrier_open.png)

## 🏠 Brief description
This is an educational project that implements smart home barrier control on the ESP8266 Witty Cloud WiFi microcontroller using: an Ultrasonic HC-SR04 distance sensor, a 28BYJ-48 stepper motor (with a ULN2003 driver), a TM1637 7-segment display, and an RGB LED.

## ⚙️ Main features
Barrier control is implemented in automatic mode based on a signal from a distance sensor and in manual mode - from a website.

• When an object approaches the sensor at a specified distance, the barrier opens and the timer countdown starts.

• After the countdown is complete, the barrier closes.

• Manual opening of the barrier is done by pressing the OPEN button on the website.

• Before the timer ends, the barrier can be closed by pressing the CLOSE button.

• Pressing the BLOCK button places the barrier in blocking mode, when automatic opening is not possible.

• Approaching an object in blocking mode does not cause the barrier to open.

• You can open the barrier in lock mode by pressing the OPEN button on the website.

• After the timer counts down, the barrier closes.

The website looks like this:

![Real Photo](/images/sh_barrier_web.png)

## 🔌 Elements and electrical schematic

The following elements were used in the project:

• microcontroller WiFi ESP8266 Witty Cloud

• ultrasonic distance sensor HC-SR04 (ultrasonic sonar)

• 28BYJ-48 stepper motor with ULN2003 driver

• 4-bit seven-segment LED indicator with TM1637 driver

• RGB LED module KY-016

The elements are connected according to the electrical diagram:

![Real Photo](/images/Sheet_Electrical.png)

## 👓 Program code

The project program was created in the Arduino IDE. The code is presented in the sh_barrier folder.

To compile the project you need:

1. install Arduino IDE

2. install board manager ESP8266  
   File - Preferenses:   
   Additional boards manager URLs:  
   http://arduino.esp8266.com/stable/package_esp8266com_index.json   
   Boards manager: ESP8266 by ESP8266 Community  
   Tolls Board: "NodeMCU 1.0 (ESP-12E Module)"   

3. installing additional library:  
    TM1637 Driver by AKJ   

4. create and add the sh_ssid.h file with your WIFI network settings to the sh_barrier folder:   
  #define SH_WIFI_SSID "xxxxxxxxx"   
  #define SH_WIFI_PASS "xxxxxxxxx"   

## ✅ Conclusion

The project demonstrates the capabilities of programming the ESP8266 microcontroller, monitoring and controlling external devices, creating smart home elements, creating a web server on the ESP8266, and remotely controlling the microcontroller via a WiFi network using a website from a smartphone or computer.

I wish you every success!

P.S.  
This project was developed as a student coursework based on the results of the Smart Home, ESP8266 and GITHUB courses at Nanit Academy https://www.linkedin.com/company/nanitrobot/

A short video demonstrating how this project works is presented in a publication on LinkedIn
https://www.linkedin.com/feed/update/urn:li:activity:7415375095264608256/
 
