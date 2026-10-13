/**
 * DESCRIPCION:
 * Este programa toma arreglos de longitud variable con datos de 4 bits (b0 a b3)
 * obtenidos en MATLAB (tanto de senales periodicas como aperiodicas) y saca
 * cada valor por 4 pines digitales hacia el DAC R-2R, manteniendo cada dato
 * estable por un tiempo configurable entre 1 ms y 10000 ms (10 s).
 * 
 * MAPEO DE PINES HACIA EL DAC R-2R:
 *   - Pin D8  -> Bit 0 (LSB - Bit menos significativo, peso 1)
 *   - Pin D9  -> Bit 1 (peso 2)
 *   - Pin D10 -> Bit 2 (peso 4)
 *   - Pin D11 -> Bit 3 (MSB - Bit mas significativo, peso 8)
 * 
 * COMANDOS EN EL MONITOR SERIAL (9600 baudios):
 *   - 'A' o 'a': Modo escalera completa (16 niveles: 0 a 15).
 *   - 'P' o 'p': Senal PERIODICA generada en MATLAB (20 muestras).
 *   - 'S' o 's': Senal APERIODICA generada en MATLAB (18 muestras).
 *   - 'C' o 'c': Modo Calibracion (alterna entre 0 y 15).
 *   - 'M' o 'm': Modo Manual (deja fijo el valor actual).
 *   - '0'..'15': Pone manualmente un valor de 4 bits en las salidas.
 *   - 'T<ms>'  : Cambia el tiempo por dato entre 1 ms y 10000 ms
 *                (ej: 'T500' para 500 ms, 'T2000' para 2 s).
 * ============================================================================
 */

// Pines de salida digital hacia la red R-2R
const uint8_t PIN_B0_LSB = 8;  // D8 -> Bit 0 (LSB)
const uint8_t PIN_B1     = 9;  // D9 -> Bit 1
const uint8_t PIN_B2     = 10; // D10 -> Bit 2
const uint8_t PIN_B3_MSB = 11; // D11 -> Bit 3 (MSB)

// Rango de tiempo permitido para cada muestra (1 ms a 10 s)
const unsigned long TIEMPO_MIN_MS = 1;
const unsigned long TIEMPO_MAX_MS = 10000;

// Arreglo 1: Escalera de 0 a 15 para pruebas (16 muestras)
const uint8_t PATRON_ESCALERA[16] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};
const size_t LONGITUD_ESCALERA = sizeof(PATRON_ESCALERA) / sizeof(PATRON_ESCALERA[0]);

// Arreglo 2: Senal periodica senoidal sacada de MATLAB (20 muestras)
const uint8_t PATRON_PERIODICO_MATLAB[20] = {
   7, 10, 12, 13, 14, 14, 13, 12, 10,  8,
   7,  5,  3,  2,  1,  1,  2,  3,  5,  7
};
const size_t LONGITUD_PERIODICO = sizeof(PATRON_PERIODICO_MATLAB) / sizeof(PATRON_PERIODICO_MATLAB[0]);

// Arreglo 3: Senal aperiodica amortiguada sacada de MATLAB (18 muestras)
const uint8_t PATRON_APERIODICO_MATLAB[18] = {
   5, 11, 14, 12,  6,  2,  7, 10,  9,  6,
   2,  4,  6,  6,  4,  2,  2,  3
};
const size_t LONGITUD_APERIODICO = sizeof(PATRON_APERIODICO_MATLAB) / sizeof(PATRON_APERIODICO_MATLAB[0]);

// Variables globales para el control de tiempo y recorrido de los arreglos
unsigned long intervaloCambioMs = 1500;          // Tiempo por defecto en cada dato (1.5 s)
unsigned long tiempoPrevio = 0;                  // Guarda el ultimo valor de millis()
const uint8_t* arregloActivo = PATRON_ESCALERA;  // Apunta al arreglo que se esta usando
size_t longitudActiva = LONGITUD_ESCALERA;       // Cantidad de datos del arreglo actual
size_t indiceActual = 0;                         // Posicion actual en el arreglo
uint8_t valorActual = 0;                         // Valor de 4 bits que esta puesto en los pines
char modoOperacion = 'A';                        // Modo inicial: 'A' (Escalera)
bool estadoCalibracion = false;                  // Para alternar entre 0 y 15 en calibracion

// Prototipos de funciones
void aplicarBits(uint8_t valor);
void imprimirEstado(uint8_t valor);
void procesarComandosSeriales();
void seleccionarArreglo(const uint8_t* nuevoArreglo, size_t nuevaLongitud, char nuevoModo);
bool configurarTiempoPermanencia(unsigned long nuevoTiempoMs);

/**
 * ----------------------------------------------------------------------------
 * Funcion: setup
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Configura los 4 pines digitales como salidas para el DAC R-2R, pone el
 *   primer valor en los pines, inicia la comunicacion serial a 9600 baudios
 *   y muestra la lista de comandos en el Monitor Serial.
 *
 * Entrada:
 *   Ninguna (void).
 *
 * Salida:
 *   Ninguna (void). Deja configurados los pines y el puerto serial.
 *
 * Restricciones:
 *   - Los pines D8 a D11 van conectados a las entradas del DAC R-2R.
 *   - Solo se debe compartir tierra (GND) con el circuito analogico, sin
 *     usar los 5V del Arduino para alimentar op-amps.
 * ----------------------------------------------------------------------------
 */
void setup() {
  // Pines del DAC como salidas
  pinMode(PIN_B0_LSB, OUTPUT);
  pinMode(PIN_B1,     OUTPUT);
  pinMode(PIN_B2,     OUTPUT);
  pinMode(PIN_B3_MSB, OUTPUT);

  // Sacar el primer valor del arreglo inicial
  valorActual = arregloActivo[0];
  aplicarBits(valorActual);

  Serial.begin(9600);
  while (!Serial) {
    ; // Espera a que conecte el puerto serial
  }

  // Menu de ayuda al iniciar
  Serial.println("======================================================");
  Serial.println("  SISTEMA DE COMUNICACION POR MEDIOS OPTICOS (EL2113)");
  Serial.println("  Etapa 1: Generador de Patrones de 4 Bits");
  Serial.println("======================================================");
  Serial.println("Comandos disponibles:");
  Serial.println("  'A' -> Modo Escalera (16 datos: 0 a 15)");
  Serial.println("  'P' -> Arreglo MATLAB Senal Periodica (20 datos)");
  Serial.println("  'S' -> Arreglo MATLAB Senal Aperiodica (18 datos)");
  Serial.println("  'C' -> Modo Calibracion (alterna 0 y 15)");
  Serial.println("  'M' -> Modo Manual en espera");
  Serial.println("  '0'..'15' -> Fijar valor manual de 4 bits");
  Serial.println("  'T<ms>'   -> Cambiar tiempo entre 1 y 10000 ms (ej. T500)");
  Serial.println("======================================================\n");

  tiempoPrevio = millis();
  imprimirEstado(valorActual);
}

/**
 * ----------------------------------------------------------------------------
 * Funcion: loop
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Ciclo principal del programa. Revisa si entraron comandos por el puerto
 *   serial y va avanzando por el arreglo seleccionado ('A', 'P' o 'S') o
 *   alternando en calibracion ('C') usando millis() para no bloquear el codigo.
 *
 * Entrada:
 *   Ninguna (void). Usa las variables globales de estado y millis().
 *
 * Salida:
 *   Ninguna (void). Actualiza los pines digitales y manda datos al monitor.
 *
 * Restricciones:
 *   - No usar delay() para no trabar la lectura del Monitor Serial.
 *   - El tiempo de cambio debe mantenerse entre 1 ms y 10000 ms.
 * ----------------------------------------------------------------------------
 */
void loop() {
  // Revisar si el usuario mando algun comando
  procesarComandosSeriales();

  unsigned long tiempoActual = millis();

  // Si esta en alguno de los modos que recorren un arreglo
  if (modoOperacion == 'A' || modoOperacion == 'P' || modoOperacion == 'S') {
    if (tiempoActual - tiempoPrevio >= intervaloCambioMs) {
      tiempoPrevio = tiempoActual;
      
      // Avanza al siguiente dato y si llega al final vuelve a 0
      indiceActual = (indiceActual + 1) % longitudActiva;
      valorActual = arregloActivo[indiceActual];
      
      aplicarBits(valorActual);
      imprimirEstado(valorActual);
    }
  } 
  // Si esta en modo calibracion, alterna entre el minimo (0) y el maximo (15)
  else if (modoOperacion == 'C') {
    if (tiempoActual - tiempoPrevio >= intervaloCambioMs) {
      tiempoPrevio = tiempoActual;
      estadoCalibracion = !estadoCalibracion;
      
      if (estadoCalibracion) {
        valorActual = 15;
      } else {
        valorActual = 0;
      }
      
      aplicarBits(valorActual);
      imprimirEstado(valorActual);
    }
  }
  // En modo 'M' no entra a los if anteriores y deja el ultimo valor fijo en los pines
}

/**
 * ----------------------------------------------------------------------------
 * Funcion: seleccionarArreglo
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Cambia el puntero al arreglo que se quiere transmitir, guarda su longitud,
 *   reinicia el indice en cero y saca de una vez el primer valor por los pines.
 *
 * Entrada:
 *   - nuevoArreglo (const uint8_t*): Puntero al arreglo de 4 bits a usar.
 *   - nuevaLongitud (size_t): Cantidad de elementos que tiene el arreglo.
 *   - nuevoModo (char): Letra del modo seleccionado ('A', 'P' o 'S').
 *
 * Salida:
 *   Ninguna (void). Modifica las variables globales de recorrido y los pines.
 *
 * Restricciones:
 *   - El puntero no puede ser NULL y la longitud debe ser al menos 1.
 * ----------------------------------------------------------------------------
 */
void seleccionarArreglo(const uint8_t* nuevoArreglo, size_t nuevaLongitud, char nuevoModo) {
  if (nuevoArreglo == NULL || nuevaLongitud == 0) return;

  modoOperacion = nuevoModo;
  arregloActivo = nuevoArreglo;
  longitudActiva = nuevaLongitud;
  indiceActual = 0; // Reinicia desde el primer elemento
  
  valorActual = arregloActivo[0];
  tiempoPrevio = millis();
  
  aplicarBits(valorActual);
  imprimirEstado(valorActual);
}

/**
 * ----------------------------------------------------------------------------
 * Funcion: configurarTiempoPermanencia
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Verifica que el nuevo tiempo este dentro del rango permitido (1 ms a 10 s)
 *   y actualiza el intervalo de cambio entre muestras.
 *
 * Entrada:
 *   - nuevoTiempoMs (unsigned long): Tiempo en milisegundos ingresado por serial.
 *
 * Salida:
 *   - bool: Retorna true si el valor es valido y se aplico, o false si se sale
 *     del rango de 1 a 10000 ms.
 *
 * Restricciones:
 *   - nuevoTiempoMs debe estar entre 1 y 10000.
 * ----------------------------------------------------------------------------
 */
bool configurarTiempoPermanencia(unsigned long nuevoTiempoMs) {
  if (nuevoTiempoMs >= TIEMPO_MIN_MS && nuevoTiempoMs <= TIEMPO_MAX_MS) {
    intervaloCambioMs = nuevoTiempoMs;
    tiempoPrevio = millis(); // Reinicia el conteo con el nuevo tiempo
    return true;
  }
  return false;
}

/**
 * ----------------------------------------------------------------------------
 * Funcion: aplicarBits
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Separa un numero de 0 a 15 en sus 4 bits individuales usando mascaras y
 *   desplazamientos, y escribe HIGH o LOW en los pines D8, D9, D10 y D11.
 *
 * Entrada:
 *   - valor (uint8_t): Numero entero sin signo a enviar (0 a 15).
 *
 * Salida:
 *   Ninguna (void). Cambia el estado fisico de los pines D8 a D11.
 *
 * Restricciones:
 *   - El valor debe ser de 4 bits (0 a 15). Se le aplica una mascara 0x0F
 *     por seguridad para ignorar los 4 bits superiores.
 * ----------------------------------------------------------------------------
 */
void aplicarBits(uint8_t valor) {
  // Asegurar que solo pasen los primeros 4 bits (00001111)
  uint8_t dato4Bits = valor & 0x0F;

  // Sacar cada bit a su pin correspondiente
  digitalWrite(PIN_B0_LSB, (dato4Bits & 0x01) ? HIGH : LOW); // Bit 0
  digitalWrite(PIN_B1,     (dato4Bits & 0x02) ? HIGH : LOW); // Bit 1
  digitalWrite(PIN_B2,     (dato4Bits & 0x04) ? HIGH : LOW); // Bit 2
  digitalWrite(PIN_B3_MSB, (dato4Bits & 0x08) ? HIGH : LOW); // Bit 3
}

/**
 * ----------------------------------------------------------------------------
 * Funcion: imprimirEstado
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Calcula el voltaje teorico a la salida del DAC R-2R y muestra en el
 *   Monitor Serial el dato en decimal, en binario de 4 bits y el voltaje.
 *
 * Entrada:
 *   - valor (uint8_t): Valor digital actual entre 0 y 15.
 *
 * Salida:
 *   Ninguna (void). Imprime el estado por el puerto serial.
 *
 * Restricciones:
 *   - El puerto serial debe estar iniciado.
 *   - Se asume Vref = 5.0 V de las salidas digitales del Arduino.
 * ----------------------------------------------------------------------------
 */
void imprimirEstado(uint8_t valor) {
  uint8_t dato = valor & 0x0F;

  // Formula teorica del R-2R: Vout = Vref * (dato / 2^4)
  float voltajeTeorico = (dato * 5.0) / 16.0;

  Serial.print("[TX] Decimal: ");
  if (dato < 10) Serial.print(" "); // Espacio para que quede alineado en consola
  Serial.print(dato);

  Serial.print(" | Binario [b3 b2 b1 b0]: ");
  // Imprime bit por bit desde el MSB (b3) hasta el LSB (b0)
  for (int i = 3; i >= 0; i--) {
    Serial.print((dato >> i) & 0x01);
  }

  Serial.print(" | V_DAC Teorico: ");
  Serial.print(voltajeTeorico, 3);
  Serial.println(" V");
}

/**
 * ----------------------------------------------------------------------------
 * Funcion: procesarComandosSeriales
 * ----------------------------------------------------------------------------
 * Descripcion:
 *   Lee lo que el usuario escriba en el Monitor Serial hasta el salto de linea
 *   y ejecuta la accion correspondiente: cambiar de arreglo ('A', 'P', 'S'),
 *   ir a calibracion ('C'), modo manual ('M' o numero 0-15) o cambiar el
 *   tiempo con 'T<ms>'.
 *
 * Entrada:
 *   Ninguna (void). Lee los datos disponibles en el buffer serial.
 *
 * Salida:
 *   Ninguna (void). Actualiza el modo de operacion, el tiempo o los pines.
 *
 * Restricciones:
 *   - El Monitor Serial debe estar configurado con "Ambos NL & CR" o "Nueva linea".
 *   - El tiempo en 'T<ms>' debe ser un entero entre 1 y 10000.
 *   - El valor manual debe ser un entero entre 0 y 15.
 * ----------------------------------------------------------------------------
 */
void procesarComandosSeriales() {
  if (Serial.available() > 0) {
    String entrada = Serial.readStringUntil('\n');
    entrada.trim(); // Quita espacios o saltos de linea extra

    if (entrada.length() == 0) return;

    // Seleccion de modos principales
    if (entrada.equalsIgnoreCase("A")) {
      Serial.println(">>> Modo ESCALERA activado (16 datos: 0 a 15) <<<");
      seleccionarArreglo(PATRON_ESCALERA, LONGITUD_ESCALERA, 'A');
    } 
    else if (entrada.equalsIgnoreCase("P")) {
      Serial.println(">>> Arreglo MATLAB PERIODICO activado (20 muestras) <<<");
      seleccionarArreglo(PATRON_PERIODICO_MATLAB, LONGITUD_PERIODICO, 'P');
    }
    else if (entrada.equalsIgnoreCase("S")) {
      Serial.println(">>> Arreglo MATLAB APERIODICO activado (18 muestras) <<<");
      seleccionarArreglo(PATRON_APERIODICO_MATLAB, LONGITUD_APERIODICO, 'S');
    }
    else if (entrada.equalsIgnoreCase("C")) {
      modoOperacion = 'C';
      Serial.println(">>> Modo CALIBRACION activado (Alterna 0 y 15) <<<");
    } 
    else if (entrada.equalsIgnoreCase("M")) {
      modoOperacion = 'M';
      Serial.println(">>> Modo MANUAL activado. Escriba un numero del 0 al 15 <<<");
    }
    // Cambio de tiempo con formato T<ms>
    else if ((entrada.charAt(0) == 'T' || entrada.charAt(0) == 't') && entrada.length() > 1) {
      String subTiempo = entrada.substring(1);
      bool tiempoValido = true;
      
      // Revisar que lo que sigue despues de la T sean solo numeros
      for (unsigned int i = 0; i < subTiempo.length(); i++) {
        if (!isDigit(subTiempo.charAt(i))) {
          tiempoValido = false;
          break;
        }
      }
      
      if (tiempoValido) {
        unsigned long tMs = (unsigned long)subTiempo.toInt();
        if (configurarTiempoPermanencia(tMs)) {
          Serial.print(">>> Tiempo actualizado a: ");
          Serial.print(intervaloCambioMs);
          Serial.println(" ms <<<");
          return;
        }
      }
      Serial.println("Error: El tiempo debe estar entre 1 ms y 10000 ms (ej. T500).");
    }
    // Si no fue una letra, revisar si el usuario metio un numero del 0 al 15
    else {
      bool esNumero = true;
      for (unsigned int i = 0; i < entrada.length(); i++) {
        if (!isDigit(entrada.charAt(i))) {
          esNumero = false;
          break;
        }
      }

      if (esNumero) {
        int valorIngresado = entrada.toInt();
        if (valorIngresado >= 0 && valorIngresado <= 15) {
          modoOperacion = 'M';
          valorActual = (uint8_t)valorIngresado;
          aplicarBits(valorActual);
          
          Serial.print(">>> Dato manual fijado: ");
          imprimirEstado(valorActual);
          return;
        }
      }

      Serial.println("Comando no valido. Use 'A', 'P', 'S', 'C', 'M', 'T<ms>' o un numero de 0 a 15.");
    }
  }
}
