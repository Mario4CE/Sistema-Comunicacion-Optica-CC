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

/**
 * ----------------------------------------------------------------------------
 * Función: setup
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Inicializa los pines digitales de salida hacia el DAC R-2R, establece el
 *   estado inicial en cero (0000), configura el puerto serial a 9600 baudios
 *   y despliega el menú de instrucciones en el Monitor Serial.
 *
 * Entrada:
 *   Ninguna (void).
 *
 * Salida:
 *   Ninguna (void). Configura el estado inicial del hardware y periféricos.
 *
 * Restricciones:
 *   - Los pines D8, D9, D10 y D11 deben estar libres y dedicados al DAC R-2R.
 *   - El Monitor Serial debe estar configurado exactamente a 9600 baudios.
 * ----------------------------------------------------------------------------
 */
void setup() {
  // Configuración de los 4 pines digitales como salidas de voltaje (0V / 5V)
  pinMode(PIN_B0_LSB, OUTPUT);
  pinMode(PIN_B1,     OUTPUT);
  pinMode(PIN_B2,     OUTPUT);
  pinMode(PIN_B3_MSB, OUTPUT);

  // Inicializar todas las salidas en nivel lógico bajo (0000 -> 0.000 V)
  aplicarBits(0);

  // Inicializar comunicación serial a 9600 bps para monitoreo y control
  Serial.begin(9600);
  while (!Serial) {
    ; // Esperar a que el puerto serial se conecte (necesario en placas con USB nativo)
  }

  // Mostrar encabezado e instrucciones de uso en consola
  Serial.println(F("======================================================"));
  Serial.println(F("  SISTEMA DE COMUNICACIÓN POR MEDIOS ÓPTICOS"));
  Serial.println(F("  Etapa 1: Transmisor Digital de 4 Bits (Arduino)"));
  Serial.println(F("======================================================"));
  Serial.println(F("Comandos disponibles en el Monitor Serial:"));
  Serial.println(F("  'A' -> Iniciar modo automático (cuenta 0 a 15)"));
  Serial.println(F("  'C' -> Modo calibración (alterna 0 y 15)"));
  Serial.println(F("  'M' -> Activar modo manual en espera"));
  Serial.println(F("  '0'..'15' -> Enviar número manual específico"));
  Serial.println(F("======================================================\n"));

  // Registrar marca de tiempo inicial e imprimir el estado de arranque
  tiempoPrevio = millis();
  imprimirEstado(valorActual);
}

/**
 * ----------------------------------------------------------------------------
 * Función: loop
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Ciclo principal de ejecución continua. Atiende comandos entrantes por el
 *   puerto serial y actualiza el patrón de bits de salida según el modo de
 *   operación activo ('A' Automático, 'C' Calibración o 'M' Manual) usando
 *   temporización no bloqueante con millis().
 *
 * Entrada:
 *   Ninguna (void). Utiliza las variables globales de estado y temporización.
 *
 * Salida:
 *   Ninguna (void). Actualiza periódicamente las salidas físicas y el Serial.
 *
 * Restricciones:
 *   - No debe utilizarse delay() bloqueante para no perder comandos seriales.
 *   - El intervalo de cambio debe ser mayor al tiempo de respuesta de la LDR.
 * ----------------------------------------------------------------------------
 */
void loop() {
  // 1. Verificar y procesar si el usuario envió algún comando por puerto serial
  procesarComandosSeriales();

  // Capturar el tiempo actual del sistema en milisegundos
  unsigned long tiempoActual = millis();

  // 2. Ejecutar la máquina de estados según el modo de operación activo
  if (modoOperacion == 'A') {
    // Modo automático: avanza al siguiente estado cuando se cumple INTERVALO_CAMBIO_MS
    if (tiempoActual - tiempoPrevio >= INTERVALO_CAMBIO_MS) {
      tiempoPrevio = tiempoActual;          // Actualizar referencia de tiempo
      valorActual = (valorActual + 1) % 16; // Incremento cíclico de 0 a 15 (módulo 16)
      aplicarBits(valorActual);             // Actualizar pines físicos D8..D11
      imprimirEstado(valorActual);          // Reportar estado en el Monitor Serial
    }
  } 
  else if (modoOperacion == 'C') {
    // Modo calibración: alterna cada 2000 ms entre oscuridad (0) e iluminación máxima (15)
    if (tiempoActual - tiempoPrevio >= 2000) {
      tiempoPrevio = tiempoActual;              // Actualizar referencia de tiempo
      estadoCalibracion = !estadoCalibracion;   // Invertir bandera booleana de extremo
      valorActual = estadoCalibracion ? 15 : 0; // Seleccionar 15 (1111) o 0 (0000)
      aplicarBits(valorActual);                 // Actualizar pines físicos D8..D11
      imprimirEstado(valorActual);              // Reportar estado en el Monitor Serial
    }
  }
  // Nota: En modo 'M' (Manual), el valor se mantiene fijo hasta recibir otro comando serial
}

/**
 * ----------------------------------------------------------------------------
 * Función: aplicarBits
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Descompone un número entero en sus 4 bits menos significativos (b0 a b3)
 *   mediante máscaras binarias y escribe los niveles lógicos (HIGH = 5V, LOW = 0V)
 *   en los pines digitales conectados a la escalera R-2R.
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
  // Aplicar máscara 0x0F (00001111 en binario) para aislar los primeros 4 bits
  uint8_t dato4Bits = valor & 0x0F;

  // Extraer cada bit individual con AND bit a bit y escribir en el pin correspondiente
  digitalWrite(PIN_B0_LSB, (dato4Bits & 0x01) ? HIGH : LOW); // Bit 0 (peso 1) -> Pin D8
  digitalWrite(PIN_B1,     (dato4Bits & 0x02) ? HIGH : LOW); // Bit 1 (peso 2) -> Pin D9
  digitalWrite(PIN_B2,     (dato4Bits & 0x04) ? HIGH : LOW); // Bit 2 (peso 4) -> Pin D10
  digitalWrite(PIN_B3_MSB, (dato4Bits & 0x08) ? HIGH : LOW); // Bit 3 (peso 8) -> Pin D11
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
  // Asegurar que el cálculo se realice sobre un valor de 4 bits (0 a 15)
  uint8_t dato = valor & 0x0F;

  // Ecuación del DAC R-2R de 4 bits: V_out = V_ref * (D / 2^4) = 5.0 * (dato / 16.0)
  float voltajeTeorico = (dato * 5.0) / 16.0;

  // Imprimir valor en base decimal con alineación a dos dígitos
  Serial.print(F("[TX] Decimal: "));
  if (dato < 10) Serial.print(F(" ")); // Espacio adicional para alinear números de 1 dígito
  Serial.print(dato);

  // Recorrer desde el bit más significativo (b3) hasta el menos significativo (b0)
  Serial.print(F(" | Binario [b3 b2 b1 b0]: "));
  for (int8_t i = 3; i >= 0; i--) {
    Serial.print((dato >> i) & 0x01); // Desplazar i posiciones a la derecha y extraer bit
  }

  // Imprimir voltaje analógico teórico con 3 decimales de precisión
  Serial.print(F(" | V_DAC Teórico: "));
  Serial.print(voltajeTeorico, 3);
  Serial.println(F(" V"));
}

/**
 * ----------------------------------------------------------------------------
 * Función: procesarComandosSeriales
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Verifica si existen datos disponibles en el búfer de recepción serial, lee
 *   la cadena enviada hasta encontrar un salto de línea ('\n') e interpreta el
 *   comando para cambiar de modo ('A', 'C', 'M') o fijar un estado numérico
 *   manual entre 0 y 15.
 *
 * Entrada:
 *   Ninguna (void). Lee los caracteres directamente del búfer Serial.
 *
 * Salida:
 *   Ninguna (void). Modifica las variables globales 'modoOperacion' y 'valorActual',
 *   actualiza los pines de salida y notifica al usuario por el Monitor Serial.
 *
 * Restricciones:
 *   - La entrada por Monitor Serial debe finalizar con salto de línea ('\n').
 *   - Solo se aceptan las letras 'A', 'C', 'M' (sin importar mayúsculas/minúsculas)
 *     o cadenas compuestas exclusivamente por dígitos cuyo valor esté entre 0 y 15.
 * ----------------------------------------------------------------------------
 */
void procesarComandosSeriales() {
  // Comprobar si llegaron bytes al búfer serial
  if (Serial.available() > 0) {
    // Leer la línea completa hasta el carácter de nueva línea y limpiar espacios
    String entrada = Serial.readStringUntil('\n');
    entrada.trim();

    // Si la cadena quedó vacía (por ejemplo, solo se presionó Enter), ignorar
    if (entrada.length() == 0) return;

    // Evaluar comandos de selección de modo
    if (entrada.equalsIgnoreCase("A")) {
      modoOperacion = 'A';
      Serial.println(F(">>> Activado MODO AUTOMÁTICO (0 a 15) <<<"));
    } 
    else if (entrada.equalsIgnoreCase("C")) {
      modoOperacion = 'C';
      Serial.println(F(">>> Activado MODO CALIBRACIÓN (Alterna 0 y 15) <<<"));
    } 
    else if (entrada.equalsIgnoreCase("M")) {
      modoOperacion = 'M';
      Serial.println(F(">>> Activado MODO MANUAL. Ingrese un número del 0 al 15 <<<"));
    }
    else {
      // Validar carácter por carácter que la entrada sea estrictamente numérica
      // (evita que String.toInt() convierta letras inválidas en el número 0)
      bool esNumero = true;
      for (unsigned int i = 0; i < entrada.length(); i++) {
        if (!isDigit(entrada.charAt(i))) {
          esNumero = false;
          break;
        }
      }

      // Si es un número válido, verificar que esté dentro del rango de 4 bits [0, 15]
      if (esNumero) {
        int valorIngresado = entrada.toInt();
        if (valorIngresado >= 0 && valorIngresado <= 15) {
          modoOperacion = 'M';                  // Cambiar automáticamente a modo manual fijo
          valorActual = (uint8_t)valorIngresado; // Guardar nuevo estado
          aplicarBits(valorActual);             // Escribir bits en pines D8..D11
          Serial.print(F(">>> MODO MANUAL FIJO: "));
          imprimirEstado(valorActual);          // Confirmar nuevo estado por consola
          return;
        }
      }

      // Informar error cuando el comando no cumple las restricciones
      Serial.println(F("Comando no reconocido. Ingrese 'A', 'C', 'M' o un número del 0 al 15."));
    }
  }
}
