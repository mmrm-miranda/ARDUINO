#  Arduino

Bienvenido a mi repositorio de proyectos de hardware, Internet de las Cosas (IoT) y sistemas integrados desarrollados con microcontroladores (Arduino / ESP8266). En este repositorio se encuentran implementaciones de control automático, conectividad en la nube y lógica de programación interactiva.

##  Proyectos 

### 1. Sistema de Alarma IoT con Firebase 
Un sistema de monitoreo en tiempo real para detección de incendios y variables ambientales utilizando un NodeMCU (ESP8266).
* **Características principales:**
  * Uso de **WiFiManager** para el aprovisionamiento de red (sin contraseñas hardcodeadas).
  * Sincronización de tiempo precisa mediante protocolo **NTP**.
  * Lectura de sensores de temperatura/humedad (DHT11) y sensor de flama.
  * Comunicación asíncrona enviando reportes en formato JSON a **Firebase Realtime Database**.
* **Tecnologías:** C++, ESP8266, IoT, Firebase RTDB.

### 2. Robot Balancín con MPU6050 y Bluetooth 
Un robot de dos ruedas auto-equilibrado con control direccional a distancia.
* **Características principales:**
  * Integración de giroscopio y acelerómetro (**MPU6050**) a través del bus **I2C**.
  * Cálculo de ángulos de inclinación y estabilización de motores utilizando modulación de ancho de pulso (**PWM**).
  * Control remoto mediante módulo **Bluetooth** gestionando comandos Seriales de forma asíncrona.
* **Tecnologías:** C++, Puente H (L298N), Comunicación I2C y Serial (SoftwareSerial), Control de Motores.

### 3. Juego de Reflejos Interactivo 
Un dispositivo lúdico multijugador para medir reflejos visuales y auditivos.
* **Características principales:**
  * Máquina de estados para control de puntuación, rondas y entradas de los jugadores (botones).
  * Implementación de eventos con temporizadores **aleatorios** (`random()`).
  * Generación de sonido (Buzzer) por frecuencias (PWM) manejando el tiempo con `millis()` de forma no bloqueante.
* **Tecnologías:** C++, Algoritmia y Control de Flujo, PWM (Audio).

