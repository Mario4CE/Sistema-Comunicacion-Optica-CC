# Memoria de Cálculo y Diseño Circuital
**Proyecto:** Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos y Comunicaciones (EL2113)  
**Autor:** Mario Cerdas y equipo de trabajo  

---

## 1. Etapa 2: Conversor Digital-Analógico (DAC R–2R de 4 Bits)

### 1.1 Arquitectura y Justificación de la Red R–2R
Para convertir los 4 bits digitales del microcontrolador ($b_0$ como LSB y $b_3$ como MSB) en una señal analógica continua proporcional, se selecciona la topología **Red en Escalera R–2R** frente al DAC de resistencias ponderadas por las siguientes razones de ingeniería:
1. **Disponibilidad de componentes:** Solo requiere dos valores de resistencia ($R$ y $2R$), lo cual minimiza los errores debidos a variaciones de tolerancia y coeficientes de temperatura.
2. **Impedancia de salida constante:** Vista desde la salida hacia la red, la resistencia equivalente de Thévenin es siempre igual a $R$ ($R_{th} = R$), independientemente del código binario aplicado.

```
       b0 (LSB)          b1               b2               b3 (MSB)
          │                │                │                │
        [ 2R ]           [ 2R ]           [ 2R ]           [ 2R ]
          │                │                │                │
          o──────[ R ]─────o──────[ R ]─────o──────[ R ]─────o──────► V_out
          │
        [ 2R ]
          │
         GND
```

Valores seleccionados:
* $R = 10\text{ k}\Omega$ (tolerancia 1%)
* $2R = 20\text{ k}\Omega$ (o dos resistencias de $10\text{ k}\Omega$ al 1% en serie)

---

### 1.2 Demostración Matemática por Equivalente de Thévenin y Superposición

Considerando el principio de superposición, cada bit $b_i \in \{0, 1\}$ contribuye independientemente a la salida según su peso binario cuando el nivel lógico alto es $V_{ref} = 5.0\text{ V}$ y el nivel bajo es $0.0\text{ V}$.

#### A. Aporte del Bit Menos Significativo ($b_0 = 1$, $b_1 = b_2 = b_3 = 0$):
1. En el primer nodo (extremo izquierdo), la resistencia de terminación $2R$ en paralelo con la rama del bit $b_0$ ($2R$) equivale a:
   $$R_{eq1} = 2R \parallel 2R = R$$
2. El voltaje de Thévenin en ese nodo es:
   $$V_{th1} = V_{ref} \cdot \frac{2R}{2R + 2R} = \frac{V_{ref}}{2}$$
3. Al sumar en serie la resistencia horizontal $R$, la impedancia resultante hacia el siguiente nodo es $R + R = 2R$.
4. Este divisor sucesivo se repite 4 veces a lo largo de la escalera, atenuando la señal por un factor de 2 en cada etapa:
   $$V_{out}(b_0) = V_{ref} \cdot \left(\frac{1}{2}\right)^4 = \frac{V_{ref}}{16}$$

#### B. Aporte del Bit Más Significativo ($b_3 = 1$, $b_0 = b_1 = b_2 = 0$):
Hacia la izquierda del nodo de salida final, toda la red restante tiene una impedancia equivalente a tierra de $2R$. Por tanto, el divisor de tensión para $b_3$ es:
$$V_{out}(b_3) = V_{ref} \cdot \frac{2R}{2R + 2R} = \frac{V_{ref}}{2}$$

#### C. Ecuación General de Transferencia:
Por superposición lineal:
$$V_{out} = V_{ref} \left( \frac{b_3}{2} + \frac{b_2}{4} + \frac{b_1}{8} + \frac{b_0}{16} \right) = \frac{V_{ref}}{16} \sum_{i=0}^{3} b_i 2^i = V_{ref} \cdot \frac{D}{16}$$
donde $D \in [0, 15]$ es el número decimal equivalente.

#### D. Parámetros de Desempeño:
* **Resolución ($\Delta V_{LSB}$):**
  $$\Delta V_{LSB} = \frac{V_{ref}}{2^N} = \frac{5.000\text{ V}}{16} = 0.3125\text{ V} = 312.5\text{ mV}$$
* **Rango Dinámico ($V_{FS}$):**
  $$V_{FS} = V_{out}(15) = 5.0\text{ V} \cdot \frac{15}{16} = 4.6875\text{ V}$$

---

### 1.3 Tabla de Voltajes Teóricos del DAC (16 Estados)

| Decimal ($D$) | Binario ($b_3 b_2 b_1 b_0$) | Ecuación Fraccionaria | Voltaje Teórico $V_{out}$ (V) |
|:---:|:---:|:---:|:---:|
| 0 | 0 0 0 0 | $0 \times 0.3125$ | **0.0000** |
| 1 | 0 0 0 1 | $1 \times 0.3125$ | **0.3125** |
| 2 | 0 0 1 0 | $2 \times 0.3125$ | **0.6250** |
| 3 | 0 0 1 1 | $3 \times 0.3125$ | **0.9375** |
| 4 | 0 1 0 0 | $4 \times 0.3125$ | **1.2500** |
| 5 | 0 1 0 1 | $5 \times 0.3125$ | **1.5625** |
| 6 | 0 1 1 0 | $6 \times 0.3125$ | **1.8750** |
| 7 | 0 1 1 1 | $7 \times 0.3125$ | **2.1875** |
| 8 | 1 0 0 0 | $8 \times 0.3125$ | **2.5000** |
| 9 | 1 0 0 1 | $9 \times 0.3125$ | **2.8125** |
| 10 | 1 0 1 0 | $10 \times 0.3125$ | **3.1250** |
| 11 | 1 0 1 1 | $11 \times 0.3125$ | **3.4375** |
| 12 | 1 1 0 0 | $12 \times 0.3125$ | **3.7500** |
| 13 | 1 1 0 1 | $13 \times 0.3125$ | **4.0625** |
| 14 | 1 1 1 0 | $14 \times 0.3125$ | **4.3750** |
| 15 | 1 1 1 1 | $15 \times 0.3125$ | **4.6875** |

---

## 2. Etapa 3: Interfaz con Amplificador Operacional (Buffer / Seguidor)

### 2.1 Demostración del Efecto de Carga
Dado que la impedancia de salida de la red R–2R es $R_{th} = R = 10\text{ k}\Omega$, si se conectara directamente una carga $R_L$ (por ejemplo, el circuito del LED de ~150 $\Omega$), la tensión real entregada colapsaría por el divisor:
$$V_L = V_{out\_ideal} \cdot \frac{R_L}{R_{th} + R_L} = V_{out\_ideal} \cdot \frac{150}{10000 + 150} \approx 0.0147 \cdot V_{out\_ideal}$$
provocando una pérdida de más del **98.5%** de la señal y destruyendo la linealidad.

### 2.2 Solución con Seguidor de Voltaje (LM358)
Se implementa un amplificador operacional en configuración seguidor de voltaje (ganancia unitaria $A_v = 1$):
* **Impedancia de entrada:** $Z_{in} > 10^{12}\ \Omega$ (prácticamente infinita, corriente de entrada $I_B \approx 0$).
* **Impedancia de salida:** $Z_{out} \approx 0\ \Omega$.
* **Función de transferencia:**
  $$V_{buffer} = V_{out\_DAC}$$

*Nota de diseño:* El integrado **LM358** puede alimentarse con fuente simple ($0\text{ V} - 5\text{ V}$) y su rango de modo común de entrada incluye el riel negativo (tierra / $0\text{ V}$), lo que permite procesar adecuadamente el código binario `0000`.

---

## 3. Etapa 4 y 5: Enlace Óptico (Driver LED, Fibra y LDR)

### 3.1 Driver del Generador Óptico (LED Rojo)
* Caída de tensión del diodo LED rojo: $V_D \approx 1.8\text{ V}$ a $2.0\text{ V}$.
* Resistencia limitadora de corriente calculada para $I_{max} \approx 18\text{ mA}$ a voltaje máximo ($V_{buffer} = 4.6875\text{ V}$):
  $$R_{driver} = \frac{V_{buffer(max)} - V_D}{I_{max}} = \frac{4.6875\text{ V} - 1.8\text{ V}}{18\text{ mA}} = \frac{2.8875\text{ V}}{0.018\text{ A}} \approx 160\ \Omega$$
  *Valor comercial estandarizado:* **$150\ \Omega$** o **$180\ \Omega$** a $1/4\text{ W}$.

### 3.2 Receptor Óptico (LDR)
La fotorresistencia experimenta una disminución de resistencia eléctrica inversamente proporcional al flujo luminoso recibido a través de la fibra óptica plástica:
* En oscuridad (LED apagado, código `0000`): $R_{LDR} \approx 30\text{ k}\Omega - 100\text{ k}\Omega$.
* Con iluminación máxima (LED al máximo, código `1111`): $R_{LDR} \approx 1\text{ k}\Omega - 2\text{ k}\Omega$.

---

## 4. Etapa 6 y 7: Generador de Onda Cuadrada y Salida Acústica

### 4.1 Temporizador NE555 en Configuración Astable
La LDR se ubica en la posición de temporización $R_B$ (o $R_2$) del circuito astable del temporizador 555:
* Resistencia fija de protección: $R_1 = 1\text{ k}\Omega$.
* Elemento variable óptico: $R_2 = R_{LDR}$.
* Condensador de temporización: $C_1 = 100\text{ nF}$ ($0.1\ \mu\text{F}$).

Ecuaciones de diseño del 555 astable:
* Tiempo en alto ($T_{high}$):
  $$t_H = 0.693 \cdot (R_1 + R_{LDR}) \cdot C_1$$
* Tiempo en bajo ($T_{low}$):
  $$t_L = 0.693 \cdot R_{LDR} \cdot C_1$$
* Periodo total ($T$):
  $$T = t_H + t_L = 0.693 \cdot (R_1 + 2 R_{LDR}) \cdot C_1$$
* **Frecuencia de oscilación ($f$):**
  $$f = \frac{1}{T} = \frac{1.44}{(R_1 + 2 R_{LDR}) \cdot C_1}$$

### 4.2 Rango de Frecuencias Audibles Resultante
* A resistencia alta ($R_{LDR} \approx 40\text{ k}\Omega$, luz mínima):
  $$f_{min} \approx \frac{1.44}{(1000 + 2 \times 40000) \times 100 \times 10^{-9}} \approx \frac{1.44}{0.0081} \approx 177\text{ Hz}\quad\text{(Tono grave)}$$
* A resistencia baja ($R_{LDR} \approx 1.5\text{ k}\Omega$, luz máxima):
  $$f_{max} \approx \frac{1.44}{(1000 + 2 \times 1500) \times 100 \times 10^{-9}} \approx \frac{1.44}{0.0004} \approx 3600\text{ Hz}\quad\text{(Tono agudo)}$$

Todo el rango generado ($180\text{ Hz} - 3.6\text{ kHz}$) cae perfectamente dentro del **espectro audible del oído humano** y en la zona de mayor sensibilidad del buzzer piezoeléctrico pasivo.

### 4.3 Acoplamiento hacia el Buzzer Pasivo
Dado que el pin 3 del 555 entrega una onda cuadrada con componente de corriente continua (DC), se intercala un **condensador de desacoplo de $10\ \mu\text{F}$** en serie con el buzzer pasivo para filtrar el nivel DC y dejar pasar únicamente la componente de corriente alterna (AC / oscilación), protegiendo el transductor y mejorando la nitidez acústica.
