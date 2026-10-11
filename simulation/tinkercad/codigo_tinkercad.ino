/**
 * ============================================================================
 * PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
 * CÓDIGO OPTIMIZADO PARA SIMULACIÓN EN TINKERCAD CIRCUITS
 * AUTOR: Mario Cerdas y equipo de trabajo
 * CUMPLIMIENTO: Requerimientos C.3, D.4, P.17 y Punto 8 (pág. 6) del manual
 * ============================================================================
 * 
 * INSTRUCCIONES PARA TINKERCAD:
 * 1. En TinkerCAD Circuits, haz clic en el botón "Código" (Code).
 * 2. Cambia el desplegable de "Bloques" a "Texto".
 * 3. Borra todo el código por defecto y pega este archivo completo.
 * 4. Haz clic en "Iniciar simulación".
 * 5. Abre el "Monitor en serie" en la esquina inferior derecha (puedes enviar
 *    'A' para escalera, 'P' para señal periódica MATLAB, 'S' para aperiódica
 *    MATLAB, o 'T<ms>' para ajustar el tiempo entre 1 ms y 10000 ms).
 * ============================================================================
 */

// Definición de los cuatro pines digitales de salida hacia el DAC R-2R (Req. C.3)
const uint8_t PIN_B0 = 8;   // Pin digital D8 asignado al Bit 0 (LSB, peso 1)
const uint8_t PIN_B1 = 9;   // Pin digital D9 asignado al Bit 1 (peso 2)
const uint8_t PIN_B2 = 10;  // Pin digital D10 asignado al Bit 2 (peso 4)
const uint8_t PIN_B3 = 11;  // Pin digital D11 asignado al Bit 3 (MSB, peso 8)

// Pin indicador visual de actividad en la placa Arduino Uno
const uint8_t PIN_LED_ESTADO = 13; // LED integrado en placa (Pin 13)

// Límites oficiales de tiempo de permanencia por dato según Requerimiento C.3
const unsigned long TIEMPO_MIN_MS = 1UL;     // Mínimo permitido: 1 ms
const unsigned long TIEMPO_MAX_MS = 10000UL; // Máximo permitido: 10 s (10000 ms)

// Arreglo 1: Escalera de 16 estados (0 a 15) para caracterización del DAC
const uint8_t PATRON_ESCALERA[16] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};
const size_t LONGITUD_ESCALERA = sizeof(PATRON_ESCALERA) / sizeof(PATRON_ESCALERA[0]); // 16 datos

// Arreglo 2: Señal PERIÓDICA generada en MATLAB (Longitud variable N = 20, Req. D.4)
const uint8_t PATRON_PERIODICO_MATLAB[20] = {
   7, 10, 12, 13, 14, 14, 13, 12, 10,  8,
   7,  5,  3,  2,  1,  1,  2,  3,  5,  7
};
const size_t LONGITUD_PERIODICO = sizeof(PATRON_PERIODICO_MATLAB) / sizeof(PATRON_PERIODICO_MATLAB[0]); // 20 datos

// Arreglo 3: Señal APERIÓDICA generada en MATLAB (Longitud variable N = 18, Req. D.4)
const uint8_t PATRON_APERIODICO_MATLAB[18] = {
   5, 11, 14, 12,  6,  2,  7, 10,  9,  6,
   2,  4,  6,  6,  4,  2,  2,  3
};
const size_t LONGITUD_APERIODICO = sizeof(PATRON_APERIODICO_MATLAB) / sizeof(PATRON_APERIODICO_MATLAB[0]); // 18 datos

// Variables globales de recorrido de arreglos de longitud variable y temporización
unsigned long intervaloMs = 1000UL;                  // Tiempo por dato en ms (dentro de [1, 10000] ms)
unsigned long ultimoCambio = 0UL;                    // Marca de tiempo del último cambio de estado
const uint8_t* arregloActivo = PATRON_ESCALERA;      // Puntero al arreglo activo actual
size_t longitudActiva = LONGITUD_ESCALERA;           // Longitud N del arreglo activo
size_t indice = 0;                                   // Posición actual en el arreglo [0 .. N-1]

/**
 * ----------------------------------------------------------------------------
 * Función: setup
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Configura los pines digitales de salida del Arduino en TinkerCAD (D8-D11
 *   para el DAC R-2R y D13 para el LED de estado) e inicializa el Monitor en
 *   Serie a 9600 baudios imprimiendo el encabezado de la tabla de mediciones.
 *
 * Entrada:
 *   Ninguna (void).
 *
 * Salida:
 *   Ninguna (void). Inicializa los periféricos de entrada/salida y Serial.
 *
 * Restricciones:
 *   - Los pines D8 a D11 deben conectarse en orden de LSB (D8) a MSB (D11)
 *     hacia las resistencias 2R de la red en escalera.
 *   - Las etapas analógicas no deben alimentarse del pin 5V del Arduino (Req. P.17).
 * ----------------------------------------------------------------------------
 */
void setup() {
  // Configurar pin D8 (Bit 0 - LSB) como salida digital
  pinMode(PIN_B0, OUTPUT);
  // Configurar pin D9 (Bit 1) como salida digital
  pinMode(PIN_B1, OUTPUT);
  // Configurar pin D10 (Bit 2) como salida digital
  pinMode(PIN_B2, OUTPUT);
  // Configurar pin D11 (Bit 3 - MSB) como salida digital
  pinMode(PIN_B3, OUTPUT);

  // Configurar el LED integrado en placa (Pin 13) como indicador de pulso
  pinMode(PIN_LED_ESTADO, OUTPUT);

  // Iniciar comunicación serial a 9600 bps y mostrar encabezado de tabla
  Serial.begin(9600);
  Serial.println(F("--- SIMULACION TINKERCAD: TRANSMISOR 4 BITS (Req. C.3 y D.4) ---"));
  Serial.println(F("Comandos: 'A'=Escalera(16), 'P'=Periódica MATLAB(20), 'S'=Aperiódica MATLAB(18), 'T<ms>'"));
  Serial.println(F("Índice | Decimal | [b3 b2 b1 b0] | V_DAC Teórico"));
  Serial.println(F("----------------------------------------------------------------"));
}

/**
 * ----------------------------------------------------------------------------
 * Función: loop
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Atiende comandos seriales opcionales en TinkerCAD y recorre cíclicamente el
 *   arreglo de longitud variable activo cada 'intervaloMs' milisegundos sin
 *   bloquear el procesador, actualizando las salidas físicas hacia el DAC R-2R.
 *
 * Entrada:
 *   Ninguna (void). Utiliza el puntero 'arregloActivo', 'longitudActiva' e 'indice'.
 *
 * Salida:
 *   Ninguna (void). Modifica el estado de los pines D8-D11, D13 y escribe en Serial.
 *
 * Restricciones:
 *   - 'intervaloMs' debe mantenerse dentro del rango oficial [1, 10000] ms (Req. C.3).
 *   - Cada dato del arreglo se enmascara a 4 bits (0 a 15).
 * ----------------------------------------------------------------------------
 */
void loop() {
  // Verificar si el usuario ingresó un comando en el Monitor en Serie de TinkerCAD
  if (Serial.available() > 0) {
    // Leer comando hasta el salto de línea y eliminar espacios
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    // Si se envió 'A', seleccionar el arreglo de escalera de 16 niveles
    if (cmd.equalsIgnoreCase("A")) {
      arregloActivo = PATRON_ESCALERA;
      longitudActiva = LONGITUD_ESCALERA;
      indice = 0;
      Serial.println(F(">>> Arreglo activo: ESCALERA (N = 16) <<<"));
    }
    // Si se envió 'P', seleccionar el arreglo periódico generado en MATLAB (N = 20)
    else if (cmd.equalsIgnoreCase("P")) {
      arregloActivo = PATRON_PERIODICO_MATLAB;
      longitudActiva = LONGITUD_PERIODICO;
      indice = 0;
      Serial.println(F(">>> Arreglo activo: SEÑAL PERIÓDICA MATLAB (N = 20) <<<"));
    }
    // Si se envió 'S', seleccionar el arreglo aperiódico generado en MATLAB (N = 18)
    else if (cmd.equalsIgnoreCase("S")) {
      arregloActivo = PATRON_APERIODICO_MATLAB;
      longitudActiva = LONGITUD_APERIODICO;
      indice = 0;
      Serial.println(F(">>> Arreglo activo: SEÑAL APERIÓDICA MATLAB (N = 18) <<<"));
    }
    // Si se envió 'T<ms>', validar y actualizar el tiempo de permanencia en [1, 10000] ms
    else if ((cmd.charAt(0) == 'T' || cmd.charAt(0) == 't') && cmd.length() > 1) {
      unsigned long nuevoT = (unsigned long)cmd.substring(1).toInt();
      if (nuevoT >= TIEMPO_MIN_MS && nuevoT <= TIEMPO_MAX_MS) {
        intervaloMs = nuevoT;
        Serial.print(F(">>> Tiempo por dato actualizado a: "));
        Serial.print(intervaloMs);
        Serial.println(F(" ms <<<"));
      }
    }
  }

  // Obtener tiempo transcurrido desde el inicio de la simulación en milisegundos
  unsigned long ahora = millis();

  // Comprobar si se ha cumplido el tiempo de permanencia del estado actual
  if (ahora - ultimoCambio >= intervaloMs) {
    // Guardar instante del cambio actual
    ultimoCambio = ahora;

    // Extraer el dato de 4 bits correspondiente al índice actual del arreglo activo
    uint8_t dato = arregloActivo[indice] & 0x0F;

    // 1. Extraer cada bit mediante máscara binaria y escribirlo en su pin de salida
    digitalWrite(PIN_B0, (dato & 0x01) ? HIGH : LOW); // Bit 0 (LSB, peso 1) -> D8
    digitalWrite(PIN_B1, (dato & 0x02) ? HIGH : LOW); // Bit 1 (peso 2)      -> D9
    digitalWrite(PIN_B2, (dato & 0x04) ? HIGH : LOW); // Bit 2 (peso 4)      -> D10
    digitalWrite(PIN_B3, (dato & 0x08) ? HIGH : LOW); // Bit 3 (MSB, peso 8) -> D11

    // Conmutar el LED del pin 13 para indicar visualmente el cambio de muestra
    digitalWrite(PIN_LED_ESTADO, !digitalRead(PIN_LED_ESTADO));

    // 2. Calcular el voltaje analógico teórico esperado: V_DAC = 5.0 V * (dato / 16.0)
    float vTeorico = (dato * 5.0) / 16.0;

    // 3. Imprimir fila de resultados formateada en el Monitor en Serie
    Serial.print(F("  "));
    if (indice < 10) Serial.print(F(" ")); // Alinear índice de un dígito
    Serial.print(indice);
    Serial.print(F("   |   "));
    if (dato < 10) Serial.print(F(" "));   // Alinear dato decimal de un dígito
    Serial.print(dato);
    Serial.print(F("    |    "));

    // Imprimir los 4 bits en orden de MSB (b3) a LSB (b0)
    for (int8_t i = 3; i >= 0; i--) {
      Serial.print((dato >> i) & 0x01); // Desplazar 'i' posiciones y aplicar máscara
      Serial.print(F(" "));
    }
    Serial.print(F("   |   "));
    Serial.print(vTeorico, 3); // Mostrar voltaje con 3 decimales
    Serial.println(F(" V"));

    // Avanzar al siguiente elemento del arreglo de longitud variable y reiniciar al llegar a N
    indice = (indice + 1) % longitudActiva;
  }
}
