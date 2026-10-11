/**
 * ============================================================================
 * PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
 * ETAPA 1: Patrón de datos de prueba digital de longitud variable (4 bits)
 * AUTOR: Mario Cerdas y equipo de trabajo
 * MICROCONTROLADOR: Arduino UNO / Arduino Nano / Arduino Mega
 * CUMPLIMIENTO: Requerimientos C.3, D.4, P.17 y Punto 8 (pág. 6) del manual
 * ============================================================================
 * 
 * DESCRIPCIÓN:
 * Este programa toma arreglos de longitud variable de datos de 4 bits (b0 a b3)
 * generados mediante MATLAB (tanto de señales periódicas como aperiódicas) y
 * coloca cada palabra digital de manera estable en 4 pines digitales durante un
 * intervalo de tiempo configurable dentro del rango oficial [1 ms, 10000 ms].
 * 
 * MAPEO DE PINES HACIA EL DAC R-2R:
 *   - Pin D8  -> Bit 0 (LSB - Bit Menos Significativo, peso 2^0 = 1)
 *   - Pin D9  -> Bit 1 (peso 2^1 = 2)
 *   - Pin D10 -> Bit 2 (peso 2^2 = 4)
 *   - Pin D11 -> Bit 3 (MSB - Bit Más Significativo, peso 2^3 = 8)
 * 
 * MODOS DE OPERACIÓN POR MONITOR SERIAL (9600 baudios):
 *   - 'A' o 'a': Recorre el arreglo de escalera completa (16 niveles: 0 a 15).
 *   - 'P' o 'p': Recorre el arreglo de señal PERIÓDICA generado en MATLAB (N=20).
 *   - 'S' o 's': Recorre el arreglo de señal APERIÓDICA generado en MATLAB (N=18).
 *   - 'C' o 'c': Modo Calibración (alterna extremos 0 y 15).
 *   - 'M' o 'm': Modo Manual en espera.
 *   - '0'..'15': Fija manualmente un código de 4 bits en los pines de salida.
 *   - 'T<ms>'  : Ajusta el tiempo de permanencia por dato entre 1 ms y 10000 ms
 *                (ejemplo: 'T500' fija 500 ms; 'T2000' fija 2 s).
 * ============================================================================
 */

// Definición de los 4 pines digitales de salida hacia la red R-2R (Req. C.3)
const uint8_t PIN_B0_LSB = 8;  // Pin digital D8 asignado al Bit 0 (LSB)
const uint8_t PIN_B1     = 9;  // Pin digital D9 asignado al Bit 1
const uint8_t PIN_B2     = 10; // Pin digital D10 asignado al Bit 2
const uint8_t PIN_B3_MSB = 11; // Pin digital D11 asignado al Bit 3 (MSB)

// Límites oficiales de tiempo de estabilidad por dato según Requerimiento C.3
const unsigned long TIEMPO_MIN_MS = 1UL;     // Tiempo mínimo permitido: 1 ms
const unsigned long TIEMPO_MAX_MS = 10000UL; // Tiempo máximo permitido: 10 s (10000 ms)

// Arreglo 1: Escalera ascendente de caracterización (Longitud = 16 muestras)
const uint8_t PATRON_ESCALERA[16] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};
const size_t LONGITUD_ESCALERA = sizeof(PATRON_ESCALERA) / sizeof(PATRON_ESCALERA[0]); // Calcula 16 elementos

// Arreglo 2: Señal PERIÓDICA senoidal generada en MATLAB (Longitud variable = 20 muestras, Req. D.4)
const uint8_t PATRON_PERIODICO_MATLAB[20] = {
   7, 10, 12, 13, 14, 14, 13, 12, 10,  8,
   7,  5,  3,  2,  1,  1,  2,  3,  5,  7
};
const size_t LONGITUD_PERIODICO = sizeof(PATRON_PERIODICO_MATLAB) / sizeof(PATRON_PERIODICO_MATLAB[0]); // Calcula 20 elementos

// Arreglo 3: Señal APERIÓDICA amortiguada generada en MATLAB (Longitud variable = 18 muestras, Req. D.4)
const uint8_t PATRON_APERIODICO_MATLAB[18] = {
   5, 11, 14, 12,  6,  2,  7, 10,  9,  6,
   2,  4,  6,  6,  4,  2,  2,  3
};
const size_t LONGITUD_APERIODICO = sizeof(PATRON_APERIODICO_MATLAB) / sizeof(PATRON_APERIODICO_MATLAB[0]); // Calcula 18 elementos

// Variables globales de control de tiempo y recorrido de arreglos de longitud variable
unsigned long intervaloCambioMs = 1500UL;            // Tiempo activo por palabra digital (por defecto 1500 ms)
unsigned long tiempoPrevio = 0UL;                    // Marca de tiempo del último cambio no bloqueante
const uint8_t* arregloActivo = PATRON_ESCALERA;      // Puntero al arreglo de longitud variable actualmente seleccionado
size_t longitudActiva = LONGITUD_ESCALERA;           // Cantidad de elementos N del arreglo activo
size_t indiceActual = 0;                             // Índice actual dentro del arreglo activo [0 .. N-1]
uint8_t valorActual = 0;                             // Palabra digital de 4 bits actualmente presente en los pines
char modoOperacion = 'A';                            // Modo actual: 'A' Escalera, 'P' Periódico, 'S' Aperiódico, 'C' Calibración, 'M' Manual
bool estadoCalibracion = false;                      // Bandera booleana para alternar entre 0 y 15 en calibración

// Prototipos de las funciones modulares del sistema
void aplicarBits(uint8_t valor);
void imprimirEstado(uint8_t valor);
void procesarComandosSeriales();
void seleccionarArreglo(const uint8_t* nuevoArreglo, size_t nuevaLongitud, char nuevoModo);
bool configurarTiempoPermanencia(unsigned long nuevoTiempoMs);

/**
 * ----------------------------------------------------------------------------
 * Función: setup
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Inicializa los cuatro pines digitales de salida hacia el DAC R-2R, coloca
 *   el primer elemento del arreglo activo en las salidas, inicia la comunicación
 *   serial a 9600 baudios y muestra el menú interactivo en el Monitor Serial.
 *
 * Entrada:
 *   Ninguna (void).
 *
 * Salida:
 *   Ninguna (void). Configura el estado inicial de los registros de E/S y UART.
 *
 * Restricciones:
 *   - Los pines D8, D9, D10 y D11 deben conectarse exclusivamente al DAC R-2R.
 *   - El pin 5V del Arduino NO debe alimentar las etapas analógicas (Req. P.17);
 *     solo se comparte la referencia de tierra común (GND).
 * ----------------------------------------------------------------------------
 */
void setup() {
  // Configurar el pin D8 (Bit 0 - LSB) como salida digital
  pinMode(PIN_B0_LSB, OUTPUT);
  // Configurar el pin D9 (Bit 1) como salida digital
  pinMode(PIN_B1,     OUTPUT);
  // Configurar el pin D10 (Bit 2) como salida digital
  pinMode(PIN_B2,     OUTPUT);
  // Configurar el pin D11 (Bit 3 - MSB) como salida digital
  pinMode(PIN_B3_MSB, OUTPUT);

  // Cargar el primer dato del arreglo activo inicial
  valorActual = arregloActivo[0];
  // Colocar la primera palabra de 4 bits en los pines físicos D8..D11
  aplicarBits(valorActual);

  // Inicializar el puerto de comunicación serial a 9600 bits por segundo
  Serial.begin(9600);
  // Esperar hasta que el puerto serial esté listo (en placas con USB nativo)
  while (!Serial) {
    ; // Ciclo de espera vacío hasta establecer enlace serial
  }

  // Imprimir encabezado informativo del sistema en el Monitor Serial
  Serial.println(F("======================================================"));
  Serial.println(F("  SISTEMA DE COMUNICACIÓN POR MEDIOS ÓPTICOS (EL2113)"));
  Serial.println(F("  Etapa 1: Patrón Digital de Longitud Variable (4 Bits)"));
  Serial.println(F("======================================================"));
  Serial.println(F("Comandos disponibles en el Monitor Serial:"));
  Serial.println(F("  'A' -> Modo Automático Escalera (16 datos: 0 a 15)"));
  Serial.println(F("  'P' -> Arreglo MATLAB Señal Periódica (20 datos)"));
  Serial.println(F("  'S' -> Arreglo MATLAB Señal Aperiódica (18 datos)"));
  Serial.println(F("  'C' -> Modo Calibración (alterna extremos 0 y 15)"));
  Serial.println(F("  'M' -> Modo Manual en espera"));
  Serial.println(F("  '0'..'15' -> Fijar código manual de 4 bits"));
  Serial.println(F("  'T<ms>'   -> Fijar tiempo entre 1 y 10000 ms (ej. T500)"));
  Serial.println(F("======================================================\n"));

  // Guardar la marca de tiempo inicial del sistema en milisegundos
  tiempoPrevio = millis();
  // Mostrar por consola el estado inicial aplicado en los pines
  imprimirEstado(valorActual);
}

/**
 * ----------------------------------------------------------------------------
 * Función: loop
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Ciclo principal no bloqueante que revisa comandos seriales y recorre el
 *   arreglo de longitud variable seleccionado ('A', 'P' o 'S') o el modo de
 *   calibración ('C'), manteniendo cada palabra digital estable durante el
 *   tiempo configurado 'intervaloCambioMs' (entre 1 ms y 10000 ms).
 *
 * Entrada:
 *   Ninguna (void). Lee variables globales de estado y el reloj millis().
 *
 * Salida:
 *   Ninguna (void). Actualiza los 4 pines digitales y envía telemetría serial.
 *
 * Restricciones:
 *   - Prohibido el uso de delay() bloqueante para mantener respuesta en tiempo real.
 *   - El intervalo 'intervaloCambioMs' se mantiene restringido a [1, 10000] ms.
 * ----------------------------------------------------------------------------
 */
void loop() {
  // Atender cualquier comando recibido desde la computadora por el Monitor Serial
  procesarComandosSeriales();

  // Leer el contador de milisegundos actual desde el encendido del Arduino
  unsigned long tiempoActual = millis();

  // Verificar si el modo activo recorre un arreglo de longitud variable ('A', 'P' o 'S')
  if (modoOperacion == 'A' || modoOperacion == 'P' || modoOperacion == 'S') {
    // Comprobar si ya transcurrió el tiempo de estabilidad configurado para el dato actual
    if (tiempoActual - tiempoPrevio >= intervaloCambioMs) {
      // Actualizar la marca de tiempo de referencia para el siguiente dato
      tiempoPrevio = tiempoActual;
      // Avanzar el índice de forma circular según la longitud variable 'longitudActiva'
      indiceActual = (indiceActual + 1) % longitudActiva;
      // Extraer el nuevo dato de 4 bits desde el arreglo activo
      valorActual = arregloActivo[indiceActual];
      // Escribir los 4 bits de manera estable en los pines D8, D9, D10 y D11
      aplicarBits(valorActual);
      // Imprimir el estado actual y su tensión teórica en el Monitor Serial
      imprimirEstado(valorActual);
    }
  } 
  // Verificar si el sistema se encuentra en el modo de calibración óptica ('C')
  else if (modoOperacion == 'C') {
    // Alternar entre los estados extremos cada 'intervaloCambioMs' milisegundos
    if (tiempoActual - tiempoPrevio >= intervaloCambioMs) {
      // Actualizar la marca de tiempo de referencia
      tiempoPrevio = tiempoActual;
      // Conmutar el estado booleano entre verdadero y falso
      estadoCalibracion = !estadoCalibracion;
      // Asignar 15 (1111_2 = máxima luz) o 0 (0000_2 = oscuridad)
      valorActual = estadoCalibracion ? 15 : 0;
      // Escribir los 4 bits del extremo seleccionado en los pines D8..D11
      aplicarBits(valorActual);
      // Reportar el estado de calibración en el Monitor Serial
      imprimirEstado(valorActual);
    }
  }
  // En modo 'M' (Manual), la palabra digital permanece fija indefinidamente en los 4 pines
}

/**
 * ----------------------------------------------------------------------------
 * Función: seleccionarArreglo
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Asigna un nuevo arreglo de datos de 4 bits de longitud variable para ser
 *   transmitido secuencialmente, reinicia el índice a la primera posición y
 *   aplica inmediatamente el primer elemento en los pines de salida.
 *
 * Entrada:
 *   - nuevoArreglo (const uint8_t*): Puntero al arreglo de datos de 4 bits.
 *   - nuevaLongitud (size_t): Cantidad de elementos del arreglo (longitud variable).
 *   - nuevoModo (char): Carácter identificador del modo ('A', 'P' o 'S').
 *
 * Salida:
 *   Ninguna (void). Actualiza los punteros globales y los pines D8-D11.
 *
 * Restricciones:
 *   - El puntero 'nuevoArreglo' no debe ser nulo (NULL) y 'nuevaLongitud' >= 1.
 * ----------------------------------------------------------------------------
 */
void seleccionarArreglo(const uint8_t* nuevoArreglo, size_t nuevaLongitud, char nuevoModo) {
  // Verificar que el arreglo sea válido y tenga al menos un elemento
  if (nuevoArreglo == NULL || nuevaLongitud == 0) return;

  // Actualizar el modo de operación activo
  modoOperacion = nuevoModo;
  // Apuntar al nuevo arreglo de datos generado en MATLAB
  arregloActivo = nuevoArreglo;
  // Almacenar la longitud particular del nuevo arreglo
  longitudActiva = nuevaLongitud;
  // Reiniciar el recorrido en el índice cero
  indiceActual = 0;
  // Leer el primer dato de 4 bits del arreglo seleccionado
  valorActual = arregloActivo[0];
  //Reiniciar el temporizador no bloqueante a partir del instante actual
  tiempoPrevio = millis();
  // Colocar el primer dato en los 4 pines digitales
  aplicarBits(valorActual);
  // Mostrar en consola el primer dato transmitido
  imprimirEstado(valorActual);
}

/**
 * ----------------------------------------------------------------------------
 * Función: configurarTiempoPermanencia
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Valida y actualiza el tiempo durante el cual cada dato de 4 bits permanece
 *   estable en las salidas digitales, garantizando el cumplimiento estricto del
 *   Requerimiento C.3 (entre 1 ms y 10 s = 10000 ms).
 *
 * Entrada:
 *   - nuevoTiempoMs (unsigned long): Tiempo deseado en milisegundos.
 *
 * Salida:
 *   - bool: Devuelve 'true' si el tiempo está en [1, 10000] ms y fue aplicado;
 *     devuelve 'false' si está fuera del rango permitido por el instructivo.
 *
 * Restricciones:
 *   - 'nuevoTiempoMs' debe cumplir: 1 <= nuevoTiempoMs <= 10000.
 * ----------------------------------------------------------------------------
 */
bool configurarTiempoPermanencia(unsigned long nuevoTiempoMs) {
  // Comprobar que el valor solicitado respete los límites [1 ms, 10000 ms] de C.3
  if (nuevoTiempoMs >= TIEMPO_MIN_MS && nuevoTiempoMs <= TIEMPO_MAX_MS) {
    // Actualizar la variable global de temporización
    intervaloCambioMs = nuevoTiempoMs;
    // Reiniciar la referencia de tiempo para aplicar el cambio de inmediato
    tiempoPrevio = millis();
    // Retornar éxito en la configuración
    return true;
  }
  // Retornar falso si el valor viola la restricción C.3
  return false;
}

/**
 * ----------------------------------------------------------------------------
 * Función: aplicarBits
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Descompone un número entero en sus 4 bits menos significativos (b0 a b3)
 *   mediante máscaras binarias y escribe los niveles lógicos (HIGH = 5V, LOW = 0V)
 *   en los cuatro pines digitales conectados a la escalera R-2R.
 *
 * Entrada:
 *   - valor (uint8_t): Número entero sin signo que representa el estado digital
 *     a transmitir (rango esperado de 0 a 15).
 *
 * Salida:
 *   Ninguna (void). Modifica directamente el estado eléctrico de los pines D8-D11.
 *
 * Restricciones:
 *   - El valor de entrada debe estar entre 0 y 15. Si es mayor a 15, se aplica
 *     una máscara AND (0x0F) para conservar únicamente los 4 bits inferiores.
 * ----------------------------------------------------------------------------
 */
void aplicarBits(uint8_t valor) {
  // Aplicar máscara 0x0F (00001111_2) para asegurar un dato puro de 4 bits
  uint8_t dato4Bits = valor & 0x0F;

  // Extraer el Bit 0 (LSB, peso 1) y escribirlo en el pin digital D8
  digitalWrite(PIN_B0_LSB, (dato4Bits & 0x01) ? HIGH : LOW);
  // Extraer el Bit 1 (peso 2) y escribirlo en el pin digital D9
  digitalWrite(PIN_B1,     (dato4Bits & 0x02) ? HIGH : LOW);
  // Extraer el Bit 2 (peso 4) y escribirlo en el pin digital D10
  digitalWrite(PIN_B2,     (dato4Bits & 0x04) ? HIGH : LOW);
  // Extraer el Bit 3 (MSB, peso 8) y escribirlo en el pin digital D11
  digitalWrite(PIN_B3_MSB, (dato4Bits & 0x08) ? HIGH : LOW);
}

/**
 * ----------------------------------------------------------------------------
 * Función: imprimirEstado
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Calcula el voltaje analógico teórico esperado a la salida de la red R-2R y
 *   envía por el Monitor Serial una línea formateada con el valor decimal, la
 *   palabra binaria [b3 b2 b1 b0] y el voltaje teórico en voltios.
 *
 * Entrada:
 *   - valor (uint8_t): Estado digital actual entre 0 y 15.
 *
 * Salida:
 *   Ninguna (void). Envía texto formateado a través de la interfaz Serial.
 *
 * Restricciones:
 *   - La comunicación Serial debe haber sido inicializada previamente con Serial.begin().
 *   - El cálculo asume un voltaje de referencia V_ref = 5.0 V en los pines del Arduino.
 * ----------------------------------------------------------------------------
 */
void imprimirEstado(uint8_t valor) {
  // Conservar únicamente los 4 bits menos significativos del dato (0 a 15)
  uint8_t dato = valor & 0x0F;

  // Calcular tensión teórica del DAC R-2R: V_out = 5.0 V * (dato / 16.0)
  float voltajeTeorico = (dato * 5.0) / 16.0;

  // Imprimir etiqueta de transmisión y valor decimal alineado
  Serial.print(F("[TX] Decimal: "));
  // Agregar espacio de relleno si el número tiene un solo dígito
  if (dato < 10) Serial.print(F(" "));
  // Imprimir el número decimal (0 a 15)
  Serial.print(dato);

  // Imprimir encabezado de la representación binaria de 4 bits
  Serial.print(F(" | Binario [b3 b2 b1 b0]: "));
  // Recorrer desde el bit más significativo (b3) hasta el menos significativo (b0)
  for (int8_t i = 3; i >= 0; i--) {
    // Desplazar 'i' bits a la derecha, aislar el bit con AND 1 e imprimirlo
    Serial.print((dato >> i) & 0x01);
  }

  // Imprimir el voltaje analógico teórico esperado con 3 cifras decimales
  Serial.print(F(" | V_DAC Teórico: "));
  Serial.print(voltajeTeorico, 3);
  Serial.println(F(" V"));
}

/**
 * ----------------------------------------------------------------------------
 * Función: procesarComandosSeriales
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Lee las cadenas recibidas por el puerto serial hasta encontrar un salto de
 *   línea ('\n') y ejecuta el cambio de modo ('A', 'P', 'S', 'C', 'M'), el ajuste
 *   de tiempo 'T<ms>' en [1, 10000] ms o la fijación manual de un dato en [0, 15].
 *
 * Entrada:
 *   Ninguna (void). Lee directamente los bytes del búfer de recepción UART.
 *
 * Salida:
 *   Ninguna (void). Actualiza el estado global del sistema y los pines D8-D11.
 *
 * Restricciones:
 *   - Los comandos deben finalizar con carácter de nueva línea ('\n').
 *   - El comando 'T<ms>' solo acepta enteros dentro de [1, 10000] ms (Req. C.3).
 *   - Los estados manuales solo aceptan números enteros dentro de [0, 15].
 * ----------------------------------------------------------------------------
 */
void procesarComandosSeriales() {
  // Verificar si hay al menos un carácter disponible en el búfer serial
  if (Serial.available() > 0) {
    // Leer toda la cadena hasta el salto de línea
    String entrada = Serial.readStringUntil('\n');
    // Eliminar espacios en blanco o retornos de carro al inicio y al final
    entrada.trim();

    // Si la cadena recibida está vacía, salir sin realizar cambios
    if (entrada.length() == 0) return;

    // Comando 'A': Seleccionar arreglo de escalera completa (16 niveles)
    if (entrada.equalsIgnoreCase("A")) {
      Serial.println(F(">>> Activado MODO AUTOMÁTICO ESCALERA (16 datos: 0 a 15) <<<"));
      seleccionarArreglo(PATRON_ESCALERA, LONGITUD_ESCALERA, 'A');
    } 
    // Comando 'P': Seleccionar arreglo periódico generado en MATLAB (20 datos)
    else if (entrada.equalsIgnoreCase("P")) {
      Serial.println(F(">>> Activado ARREGLO MATLAB PERIÓDICO (Longitud N = 20) <<<"));
      seleccionarArreglo(PATRON_PERIODICO_MATLAB, LONGITUD_PERIODICO, 'P');
    }
    // Comando 'S': Seleccionar arreglo aperiódico generado en MATLAB (18 datos)
    else if (entrada.equalsIgnoreCase("S")) {
      Serial.println(F(">>> Activado ARREGLO MATLAB APERIÓDICO (Longitud N = 18) <<<"));
      seleccionarArreglo(PATRON_APERIODICO_MATLAB, LONGITUD_APERIODICO, 'S');
    }
    // Comando 'C': Activar modo de calibración óptica entre extremos 0 y 15
    else if (entrada.equalsIgnoreCase("C")) {
      modoOperacion = 'C';
      Serial.println(F(">>> Activado MODO CALIBRACIÓN (Alterna 0 y 15) <<<"));
    } 
    // Comando 'M': Activar modo manual manteniendo el estado actual fijo
    else if (entrada.equalsIgnoreCase("M")) {
      modoOperacion = 'M';
      Serial.println(F(">>> Activado MODO MANUAL. Ingrese un número del 0 al 15 <<<"));
    }
    // Comando 'T<ms>': Configurar el tiempo de estabilidad por dato entre 1 ms y 10000 ms
    else if ((entrada.charAt(0) == 'T' || entrada.charAt(0) == 't') && entrada.length() > 1) {
      // Extraer la subcadena numérica posterior a la letra 'T'
      String subTiempo = entrada.substring(1);
      // Verificar que todos los caracteres posteriores a 'T' sean dígitos
      bool tiempoValido = true;
      for (unsigned int i = 0; i < subTiempo.length(); i++) {
        if (!isDigit(subTiempo.charAt(i))) {
          tiempoValido = false;
          break;
        }
      }
      // Si es numérico, convertir a entero largo y validar rango [1, 10000] ms
      if (tiempoValido) {
        unsigned long tMs = (unsigned long)subTiempo.toInt();
        if (configurarTiempoPermanencia(tMs)) {
          Serial.print(F(">>> Tiempo de permanencia actualizado (Req. C.3): "));
          Serial.print(intervaloCambioMs);
          Serial.println(F(" ms <<<"));
          return;
        }
      }
      // Informar si el tiempo solicitado está fuera del rango [1 ms, 10000 ms]
      Serial.println(F("Error: El tiempo debe estar entre 1 ms y 10000 ms (ej. T500)."));
    }
    else {
      // Comprobar si la entrada corresponde a un número manual entre 0 y 15
      bool esNumero = true;
      // Recorrer cada carácter para confirmar que sea un dígito decimal
      for (unsigned int i = 0; i < entrada.length(); i++) {
        if (!isDigit(entrada.charAt(i))) {
          esNumero = false;
          break;
        }
      }

      // Si todos los caracteres son dígitos, verificar que esté en el rango [0, 15]
      if (esNumero) {
        // Convertir la cadena validada a número entero
        int valorIngresado = entrada.toInt();
        // Verificar que represente un código válido de 4 bits (0 a 15)
        if (valorIngresado >= 0 && valorIngresado <= 15) {
          // Cambiar a modo manual fijo
          modoOperacion = 'M';
          // Actualizar la variable de estado global
          valorActual = (uint8_t)valorIngresado;
          // Aplicar los 4 bits de inmediato en los pines D8..D11
          aplicarBits(valorActual);
          // Confirmar la acción en el Monitor Serial
          Serial.print(F(">>> MODO MANUAL FIJO: "));
          imprimirEstado(valorActual);
          return;
        }
      }

      // Informar error si el comando no coincide con ningún formato válido
      Serial.println(F("Comando no reconocido. Use 'A', 'P', 'S', 'C', 'M', 'T<ms>' o '0'..'15'."));
    }
  }
}
