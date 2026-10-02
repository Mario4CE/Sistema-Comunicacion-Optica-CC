/**
 * ============================================================================
 * PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
 * ETAPA 1: Patrón de datos de prueba digital (4 bits)
 * AUTOR: Mario Cerdas y equipo de trabajo
 * MICROCONTROLADOR: Arduino UNO / Arduino Nano
 * ============================================================================
 * 
 * DESCRIPCIÓN:
 * Este programa genera un patrón de datos binarios de 4 bits (b0 a b3)
 * para ser enviados a un conversor digital-analógico (DAC) R-2R.
 * 
 * MAPEO DE PINES HACIA EL DAC R-2R:
 *   - Pin D8  -> Bit 0 (LSB - Bit Menos Significativo)
 *   - Pin D9  -> Bit 1
 *   - Pin D10 -> Bit 2
 *   - Pin D11 -> Bit 3 (MSB - Bit Más Significativo)
 * 
 * MODOS DE OPERACIÓN:
 *   - 'A' o 'a': Modo Automático (recorre cíclicamente de 0 a 15).
 *   - 'M' o 'm': Modo Manual (permite ingresar valores 0-15 por Serial).
 *   - 'C' o 'c': Modo Calibración (alterna entre 0 y 15 para calibrar el enlace óptico).
 * ============================================================================
 */

// Definición de pines para los 4 bits
const uint8_t PIN_B0_LSB = 8;
const uint8_t PIN_B1     = 9;
const uint8_t PIN_B2     = 10;
const uint8_t PIN_B3_MSB = 11;

// Tiempo de permanencia por estado en modo automático (milisegundos)
// Nota: La LDR tiene un tiempo de respuesta de decenas a centenas de ms.
// Se recomienda un mínimo de 1000 ms a 1500 ms para pruebas estables de audio.
const unsigned long INTERVALO_CAMBIO_MS = 1500;

// Variables de estado
uint8_t valorActual = 0;
char modoOperacion = 'A'; // 'A' = Automático, 'M' = Manual, 'C' = Calibración
unsigned long tiempoPrevio = 0;
bool estadoCalibracion = false;

// Prototipos de funciones
void aplicarBits(uint8_t valor);
void imprimirEstado(uint8_t valor);
void procesarComandosSeriales();

void setup() {
  // Configuración de pines digitales como salida
  pinMode(PIN_B0_LSB, OUTPUT);
  pinMode(PIN_B1,     OUTPUT);
  pinMode(PIN_B2,     OUTPUT);
  pinMode(PIN_B3_MSB, OUTPUT);

  // Inicializar todas las salidas en nivel bajo (0000)
  aplicarBits(0);

  // Inicializar comunicación serial para monitoreo y depuración
  Serial.begin(9600);
  while (!Serial) {
    ; // Esperar conexión serial si es necesario
  }

  Serial.println(F("======================================================"));
  Serial.println(F("  SISTEMA DE COMUNICACIÓN POR MEDIOS ÓPTICOS"));
  Serial.println(F("  Etapa 1: Transmisor Digital de 4 Bits (Arduino)"));
  Serial.println(F("======================================================"));
  Serial.println(F("Comandos disponibles en el Monitor Serial:"));
  Serial.println(F("  'A' -> Iniciar modo automático (cuenta 0 a 15)"));
  Serial.println(F("  'C' -> Modo calibración (alterna 0 y 15)"));
  Serial.println(F("  '0'..'15' -> Enviar número manual específico"));
  Serial.println(F("======================================================\n"));

  tiempoPrevio = millis();
  imprimirEstado(valorActual);
}

void loop() {
  // 1. Revisar si hay comandos enviados por el usuario desde la computadora
  procesarComandosSeriales();

  unsigned long tiempoActual = millis();

  // 2. Ejecutar según el modo seleccionado
  if (modoOperacion == 'A') {
    // Modo automático por temporizador sin bloquear el procesador (no-blocking delay)
    if (tiempoActual - tiempoPrevio >= INTERVALO_CAMBIO_MS) {
      tiempoPrevio = tiempoActual;
      valorActual = (valorActual + 1) % 16; // Incremento de 0 a 15 cíclico
      aplicarBits(valorActual);
      imprimirEstado(valorActual);
    }
  } 
  else if (modoOperacion == 'C') {
    // Modo calibración: alterna cada 2 segundos entre nivel mínimo y máximo
    if (tiempoActual - tiempoPrevio >= 2000) {
      tiempoPrevio = tiempoActual;
      estadoCalibracion = !estadoCalibracion;
      valorActual = estadoCalibracion ? 15 : 0;
      aplicarBits(valorActual);
      imprimirEstado(valorActual);
    }
  }
}

/**
 * Escribe los 4 bits del número en los pines físicos correspondientes.
 * @param valor Número entero entre 0 y 15 (4 bits)
 */
void aplicarBits(uint8_t valor) {
  // Asegurar que solo usamos los primeros 4 bits
  uint8_t dato4Bits = valor & 0x0F;

  // Extracción bit a bit y asignación directa a pines
  digitalWrite(PIN_B0_LSB, (dato4Bits & 0x01) ? HIGH : LOW);
  digitalWrite(PIN_B1,     (dato4Bits & 0x02) ? HIGH : LOW);
  digitalWrite(PIN_B2,     (dato4Bits & 0x04) ? HIGH : LOW);
  digitalWrite(PIN_B3_MSB, (dato4Bits & 0x08) ? HIGH : LOW);
}

/**
 * Imprime por serial el estado binario, decimal y voltaje teórico esperado en el DAC.
 */
void imprimirEstado(uint8_t valor) {
  uint8_t dato = valor & 0x0F;
  float voltajeTeorico = (dato * 5.0) / 16.0;

  Serial.print(F("[TX] Decimal: "));
  if (dato < 10) Serial.print(F(" ")); // Alineación visual
  Serial.print(dato);

  Serial.print(F(" | Binario [b3 b2 b1 b0]: "));
  for (int8_t i = 3; i >= 0; i--) {
    Serial.print((dato >> i) & 0x01);
  }

  Serial.print(F(" | V_DAC Teórico: "));
  Serial.print(voltajeTeorico, 3);
  Serial.println(F(" V"));
}

/**
 * Lee la entrada serial para cambiar de modo o fijar valores fijos.
 */
void procesarComandosSeriales() {
  if (Serial.available() > 0) {
    String entrada = Serial.readStringUntil('\n');
    entrada.trim();

    if (entrada.length() == 0) return;

    if (entrada.equalsIgnoreCase("A")) {
      modoOperacion = 'A';
      Serial.println(F(">>> Activado MODO AUTOMÁTICO (0 a 15) <<<"));
    } 
    else if (entrada.equalsIgnoreCase("C")) {
      modoOperacion = 'C';
      Serial.println(F(">>> Activado MODO CALIBRACIÓN (Alterna 0 y 15) <<<"));
    } 
    else {
      // Intentar convertir a número entero para modo manual
      int valorIngresado = entrada.toInt();
      if (valorIngresado >= 0 && valorIngresado <= 15) {
        modoOperacion = 'M';
        valorActual = (uint8_t)valorIngresado;
        aplicarBits(valorActual);
        Serial.print(F(">>> MODO MANUAL FIJO: "));
        imprimirEstado(valorActual);
      } else {
        Serial.println(F("Comando no reconocido. Ingrese 'A', 'C' o un número del 0 al 15."));
      }
    }
  }
}
