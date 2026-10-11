# Guía de Conexión y Montaje Físico en Protoboard (Reglas de Laboratorio)
**Proyecto:** Sistema de Comunicación por Medios Ópticos (EL2113)  
**Cumplimiento:** Requerimientos A.1–Q.18 (incluyendo P.17, Q.18 y M.13 de uso de laboratorios)

---

## 1. Diagrama de Distribución General de las 7 Etapas Delimitadas (Req. A.1 y B.2)

```
                     [ FUENTE DC DE LABORATORIO (Bodega Escuela de Electrónica) ]
                                  │ (+VCC y GND común; NO usar 5V de Arduino)
                                  ▼
[ Etapa 1: Arduino ] ──► [ Etapa 2: R-2R + OpAmp 1A ] ──► [ Etapa 3: Interfaz OpAmp 1B ] ──► [ Etapa 4: R_drv + LED ]
                                                                                                       ║
                                                                                         (Fibra 45 cm + Soportes 3D/Láser)
                                                                                                       ║
                                                                                                       ▼
                                   [ Etapa 7: Buzzer Pasivo ] ◄── [ Etapa 6: 555 Astable ] ◄── [ Etapa 5: LDR Pasiva ]
```

---

## 2. Regla Estricta de Alimentación en el Laboratorio (Req. P.17 y Q.18)

> [!CAUTION]
> **Cumplimiento obligatorio de P.17 y Q.18:**
> 1. **PROHIBIDO usar el pin `5V` del Arduino** para alimentar el protoboard. El pin `5V` del Arduino debe quedar **totalmente libre/desconectado**.
> 2. **PROHIBIDO usar baterías AA, AAA o cuadradas (9V).**
> 3. Solicita en la **bodega de la Escuela de Electrónica** una **Fuente de Alimentación DC de Laboratorio**:
>    * Ajusta la salida de la fuente de laboratorio a **$+5.0\text{ V}$** (o usa el segundo canal a $+6.5\text{ V}\text{--}9.0\text{ V}$ para el pin 8 del LM358 si deseas excursión completa sin caída de riel superior, y $+5.0\text{ V}$ para el pin 8 del NE555).
>    * Conecta el terminal positivo (`+`) de la fuente de laboratorio al riel rojo (`+VCC`) del protoboard.
>    * Conecta el terminal negativo (`- / GND`) de la fuente de laboratorio al riel azul/negro (`GND`) del protoboard.
>    * Conecta un único cable desde el pin **`GND` del Arduino** hacia el riel azul/negro (`GND`) del protoboard para referenciar las señales digitales.

---

## 3. Conexiones Detalladas Etapa por Etapa (Protoboard — Req. Ñ.15)

### Etapa 1: Patrón de Datos de Prueba (Arduino UNO / Nano — Req. C.3 y D.4)
* **Pin D8** (Arduino) $\rightarrow$ Entrada de **Bit 0 (LSB)** de la Etapa 2.
* **Pin D9** (Arduino) $\rightarrow$ Entrada de **Bit 1** de la Etapa 2.
* **Pin D10** (Arduino) $\rightarrow$ Entrada de **Bit 2** de la Etapa 2.
* **Pin D11** (Arduino) $\rightarrow$ Entrada de **Bit 3 (MSB)** de la Etapa 2.
* **Pin GND** (Arduino) $\rightarrow$ Riel común de tierra (`GND`) del protoboard.

---

### Etapa 2: Convertidor Digital-Analógico Pasivo + Activo (Red R–2R + OpAmp 1A LM358 — Req. E.5)
1. **Red Pasiva en Escalera R–2R:**
   * **Resistencia de terminación $2R$ ($20\text{ k}\Omega$):** Entre el nodo $V_1$ (Bit 0) y `GND`.
   * **Ramas verticales $2R$ ($20\text{ k}\Omega$):**
     * Desde Pin **D8** hasta nodo $V_1$.
     * Desde Pin **D9** hasta nodo $V_2$.
     * Desde Pin **D10** hasta nodo $V_3$.
     * Desde Pin **D11** hasta nodo $V_4$.
   * **Ramas horizontales $R$ ($10\text{ k}\Omega$):**
     * Entre nodo $V_1$ y nodo $V_2$.
     * Entre nodo $V_2$ y nodo $V_3$.
     * Entre nodo $V_3$ y nodo $V_4$ (salida de la escalera pasiva).
2. **Componente Activo del DAC (Mitad A del LM358N):**
   * **Pin 8 del LM358 (VCC):** Al riel positivo de la **Fuente DC de Laboratorio** (nunca al 5V del Arduino).
   * **Pin 4 del LM358 (GND):** Al riel de tierra común (`GND`).
   * **Pin 3 (IN1+ no inversora):** Conectar al nodo $V_4$ de la red R–2R.
   * **Pin 2 (IN1- inversora):** Puente directo hacia el **Pin 1 (OUT1)**.
   * **Pin 1 (OUT1 — Salida de la Etapa 2):** Entrega la tensión analógica del DAC ($V_{out\_DAC}$).

---

### Etapa 3: Etapa de Interfaz con Amplificador Operacional (OpAmp 1B LM358 — Req. F.6)
Para aislar eléctricamente la Etapa 2 de la Etapa 4 cumpliendo **B.2** y **F.6**, se utiliza el segundo amplificador operacional contenido dentro del mismo integrado **LM358N**:
* **Pin 5 (IN2+ no inversora):** Conectar al **Pin 1 (OUT1)** de la Etapa 2.
* **Pin 6 (IN2- inversora):** Hacer un puente directo hacia el **Pin 7 (OUT2)** (seguidor de tensión aislador de ganancia unitaria $A_v = 1$).
* **Pin 7 (OUT2 — Salida de la Etapa 3):** Entrega la tensión acondicionada y aislada ($V_{interfaz}$) hacia la Etapa 4.

---

### Etapa 4: Generador Óptico Pasivo + LED (Req. G.7 y H.8)
* **Pin 7 del LM358 (Salida de Interfaz):** Conectar a un terminal de la resistencia limitadora de **$150\ \Omega$**.
* Segundo terminal de la resistencia de **$150\ \Omega$** $\rightarrow$ Ánodo (terminal largo) del **LED Rojo de $5\text{ mm}$ ($\lambda \approx 650\text{ nm}$)**.
* Cátodo (terminal corto) del LED Rojo $\rightarrow$ Riel de tierra común (`GND`).

---

### Medio Óptico y Etapa 5: Receptor Óptico Pasivo + Piezas de Laboratorio 3D/Láser (Req. I.9, J.10 y M.13)
1. **Base de Corte Láser (`base_soporte_optico.svg` / `.dxf`):** Montar el protoboard sobre la base fabricada con la cortadora láser de la Escuela de Electrónica, la cual fija las guías del canal óptico a **$45\text{ cm}$** (dentro del rango oficial de $30\text{ cm}$ a $60\text{ cm}$).
2. **Acopladores Impresos en 3D (`acoplador_optico_led_ldr.stl`):**
   * Insertar el LED rojo de $5\text{ mm}$ en un extremo del cabezal cilíndrico opaco impreso en 3D y el extremo de la fibra óptica plástica (POF) en el conducto axial opuesto.
   * En el extremo receptor de la fibra, insertar la **fotorresistencia pasiva (LDR)** dentro del segundo cabezal impreso en 3D para bloquear al 100% la iluminación ambiental del laboratorio.

---

### Etapa 6: Generador de Onda Cuadrada (Temporizador NE555 Astable — Req. K.11)
* **Pin 1 (GND):** Al riel de tierra común (`GND`).
* **Pin 8 (VCC) y Pin 4 (RESET):** Conectados al riel de $+5.0\text{ V}$ de la **Fuente DC de Laboratorio**.
* **Pin 5 (CONTROL):** Condensador cerámico pasivo de $10\text{ nF}$ (código `103`) hacia `GND`.
* **Resistencia pasiva $R_1$ ($1\text{ k}\Omega$):** Entre Pin 8 (`+5V` de fuente de laboratorio) y Pin 7 (`DISCHARGE`).
* **Receptor LDR (proveniente de Etapa 5):** Un terminal al Pin 7 (`DISCHARGE`) y el otro terminal al nodo común de los Pines 2 y 6.
* **Puente entre Pin 2 (`TRIGGER`) y Pin 6 (`THRESHOLD`):** Unir ambos pines con un cable corto.
* **Condensador pasivo $C_1$ ($100\text{ nF}$, código `104`):** Entre el nodo común (Pines 2 y 6) y `GND`.

---

### Etapa 7: Buzzer Piezoeléctrico Pasivo (Req. L.12)
* **Pin 3 (`OUTPUT` del NE555):** Conectar al terminal positivo del condensador de acoplamiento de **$10\ \mu\text{F}$**.
* Terminal negativo del condensador de $10\ \mu\text{F}$ $\rightarrow$ Terminal positivo (`+`) del **Buzzer piezoeléctrico pasivo**.
* Terminal negativo (`-`) del Buzzer pasivo $\rightarrow$ Riel de tierra común (`GND`).

---

## 4. Puntos de Prueba Oficiales en Laboratorio (Multímetro y Osciloscopio)

| Punto de Prueba | Ubicación Física | Valor Teórico Esperado | Criterio de Aceptación |
|:---|:---|:---|:---|
| **TP1 (`V_R2R`)** | Nodo $V_4$ (Pin 3 del LM358) | $0.0000\text{ V}$ a $4.6875\text{ V}$ ($\Delta V = 0.3125\text{ V}$) | Error $< 10\%$ (Req. O.16) |
| **TP2 (`V_DAC_ACT`)** | Pin 1 del LM358 (Salida Etapa 2) | Idéntico a TP1 ($0.00\text{ V}$ a $4.69\text{ V}$) | Error $< 10\%$ (Req. E.5 y O.16) |
| **TP3 (`V_INTERFAZ`)**| Pin 7 del LM358 (Salida Etapa 3) | Idéntico a TP2 sin caída bajo carga del LED | Aislamiento verificado (Req. F.6) |
| **TP4 (`I_LED`)** | En serie con $R_{driver}$ ($150\ \Omega$) | $0\text{ mA}$ ($D \le 5$) hasta $\approx 19.25\text{ mA}$ ($D = 15$) | Modulación lumínica continua |
| **TP5 (`R_LDR`)** | Terminales de la LDR | $\approx 40\text{ k}\Omega$ (oscuridad) a $\approx 1.5\text{ k}\Omega$ (luz máx.) | Respuesta inversa pasiva (Req. J.10) |
| **TP6 (`OUT_555`)** | Pin 3 del NE555 (Osciloscopio) | Onda cuadrada de $\approx 178\text{ Hz}$ a $\approx 3600\text{ Hz}$ | Variación audible en Buzzer (Req. L.12) |
