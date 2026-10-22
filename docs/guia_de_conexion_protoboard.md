# Guía de Conexión y Montaje Físico en Protoboard (Reglas de Laboratorio)
**Proyecto:** Sistema de Comunicación por Medios Ópticos (EL2113)  
**Curso:** Circuitos Eléctricos en Corriente Continua — Escuela de Ingeniería Electrónica (ITCR)  
**Autor:** Mario Cerdas y equipo de trabajo  
**Cumplimiento Estricto:** Requerimientos A.1–Q.18 (incluyendo P.17, Q.18 y M.13 de uso de laboratorios)

---

## 1. Diagrama de Distribución General de las 7 Etapas Delimitadas (Req. A.1 y B.2)

```
                     [ FUENTE DC DE LABORATORIO (Bodega Escuela de Electrónica) ]
                                  │ (+VCC = +5.0V y GND común; PROHIBIDO 5V Arduino)
                                  ▼
[ Etapa 1: Arduino ] ──► [ Etapa 2: R-2R + OpAmp 1A ] ──► [ Etapa 3: Interfaz OpAmp 1B ] ──► [ Etapa 4: R_drv + LED ]
 (Pines D8..D11)          (Escalera pasiva + LM358)        (Aislamiento de carga)             (R=150 Ohm + LED Rojo)
                                                                                                       ║
                                                                                         (Fibra 45 cm + Soportes 3D/Láser)
                                                                                         (Req. I.9, M.13 y H.8)
                                                                                                       ║
                                                                                                       ▼
 [ Etapa 7: Buzzer Pasivo ] ◄── [ Etapa 6: 555 Astable ] ◄────────────────────────────── [ Etapa 5: LDR Pasiva ]
  (Cap 10 uF + Piezo AC)         (R1=1k, C1=100nF, C2=10nF)                               (Resistencia variable en luz)
```

---

## 2. Regla Estricta de Alimentación en el Laboratorio (Req. P.17 y Q.18)

> [!CAUTION]
> **Cumplimiento obligatorio de P.17 y Q.18 para no perder puntos:**
> 1. **PROHIBIDO usar el pin `5V` del Arduino** para alimentar el protoboard. El pin `5V` del Arduino debe quedar **totalmente libre/desconectado**.
> 2. **PROHIBIDO usar baterías AA, AAA o cuadradas (9V).**
> 3. Solicita en la **bodega de la Escuela de Electrónica** una **Fuente de Alimentación DC de Laboratorio** (regulada):
>    * Ajusta el canal principal de la fuente de laboratorio a **$+5.00\text{ V}$** y fija el límite de corriente de seguridad en **$0.25\text{ A}$** ($250\text{ mA}$).
>    * *(Recomendación para rendimiento óptimo):* Si usas una fuente doble de laboratorio, puedes aplicar $+6.5\text{ V}\text{--}9.0\text{ V}$ al Pin 8 del LM358 para evitar la caída de riel superior ($V_{CC} - 1.5\text{ V}$) en el estado 15, y $+5.0\text{ V}$ al Pin 8 del NE555. Si usas un solo canal a $+5.0\text{ V}$, el sistema funciona adecuadamente con un pequeño aplanamiento en los estados 14 y 15.
>    * Conecta el borne **rojo (+)** de la fuente de laboratorio al riel superior e inferior rojo (`+VCC`) del protoboard.
>    * Conecta el borne **negro (- / GND)** de la fuente de laboratorio al riel azul/negro (`GND`) del protoboard.
>    * Conecta un único cable puente desde el pin **`GND` del Arduino UNO** hacia el riel azul/negro (`GND`) del protoboard para que las señales digitales compartan la misma referencia de $0\text{ V}$.

---

## 3. Mapa Visual y Matriz de Coordenadas Exactas en Protoboard Grande (Filas `0` a `60`, Columnas `a`–`j`)

Tu protoboard cuenta con **60 filas numeradas (`0` a `60`)** y **10 columnas de contactos divididas por el canal central (`a, b, c, d, e` a la izquierda/abajo y `f, g, h, i, j` a la derecha/arriba)**, además de los buses laterales de alimentación (`+` rojo y `-` azul).

> [!IMPORTANT]
> **Atención sobre los rieles laterales en protoboards de 60 filas:** En muchas protoboards de 0 a 60, las líneas rojas y azules de alimentación tienen un **corte interno a la mitad (entre la fila 30 y la 31)**. Coloca siempre pequeños puentes en la fila 30–31 uniendo la mitad izquierda con la mitad derecha del riel rojo (`+`) y del riel azul (`-`) para que toda la regleta tenga energía continua.

```
┌────────────────────────────────────────────────────────────────────────────────────────┐
│ (+) Riel Superior Rojo (+VCC de Fuente de Laboratorio = +5.0V)  [Puente en fila 30-31] │
│ (-) Riel Superior Azul (GND común de Fuente de Laboratorio y Arduino)                  │
├────────────────────────────────────────┬───────────────────────────────────────────────┤
│   SECCIÓN TRANSMISORA (FILAS 0 A 30)   │      SECCIÓN RECEPTORA (FILAS 33 A 60)        │
│   Columnas: [a-b-c-d-e] | [f-g-h-i-j]  │      Columnas: [a-b-c-d-e] | [f-g-h-i-j]      │
│                                        │                                               │
│  [Filas 02 a 14]:                      │  [Filas 33 a 35]:                             │
│   Red en Escalera R-2R (Etapa 2A)      │   Fotorresistencia LDR (Etapa 5)              │
│   Nodos: F.04(V1), F.07(V2),           │   2 Jumpers desde Torre 3D RX (2022437519-01) │
│          F.10(V3), F.13(V4)            │                                               │
│                                        │  [Filas 39 a 42]:                             │
│  [Filas 19 a 22]:                      │   Temporizador NE555 Astable (Etapa 6)        │
│   Amplificador Operacional Dual LM358  │   Insertado en cols e (1-4) y f (8-5)         │
│   Insertado en cols e (1-4) y f (8-5)  │                                               │
│                                        │  [Filas 52 a 58]:                             │
│  [Filas 28 a 30]:                      │   Condensador de desacoplo AC (10 uF)         │
│   Resistencia limitadora (150 Ohm)     │   Buzzer piezoeléctrico pasivo (Etapa 7)      │
│   2 Jumpers hacia Torre 3D TX (LED)    │                                               │
├────────────────────────────────────────┴───────────────────────────────────────────────┤
│ (+) Riel Inferior Rojo (+VCC puenteado con riel superior)       [Puente en fila 30-31] │
│ (-) Riel Inferior Azul (GND puenteado con riel superior)                               │
└────────────────────────────────────────────────────────────────────────────────────────┘
```

### Tabla Maestra de Coordenadas Pin por Pin (Filas `0`–`60`, Columnas `a`–`j`)

Si sigues esta tabla coordenada por coordenada (como un tablero de ajedrez), el circuito quedará armado al primer intento sin un solo cruce erróneo:

| Etapa | Componente / Cable | Terminal 1 (Origen) | Terminal 2 (Destino) | Función en el Circuito |
|:---|:---|:---:|:---:|:---|
| **Alimentación** | Cable GND Arduino | Pin `GND` Arduino | **Riel Azul (`-`)** | Tierra común obligatoria (Req. P.17) |
| **Alimentación** | Fuente Bodega (`+5.0V`) | Borne Rojo (`+`) | **Riel Rojo (`+`)** | Energía global (Req. Q.18) |
| **Alimentación** | Fuente Bodega (`GND`) | Borne Negro (`-`) | **Riel Azul (`-`)** | Referencia de $0\text{ V}$ |
| **Etapa 1 $\rightarrow$ 2** | Jumper Bit 0 (LSB) | Pin `D8` Arduino | **`a02`** | Entrada digital Bit 0 |
| **Etapa 1 $\rightarrow$ 2** | Jumper Bit 1 | Pin `D9` Arduino | **`a05`** | Entrada digital Bit 1 |
| **Etapa 1 $\rightarrow$ 2** | Jumper Bit 2 | Pin `D10` Arduino | **`a08`** | Entrada digital Bit 2 |
| **Etapa 1 $\rightarrow$ 2** | Jumper Bit 3 (MSB) | Pin `D11` Arduino | **`a11`** | Entrada digital Bit 3 |
| **Etapa 2 (R–2R)** | Resistor $2R_{term}$ ($20\text{ k}\Omega$) | **Riel Azul (`-`)** | **`b04`** | Terminación $2R$ del Nodo $V_1$ a tierra |
| **Etapa 2 (R–2R)** | Resistor $2R_0$ ($20\text{ k}\Omega$) | **`c02`** (viene de D8) | **`c04`** (Nodo $V_1$) | Rama vertical Bit 0 |
| **Etapa 2 (R–2R)** | Resistor $R_1$ ($10\text{ k}\Omega$) | **`d04`** (Nodo $V_1$) | **`d07`** (Nodo $V_2$) | Rama horizontal $V_1 \leftrightarrow V_2$ |
| **Etapa 2 (R–2R)** | Resistor $2R_1$ ($20\text{ k}\Omega$) | **`c05`** (viene de D9) | **`c07`** (Nodo $V_2$) | Rama vertical Bit 1 |
| **Etapa 2 (R–2R)** | Resistor $R_2$ ($10\text{ k}\Omega$) | **`e07`** (Nodo $V_2$) | **`e10`** (Nodo $V_3$) | Rama horizontal $V_2 \leftrightarrow V_3$ |
| **Etapa 2 (R–2R)** | Resistor $2R_2$ ($20\text{ k}\Omega$) | **`c08`** (viene de D10) | **`c10`** (Nodo $V_3$) | Rama vertical Bit 2 |
| **Etapa 2 (R–2R)** | Resistor $R_3$ ($10\text{ k}\Omega$) | **`d10`** (Nodo $V_3$) | **`d13`** (Nodo $V_4$) | Rama horizontal $V_3 \leftrightarrow V_4$ |
| **Etapa 2 (R–2R)** | Resistor $2R_3$ ($20\text{ k}\Omega$) | **`c11`** (viene de D11) | **`c13`** (Nodo $V_4$) | Rama vertical Bit 3 (Salida R–2R) |
| **Etapa 2B y 3** | **Chip LM358N** (8 pines) | Pines 1–4: **`e19` a `e22`** | Pines 8–5: **`f19` a `f22`** | Muesca mirando hacia la fila 19 |
| **Etapa 2B (1A)** | Jumper Entrada DAC | **`e13`** (Nodo $V_4$) | **`d21`** (Pin 3 LM358) | Conecta escalera a entrada `IN1+` |
| **Etapa 2B (1A)** | Puente Seguidor 1A | **`c19`** (Pin 1 LM358) | **`c20`** (Pin 2 LM358) | Realimentación unitaria `OUT1` a `IN1-` |
| **Etapa 2B/3** | Cable GND LM358 | **`b22`** (Pin 4 LM358) | **Riel Azul (`-`)** | Tierra del amplificador operacional |
| **Etapa 2B/3** | Cable VCC LM358 | **`h19`** (Pin 8 LM358) | **Riel Rojo (`+`)** | Alimentación positiva del LM358 |
| **Etapa 3 (1B)** | Jumper Etapa 2 $\rightarrow$ 3 | **`b19`** (Pin 1 LM358) | **`g22`** (Pin 5 LM358) | Lleva salida del DAC a entrada `IN2+` |
| **Etapa 3 (1B)** | Puente Seguidor 1B | **`h20`** (Pin 7 LM358) | **`h21`** (Pin 6 LM358) | Realimentación unitaria `OUT2` a `IN2-` |
| **Etapa 4 (TX)** | Resistor $R_{drv}$ ($150\ \Omega$) | **`i20`** (Pin 7 LM358) | **`i28`** | Limita corriente del LED a $<20\text{ mA}$ |
| **Etapa 4 (TX)** | **LED Rojo $5\text{ mm}$** | **Ánodo:** **`j28`** (Jumper) | **Cátodo:** **Riel Azul (`-`)** | En Torre 3D TX (o directo en `j28` y `-`) |
| **Etapa 5 (RX)** | **LDR GL5528** | **Pata 1:** **`d40`** (Jumper) | **Pata 2:** **`d41`** (Jumper) | En Torre 3D RX (va entre Pin 7 y Pin 6 del 555) |
| **Etapa 6 (555)** | **Chip NE555** (8 pines) | Pines 1–4: **`e39` a `e42`** | Pines 8–5: **`f39` a `f42`** | Muesca mirando hacia la fila 39 |
| **Etapa 6 (555)** | Cable GND 555 | **`b39`** (Pin 1 NE555) | **Riel Azul (`-`)** | Tierra del temporizador 555 |
| **Etapa 6 (555)** | Cable VCC 555 | **`h39`** (Pin 8 NE555) | **Riel Rojo (`+`)** | Alimentación $+5.0\text{ V}$ |
| **Etapa 6 (555)** | Cable RESET 555 | **`b42`** (Pin 4 NE555) | **Riel Rojo (`+`)** | Habilita oscilación (¡No dejar suelto!) |
| **Etapa 6 (555)** | Condensador $C_2$ ($10\text{ nF}$) | **`h42`** (Pin 5 NE555) | **Riel Azul (`-`)** | Filtro de voltaje de control |
| **Etapa 6 (555)** | Resistor $R_1$ ($1\text{ k}\Omega$) | **Riel Rojo (`+`)** | **`h40`** (Pin 7 NE555) | Carga hacia Pin 7 (`DISCHARGE`) |
| **Etapa 6 (555)** | Puente Trigger-Threshold | **`c40`** (Pin 2 NE555) | **`g41`** (Pin 6 NE555) | Une Pin 2 con Pin 6 para modo astable |
| **Etapa 6 (555)** | Jumper LDR a Pin 7 | **`d40`** (de LDR Pata 1) | **`g40`** (Pin 7 NE555) | Conecta terminal superior de LDR a Pin 7 |
| **Etapa 6 (555)** | Jumper LDR a Pin 6 | **`d41`** (de LDR Pata 2) | **`h41`** (Pin 6 NE555) | Conecta terminal inferior de LDR a Pin 6/2 |
| **Etapa 6 (555)** | Condensador $C_1$ ($100\text{ nF}$) | **`b40`** (Pin 2 NE555) | **Riel Azul (`-`)** | Condensador de temporización principal |
| **Etapa 7 (Audio)** | Condensador $C_{ac}$ ($10\ \mu\text{F}$) | **Positivo (`+`):** **`c41`** (Pin 3) | **Negativo (`-`):** **`c54`** | Acoplo AC desde Pin 3 del 555 |
| **Etapa 7 (Audio)** | **Buzzer Pasivo** | **Positivo (`+`):** **`e54`** | **Negativo (`-`):** **Riel Azul (`-`)** | Transductor acústico final |

---

## 4. Procedimiento de Montaje Paso a Paso e Inspección Incremental

No montes todo el circuito a ciegas antes de probar. Sigue estos 6 pasos secuenciales:

### PASO 1: Buses de Alimentación y Tierra Común (Req. P.17 y Q.18)
1. Coloca un cable puente entre el riel azul superior y el riel azul inferior para que ambos rieles sean `GND`.
2. Coloca un cable puente entre el riel rojo superior y el riel rojo inferior para que ambos rieles sean `+VCC`.
3. Conecta el cable USB del Arduino a la computadora.
4. Conecta un cable desde el pin **`GND` del Arduino** al riel azul (`GND`) del protoboard.
5. Conecta las puntas banana-lagarto de la **Fuente de Laboratorio**:
   * Caimán rojo a riel rojo (`+VCC`).
   * Caimán negro a riel azul (`GND`).
6. Enciende la fuente de laboratorio a **$5.00\text{ V}$**. Con el multímetro en voltios DC, verifica que entre el riel rojo y azul haya exactamente $5.00\text{ V}$.

---

### PASO 2: Red Pasiva en Escalera R–2R (Etapa 2A — Req. E.5 y O.16)
1. Identifica los cuatro nodos de la escalera:
   * **Nodo $V_1$ (Bit 0 - LSB):** Conecta una resistencia de **$20\text{ k}\Omega$** hacia `GND` (resistencia de terminación). Conecta otra resistencia de **$20\text{ k}\Omega$** desde este nodo hacia el pin **D8** del Arduino.
   * Conecta una resistencia horizontal de **$10\text{ k}\Omega$** entre el Nodo $V_1$ y el Nodo $V_2$.
   * **Nodo $V_2$ (Bit 1):** Conecta una resistencia de **$20\text{ k}\Omega$** desde este nodo hacia el pin **D9** del Arduino.
   * Conecta una resistencia horizontal de **$10\text{ k}\Omega$** entre el Nodo $V_2$ y el Nodo $V_3$.
   * **Nodo $V_3$ (Bit 2):** Conecta una resistencia de **$20\text{ k}\Omega$** desde este nodo hacia el pin **D10** del Arduino.
   * Conecta una resistencia horizontal de **$10\text{ k}\Omega$** entre el Nodo $V_3$ y el Nodo $V_4$.
   * **Nodo $V_4$ (Bit 3 - MSB y Salida $V_{DAC}$):** Conecta una resistencia de **$20\text{ k}\Omega$** desde este nodo hacia el pin **D11** del Arduino.
2. **Prueba de Verificación Inmediata (TP1):**
   * Abre el Monitor Serial en la computadora a 9600 baudios con [`src/arduino/generador_patrones_tinkercad.ino`](file:///d:/Proyecto%20CC/src/arduino/generador_patrones_tinkercad.ino) cargado.
   * Escribe el comando `'0'` $\rightarrow$ Mide con el multímetro en el Nodo $V_4$: debe marcar **$0.000\text{ V}$**.
   * Escribe el comando `'8'` $\rightarrow$ Mide en el Nodo $V_4$: debe marcar **$2.500\text{ V}$**.
   * Escribe el comando `'15'` $\rightarrow$ Mide en el Nodo $V_4$: debe marcar **$4.688\text{ V}$**.
   * Si las tensiones coinciden con la tabla teórica con error $< 1\%$, la red R–2R está perfecta.

---

### PASO 3: Integración del Amplificador Operacional Dual LM358N (Etapa 2B y Etapa 3 — Req. E.5 y F.6)
Inserta el circuito integrado **LM358N** a horcajadas en el canal central del protoboard (muesca orientada hacia la izquierda).
1. **Alimentación del LM358:**
   * **Pin 8 ($V_{CC}$):** Al riel rojo ($+5.0\text{ V}$ a $+9.0\text{ V}$ de la fuente de laboratorio).
   * **Pin 4 ($GND$):** Al riel azul (`GND`).
2. **OpAmp 1A (Componente Activo del DAC — Etapa 2):**
   * **Pin 3 ($\text{IN }1+$):** Cable jumper corto hacia el Nodo $V_4$ de la red R–2R.
   * **Pin 2 ($\text{IN }1-$):** Cable jumper puenteando directamente al **Pin 1 ($\text{OUT }1$)**.
   * **Pin 1 ($\text{OUT }1$):** Es la salida analógica activa del DAC.
3. **OpAmp 1B (Etapa de Interfaz de Aislamiento — Etapa 3):**
   * **Pin 5 ($\text{IN }2+$):** Cable jumper desde el **Pin 1 ($\text{OUT }1$)** del OpAmp 1A.
   * **Pin 6 ($\text{IN }2-$):** Cable jumper puenteando directamente al **Pin 7 ($\text{OUT }2$)**.
   * **Pin 7 ($\text{OUT }2$):** Es la salida acondicionada y aislada ($V_{interfaz}$).
4. **Prueba de Verificación Inmediata (TP2 y TP3):**
   * Mide con el multímetro entre el Pin 7 y `GND` enviando valores por serial: el voltaje debe ser exactamente idéntico al de TP1 ($0.00\text{ V}$ a $4.69\text{ V}$) con error inferior al $0.1\%$.

---

### PASO 4: Generador Óptico (Etapa 4 — Req. G.7 y H.8)
1. Conecta un extremo de la resistencia de **$150\ \Omega$** (o $180\ \Omega$) al **Pin 7 del LM358**.
2. Conecta el otro extremo de la resistencia de $150\ \Omega$ al **Ánodo (pata larga)** del **LED Rojo de $5\text{ mm}$**.
3. Conecta el **Cátodo (pata corta / chaflán plano)** del LED Rojo al riel azul (`GND`).
4. **Prueba de Verificación Inmediata (TP4):**
   * Envía por serial el comando `'0'` a `'5'`: El LED debe permanecer apagado ($V_{interfaz} < 1.8\text{ V}$).
   * Envía el comando `'6'` ($1.875\text{ V}$): El LED debe comenzar a encender tenuemente.
   * Envía el comando `'15'` ($4.688\text{ V}$): El LED debe brillar con intensidad máxima ($I_{LED} \approx 19.25\text{ mA}$).
   * Si envías `'A'`, verás el LED aumentar su brillo en 16 pasos progresivos cada 1.5 segundos.

---

### PASO 5: Enlace de Fibra Óptica, LDR y Oscilador 555 Astable (Etapa 5 y 6 — Req. I.9, J.10 y K.11)
1. **Montaje del Temporizador NE555:** Inserta el NE555 en el protoboard (muesca hacia la izquierda).
   * **Pin 1 (GND):** Al riel azul (`GND`).
   * **Pin 8 (VCC):** Al riel rojo ($+5.0\text{ V}$ de la fuente de laboratorio).
   * **Pin 4 (RESET):** Al riel rojo ($+5.0\text{ V}$). **¡CRÍTICO!** Si dejas el pin 4 flotando, el 555 se inhibe.
   * **Pin 5 (CONTROL):** Conecta el condensador cerámico de **$10\text{ nF}$** (código `103`) hacia `GND`.
   * **Pin 7 (DISCHARGE):** Conecta la resistencia fija $R_1 = \mathbf{1\text{ k}\Omega}$ hacia el riel rojo ($+5.0\text{ V}$).
   * **Puente Pin 2 y Pin 6:** Conecta un cable jumper corto entre el Pin 2 (`TRIGGER`) y el Pin 6 (`THRESHOLD`).
   * **Condensador $C_1$ ($100\text{ nF}$, código `104`):** Entre el Pin 2 y el riel azul (`GND`).
2. **Conexión de la LDR (Etapa 5):**
   * Un terminal de la fotorresistencia LDR al **Pin 7**.
   * El otro terminal de la fotorresistencia LDR al **Pin 6 (y Pin 2)**.
3. **Acoplamiento Óptico con Fibra Plástica ($45\text{ cm}$):**
   * Corta $45\text{ cm}$ de fibra óptica plástica con bisturí o alicate en ángulo recto ($90^\circ$).
   * Inserta un extremo en el acoplador 3D del LED rojo y el otro en el acoplador 3D de la LDR (o aísla provisionalmente con cinta aislante negra / termoencogible).
4. **Prueba de Verificación Inmediata (TP5 y TP6):**
   * Conecta la punta del osciloscopio en el **Pin 3 del 555** y la pinza de tierra a `GND`.
   * En oscuridad total (comando `'0'`), la frecuencia debe ser de $\approx 178\text{ Hz}$.
   * A máxima luz (comando `'15'`), la frecuencia debe subir hasta $\approx 3.6\text{ kHz}$.

---

### PASO 6: Salida Acústica con Buzzer Pasivo (Etapa 7 — Req. L.12)
1. Conecta el terminal positivo del **condensador electrolítico de $10\ \mu\text{F}$** al **Pin 3 del 555**.
2. Conecta el terminal negativo del condensador de $10\ \mu\text{F}$ al terminal positivo (`+`) del **Buzzer piezoeléctrico pasivo**.
3. Conecta el terminal negativo (`-`) del Buzzer al riel azul (`GND`).
   * *Nota de impedancia:* Se debe usar un **buzzer piezoeléctrico pasivo** (alta impedancia $\approx 1\text{ k}\Omega$). Con $10\ \mu\text{F}$, la frecuencia de corte es de $\approx 16\text{ Hz}$, transmitiendo desde los 177 Hz sin pérdidas. Si se conectara un parlante magnético de $32\ \Omega$, la frecuencia de corte subiría a $\approx 497\text{ Hz}$ y se perderían los tonos graves; en ese caso se requeriría un condensador de $100\ \mu\text{F}$ o $220\ \mu\text{F}$.
4. **Prueba de Verificación Final del Sistema Completo:**
   * Envía por el Monitor Serial el comando `'A'` (Escalera automática) o `'P'` (Señal periódica de MATLAB).
   * **Resultado audible:** El buzzer reproducirá una secuencia ascendente de 16 notas musicales distintas (desde un tono grave profundo hasta un tono agudo claro).

---

## 5. Tabla Oficial de Puntos de Prueba (TP1 a TP6) para la Defensa Presencial

Lleva esta tabla con las mediciones reales anotadas para presentársela al profesor en los puntos de control y la defensa final:

| Punto de Prueba | Nodo Físico del Circuito | Instrumento | Estado 0 (`0000`) | Estado 8 (`1000`) | Estado 15 (`1111`) | Criterio de Aprobación |
|:---|:---|:---|:---:|:---:|:---:|:---|
| **TP1 ($V_{R2R}$)** | Nodo $V_4$ (Pin 3 del LM358) | Multímetro DC | $0.000\text{ V}$ | $2.500\text{ V}$ | $4.688\text{ V}$ | Error $< 10\%$ (Req. O.16) |
| **TP2 ($V_{DAC}$)** | Pin 1 del LM358 (Salida Etapa 2) | Multímetro DC | $0.000\text{ V}$ | $2.500\text{ V}$ | $4.688\text{ V}$ | OpAmp 1A activo (Req. E.5) |
| **TP3 ($V_{int}$)** | Pin 7 del LM358 (Salida Etapa 3) | Multímetro DC | $0.000\text{ V}$ | $2.500\text{ V}$ | $4.688\text{ V}$ | Aislamiento sin carga (Req. F.6) |
| **TP4 ($I_{LED}$)** | En serie con resistor $150\ \Omega$ | Multímetro mA | $0.00\text{ mA}$ | $\approx 4.67\text{ mA}$ | $\approx 19.25\text{ mA}$ | Corriente dentro de rango seguro |
| **TP5 ($R_{LDR}$)** | Terminales de la LDR | Multímetro $\text{k}\Omega$ | $\approx 40\text{ k}\Omega$ | $\approx 22\text{ k}\Omega$ | $\approx 1.5\text{ k}\Omega$ | Respuesta fotoconductora pasiva |
| **TP6 ($f_{out}$)** | Pin 3 del 555 | Osciloscopio / Frec. | $\approx 178\text{ Hz}$ | $\approx 315\text{ Hz}$ | $\approx 3600\text{ Hz}$ | Rango audible comprobado (Req. L.12) |

---

## 6. Guía Rápida de Solución de Problemas (Troubleshooting en Laboratorio)

* **Problema: El voltaje del DAC no cambia o se queda fijo en 5V o 0V.**
  * *Causa común:* Los pines digitales no están en los pines correctos D8..D11, o falta la conexión de tierra común entre el pin `GND` de Arduino y el riel `GND` del protoboard.
  * *Solución:* Verifica que D8 vaya al Bit 0 (LSB) y D11 al Bit 3 (MSB), y que el cable `GND` del Arduino esté firmemente insertado.
* **Problema: El LM358 entrega como máximo $\approx 3.6\text{ V}$ en lugar de $4.688\text{ V}$.**
  * *Causa común:* El LM358 no es un amplificador rail-to-rail en su riel positivo; su salida satura típicamente a $V_{CC} - 1.5\text{ V}$. Si lo alimentas con $5.0\text{ V}$, el tope será $\approx 3.5\text{ V}$.
  * *Solución:* En la fuente de laboratorio, alimenta el Pin 8 del LM358 con **$+7.0\text{ V}$ a $+9.0\text{ V}$** (su rango máximo es $32\text{ V}$), manteniendo el 555 en $+5.0\text{ V}$ y tierra común. Con $+7.0\text{ V}$ en $V_{CC}$, el LM358 alcanza $4.688\text{ V}$ con linealidad perfecta.
* **Problema: El LED no enciende nunca.**
  * *Causa común:* Diodo LED invertido (ánodo y cátodo cruzados) o código por debajo de 6 ($V < 1.8\text{ V}$).
  * *Solución:* Envía el comando manual `'15'` en el Monitor Serial. Si no enciende, invierte la posición de las patas del LED.
* **Problema: El temporizador 555 no genera ninguna oscilación.**
  * *Causa común:* El Pin 4 (RESET) quedó sin conectar, o no se hizo el puente obligatorio entre el Pin 2 y el Pin 6.
  * *Solución:* Revisa que el Pin 4 esté conectado directamente a $+5.0\text{ V}$, y que el Pin 2 esté conectado con un cable al Pin 6.
* **Problema: El buzzer emite un pitido fijo que no cambia con la luz.**
  * *Causa común:* Compraste por error un buzzer activo en lugar de un buzzer pasivo.
  * *Solución:* Desconecta el buzzer y pruébalo con una fuente continua de 5V. Si suena solo, es activo. Reemplázalo de inmediato por un **buzzer pasivo**.
