# Guía de Conexión y Montaje en Protoboard
**Proyecto:** Sistema de Comunicación por Medios Ópticos (EL2113)  

---

## 1. Diagrama de Distribución General

```
[ Arduino ] ──► [ Red R-2R ] ──► [ Buffer LM358 ] ──► [ LED ] ════ (Fibra) ════► [ LDR ] ──► [ 555 Astable ] ──► [ Buzzer ]
```

---

## 2. Conexiones Detalladas Etapa por Etapa

### Etapa 1 a 2: Pines de Arduino hacia la Red R–2R
* **Pin D8** (Arduino) $\rightarrow$ Resistencia $2R$ ($20\text{ k}\Omega$) del **Bit 0 (LSB)**.
* **Pin D9** (Arduino) $\rightarrow$ Resistencia $2R$ ($20\text{ k}\Omega$) del **Bit 1**.
* **Pin D10** (Arduino) $\rightarrow$ Resistencia $2R$ ($20\text{ k}\Omega$) del **Bit 2**.
* **Pin D11** (Arduino) $\rightarrow$ Resistencia $2R$ ($20\text{ k}\Omega$) del **Bit 3 (MSB)**.
* **GND** (Arduino) $\rightarrow$ Riel común de tierra en el protoboard.
* **Resistencia de terminación $2R$ ($20\text{ k}\Omega$):** Conectada entre el nodo inferior de Bit 0 y GND.
* **Resistencias intermedias $R$ ($10\text{ k}\Omega$):** Conectadas en serie entre los nodos de cada bit.

---

### Etapa 3: Buffer Seguidor con Amplificador Operacional LM358
* **Pin 8 (VCC):** A $+5\text{ V}$ (desde el pin de $5\text{ V}$ del Arduino o fuente externa regulada).
* **Pin 4 (GND):** A tierra común (GND).
* **Pin 3 (Entrada no inversora IN+):** Conectar a la salida analógica de la red R–2R ($V_{out\_DAC}$, nodo del Bit 3).
* **Pin 2 (Entrada inversora IN-):** Hacer un puente directo con un cable corto hacia el **Pin 1 (Salida OUT)** (retroalimentación negativa unitaria $A_v = 1$).
* **Pin 1 (Salida del buffer):** Entrega el voltaje analógico sin sufrir caída de tensión por carga.

---

### Etapa 4: Driver de Transmisión Óptica (LED)
* **Pin 1 del LM358:** Conectar a un extremo de la resistencia limitadora de **$150\ \Omega$**.
* El otro extremo de la resistencia de **$150\ \Omega$** $\rightarrow$ Ánodo (pata larga) del **LED Rojo**.
* Cátodo (pata corta) del LED $\rightarrow$ Tierra común (GND).

---

### Etapa 5: Acoplamiento Óptico con Fibra Plástica
1. Cortar un tramo limpio y perpendicular de fibra óptica plástica de entre **$30\text{ cm}$ y $60\text{ cm}$**.
2. Enfrentar un extremo de la fibra directamente al domo del LED emisor.
3. Enfrentar el otro extremo de la fibra directamente a la superficie fotosensible de la LDR.
4. **Muy importante:** Envolver ambas uniones con termoencogible negro o cinta aislante para que la luz ambiental no sature la fotorresistencia.

---

### Etapa 6: Generador Astable con Temporizador NE555
* **Pin 1 (GND):** A tierra común (GND).
* **Pin 8 (VCC) y Pin 4 (RESET):** Ambos conectados al riel de $+5\text{ V}$.
* **Pin 5 (CONTROL):** Conectar condensador cerámico de $10\text{ nF}$ a tierra (filtro de ruido).
* **Resistencia $R_1$ ($1\text{ k}\Omega$):** Entre Pin 8 ($+5\text{ V}$) y Pin 7 (DISCHARGE).
* **Receptor LDR:** Un terminal al Pin 7 (DISCHARGE) y el otro terminal a los Pines 2 y 6.
* **Puente entre Pin 2 (TRIGGER) y Pin 6 (THRESHOLD):** Unir ambos pines con un cable jumper.
* **Condensador $C_1$ ($100\text{ nF}$):** Conectar entre el nodo común (Pines 2 y 6) y tierra (GND).

---

### Etapa 7: Salida de Audio (Buzzer Pasivo)
* **Pin 3 (OUTPUT del 555):** Conectar al polo positivo del condensador electrolítico de **$10\ \mu\text{F}$**.
* El polo negativo del condensador de $10\ \mu\text{F}$ $\rightarrow$ Terminal positivo del **Buzzer piezoeléctrico pasivo**.
* Terminal negativo del buzzer $\rightarrow$ Tierra común (GND).

---

## 3. Puntos de Prueba Recomendados con Multímetro

| Punto de Prueba | Ubicación | Valor Esperado |
|:---|:---|:---|
| **TP1 (V_DAC)** | Nodo de salida de la red R–2R | Escalones de $0.0\text{ V}$ a $4.68\text{ V}$ en saltos de $\approx 0.31\text{ V}$ |
| **TP2 (V_BUF)** | Pin 1 del LM358 | Idéntico a TP1 (error $< 1\%$) |
| **TP3 (I_LED)** | En serie con el LED | $0\text{ mA}$ hasta $\approx 18\text{ mA}$ |
| **TP4 (R_LDR)** | Terminales de la LDR (sin alimentar 555) | De $\approx 40\text{ k}\Omega$ (oscuridad) a $\approx 1.5\text{ k}\Omega$ (máx. luz) |
| **TP5 (OUT_555)**| Pin 3 del 555 con osciloscopio o frecuencímetro | Onda cuadrada entre $\approx 180\text{ Hz}$ y $\approx 3.5\text{ kHz}$ |
