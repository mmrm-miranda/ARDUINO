//Inicializacion  de variables
int rondas = 0;  // Inicio de la  cantidad de rondas
bool esperandoEntrada = true;  // Bandera para que cuando se de la cantidad de entradas comienze la secuancia (Empueza en true)
bool bandera = false;  // Bandera para saber si el LED está encendido 
bool secuenciaTerminada = false;  // Bandera para saber si la secuencia de LEDs ha terminado para que empieze el random de el led
int j1 = 0;  // Contador para el jugador 1
int j2 = 0;  // Contador para el jugador 2
int rondaActual = 0;  // Ronda actual




// Dentro de la cantidad de rondas 
int umbralGanador() { //Funcion para devolver un valor entero y es este caso devielve la mitad de las rondas
  if (rondas % 2 == 0) {
    return rondas / 2;  // Para un número par de rondas
  } else {
    return (rondas / 2) + 1;  // Para un número impar de rondas
  }
}


//Metodo para preguntar al usuario la cantidad de rondas
void juegos() {
  if (esperandoEntrada) { //Si esparando entrada es true
    Serial.println("Cantidad de rondas entre 1 y 4: "); //Pregunta en el pueto seria{}
    esperandoEntrada = false;  // Ya se solicito entrada y se vuelve falce
  }

  if (Serial.available() > 0) {  // checa si hay datos en el buffer serial
    String entrada = Serial.readStringUntil('\n');  // Lee hasta el carácter de nueva línea
    rondas = entrada.toInt();  // Se convierte a entero
    if (rondas >= 1 && rondas <= 4) {  //Si esta en lo planteado se implime las rondas y se hace la secuencia de leds indicando las rondas
      Serial.print("Rondas: ");
      Serial.println(rondas);
      secuenciaLEDs();  //Secuencia de leds
      secuenciaTerminada = true;  // Secuencia se termino true
      rondaActual = 0;  // Se nicia en cero la ronda actual
    } else {
      Serial.println("Número inválido. Debe estar entre 1 y 4."); //Si no es la cantidad de rontas planteada se vuelva a preguntar
      esperandoEntrada = true;  // Permite preguntar nuevamente si es necesario
    }
  }
}




// Función de la secuencia de los leds 
void secuenciaLEDs() {
  for (int shooky = 1;  shooky<= 1+ rondas; shooky++) { //Se 
    digitalWrite(shooky, HIGH);  // Enciende el LED
    delay(500);                  
    digitalWrite(shooky, LOW);   // Apaga el LED
    delay(50);                
  }
}




//Buzzer de intriga
int melody[] = {
  98, 110, 123, 131, 147, 165, 175, 196, // Secuencia de notas
  98, 110, 123, 131, 147, 165, 175, 196,
  261, 294, 329, 349, 392 // Un clímax hacia notas más agudas
};

int noteDurations[] = {
  8, 8, 8, 8, 8, 8, 8, 8,  // Duración corta para las notas graves (1/8)
  8, 8, 8, 8, 8, 8, 8, 8,
  4, 4, 4, 4, 4  // Notas más largas hacia el final (1/4)
};

void suspenso(unsigned long duracionTotal) {
  unsigned long startTime = millis();
  
  while (millis() - startTime < duracionTotal) {  // Bucle que dura hasta completar la duración total
    for (int thisNote = 0; thisNote < 21; thisNote++) {
      if (millis() - startTime >= duracionTotal) break;  // Detener si ya se ha alcanzado la duración total

      int noteDuration = 1000 / noteDurations[thisNote];

      tone(11, melody[thisNote], noteDuration);  // Genera la frecuencia en el pin 11

      // Esperar la duración de la nota
      delay(noteDuration);

      // Apagar el buzzer inmediatamente después de la nota
      noTone(11);
      
      // Si la duración total se cumple en mitad de la canción, salimos
      if (millis() - startTime >= duracionTotal) break;
    }
  }

  // Asegurarse de que la duración total sea respetada
  unsigned long elapsedTime = millis() - startTime;
  if (elapsedTime < duracionTotal) {
    delay(duracionTotal - elapsedTime);
  }
}







// Led con una duracion random
void ledRandom() {
  if (!bandera && secuenciaTerminada) {  // Solo enciende el LED si la secuencia ha terminado y no ha sido apagado por el botón
    int rj = random(500, 10000);  // Rango
    
    unsigned long startTime = millis();  // Guarda el tiempo de inicio
    while (millis() - startTime < rj) {  // Mientras no haya pasado el tiempo aleatorio
      suspenso(rj);  // Reproduce la melodía de suspenso durante el tiempo aleatorio
    }
    
    // Una vez pasado el tiempo aleatorio, apaga el buzzer y enciende el LED
    noTone(11);  // Asegura que el buzzer esté apagado antes de encender el LED
    digitalWrite(10, HIGH);  // Enciende el LED en el pin 10
    bandera = true;  // Marca la bandera como encendido
  }
}






// Función para verificar el estado de los botones y apagar el LED
void boton() {
  ledRandom(); //Se llama el led random
  int p1 = digitalRead(13);  // Lee el estado del botón 1
  int p2 = digitalRead(12);  // Lee el estado del botón 2

  // Verifica si alguno de los botones fue presionado
  if (p1 == HIGH && bandera) {
    digitalWrite(10, LOW);  // Apaga el LED en el pin 10
    noTone(11);  // Apaga el sonido del buzzer
    bandera = false;  // Marca la bandera como apagado
    Serial.print(p1);
    Serial.print(" : ");
    Serial.println(p2);
    Serial.println("______");
    digitalWrite(j1 + 6, HIGH);  // Enciende el LED del de puntaje
    j1++;  // Incrementa el contador del jugador 1
    rondaActual++;  // Incrementa el contador de la ronda actual
  } else if (p2 == HIGH && bandera) {
    digitalWrite(10, LOW);  // Apaga el LED en el pin 10
    noTone(11);  // Apaga el sonido del buzzer
    bandera = false;  // Marca la bandera como apagado
    Serial.print(p1);
    Serial.print(" : ");
    Serial.println(p2);
    Serial.println("______");
    digitalWrite(j2 + 2, HIGH);  // Enciende el LED del puntaje
    j2++;  // Incrementa el contador del jugador 2
    rondaActual++;  // Incrementa el contador de la ronda actual
  }

  // Verifica si alguno de los jugadores ha ganado más de la mitad de las rondas
  if (j1 >= umbralGanador() || j2 >= umbralGanador()) {
    mostrarResultados();  // Termina el juego y muestra los resultados
  } else if (rondaActual >= rondas) {  // Si se alcanzaron todas las rondas sin un ganador claro
    mostrarResultados();  // Muestra los resultados y reinicia el juego
  }
}






//Cancion buzzer de fin}
// Definición de las notas 
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

const int buzzerPin = 11; //Definicion del pin del buzzer

// Notas de la melodía 
int m[] = {
  NOTE_C4, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// Duración de cada nota (en ms)
int note[] = {
  4, 8, 8, 4, 4, 4, 4
};

// Número de ciclos
const int totalCycles = 4; //Tienen que ser 4 ciclos sin que se modifique
void playMelodyCycles(int cycles) {
  // Ejecutar la melodía los ciclos indicados
  for (int cycle = 0; cycle < cycles; cycle++) {
    for (int thisNote = 0; thisNote < 7; thisNote++) {
      int noteDuration = 1000 / note[thisNote];
      tone(buzzerPin, m[thisNote], noteDuration);

      // Pausa entre notas
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // Apagar el buzzer entre notas
      noTone(buzzerPin);
    }
  }
}









// Función para mostrar los resultados y reiniciar el juego
void mostrarResultados() {
  Serial.print(j1);
  Serial.print(" :::::::::::: ");
  Serial.println(j2);
  playMelodyCycles(totalCycles);


  apagarLeds();  // Apaga todos los LEDs
  reiniciarPuntaje();  // Reinicia los contadores de puntaje

  esperandoEntrada = true;  // Pregunta nuevamente cuántas rondas quiere jugar
  secuenciaTerminada = false;  // Reinicia la bandera de secuencia terminada
  rondaActual = 0;  // Reinicia la ronda actual
}





// Función para apagar todos los LEDs
void apagarLeds() {
  for (int i = 2; i <= 10; i++) {
    digitalWrite(i, LOW);  // Apaga todos los LEDs del pin 2 al 10
  }
}




// Función para reiniciar el puntaje
void reiniciarPuntaje() {
  j1 = 0;
  j2 = 0;
}

void setup() {
  Serial.begin(9600);

  pinMode(buzzerPin, OUTPUT);

  // Configuración de los pines de los botones como entradas
  for (int jim = 12; jim <= 13; jim++) {
    pinMode(jim, INPUT);
  }

  // Configuración de los pines para los LEDs
  for (int jin = 2; jin <= 10; jin++) {
    pinMode(jin, OUTPUT);
  }

}


void loop() {
  juegos();  // Ejecuta la función de entrada y secuencia 
  if (secuenciaTerminada && rondaActual < rondas) {
    boton();  // Verifica el estado de los botones y sigue con el juego hasta que todas las rondas terminen o haya un ganador
  }
 
}


