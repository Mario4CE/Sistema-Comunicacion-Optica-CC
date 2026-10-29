# Memoria de Cálculo y Análisis Teórico Completo
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos  
**Curso:** EL-2113 Circuitos Eléctricos en Corriente Continua  
**Institución:** Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR)  
**Equipo de Trabajo:**
- **Mario Cerdas** (Carné: 2022437519, `m.cerdas@estudiantec.cr`)
- **Sebastián Quirós Fernández** (Carné: 2024073526, `s.quiros.4@estudiantec.cr`)
- **Ian Fuentes Núñez** (Carné: 2025100912, `I.fuentes.1@estudiantec.cr`)  
**Profesor:** Ing. Fabián Alberto Picado García  
**Semestre:** II Semestre 2026  

---

## 1. Etapa 1: Patrón de Datos de Prueba (MATLAB y Arduino — Req. C.3 y D.4)

### 1.1 Cuantización Digital de 4 Bits en MATLAB
De acuerdo con la teoría clásica de procesamiento de señales discretas (Proakis y Manolakis, 2007; Sedra y Smith, 2020), para representar digitalmente una señal continua periódica o aperiódica $x(t)$ acotada en el intervalo $[0, V_{\text{ref}}]$ con $V_{\text{ref}} = 5.0000\text{ V}$ mediante una palabra binaria de $N = 4\text{ bits}$, el número total de estados discretos de cuantización es:
$$M = 2^N = 2^4 = 16\text{ niveles}\quad (D \in \{0, 1, 2, \dots, 15\})$$

El incremento de tensión uniforme o paso de cuantización correspondiente al bit menos significativo ($\Delta V_{\text{LSB}}$) se calcula formalmente como:
$$\Delta V_{\text{LSB}} = \frac{V_{\text{ref}}}{2^N} = \frac{5.0000\text{ V}}{16} = 0.3125\text{ V} = 312.5\text{ mV}$$

La palabra digital de 4 bits para la muestra $k$-ésima de un arreglo temporal de longitud $L$ se obtiene en MATLAB mediante el operador de cuantización uniforme con redondeo simétrico y saturación de rango:
$$D[k] = \text{sat}_{[0,15]} \left( \text{round}\left( \frac{x(t_k)}{\Delta V_{\text{LSB}}} \right) \right), \quad k = 0, 1, \dots, L-1$$
donde cada código entero $D[k]$ se descompone en su representación posicional binaria natural $(b_3 b_2 b_1 b_0)_2$:
$$D[k] = 8 b_3 + 4 b_2 + 2 b_1 + 1 b_0, \quad b_i \in \{0, 1\}$$

En el script `Matlab/Primero.m` se generan dos secuencias de longitud variable:
1. **Señal Periódica ($L = 20$ muestras):** Onda sinusoidal $x(t) = 2.5 + 2.1875\sin(2\pi f_0 t)$ contenida en $[0.3125\text{ V}, 4.6875\text{ V}]$.
2. **Señal Aperiódica ($L = 18$ muestras):** Rampa exponencial ascendente y descendente que recorre los estados extremos.

### 1.2 Temporización Estabilizada en Arduino (Req. C.3)
Cada palabra digital $D[k]$ se envía a los cuatro terminales digitales de salida del ATmega328P (`D8` = $b_0$, `D9` = $b_1$, `D10` = $b_2$, `D11` = $b_3$) durante un tiempo de permanencia $T_{\text{est}}$ controlado por software no bloqueante (`millis()`) que cumple estrictamente la restricción del diseño:
$$1\text{ ms} \le T_{\text{est}} \le 10\text{ s}\quad (1\text{ ms} \le T_{\text{est}} \le 10\,000\text{ ms})$$

---

## 2. Etapa 2: Convertidor Digital-Analógico (DAC Pasivo + Activo — Req. E.5 y O.16)

### 2.1 Comparación Técnica de Topologías de DAC y del Amplificador Operacional (Req. E.5)
El requerimiento **E.5** exige implementar la conversión digital-analógica empleando **componentes pasivos y un componente activo (OpAmp)**. En la literatura de diseño analógico (Franco, 2015; Sedra y Smith, 2020) se comparan las dos arquitecturas clásicas:

| Criterio Técnico | Alternativa A: DAC de Resistencias Ponderadas ($R, 2R, 4R, 8R$) | Alternativa B: Escalera R–2R + OpAmp No Inversor (Seleccionada) | Justificación Académica / Cita |
|:---|:---|:---|:---|
| **Valores resistivos pasivos** | Requiere 4 valores distintos ($10\text{ k}\Omega, 20\text{ k}\Omega, 40\text{ k}\Omega, 80\text{ k}\Omega$), varios no estándar en series E12/E24. | Requiere **únicamente 2 valores estándar comerciales**: $R = 10\text{ k}\Omega$ y $2R = 20\text{ k}\Omega$ (tolerancia $1\%$). | Franco (2015, Cap. 12); Sedra y Smith (2020, Cap. 14). |
| **Impedancia de salida vista por el nodo** | Variable en función del código binario aplicado. | **Rigurosamente constante** e igual a $R_{\text{th}} = R = 10\text{ k}\Omega$ para los 16 estados binarios. | Hayt, Kemmerly y Durbin (2012, Cap. 5); Alexander y Sadiku (2018). |
| **Topología del OpAmp activo** | Sumador inversor: entrega tensión negativa ($V_{\text{out}} < 0\text{ V}$), requiriendo fuente de alimentación dual simétrica ($\pm V_{\text{CC}}$). | **Seguidor No Inversor unitario (OpAmp 1A del LM358):** entrega tensión positiva ($0\text{ a }4.6875\text{ V}$) operando con fuente simple referenciada a tierra. | Texas Instruments (2023, LM358N Datasheet); Franco (2015). |
| **Sensibilidad a tolerancias (Req. O.16)** | Alta sensibilidad al desemparejamiento entre $R$ y $8R$, provocando saltos no monótonos. | Baja sensibilidad al emparejamiento; con resistores al $1\%$ garantiza un error experimental $< 1.0\% \ll 10\%$. | Sedra y Smith (2020). |

Por consiguiente, la **Etapa 2** se implementa mediante la **red pasiva en escalera R–2R** ($R = 10\text{ k}\Omega$, $2R = 20\text{ k}\Omega$) acoplada a su **componente activo: el Amplificador Operacional 1A del circuito integrado LM358N (Pines 1, 2 y 3)**.

---

### 2.2 Deducción Matemática Rigurosa por Leyes de Kirchhoff (Análisis Nodal LCK)
Conforme a la metodología analítica en circuitos lineales en corriente continua (Hayt, Kemmerly y Durbin, 2012; Alexander y Sadiku, 2018), se demuestra la función de transferencia aplicando la **Ley de Corrientes de Kirchhoff (LCK)** en cada uno de los cuatro nodos principales $V_1, V_2, V_3, V_4$ de la escalera R–2R:

```
       V_b0 (D8)        V_b1 (D9)        V_b2 (D10)       V_b3 (D11)
          │                │                │                │
        [ 2R ]           [ 2R ]           [ 2R ]           [ 2R ]
          │                │                │                │
          ●─────[ R ]──────●─────[ R ]──────●─────[ R ]──────●──────► (+) OpAmp 1A (Etapa 2)
         V_1              V_2              V_3              V_4 = V_DAC
          │
        [ 2R ] (Terminación a GND)
          │
         GND
```

Sean $V_{b0}, V_{b1}, V_{b2}, V_{b3} \in \{0\text{ V}, 5\text{ V}\}$ los niveles lógicos provistos por los pines del microcontrolador ($V_{bi} = V_{\text{ref}} \cdot b_i$). Asumiendo que la entrada no inversora del OpAmp 1A presenta impedancia idealmente infinita ($I_{\text{in+}} = 0\text{ A}$), se plantea la **LCK ($\sum I_{\text{salientes}} = 0$)**:

1. **Ecuación del Nodo 1 ($V_1$):**
   $$\frac{V_1 - 0}{2R} + \frac{V_1 - V_{b0}}{2R} + \frac{V_1 - V_2}{R} = 0$$
   Multiplicando por $2R$:
   $$V_1 + (V_1 - V_{b0}) + 2(V_1 - V_2) = 0 \implies 4V_1 - 2V_2 = V_{b0} \implies V_1 = \frac{V_2}{2} + \frac{V_{b0}}{4} \quad \text{(Ec. 1)}$$

2. **Ecuación del Nodo 2 ($V_2$):**
   $$\frac{V_2 - V_1}{R} + \frac{V_2 - V_{b1}}{2R} + \frac{V_2 - V_3}{R} = 0$$
   Multiplicando por $2R$:
   $$2(V_2 - V_1) + (V_2 - V_{b1}) + 2(V_2 - V_3) = 0 \implies -2V_1 + 5V_2 - 2V_3 = V_{b1} \quad \text{(Ec. 2)}$$
   Sustituyendo $\text{(Ec. 1)}$ en $\text{(Ec. 2)}$:
   $$-2\left(\frac{V_2}{2} + \frac{V_{b0}}{4}\right) + 5V_2 - 2V_3 = V_{b1} \implies 4V_2 - 2V_3 = V_{b1} + \frac{V_{b0}}{2}$$
   $$V_2 = \frac{V_3}{2} + \frac{V_{b1}}{4} + \frac{V_{b0}}{8} \quad \text{(Ec. 3)}$$

3. **Ecuación del Nodo 3 ($V_3$):**
   $$\frac{V_3 - V_2}{R} + \frac{V_3 - V_{b2}}{2R} + \frac{V_3 - V_4}{R} = 0$$
   Multiplicando por $2R$:
   $$2(V_3 - V_2) + (V_3 - V_{b2}) + 2(V_3 - V_4) = 0 \implies -2V_2 + 5V_3 - 2V_4 = V_{b2} \quad \text{(Ec. 4)}$$
   Sustituyendo $\text{(Ec. 3)}$ en $\text{(Ec. 4)}$:
   $$-2\left(\frac{V_3}{2} + \frac{V_{b1}}{4} + \frac{V_{b0}}{8}\right) + 5V_3 - 2V_4 = V_{b2} \implies 4V_3 - 2V_4 = V_{b2} + \frac{V_{b1}}{2} + \frac{V_{b0}}{4}$$
   $$V_3 = \frac{V_4}{2} + \frac{V_{b2}}{4} + \frac{V_{b1}}{8} + \frac{V_{b0}}{16} \quad \text{(Ec. 5)}$$

4. **Ecuación del Nodo 4 ($V_4 = V_{\text{DAC}}$, entrada del OpAmp 1A con $I_{\text{in+}} = 0$):**
   $$\frac{V_4 - V_3}{R} + \frac{V_4 - V_{b3}}{2R} = 0$$
   Multiplicando por $2R$:
   $$2(V_4 - V_3) + (V_4 - V_{b3}) = 0 \implies -2V_3 + 3V_4 = V_{b3} \quad \text{(Ec. 6)}$$
   Sustituyendo $\text{(Ec. 5)}$ en $\text{(Ec. 6)}$:
   $$-2\left(\frac{V_4}{2} + \frac{V_{b2}}{4} + \frac{V_{b1}}{8} + \frac{V_{b0}}{16}\right) + 3V_4 = V_{b3} \implies 2V_4 = V_{b3} + \frac{V_{b2}}{2} + \frac{V_{b1}}{4} + \frac{V_{b0}}{8}$$

Despejando finalmente $V_4 = V_{\text{DAC}}$ y sustituyendo $V_{bi} = V_{\text{ref}} \cdot b_i$:
$$\boxed{V_{\text{DAC}} = V_4 = V_{\text{ref}}\left(\frac{b_3}{2} + \frac{b_2}{4} + \frac{b_1}{8} + \frac{b_0}{16}\right) = \frac{V_{\text{ref}}}{16}\sum_{i=0}^{3} b_i 2^i = V_{\text{ref}}\cdot\frac{D}{16}}$$

Por la **Ley de Voltajes de Kirchhoff (LVK)** en el lazo del OpAmp 1A configurado como seguidor unitario (realimentación negativa directa Pin 1 a Pin 2):
$$V_{\text{out\_DAC}} = V_{\text{in+}} - V_{id} \approx V_4 = V_{\text{DAC}}$$

### 2.3 Justificación Técnica de $R = 10\text{ k}\Omega$ y $2R = 20\text{ k}\Omega$
La resistencia equivalente de entrada vista por cada terminal digital del microcontrolador hacia la escalera R–2R está acotada en el intervalo $[2R, 3R] = [20\text{ k}\Omega, 30\text{ k}\Omega]$ (Franco, 2015). Por la Ley de Ohm, la corriente máxima extraída de cualquier terminal es:
$$I_{\text{pin(max)}} = \frac{V_{\text{ref}}}{2R} = \frac{5.0000\text{ V}}{20\text{ k}\Omega} = 0.25\text{ mA} \ll 20\text{ mA}$$
Esta demanda de corriente despreciable previene caídas óhmicas sobre los transistores CMOS internos de conmutación del microcontrolador ($R_{\text{on}} \approx 25\ \Omega$), garantizando que $V_{\text{OH}} \approx 5.00\text{ V}$ con un error inferior al $0.05\%$ (Sedra y Smith, 2020).

---

### 2.4 Cuadro de Niveles Teóricos vs. Simulados del DAC (16 Estados — Req. O.16)
El requerimiento **O.16** exige que el error de conversión sea estrictamente menor al $10\%$, calculado como:
$$E_{\text{rel}}(\%) = \left| \frac{V_{\text{sim}} - V_{\text{teo}}}{V_{\text{teo}}} \right| \times 100\% < 10\%$$

| Decimal ($D$) | Binario ($b_3 b_2 b_1 b_0$) | Ecuación Nodal ($V_{\text{DAC}}$) | $V_{\text{teo}}$ (V) | $V_{\text{sim}}$ LTspice (V) | Error Relativo (%) | Cumple O.16 ($< 10\%$) |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0 | `0 0 0 0` | $0 \times 0.3125$ | **0.0000** | 0.0050 | Offset $V_{\text{OL}} \approx 5\text{ mV}$ (LM358) | Sí |
| 1 | `0 0 0 1` | $1 \times 0.3125$ | **0.3125** | 0.3125 | 0.0012 % | Sí |
| 2 | `0 0 1 0` | $2 \times 0.3125$ | **0.6250** | 0.6250 | 0.0000 % | Sí |
| 3 | `0 0 1 1` | $3 \times 0.3125$ | **0.9375** | 0.9375 | 0.0004 % | Sí |
| 4 | `0 1 0 0` | $4 \times 0.3125$ | **1.2500** | 1.2500 | 0.0006 % | Sí |
| 5 | `0 1 0 1` | $5 \times 0.3125$ | **1.5625** | 1.5625 | 0.0007 % | Sí |
| 6 | `0 1 1 0` | $6 \times 0.3125$ | **1.8750** | 1.8750 | 0.0008 % | Sí |
| 7 | `0 1 1 1` | $7 \times 0.3125$ | **2.1875** | 2.1875 | 0.0009 % | Sí |
| 8 | `1 0 0 0` | $8 \times 0.3125$ | **2.5000** | 2.5000 | 0.0009 % | Sí |
| 9 | `1 0 0 1` | $9 \times 0.3125$ | **2.8125** | 2.8125 | 0.0009 % | Sí |
| 10 | `1 0 1 0` | $10 \times 0.3125$ | **3.1250** | 3.1250 | 0.0010 % | Sí |
| 11 | `1 0 1 1` | $11 \times 0.3125$ | **3.4375** | 3.4375 | 0.0010 % | Sí |
| 12 | `1 1 0 0` | $12 \times 0.3125$ | **3.7500** | 3.7500 | 0.0010 % | Sí |
| 13 | `1 1 0 1` | $13 \times 0.3125$ | **4.0625** | 4.0624 | 0.0010 % | Sí |
| 14 | `1 1 1 0` | $14 \times 0.3125$ | **4.3750** | 4.3749 | 0.0010 % | Sí |
| 15 | `1 1 1 1` | $15 \times 0.3125$ | **4.6875** | 4.6874 | 0.0010 % | Sí |

*Nota sobre el estado $D=0$:* En el código `0000`, la salida nodal teórica es $0.0000\text{ V}$. En la simulación y en el circuito físico, la etapa de salida cuasi-complementaria con carga a tierra del integrado LM358 presenta una tensión residual de saturación inferior $V_{\text{OL}} \approx 5.0\text{ mV}$ (Texas Instruments, 2023), lo cual es una característica física normal del semiconductor con polarización unipolar. En todo el rango activo (códigos 1 a 15), el error relativo máximo es de apenas $0.0012\%$.

---

## 3. Etapa 3: Etapa de Interfaz con Amplificador Operacional (Req. F.6 y B.2)

### 3.1 Delimitación Funcional Modular
Para cumplir estrictamente los requerimientos **B.2** (*cada etapa claramente delimitada e identificable*), **E.5** (*DAC con pasivos + un componente activo*) y **F.6** (*interfaz implementada únicamente con amplificadores operacionales*), se aprovecha la arquitectura del encapsulado dual DIP-8 del circuito integrado **LM358N**:
- **OpAmp 1A (Pines 1, 2 y 3):** Conforma el componente activo de la **Etapa 2 (DAC activo)**.
- **OpAmp 1B (Pines 5, 6 y 7):** Conforma íntegramente la **Etapa 3 (Interfaz de Aislamiento y Adaptación de Impedancia)**.

### 3.2 Demostración del Efecto de Carga (Loading Effect)
Si la escalera R–2R ($R_{\text{th}} = 10\text{ k}\Omega$) se conectara directamente a la malla del diodo emisor ($R_{\text{driver}} = 150\ \Omega$), el divisor de tensión por impedancia interna produciría (Alexander y Sadiku, 2018):
$$V_{\text{carga}} = V_{\text{DAC}} \cdot \frac{R_{\text{driver}}}{R_{\text{th}} + R_{\text{driver}}} = V_{\text{DAC}} \cdot \frac{150}{10000 + 150} = 0.01478 \cdot V_{\text{DAC}}$$
Esto provocaría una **pérdida del 98.52% de la amplitud analógica**, impidiendo que el diodo emisor alcance jamás su tensión umbral de encendido ($V_D \approx 1.80\text{ V}$).

Al interponer la **Etapa 3** como seguidor de tensión ($A_v = 1$, puente entre Pin 6 y Pin 7, entrada en Pin 5) (Franco, 2015):
- Impedancia de entrada: $Z_{\text{in}} > 10\text{ M}\Omega \implies I_{\text{entrada}} \to 0\text{ A}$ (no carga la red R–2R).
- Impedancia de salida: $Z_{\text{out}} \approx R_o / (1 + A_{\text{OL}}) \to 0\ \Omega$ (entrega la corriente necesaria al LED).
- Tensión entregada: $V_{\text{interfaz}} = V_{\text{DAC}} = \frac{V_{\text{ref}}}{16} \cdot D$.

---

## 4. Etapa 4: Generador Óptico y Justificación Física del Color (Req. G.7 y H.8)

### 4.1 Cálculo del Componente Pasivo Limitador ($R_{\text{driver}}$ — Req. G.7)
La Etapa 4 está integrada **exclusivamente por un resistor pasivo y una fuente luminosa no láser**. Aplicando la Ley de Voltajes de Kirchhoff (LVK) en la malla del emisor:
$$-V_{\text{interfaz}} + I_{\text{LED}} \cdot R_{\text{driver}} + V_D = 0 \implies I_{\text{LED}}(D) = \begin{cases} 0\text{ A}, & V_{\text{interfaz}}(D) \le V_D \\ \dfrac{V_{\text{interfaz}}(D) - V_D}{R_{\text{driver}}}, & V_{\text{interfaz}}(D) > V_D \end{cases}$$

Para fijar una corriente de seguridad $I_{\text{LED(max)}} \approx 19.25\text{ mA} < 20\text{ mA}$ en el código máximo ($D = 15$, $V_{\text{interfaz}} = 4.6875\text{ V}$) con tensión umbral $V_D = 1.80\text{ V}$ para un LED rojo difuso:
$$R_{\text{driver}} = \frac{V_{\text{interfaz(15)}} - V_D}{I_{\text{LED(max)}}} = \frac{4.6875\text{ V} - 1.8000\text{ V}}{19.25\text{ mA}} = \frac{2.8875\text{ V}}{0.01925\text{ A}} = 150\ \Omega$$
Potencia máxima disipada:
$$P_{R_{\text{driver}}} = (0.01925\text{ A})^2 \times 150\ \Omega = 55.6\text{ mW} \ll 250\text{ mW (1/4 W)}$$

---

### 4.2 Justificación Teórica de la Selección del LED Rojo mediante Física 2 y Física 3 (Req. H.8)
La elección de radiación visible roja ($\lambda \approx 650\text{ nm}$) sobre otras opciones cromáticas se fundamenta en tres leyes de la física del estado sólido y la óptica electromagnética:

1. **Relación Cuántica de Planck-Einstein y Margen Dinámico Activo (Tipler y Mosca, 2010; Kasap, 2013):**
   La energía de cada fotón emitido por recombinación radiativa electrón-hueco en la juntura semiconductora obedece:
   $$E = h\cdot f = \frac{h\cdot c}{\lambda} \approx E_g \implies V_D \approx \frac{E_g}{q} = \frac{h\cdot c}{q\cdot\lambda}$$
   donde $h = 6.626 \times 10^{-34}\text{ J}\cdot\text{s}$, $c = 3.00 \times 10^8\text{ m/s}$ y $q = 1.602 \times 10^{-19}\text{ C}$.
   - **LED Rojo ($\lambda \approx 650\text{ nm}$):** $E = 1.91\text{ eV} \implies V_D \approx 1.80\text{ V}$. Conduce desde el estado $D=6$ ($1.875\text{ V}$) hasta $D=15$ ($4.6875\text{ V}$), garantizando **10 estados activos de modulación óptica**.
   - **LED Verde ($\lambda \approx 525\text{ nm}$):** $E = 2.36\text{ eV} \implies V_D \approx 2.4\text{ V}$ (pierde más estados bajos en corte).
   - **LED Azul/Blanco ($\lambda \approx 460\text{ nm}$):** $E = 2.70\text{ eV} \implies V_D \approx 3.2\text{ V}$ (permanece en corte en más del $65\%$ del recorrido del DAC).

2. **Propagación Electromagnética y Ventana de Mínima Atenuación en PMMA (Ziemann et al., 2008; Kasap, 2013):**
   La fibra óptica plástica (POF) emplea un núcleo de polimetilmetacrilato (PMMA, $n_1 = 1.492$) y revestimiento fluorado ($n_2 = 1.402$). Por la **Ley de Snell**, el ángulo crítico para **Reflexión Interna Total** es:
   $$\theta_c = \arcsin\left(\frac{n_2}{n_1}\right) = \arcsin\left(\frac{1.402}{1.492}\right) \approx 70.0^\circ, \quad NA = \sqrt{n_1^2 - n_2^2} \approx 0.51$$
   En polímeros ópticos, la dispersión intrínseca de Rayleigh disminuye fuertemente con la cuarta potencia de la longitud de onda ($\alpha_{\text{Rayleigh}} \propto \lambda^{-4}$), mientras que las bandas de absorción molecular debidas a los sobretonos vibracionales de los enlaces C--H crecen notablemente a partir de los $700\text{ nm}$ (infrarrojo cercano). La superposición de ambos fenómenos genera la **ventana física de mínima atenuación óptica del PMMA centrada exactamente en $\lambda = 650\text{ nm}$** con pérdidas típicas de $\approx 0.15\text{ dB/m}$ (Ziemann et al., 2008).

3. **Acoplamiento Espectral con el Sulfuro de Cadmio (CdS) de la LDR (Rose, 1963; Streetman y Banerjee, 2016):**
   El sensor pasivo LDR está constituido por Sulfuro de Cadmio policristalino ($E_g \approx 2.42\text{ eV}$). Sus estados trampa sub-banda concentran el pico de sensibilidad fotoconductora entre $540\text{ nm}$ y $650\text{ nm}$. Un emisor infrarrojo ($\lambda \approx 850\text{--}940\text{ nm}$, $E \approx 1.3\text{ eV} < E_g$) no excita portadores libres en la red de CdS, mientras que el LED rojo de $650\text{ nm}$ maximiza la generación de pares electrón-hueco y el cambio de conductancia eléctrica.

---

## 5. Etapa 5: Medio Óptico (30–60 cm) y Receptor Óptico Pasivo (Req. I.9 y J.10)

- **Medio Óptico (Req. I.9):** Fibra óptica plástica de PMMA cortada perpendicularmente a una longitud física de **$L_{\text{fibra}} = 45\text{ cm}$**, ubicándose en el punto medio del intervalo reglamentario de $30\text{ cm}$ a $60\text{ cm}$.
- **Receptor Óptico Pasivo (Req. J.10):** Fotorresistencia de sulfuro de cadmio (LDR modelo GL5528) sin amplificación activa en la recepción óptica directa. Su comportamiento resistivo responde a la ley de potencias (Rose, 1963):
  $$R_{\text{LDR}}(E_v) = R_0 \cdot E_v^{-\gamma}$$
  variando desde $R_{\text{LDR(osc)}} \approx 40\text{ k}\Omega$ en corte luminoso hasta $R_{\text{LDR(max)}} \approx 1.58\text{ k}\Omega$ bajo la máxima irradiación emitida por el LED en $D = 15$.

---

## 6. Etapa 6 y 7: Generador de Onda Cuadrada y Buzzer Pasivo (Req. K.11 y L.12)

### 6.1 Análisis Transitorio RC del Temporizador NE555 Astable (Req. K.11)
El oscilador astable basado en el circuito integrado comercial **NE555** emplea el condensador de temporización $C_1 = 100\text{ nF}$, la resistencia de descarga $R_1 = 1.0\text{ k}\Omega$ y la fotorresistencia $R_{\text{LDR}}$ (Camenzind, 2005; Texas Instruments, 2022). A partir de la ecuación diferencial general de primer orden en circuitos RC:
$$v_C(t) = V_f + (V_i - V_f)e^{-t/\tau}$$

- **Tiempo en Alto ($t_H$):** Carga del capacitor desde $\frac{1}{3}V_{\text{CC}}$ hasta $\frac{2}{3}V_{\text{CC}}$ a través de $(R_1 + R_{\text{LDR}})$:
  $$\frac{2}{3}V_{\text{CC}} = V_{\text{CC}} + \left(\frac{1}{3}V_{\text{CC}} - V_{\text{CC}}\right)e^{-t_H / \tau_c} \implies e^{-t_H/\tau_c} = \frac{1}{2} \implies t_H = \ln(2)(R_1 + R_{\text{LDR}})C_1 \approx 0.693(R_1 + R_{\text{LDR}})C_1$$
- **Tiempo en Bajo ($t_L$):** Descarga del capacitor desde $\frac{2}{3}V_{\text{CC}}$ hasta $\frac{1}{3}V_{\text{CC}}$ a través de $R_{\text{LDR}}$ hacia el transistor interno del Pin 7 (`DISCHARGE`):
  $$\frac{1}{3}V_{\text{CC}} = \frac{2}{3}V_{\text{CC}} e^{-t_L / \tau_d} \implies e^{-t_L/\tau_d} = \frac{1}{2} \implies t_L = \ln(2)R_{\text{LDR}}C_1 \approx 0.693 R_{\text{LDR}}C_1$$
- **Frecuencia Fundamental de Oscilación ($f_{555}$):**
  $$f_{555} = \frac{1}{t_H + t_L} = \frac{1}{\ln(2)(R_1 + 2R_{\text{LDR}})C_1} \approx \frac{1.443}{(R_1 + 2R_{\text{LDR}})C_1}$$

Para los componentes seleccionados ($R_1 = 1\text{ k}\Omega$, $C_1 = 100\text{ nF}$):
- En penumbra ($R_{\text{LDR}} = 40.0\text{ k}\Omega$): $f_{\text{teo}} = \frac{1.443}{(1000 + 80000)\times 10^{-7}} = 178.1\text{ Hz}$. En la simulación con el modelo nativo de LTspice se obtiene **$f_{\text{sim}} = 176.7\text{ Hz}$** (error $0.81\%$, tono grave).
- En máxima conducción lumínica ($R_{\text{LDR}} = 1.58\text{ k}\Omega$ en $D = 15$): $f_{\text{teo}} = \frac{1.443}{(1000 + 3160)\times 10^{-7}} = 3474.3\text{ Hz}$. En LTspice se obtiene **$f_{\text{sim}} = 3443.0\text{ Hz}$** (error $0.90\%$, tono agudo).

| Resistencia LDR ($R_{\text{LDR}}$) | Frecuencia Teórica ($f_{\text{teo}}$) | Frecuencia Simulada ($f_{\text{sim}}$) | Error Relativo | Ciclo de Trabajo (Duty Cycle) | Umbrales $V_{\text{THR}}$ |
|:---:|:---:|:---:|:---:|:---:|:---:|
| $1.5\text{ k}\Omega$ | $3607.5\text{ Hz}$ | $3574.6\text{ Hz}$ | $0.91\%$ | $62.08\%$ | $1.665\text{ V}\text{--}3.335\text{ V}$ |
| $5.0\text{ k}\Omega$ | $1311.8\text{ Hz}$ | $1300.8\text{ Hz}$ | $0.84\%$ | $54.19\%$ | $1.665\text{ V}\text{--}3.335\text{ V}$ |
| $10.0\text{ k}\Omega$ | $687.1\text{ Hz}$ | $681.5\text{ Hz}$ | $0.82\%$ | $52.04\%$ | $1.666\text{ V}\text{--}3.335\text{ V}$ |
| $20.0\text{ k}\Omega$ | $352.0\text{ Hz}$ | $349.1\text{ Hz}$ | $0.81\%$ | $50.89\%$ | $1.666\text{ V}\text{--}3.334\text{ V}$ |
| $40.0\text{ k}\Omega$ | $178.1\text{ Hz}$ | $176.7\text{ Hz}$ | $0.81\%$ | $50.29\%$ | $1.666\text{ V}\text{--}3.334\text{ V}$ |

---

### 6.2 Acoplamiento AC al Buzzer Piezoeléctrico Pasivo (Req. L.12)
Un buzzer piezoeléctrico pasivo requiere una excitación dinámica en corriente alterna para deformar periódicamente su diafragma cerámico (Cady, 1964). Para suprimir el nivel continuo presente en la salida del temporizador ($V_{\text{DC}} \approx V_{\text{CC}}/2 = 2.5\text{ V}$), se acopla un condensador electrolítico en serie de $C_{\text{acoplo}} = 10\ \mu\text{F}$.

El conjunto forma un filtro pasa-altos de primer orden con la impedancia equivalente del buzzer ($R_L \approx 1\text{ k}\Omega$), cuya frecuencia de corte inferior a $-3\text{ dB}$ es (Alexander y Sadiku, 2018):
$$f_c = \frac{1}{2\pi \cdot R_L \cdot C_{\text{acoplo}}} = \frac{1}{2\pi \times 1000\ \Omega \times 10 \times 10^{-6}\text{ F}} \approx 15.9\text{ Hz}$$
Dado que $f_c = 15.9\text{ Hz} \ll f_{\text{min}} = 176.7\text{ Hz}$, la señal de audio generada se transmite íntegramente sin atenuación de graves, bloqueando la componente continua que de otro modo provocaría fatiga mecánica en el transductor piezoeléctrico.

---

## 7. Esquema de Alimentación con Fuentes de Laboratorio (Req. P.17 y Q.18) y Pruebas Experimentales

### 7.1 Validación Preliminar de Banco
El circuito físico fue ensamblado sobre la protoboard fijada al chasis organizador de corte láser. Para comprobar la continuidad eléctrica general, descartar cortocircuitos y verificar preliminarmente la modulación óptica del LED, el confinamiento en la fibra POF y la emisión acústica del buzzer, se efectuó una **prueba preliminar de banco utilizando una batería portátil de $9\text{ V}$**, corroborando el funcionamiento coordinado de las siete etapas.

### 7.2 Cumplimiento Normativo de Alimentación (Req. P.17 y Q.18)
En apego irrestricto a los requerimientos **P.17** y **Q.18** de la especificación oficial:
1. **El microcontrolador Arduino UNO no suministra corriente a ninguna de las etapas analógicas de transmisión o recepción.** Su terminal de `+5V` permanece completamente desconectado de la protoboard.
2. **Queda terminantemente prohibido el empleo de baterías portátiles durante la toma de datos cuantitativos finales y para la defensa evaluativa formal ante el profesor.**
3. Todas las etapas de acondicionamiento analógico, emisión óptica y oscilación acústica se alimentan exclusivamente desde la **Fuente de Alimentación DC Regulada de Laboratorio de doble canal**:
   - **Canal 1:** Ajustado a $+5.00\text{ V}$ DC (alimenta el temporizador NE555, pines 8 y 4).
   - **Canal 2:** Ajustado a $+9.00\text{ V}$ DC (alimenta el pin 8 del LM358N para evitar saturación de riel en estados 12 a 15).
   - **Tierra común:** Los bornes negativos de ambos canales de la fuente y el pin `GND` del Arduino se conectan en un único bus común de tierra ($0\text{ V}$).

### 7.3 Diagnóstico de Laboratorio y Corrección de Puentes del LM358N
Durante la primera sesión de pruebas experimentales en el laboratorio de la Escuela de Electrónica efectuada por Mario Cerdas e Ian Fuentes, se detectaron discrepancias en las lecturas de tensión de salida de la escalera DAC y la interfaz seguidora. Al depurar nodo a nodo el montaje frente a la simulación en TinkerCAD, se identificó que los lazos de realimentación negativa del circuito integrado dual **LM358N** (DIP-8) estaban abiertos, impidiendo su funcionamiento como buffers seguidores unitarios ($A_v = 1$).

Se corrigió el montaje mediante tres puentes directos:
1. **Puente 1 (Pin 1 a Pin 2):** Cierra el lazo de realimentación del buffer del DAC R--2R (Etapa 2).
2. **Puente 2 (Pin 6 a Pin 7):** Cierra el lazo de realimentación del buffer de la interfaz (Etapa 3).
3. **Cable en U Inter-etapas (Pin 1 a Pin 5):** Conecta la salida de la Etapa 2 a la entrada no inversora de la Etapa 3 sin efecto de carga ($Z_{\text{in}} \to \infty$).

Las pruebas posteriores en banco validaron la recuperación completa de la linealidad de la escalera de voltajes ($0\text{ V}$ a $4.6875\text{ V}$). La toma cuantitativa de las 16 lecturas de $V_{\text{exp}}$ y la caracterización osciloscópica se completarán en la sesión conjunta programada para el lunes 2 de noviembre de 2026.

---

## 8. Referencias Bibliográficas Académicas (Normadas)

Todas las ecuaciones, modelos físicos y criterios de selección implementados en esta memoria de cálculo y en el proyecto se fundamentan en las siguientes fuentes académicas y técnicas verificables:

1. **Alexander, C. K., & Sadiku, M. N. O.** (2018). *Fundamentos de circuitos eléctricos* (6.ª ed.). México D.F.: McGraw-Hill Interamericana. ISBN: 978-607-15-1447-9.  
   *Respaldo específico:* Leyes de Kirchhoff (LCK, LVK), análisis nodal en corriente continua, divisor de tensión, análisis circuital del efecto de carga (Cap. 2 y Cap. 5) y respuesta en frecuencia de filtros RC pasa-altos (Cap. 7).
2. **Hayt, W. H., Kemmerly, J. E., & Durbin, S. M.** (2012). *Análisis de circuitos en ingeniería* (8.ª ed.). México D.F.: McGraw-Hill Interamericana Editores. ISBN: 978-607-15-0818-8.  
   *Respaldo específico:* Formulación de ecuaciones lineales por análisis nodal de mallas y escaleras resistivas, teoremas de Thévenin/Norton e impedancia equivalente (Cap. 3, 4 y 5).
3. **Sedra, A. S., Smith, K. C., Carusone, T. C., & Gaudet, V.** (2020). *Microelectronic Circuits* (8th ed.). New York, NY: Oxford University Press. ISBN: 978-0190853464.  
   *Respaldo específico:* Cuantización uniforme y etapas de conversión D/A basadas en escaleras R-2R (Cap. 14); características reales de amplificadores operacionales, impedancia de entrada, saturación de salida y corriente de polarización (Cap. 2).
4. **Franco, S.** (2015). *Design with Operational Amplifiers and Analog Integrated Circuits* (4th ed.). New York, NY: McGraw-Hill Education. ISBN: 978-0078028168.  
   *Respaldo específico:* Análisis completo y deducción de la impedancia constante $R_{\text{th}} = R$ en convertidores D/A R-2R acoplados a operacionales (Cap. 12); configuración de seguidores de tensión (buffers) e inmunidad al efecto de carga (Cap. 1).
5. **Proakis, J. G., & Manolakis, D. G.** (2007). *Digital Signal Processing: Principles, Algorithms, and Applications* (4th ed.). Upper Saddle River, NJ: Pearson Prentice Hall. ISBN: 978-0131873742.  
   *Respaldo específico:* Muestreo temporal, discretización en amplitud, cálculo del escalón de cuantización $\Delta V_{\text{LSB}} = V_{\text{ref}}/2^N$ y codificación binaria natural de señales analógicas periódicas y aperiódicas (Cap. 1).
6. **Kasap, S. O.** (2013). *Optoelectronics and Photonics: Principles and Practices* (2nd ed.). Boston, MA: Pearson. ISBN: 978-0132151498.  
   *Respaldo específico:* Física de junturas semiconductoras p-n emisoras de luz (LED), relación energía-longitud de onda ($E_g \approx qV_D$), reflexión interna total en fibras ópticas, dispersión Rayleigh y acoplamiento emisor-detector (Cap. 1 y 3).
7. **Tipler, P. A., & Mosca, G.** (2010). *Física para la ciencia y la tecnología. Vol. 2: Electricidad y magnetismo / Luz* (6.ª ed.). Barcelona: Editorial Reverté. ISBN: 978-8429144307.  
   *Respaldo específico:* Ley de refracción de Snell, determinación del ángulo crítico $\theta_c = \arcsin(n_2/n_1)$ para reflexión interna total (Cap. 31), y postulado cuántico de Planck-Einstein $E = hf = hc/\lambda$ (Cap. 34).
8. **Ziemann, O., Krauser, J., Zamzow, P. E., & Daum, W.** (2008). *POF Handbook: Optical Short Range Transmission Systems* (2nd ed.). Berlin Heidelberg: Springer-Verlag. DOI: 10.1007/978-3-540-76629-2. ISBN: 978-3-540-76628-5.  
   *Respaldo específico:* Propiedades ópticas del polimetilmetacrilato (PMMA), índice de refracción ($n_1 = 1.492$), apertura numérica ($NA \approx 0.51$) y espectro de pérdidas de fibras POF con ventana de mínima atenuación en $\lambda = 650\text{ nm}$ ($\alpha \approx 0.15\text{ dB/m}$) debido al compromiso entre dispersión Rayleigh y absorción molecular por sobretonos C-H (Cap. 2).
9. **Rose, A.** (1963). *Concepts in Photoconductivity and Allied Problems*. New York, NY: Interscience Publishers, John Wiley & Sons. ISBN: 978-0470735053.  
   *Respaldo específico:* Fundamentos de fotoconductividad en semiconductores de sulfuro de cadmio (CdS), estados trampa, cinética de recombinación de pares electrón-hueco y formulación empírica $R_{\text{LDR}} \propto E_v^{-\gamma}$.
10. **Streetman, B. G., & Banerjee, S. K.** (2016). *Solid State Electronic Devices* (7th ed.). Upper Saddle River, NJ: Pearson. ISBN: 978-0133353037.  
    *Respaldo específico:* Propiedades electrónicas del sulfuro de cadmio ($E_g \approx 2.42\text{ eV}$), espectro de absorción en la región visible roja (540--650 nm) y tiempo de recombinación de portadores libres (Cap. 8).
11. **Camenzind, H. R.** (2005). *Designing Analog Chips*. Virtualbookworm Publishing. ISBN: 978-1589397187.  
    *Respaldo específico:* Obra del diseñador original del circuito integrado 555; arquitectura interna con comparadores tripartitos a $\frac{1}{3}V_{\text{CC}}$ y $\frac{2}{3}V_{\text{CC}}$, circuito de descarga a colector abierto y deducción formal de los periodos de temporización astable $t_H$, $t_L$ y frecuencia de oscilación (Cap. 11).
12. **Cady, W. G.** (1964). *Piezoelectricity: An Introduction to the Theory and Applications of Electromechanical Phenomena in Crystals*. New York, NY: Dover Publications. ISBN: 978-0486601090.  
    *Respaldo específico:* Transducción piezoeléctrica, modelo de impedancia equivalente en transductores cerámicos pasivos y necesidad obligatoria de excitación en corriente alterna sin polarización continua sostenida.
13. **Texas Instruments.** (2023). *LMx58-N Low-Power, Dual-Operational Amplifiers* (Datasheet SNOSBT3J). Dallas, TX: Texas Instruments Incorporated.  
    *Respaldo específico:* Tensión de saturación baja típica $V_{\text{OL}} \approx 5.0\text{ mV}$ con carga a tierra, rango de tensión de modo común que incluye tierra ($0\text{ V}$) en alimentación unipolar y límite superior de excursión $V_{\text{OH}} \le V_{\text{CC}} - 1.5\text{ V}$.
14. **Texas Instruments.** (2022). *NA555, NE555, SA555, SE555 Precision Timers* (Datasheet SLFS022I). Dallas, TX: Texas Instruments Incorporated.  
    *Respaldo específico:* Diagrama funcional interno, umbrales de conmutación del comparador, resistencia interna del transistor de descarga de Pin 7 y fórmulas de oscilación astable recomendadas por el fabricante.
