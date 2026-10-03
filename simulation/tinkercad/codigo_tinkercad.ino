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

void setup() {
  pinMode(PIN_B0, OUTPUT);
  pinMode(PIN_B1, OUTPUT);
  pinMode(PIN_B2, OUTPUT);
  pinMode(PIN_B3, OUTPUT);
  pinMode(PIN_LED_ESTADO, OUTPUT);

  Serial.begin(9600);
  Serial.println(F("--- SIMULACION TINKERCAD: TRANSMISOR 4 BITS ---"));
  Serial.println(F("Decimal | [b3 b2 b1 b0] | V_DAC Teórico"));
  Serial.println(F("---------------------------------------------"));
}

void loop() {
  unsigned long ahora = millis();

  if (ahora - ultimoCambio >= INTERVALO_MS) {
    ultimoCambio = ahora;

    // 1. Aplicar bits a las salidas del Arduino
    digitalWrite(PIN_B0, (contador & 0x01) ? HIGH : LOW);
    digitalWrite(PIN_B1, (contador & 0x02) ? HIGH : LOW);
    digitalWrite(PIN_B2, (contador & 0x04) ? HIGH : LOW);
    digitalWrite(PIN_B3, (contador & 0x08) ? HIGH : LOW);

    // Parpadeo de estado en pin 13
    digitalWrite(PIN_LED_ESTADO, !digitalRead(PIN_LED_ESTADO));

    // 2. Cálculo del voltaje esperado (5V * D / 16)
    float vTeorico = (contador * 5.0) / 16.0;

    // 3. Imprimir en el Monitor Serie de TinkerCAD
    Serial.print(F("   "));
    if (contador < 10) Serial.print(F(" "));
    Serial.print(contador);
    Serial.print(F("   |    "));

    for (int8_t i = 3; i >= 0; i--) {
      Serial.print((contador >> i) & 0x01);
      Serial.print(F(" "));
    }
    Serial.print(F("   |   "));
    Serial.print(vTeorico, 3);
    Serial.println(F(" V"));

    // Incrementar de 0 a 15 y reiniciar cíclicamente
    contador = (contador + 1) % 16;
  }
}
