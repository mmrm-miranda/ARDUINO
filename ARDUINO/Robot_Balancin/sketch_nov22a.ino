// Librerías I2C para controlar el mpu6050
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <SoftwareSerial.h>

// Configuración del MPU6050
MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio
int ax, ay, az;
int gx, gy, gz;

// Variables para el control del carro
const int pinENA = 3;
const int pinIN1 = 4;
const int pinIN2 = 5;

const int pinENB = 6;
const int pinIN3 = 7;
const int pinIN4 = 8;

float balanceThreshold = 2;  // Umbral para corregir el balance (ajustar según sea necesario)

// Configuración de Bluetooth (SoftwareSerial en los pines 10 y 11)
SoftwareSerial BT(10, 11);

// Variables de inclinación
float angle;

void setup() {
  Serial.begin(9600);    // Iniciando puerto serial
  Wire.begin();           // Iniciando I2C  
  sensor.initialize();    // Iniciando el sensor

  if (sensor.testConnection()) {
    Serial.println("Sensor iniciado correctamente");
  } else {
    Serial.println("Error al iniciar el sensor");
  }

  // Configuración de pines para control de motores
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinENA, OUTPUT);

  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  pinMode(pinENB, OUTPUT);

  BT.begin(9600);  // Inicializar Bluetooth a 9600 baudios
}

void loop() {
  // Leer las aceleraciones del acelerómetro (para determinar el ángulo de inclinación)
  sensor.getAcceleration(&ax, &ay, &az);
  
  // Calcular el ángulo de inclinación (este cálculo puede necesitar ajustes según el sensor y la orientación del carro)
  angle = atan2(ax, az) * 180.0 / PI;

  Serial.print("Ángulo: ");
  Serial.println(angle);

  int velocidad = map(abs(angle), 0, 40, 120, 150); // Ajustar el rango de velocidad PWM
  int tiempo = map(abs(angle), 0, 40, 10, 20); // Correcciones más rápidas

  // Control automático para balancear el carro
  if (angle > balanceThreshold) { // Si el carro se inclina hacia adelante
    avanzarA(velocidad);
    avanzarB(velocidad);
    delay(tiempo);

  }
  else if (angle < -balanceThreshold) { // Si el carro se inclina hacia atrás
    retrocederA(velocidad);
    retrocederB(velocidad);
    delay(tiempo);
  }
  else { // Si el carro está equilibrado
    detenerA();
    detenerB();
  }

  // Comandos Bluetooth para control manual
  if (BT.available()) {  // Si hay datos disponibles desde Bluetooth
    char valor = BT.read();  // Lee el dato
    Serial.write(valor);     // Imprime en el monitor serial

    if (valor == 'A') { // Avanzar
      avanzarA(150);
      avanzarB(150);
    }
    else if (valor == 'R') { // Retroceder
      retrocederA(150);
      retrocederB(150);
    }
    else if (valor == 'I') { // Girar a la izquierda
      retrocederA(150);
      avanzarB(150);
    }
    else if (valor == 'D') { // Girar a la derecha
      avanzarA(150);
      retrocederB(150);
    }
    else if (valor == 'S') { // Detener
      detenerA();
      detenerB();
    }
  }

  // Enviar comandos desde el monitor serial al Bluetooth (si es necesario)
  if (Serial.available()) {  // Si hay datos disponibles en el monitor serial
    BT.write(Serial.read());  // Enviar al Bluetooth
  }

  delay(20);  // Espera breve para evitar sobrecarga
}

void avanzarA(int velocidad) {
  digitalWrite(pinIN1, LOW); 
  digitalWrite(pinIN2, HIGH); 
  analogWrite(pinENA, velocidad);  // Controla velocidad
}

void avanzarB(int velocidad) {
  digitalWrite(pinIN3, LOW); 
  digitalWrite(pinIN4, HIGH); 
  analogWrite(pinENB, velocidad);  // Controla velocidad
}

void retrocederA(int velocidad) {
  digitalWrite(pinIN1, HIGH); 
  digitalWrite(pinIN2, LOW); 
  analogWrite(pinENA, velocidad);  // Controla velocidad
}

void retrocederB(int velocidad) {
  digitalWrite(pinIN3, HIGH); 
  digitalWrite(pinIN4, LOW); 
  analogWrite(pinENB, velocidad);  // Controla velocidad
}

void detenerA() {
  digitalWrite(pinIN1, LOW); 
  digitalWrite(pinIN2, LOW); 
  analogWrite(pinENA, 0);  // Detiene el motor A
}

void detenerB() {
  digitalWrite(pinIN3, LOW); 
  digitalWrite(pinIN4, LOW); 
  analogWrite(pinENB, 0);  // Detiene el motor B
}