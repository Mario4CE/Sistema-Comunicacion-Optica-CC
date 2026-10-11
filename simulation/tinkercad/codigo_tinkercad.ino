/**
 * ============================================================================
 * PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
 * CÓDIGO OPTIMIZADO PARA SIMULACIÓN EN TINKERCAD
 * ============================================================================
 * 
 * INSTRUCCIONES PARA TINKERCAD:
 * 1. En TinkerCAD Circuits, haz clic en el botón "Código" (Code).
 * 2. Cambia el desplegable de "Bloques" a "Texto".
 * 3. Borra todo el código por defecto y pega este archivo completo.
 * 4. Haz clic en "Iniciar simulación".
 * 5. Abre el "Monitor en serie" en la esquina inferior derecha.
 * ============================================================================
 */

// Definición de pines para los 4 bits
const uint8_t PIN_B0 = 8;   // Bit 0 (LSB)
const uint8_t PIN_B1 = 9;   // Bit 1
const uint8_t PIN_B2 = 10;  // Bit 2
const uint8_t PIN_B3 = 11;  // Bit 3 (MSB)

// Pin indicador visual de ciclo
const uint8_t PIN_LED_ESTADO = 13;

// Retardo por cada estado en la simulación (milisegundos)
// En TinkerCAD el tiempo real a veces va un poco más lento; 1000 ms es ideal.
const unsigned long INTERVALO_MS = 1000;

uint8_t contador = 0;
unsigned long ultimoCambio = 0;

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
 * ----------------------------------------------------------------------------
 */
void setup() {
  // Configurar pines de la palabra de 4 bits como salidas digitales
  pinMode(PIN_B0, OUTPUT);
  pinMode(PIN_B1, OUTPUT);
  pinMode(PIN_B2, OUTPUT);
  pinMode(PIN_B3, OUTPUT);

  // Configurar el LED integrado en placa (Pin 13) como indicador de pulso
  pinMode(PIN_LED_ESTADO, OUTPUT);

  // Iniciar comunicación serial y mostrar encabezado de tabla
  Serial.begin(9600);
  Serial.println(F("--- SIMULACION TINKERCAD: TRANSMISOR 4 BITS ---"));
  Serial.println(F("Decimal | [b3 b2 b1 b0] | V_DAC Teórico"));
  Serial.println(F("---------------------------------------------"));
}

/**
 * ----------------------------------------------------------------------------
 * Función: loop
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Ejecuta cíclicamente un barrido ascendente de 4 bits (de 0 a 15) cada
 *   INTERVALO_MS milisegundos sin bloquear el procesador, actualizando las
 *   salidas físicas hacia el DAC R-2R y mostrando el voltaje teórico en el
 *   Monitor en Serie de TinkerCAD.
 *
 * Entrada:
 *   Ninguna (void). Utiliza las variables globales 'contador' y 'ultimoCambio'.
 *
 * Salida:
 *   Ninguna (void). Modifica el estado de los pines D8-D11, D13 y escribe en Serial.
 *
 * Restricciones:
 *   - El valor de 'contador' está restringido al rango entero [0, 15] mediante
 *     aritmética modular (% 16).
 *   - Se requiere un voltaje de alimentación de 5.0 V en el Arduino simulado.
 * ----------------------------------------------------------------------------
 */
void loop() {
  // Obtener tiempo transcurrido desde el inicio de la simulación
  unsigned long ahora = millis();

  // Comprobar si se ha cumplido el tiempo de permanencia del estado actual
  if (ahora - ultimoCambio >= INTERVALO_MS) {
    ultimoCambio = ahora; // Guardar instante del cambio actual

    // 1. Extraer cada bit mediante máscara binaria y escribirlo en su pin
    digitalWrite(PIN_B0, (contador & 0x01) ? HIGH : LOW); // Bit 0 (LSB, peso 1)
    digitalWrite(PIN_B1, (contador & 0x02) ? HIGH : LOW); // Bit 1 (peso 2)
    digitalWrite(PIN_B2, (contador & 0x04) ? HIGH : LOW); // Bit 2 (peso 4)
    digitalWrite(PIN_B3, (contador & 0x08) ? HIGH : LOW); // Bit 3 (MSB, peso 8)

    // Conmutar el LED del pin 13 para indicar visualmente el cambio de estado
    digitalWrite(PIN_LED_ESTADO, !digitalRead(PIN_LED_ESTADO));

    // 2. Calcular el voltaje analógico teórico esperado: V_DAC = 5V * (contador / 16)
    float vTeorico = (contador * 5.0) / 16.0;

    // 3. Imprimir fila de resultados formateada en el Monitor en Serie
    Serial.print(F("   "));
    if (contador < 10) Serial.print(F(" ")); // Alinear números de un solo dígito
    Serial.print(contador);
    Serial.print(F("   |    "));

    // Imprimir los 4 bits en orden de MSB (b3) a LSB (b0)
    for (int8_t i = 3; i >= 0; i--) {
      Serial.print((contador >> i) & 0x01); // Desplazar y aplicar máscara de 1 bit
      Serial.print(F(" "));
    }
    Serial.print(F("   |   "));
    Serial.print(vTeorico, 3); // Mostrar voltaje con 3 decimales
    Serial.println(F(" V"));

    // Incrementar contador de 0 a 15 y reiniciar a 0 al llegar a 16
    contador = (contador + 1) % 16;
  }
}
