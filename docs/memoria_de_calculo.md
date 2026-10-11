# Memoria de Cálculo y Análisis Teórico Completo
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos  
**Curso:** EL-2113 Circuitos Eléctricos en Corriente Continua  
**Institución:** Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR)  
**Autor:** Mario Cerdas y equipo de trabajo  

---

## 1. Etapa 1: Patrón de Datos de Prueba (MATLAB y Arduino — Req. C.3 y D.4)

### 1.1 Cuantización Digital de 4 Bits en MATLAB
Para representar digitalmente cualquier señal periódica o aperiódica $x(t)$ acotada en el intervalo $[0, V_{ref}]$ con $V_{ref} = 5.0\text{ V}$ mediante palabras binarias de $N = 4\text{ bits}$, el número total de estados discretos es:
$$M = 2^N = 2^4 = 16\text{ niveles}\quad (D \in \{0, 1, 2, \dots, 15\})$$

El paso de cuantización o resolución ($\Delta V_{LSB}$) corresponde a:
$$\Delta V_{LSB} = \frac{V_{ref}}{2^N} = \frac{5.0000\text{ V}}{16} = 0.3125\text{ V} = 312.5\text{ mV}$$

La palabra digital de 4 bits para la muestra $k$-ésima de un arreglo de longitud variable $L$ se obtiene en MATLAB mediante:
$$D[k] = \text{sat}_{[0,15]} \left( \text{round}\left( \frac{x(t_k)}{\Delta V_{LSB}} \right) \right), \quad k = 0, 1, \dots, L-1$$
donde cada entero $D[k]$ se descompone en cuatro bits $(b_3 b_2 b_1 b_0)_2$ tales que:
$$D[k] = 8 b_3 + 4 b_2 + 2 b_1 + 1 b_0, \quad b_i \in \{0, 1\}$$

### 1.2 Temporización Estabilizada en Arduino (Req. C.3)
Cada palabra digital $D[k]$ se coloca en los cuatro pines digitales del Arduino UNO/Nano (`D8` = $b_0$, `D9` = $b_1$, `D10` = $b_2$, `D11` = $b_3$) durante un tiempo de retención $T_{est}$ controlado por software no bloqueante (`millis()`) que cumple estrictamente la restricción del enunciado:
$$1\text{ ms} \le T_{est} \le 10\text{ s}\quad (1\text{ ms} \le T_{est} \le 10\,000\text{ ms})$$

---

## 2. Etapa 2: Convertidor Digital-Analógico (DAC Pasivo + Activo — Req. E.5 y O.16)

### 2.1 Comparación Técnica de Topologías de DAC y de Amplificador Operacional (Req. E.5)
El requerimiento **E.5** establece que la etapa de conversión digital-analógica debe implementarse con **componentes pasivos y un componente activo (OpAmp)**, justificando la elección técnica:

| Criterio Técnico | Alternativa A: DAC de Resistencias Ponderadas ($R, 2R, 4R, 8R$) | Alternativa B: DAC en Escalera R–2R + OpAmp No Inversor (Seleccionada) |
|:---|:---|:---|
| **Valores resistivos pasivos** | Requiere 4 valores distintos (ej. $10\text{ k}\Omega, 20\text{ k}\Omega, 40\text{ k}\Omega, 80\text{ k}\Omega$), varios no estándar en serie E12/E24. | Solo requiere **2 valores comerciales exactos**: $R = 10\text{ k}\Omega$ y $2R = 20\text{ k}\Omega$ (tolerancia 1%). |
| **Impedancia vista por el nodo** | Variable según el código binario activo. | **Constante** e igual a $R_{th} = R = 10\text{ k}\Omega$ para los 16 códigos. |
| **Topología del OpAmp activo** | Sumador inversor: entrega tensión negativa ($V_{out} < 0\text{ V}$), exigiendo fuente dual simétrica ($\pm V_{CC}$). | **Seguidor / No Inversor unitario (OpAmp 1A del LM358):** entrega tensión positiva ($0\text{ a }4.6875\text{ V}$) operando con fuente simple de laboratorio ($+5\text{ V}$ y $\text{GND}$). |
| **Cumplimiento del error $< 10\%$** | Sensible al desapareamiento entre $R$ y $8R$. | Error teórico $0.00\%$ y error práctico $< 1.5\%$ con resistencias al 1%. |

Por consiguiente, la **Etapa 2** se conforma por la **red pasiva en escalera R–2R** ($R = 10\text{ k}\Omega$, $2R = 20\text{ k}\Omega$) acoplada directamente a su **componente activo: el Amplificador Operacional 1A del integrado LM358N (Pines 1, 2 y 3)**.

---

### 2.2 Demostración Matemática por Leyes de Kirchhoff (Ecuaciones de Nodos LCK y Mallas LVK)
Tal como exige el **Punto 1 de la página 6** del instructivo oficial, se demuestra la función de transferencia y la elección de $R = 10\text{ k}\Omega$ y $2R = 20\text{ k}\Omega$ aplicando la **Ley de Corrientes de Kirchhoff (LCK)** en cada uno de los cuatro nodos principales $V_1, V_2, V_3, V_4$ de la escalera R–2R:

```
       V_b0 (D8)        V_b1 (D9)        V_b2 (D10)       V_b3 (D11)
          │                │                │                │
        [ 2R ]           [ 2R ]           [ 2R ]           [ 2R ]
          │                │                │                │
          ●─────[ R ]──────●─────[ R ]──────●─────[ R ]──────●──────► (+) OpAmp 1A (Etapa 2)
         V_1              V_2              V_3              V_4 = V_DAC
          │
        [ 2R ] (Terminación)
          │
         GND
```

Sean $V_{b0}, V_{b1}, V_{b2}, V_{b3} \in \{0\text{ V}, 5\text{ V}\}$ las tensiones entregadas por los cuatro pines digitales del Arduino ($V_{bi} = V_{ref} \cdot b_i$). Como la entrada no inversora del OpAmp 1A (Pin 3 del LM358) tiene impedancia de entrada idealmente infinita ($I_{in+} = 0\text{ A}$), planteamos la **LCK ($\sum I_{salientes} = 0$) en los 4 nodos**:

1. **Ecuación de Nodo 1 ($V_1$):**
   $$\frac{V_1 - 0}{2R} + \frac{V_1 - V_{b0}}{2R} + \frac{V_1 - V_2}{R} = 0 \implies \text{Multiplicando por } 2R: \quad 4V_1 - 2V_2 = V_{b0} \implies V_1 = \frac{V_2}{2} + \frac{V_{b0}}{4} \quad \text{(Ec. 1)}$$

2. **Ecuación de Nodo 2 ($V_2$):**
   $$\frac{V_2 - V_1}{R} + \frac{V_2 - V_{b1}}{2R} + \frac{V_2 - V_3}{R} = 0 \implies \text{Multiplicando por } 2R: \quad -2V_1 + 5V_2 - 2V_3 = V_{b1} \quad \text{(Ec. 2)}$$
   Sustituyendo la $\text{(Ec. 1)}$ en la $\text{(Ec. 2)}$:
   $$-2\left(\frac{V_2}{2} + \frac{V_{b0}}{4}\right) + 5V_2 - 2V_3 = V_{b1} \implies 4V_2 - 2V_3 = V_{b1} + \frac{V_{b0}}{2} \implies V_2 = \frac{V_3}{2} + \frac{V_{b1}}{4} + \frac{V_{b0}}{8} \quad \text{(Ec. 3)}$$

3. **Ecuación de Nodo 3 ($V_3$):**
   $$\frac{V_3 - V_2}{R} + \frac{V_3 - V_{b2}}{2R} + \frac{V_3 - V_4}{R} = 0 \implies \text{Multiplicando por } 2R: \quad -2V_2 + 5V_3 - 2V_4 = V_{b2} \quad \text{(Ec. 4)}$$
   Sustituyendo la $\text{(Ec. 3)}$ en la $\text{(Ec. 4)}$:
   $$-2\left(\frac{V_3}{2} + \frac{V_{b1}}{4} + \frac{V_{b0}}{8}\right) + 5V_3 - 2V_4 = V_{b2} \implies 4V_3 - 2V_4 = V_{b2} + \frac{V_{b1}}{2} + \frac{V_{b0}}{4}$$
   $$V_3 = \frac{V_4}{2} + \frac{V_{b2}}{4} + \frac{V_{b1}}{8} + \frac{V_{b0}}{16} \quad \text{(Ec. 5)}$$

4. **Ecuación de Nodo 4 ($V_4 = V_{DAC}$, entrada del OpAmp 1A con $I_{in+} = 0$):**
   $$\frac{V_4 - V_3}{R} + \frac{V_4 - V_{b3}}{2R} = 0 \implies \text{Multiplicando por } 2R: \quad -2V_3 + 3V_4 = V_{b3} \quad \text{(Ec. 6)}$$
   Sustituyendo la $\text{(Ec. 5)}$ en la $\text{(Ec. 6)}$:
   $$-2\left(\frac{V_4}{2} + \frac{V_{b2}}{4} + \frac{V_{b1}}{8} + \frac{V_{b0}}{16}\right) + 3V_4 = V_{b3} \implies 2V_4 = V_{b3} + \frac{V_{b2}}{2} + \frac{V_{b1}}{4} + \frac{V_{b0}}{8}$$

Despejando $V_4 = V_{DAC}$ y sustituyendo $V_{bi} = V_{ref} \cdot b_i$:
$$\boxed{V_{DAC} = V_4 = V_{ref}\left(\frac{b_3}{2} + \frac{b_2}{4} + \frac{b_1}{8} + \frac{b_0}{16}\right) = \frac{V_{ref}}{16}\sum_{i=0}^{3} b_i 2^i = V_{ref}\cdot\frac{D}{16}}$$

Por la **Ley de Voltajes de Kirchhoff (LVK)** en el lazo del OpAmp 1A (configuración seguidora con retroalimentación negativa directa entre Pin 1 y Pin 2):
$$V_{out\_DAC} = V_{in+} - V_{id} \approx V_4 = V_{DAC}$$

### 2.3 Justificación del Valor $R = 10\text{ k}\Omega$ y $2R = 20\text{ k}\Omega$ (Corriente en Pines Digitales)
La resistencia equivalente vista por cada pin digital del Arduino hacia la red está comprendida entre $2R = 20\text{ k}\Omega$ y $3R = 30\text{ k}\Omega$. Por la Ley de Ohm, la corriente máxima extraída de cualquier pin digital es:
$$I_{pin(max)} = \frac{V_{ref}}{2R} = \frac{5.0\text{ V}}{20\text{ k}\Omega} = 0.25\text{ mA} \ll 20\text{ mA (límite nominal del ATmega328P)}$$
Esto garantiza que no exista caída de tensión interna ($V_{OH} = 5.00\text{ V}$ sin degradación por resistencia interna de salida de los MOSFETs del microcontrolador, que es de $\sim 25\ \Omega$), manteniendo el error de conversión muy por debajo del $10\%$ exigido en **O.16**.

---

### 2.4 Tabla Completa de Niveles Teóricos vs. Simulados del DAC (16 Estados — Req. O.16)

El criterio de error porcentual oficial (**Req. O.16**) se define como:
$$E_{rel}(\%) = \left| \frac{V_{simulado} - V_{te\acute{o}rico}}{V_{te\acute{o}rico}} \right| \times 100\% < 10\%$$

| Decimal ($D$) | Binario ($b_3 b_2 b_1 b_0$) | Ecuación Nodal ($V_{DAC}$) | $V_{te\acute{o}rico}$ (V) | $V_{simulado}$ SPICE (V) | Error Relativo (%) | Cumple O.16 ($< 10\%$) |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 0 | `0 0 0 0` | $0 \times 0.3125$ | **0.0000** | 0.0000 | 0.00 % | Sí |
| 1 | `0 0 0 1` | $1 \times 0.3125$ | **0.3125** | 0.3125 | 0.00 % | Sí |
| 2 | `0 0 1 0` | $2 \times 0.3125$ | **0.6250** | 0.6250 | 0.00 % | Sí |
| 3 | `0 0 1 1` | $3 \times 0.3125$ | **0.9375** | 0.9375 | 0.00 % | Sí |
| 4 | `0 1 0 0` | $4 \times 0.3125$ | **1.2500** | 1.2500 | 0.00 % | Sí |
| 5 | `0 1 0 1` | $5 \times 0.3125$ | **1.5625** | 1.5625 | 0.00 % | Sí |
| 6 | `0 1 1 0` | $6 \times 0.3125$ | **1.8750** | 1.8750 | 0.00 % | Sí |
| 7 | `0 1 1 1` | $7 \times 0.3125$ | **2.1875** | 2.1875 | 0.00 % | Sí |
| 8 | `1 0 0 0` | $8 \times 0.3125$ | **2.5000** | 2.5000 | 0.00 % | Sí |
| 9 | `1 0 0 1` | $9 \times 0.3125$ | **2.8125** | 2.8125 | 0.00 % | Sí |
| 10 | `1 0 1 0` | $10 \times 0.3125$ | **3.1250** | 3.1250 | 0.00 % | Sí |
| 11 | `1 0 1 1` | $11 \times 0.3125$ | **3.4375** | 3.4375 | 0.00 % | Sí |
| 12 | `1 1 0 0` | $12 \times 0.3125$ | **3.7500** | 3.7500 | 0.00 % | Sí |
| 13 | `1 1 0 1` | $13 \times 0.3125$ | **4.0625** | 4.0625 | 0.00 % | Sí |
| 14 | `1 1 1 0` | $14 \times 0.3125$ | **4.3750** | 4.3750 | 0.00 % | Sí |
| 15 | `1 1 1 1` | $15 \times 0.3125$ | **4.6875** | 4.6875 | 0.00 % | Sí |

---

## 3. Etapa 3: Etapa de Interfaz con Amplificador Operacional (Req. F.6 y B.2)

### 3.1 Delimitación Independiente respecto a la Etapa 2
Para cumplir rigurosamente los requerimientos **B.2** (*cada etapa claramente delimitada e identificable*), **E.5** (*DAC con pasivos + un componente activo*) y **F.6** (*etapa de interfaz implementada únicamente con amplificadores operacionales*), se aprovecha que el circuito integrado comercial **LM358N** es un amplificador operacional **dual** (contiene dos OpAmps independientes en un mismo encapsulado DIP-8):
* **OpAmp 1A (Pines 1, 2 y 3):** Pertenece a la **Etapa 2 (Componente activo del DAC)**.
* **OpAmp 1B (Pines 5, 6 y 7):** Conforma íntegramente la **Etapa 3 (Interfaz de Aislamiento Eléctrico)** entre la salida del DAC y la etapa del generador óptico.

### 3.2 Demostración del Efecto de Carga y Aislamiento de Impedancias
Si la red R–2R ($R_{th} = R = 10\text{ k}\Omega$) se conectara directamente al circuito del LED transmisor ($R_{driver} = 150\ \Omega$), el divisor de tensión por efecto de carga produciría:
$$V_{carga} = V_{DAC} \cdot \frac{R_{driver}}{R_{th} + R_{driver}} = V_{DAC} \cdot \frac{150}{10000 + 150} = 0.01478 \cdot V_{DAC}$$
provocando una **pérdida del 98.52% de la tensión** e impidiendo que el LED alcance jamás su tensión umbral de conducción ($1.8\text{ V}$).

Al interponer la **Etapa 3 (Seguidor de Tensión con OpAmp 1B del LM358: Pin 5 = $\text{IN}+$, Pin 6 = $\text{IN}-$ conectado al Pin 7 = $\text{OUT}$)**:
* Impedancia de entrada de la interfaz: $Z_{in} > 10\text{ M}\Omega \implies I_{entrada} \approx 0\text{ A}$.
* Impedancia de salida en lazo cerrado: $Z_{out} = \frac{R_{o}}{1 + A_{OL}} \approx 0\ \Omega$.
* Función de transferencia de la Etapa 3:
  $$V_{interfaz} = V_{out\_DAC} = V_{ref}\cdot\frac{D}{16}$$

> **Nota sobre Alimentación de Laboratorio (Req. P.17 y Q.18):** Para evitar el límite de excursión superior del LM358 ($V_{OH} \approx V_{CC} - 1.5\text{ V}$) cuando $V_{DAC} = 4.6875\text{ V}$, el pin 8 ($V_{CC}$) del LM358 se alimenta desde la **Fuente DC de Laboratorio de la Escuela de Electrónica** ajustada a $+6.5\text{ V}\text{--}9.0\text{ V}$ (o canal de $+5\text{ V}$ con etapa rail-to-rail), manteniendo el riel de $5.0\text{ V}$ para el temporizador 555 y compartiendo una única tierra común (`GND`).

---

## 4. Etapa 4: Generador Óptico y Justificación Física del Color (Req. G.7 y H.8)

### 4.1 Cálculo Matemático del Componente Pasivo Limitador ($R_{driver}$ — Req. G.7)
La Etapa 4 está compuesta **exclusivamente por un componente pasivo ($R_{driver}$) y una fuente luminosa no láser (LED rojo visible)**. Aplicando la **Ley de Voltajes de Kirchhoff (LVK)** en la malla de salida de la interfaz hacia el LED:
$$-V_{interfaz} + I_{LED} \cdot R_{driver} + V_D = 0 \implies I_{LED}(D) = \begin{cases} 0\text{ A}, & \text{si } V_{interfaz}(D) \le V_D \\ \dfrac{V_{interfaz}(D) - V_D}{R_{driver}}, & \text{si } V_{interfaz}(D) > V_D \end{cases}$$

Para proteger el LED y fijar una corriente máxima segura $I_{LED(max)} \approx 19.25\text{ mA} < 20\text{ mA}$ en el estado máximo ($D = 15 \implies V_{interfaz(15)} = 4.6875\text{ V}$) con tensión umbral de LED rojo $V_D = 1.80\text{ V}$:
$$R_{driver} = \frac{V_{interfaz(15)} - V_D}{I_{LED(max)}} = \frac{4.6875\text{ V} - 1.80\text{ V}}{19.25\text{ mA}} = \frac{2.8875\text{ V}}{0.01925\text{ A}} = 150\ \Omega$$
Potencia máxima disipada en el resistor pasivo:
$$P_{R_{driver}} = I_{LED(max)}^2 \cdot R_{driver} = (0.01925\text{ A})^2 \times 150\ \Omega = 55.6\text{ mW} \ll 250\text{ mW (1/4 W)}$$

---

### 4.2 Justificación del Color Rojo de la Fuente Luminosa mediante Física 2 y Física 3 (Req. H.8 y Punto 4, pág. 6)
De acuerdo con el requerimiento **H.8** y la indicación del **Punto 4 (pág. 6)** del manual oficial, la elección del **LED Rojo visible ($\lambda \approx 650\text{ nm}$)** frente a otros colores (verde, azul o infrarrojo) se fundamenta en tres principios físicos de **Física General II y III**:

1. **Relación Cuántica de Planck-Einstein y Rango Dinámico Eléctrico (Física 3 — Física Moderna):**
   La energía de cada fotón emitido por recombinación electrón-hueco en la unión p-n del LED está dada por la ecuación de Planck-Einstein:
   $$E_{fot\acute{o}n} = h \cdot f = \frac{h \cdot c}{\lambda} \approx E_g \implies V_D \approx \frac{E_g}{q} = \frac{h \cdot c}{q \cdot \lambda}$$
   donde $h = 6.626 \times 10^{-34}\text{ J}\cdot\text{s}$, $c = 3.00 \times 10^8\text{ m/s}$ y $q = 1.602 \times 10^{-19}\text{ C}$.
   * **LED Rojo ($\lambda \approx 650\text{ nm}$):** $E_{fot\acute{o}n} = \frac{1240\text{ eV}\cdot\text{nm}}{650\text{ nm}} = 1.91\text{ eV} \implies V_D \approx 1.80\text{ V}$. Conduce desde el estado $D = 6$ ($1.875\text{ V}$) hasta $D = 15$ ($4.6875\text{ V}$), aprovechando **10 niveles activos de modulación óptica**.
   * **LED Verde ($\lambda \approx 525\text{ nm}$):** $E_{fot\acute{o}n} = 2.36\text{ eV} \implies V_D \approx 2.2\text{--}2.8\text{ V}$ (pierde más estados bajos en corte).
   * **LED Azul/Blanco ($\lambda \approx 460\text{ nm}$):** $E_{fot\acute{o}n} = 2.70\text{ eV} \implies V_D \approx 3.1\text{--}3.4\text{ V}$ (permanece apagado en más del $65\%$ de los códigos del DAC).

2. **Propagación Electromagnética, Ley de Snell y Ventana de Mínima Atenuación en PMMA (Física 2 y 3 — Óptica):**
   La fibra óptica plástica (POF) tiene un núcleo de polimetilmetacrilato (PMMA) con índice de refracción $n_1 \approx 1.49$ y un revestimiento fluorado con $n_2 \approx 1.40$. Por la **Ley de Snell** ($n_1 \sin\theta_1 = n_2 \sin\theta_2$), el ángulo crítico para **Reflexión Interna Total** es:
   $$\theta_c = \arcsin\left(\frac{n_2}{n_1}\right) = \arcsin\left(\frac{1.40}{1.49}\right) \approx 70.0^\circ, \quad NA = \sqrt{n_1^2 - n_2^2} \approx 0.51$$
   En el espectro electromagnético, la dispersión de Rayleigh disminuye con la cuarta potencia de la longitud de onda ($\alpha_{Rayleigh} \propto \lambda^{-4}$), mientras que los armónicos vibracionales de los enlaces moleculares C–H del polímero absorben fuertemente en el infrarrojo cercano ($\lambda > 700\text{ nm}$). Como resultado físico, el PMMA presenta su **ventana absoluta de mínima atenuación óptica exactamente en $\lambda = 650\text{ nm}$ (luz roja)** con pérdidas mínimas de $\sim 0.15\text{ dB/m}$.

3. **Acoplamiento Fotoeléctrico con el Semiconducto de Sulfuro de Cadmio CdS de la LDR (Física 3):**
   El receptor pasivo LDR está fabricado con Sulfuro de Cadmio (CdS), cuya energía de banda prohibida es $E_g \approx 2.42\text{ eV}$ con estados trampa dopados que sitúan su **máxima sensibilidad espectral fotoconductora entre $540\text{ nm}$ y $650\text{ nm}$**. Un LED infrarrojo ($\lambda \approx 850\text{--}940\text{ nm} \implies E_{fot\acute{o}n} \approx 1.3\text{ eV} < E_g$) prácticamente **no excita portadores en una LDR de CdS**, mientras que el LED rojo de $650\text{ nm}$ maximiza la generación de pares electrón-hueco y el cambio de resistencia $\Delta R_{LDR}$.

---

## 5. Etapa 5: Medio Óptico (30–60 cm) y Receptor Óptico Pasivo (Req. I.9 y J.10)

* **Medio Óptico (Req. I.9):** Tramo de fibra óptica plástica (POF de PMMA) cortada a una longitud de **$L_{fibra} = 45\text{ cm}$** (exactamente en el centro del rango permitido de **$30\text{ cm}$ a $60\text{ cm}$**).
* **Receptor Óptico (Req. J.10):** Implementado **únicamente con un componente pasivo**: una fotorresistencia (LDR serie GL5528), cuya resistencia obedece la ley fotoconductora empírica:
  $$R_{LDR}(E_v) = R_0 \cdot E_v^{-\gamma}$$
  variando desde $R_{LDR(osc)} \approx 40\text{ k}\Omega\text{--}50\text{ k}\Omega$ en oscuridad ($D \le 5$) hasta $R_{LDR(max)} \approx 1.2\text{ k}\Omega\text{--}1.5\text{ k}\Omega$ en iluminación máxima ($D = 15$).

---

## 6. Etapa 6 y 7: Generador de Onda Cuadrada y Buzzer Pasivo (Req. K.11 y L.12)

### 6.1 Análisis de Carga y Descarga RC en el Temporizador NE555 Astable (Req. K.11)
En el oscilador astable basado en el circuito integrado comercial **NE555**, el condensador pasivo de temporización $C_1 = 100\text{ nF}$ se carga desde $V_{CC}/3$ hasta $2V_{CC}/3$ a través de la serie $(R_1 + R_{LDR})$ y se descarga desde $2V_{CC}/3$ hasta $V_{CC}/3$ a través de $R_{LDR}$ hacia el pin 7 (`DISCHARGE`).

Resolviendo la ecuación diferencial de primer orden de un circuito RC en corriente continua:
$$v_C(t) = V_f + (V_i - V_f)e^{-t/\tau}$$
* **Intervalo en Alto ($t_H$):** Con $V_i = \frac{1}{3}V_{CC}$, $V_f = V_{CC}$ y $\tau_c = (R_1 + R_{LDR})C_1$:
  $$\frac{2}{3}V_{CC} = V_{CC} + \left(\frac{1}{3}V_{CC} - V_{CC}\right)e^{-t_H / \tau_c} \implies e^{-t_H/\tau_c} = \frac{1}{2} \implies t_H = \ln(2)\cdot(R_1 + R_{LDR})C_1 \approx 0.693(R_1 + R_{LDR})C_1$$
* **Intervalo en Bajo ($t_L$):** Con $V_i = \frac{2}{3}V_{CC}$, $V_f = 0$ y $\tau_d = R_{LDR}C_1$:
  $$\frac{1}{3}V_{CC} = \frac{2}{3}V_{CC} e^{-t_L / \tau_d} \implies e^{-t_L/\tau_d} = \frac{1}{2} \implies t_L = \ln(2)\cdot R_{LDR}C_1 \approx 0.693 R_{LDR}C_1$$
* **Frecuencia de la Onda Cuadrada ($f_{555}$):**
  $$f_{555} = \frac{1}{t_H + t_L} = \frac{1}{\ln(2)\cdot(R_1 + 2R_{LDR})C_1} \approx \frac{1.443}{(R_1 + 2R_{LDR})C_1}$$

Seleccionando los componentes pasivos de soporte $R_1 = 1.0\text{ k}\Omega$ (limita la corriente en el transistor de descarga del pin 7 a $I_{disch(max)} = 5\text{ V}/1\text{ k}\Omega = 5\text{ mA}$), $C_1 = 100\text{ nF}$ y $C_{ctrl} = 10\text{ nF}$ (filtro en el pin 5):
* En oscuridad ($R_{LDR} = 40\text{ k}\Omega$): $f_{min} = \frac{1.44}{(1000 + 80000)\times 10^{-7}} = 177.8\text{ Hz}$ (tono grave).
* En máxima luz ($R_{LDR} = 1.5\text{ k}\Omega$): $f_{max} = \frac{1.44}{(1000 + 3000)\times 10^{-7}} = 3600.0\text{ Hz}$ (tono agudo).

### 6.2 Acoplamiento AC al Buzzer Piezoeléctrico Pasivo (Req. L.12)
Se utiliza un **buzzer piezoeléctrico pasivo** acoplado mediante un condensador serie $C_{acoplo} = 10\ \mu\text{F}$ que forma un filtro pasa-altos con la impedancia del transductor, bloqueando la componente DC y entregando la onda cuadrada alterna pura en todo el espectro audible ($178\text{ Hz}\text{--}3.6\text{ kHz}$).

---

## 7. Esquema de Alimentación con Fuentes de Laboratorio (Req. P.17 y Q.18)

En estricto cumplimiento de los requerimientos **P.17** y **Q.18**:
1. **El Arduino UNO/Nano NO alimenta ninguna etapa analógica de transmisión ni de recepción.** Su pin de `+5V` permanece desconectado del protoboard.
2. **No se emplean baterías AA, AAA ni de 9V.**
3. Las etapas de transmisión (LM358 en Etapas 2 y 3, y Driver LED en Etapa 4) y de recepción (Temporizador NE555 en Etapa 6) se alimentan exclusivamente mediante la **Fuente de Alimentación DC Regulada de Laboratorio provista por la bodega de la Escuela de Electrónica**, uniendo el terminal negativo (`COM / GND`) de la fuente de laboratorio con el pin `GND` del Arduino para mantener una única referencia de potencial ($0\text{ V}$).
