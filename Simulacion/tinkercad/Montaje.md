# Guía de Simulación del Sistema Completo en TinkerCAD Circuits

---

## 1. Componentes Requeridos en TinkerCAD Circuits

En el panel lateral derecho de TinkerCAD Circuits (filtro **Componentes $\rightarrow$ Todos**), arrastra al área de trabajo:
* **1x Arduino Uno R3** (Etapa 1: Patrón de datos de 4 bits)
* **1x Placa de pruebas** (Protoboard estándar)
* **1x Suministro de energía** (Fuente DC de Laboratorio configurada en $5.0\text{ V}$ — cumple **Req. P.17 y Q.18**)
* **3x Resistencias de $10\text{ k}\Omega$** (Ramas horizontales $R$ de la escalera R–2R)
* **5x Resistencias de $20\text{ k}\Omega$** (4 ramas verticales $2R$ + 1 terminación $2R$ a tierra)
* **1x Amplificador operacional doble LM358** (o dos OpAmps: uno para Etapa 2 DAC activo y otro para Etapa 3 Interfaz)
* **1x Resistencia de $150\ \Omega$** y **1x LED Rojo** (Etapa 4: Generador óptico)
* **1x Fotorresistencia (LDR)** (Etapa 5: Receptor óptico pasivo)
* **1x Temporizador 555**, **1x Resistencia de $1\text{ k}\Omega$**, **1x Condensador de $100\text{ nF}$** y **1x Condensador de $10\text{ nF}$** (Etapa 6: Generador de onda cuadrada)
* **1x Condensador polarizado de $10\ \mu\text{F}$** y **1x Piezo (Buzzer pasivo)** (Etapa 7: Salida audible)
* **2x Multímetros** (modo Voltaje) y **1x Osciloscopio**

---

## 2. Cableado Paso a Paso de las 7 Etapas Delimitadas

### A. Fuente de Alimentación de Laboratorio y Tierra Común (Req. P.17 y Q.18)
1. Configura el **Suministro de energía** principal en **Voltaje = `5.0 V`** y **Corriente = `1.0 A`**.
2. Conecta el terminal **positivo (rojo)** de la fuente al riel positivo (`+`) del protoboard.
3. Conecta el terminal **negativo (negro)** de la fuente al riel negativo (`- / GND`) del protoboard.
4. Conecta un cable desde el pin **`GND` del Arduino Uno** al riel negativo (`- / GND`) del protoboard.
5. **Importante (Req. P.17):** **NO conectes el pin `5V` del Arduino** al protoboard; toda la energía de las etapas proviene de la fuente.
6. **Nota técnica sobre el LM358 (alimentación a 9V vs 5V):**
   * El amplificador operacional LM358 no es *rail-to-rail*; su tensión máxima de salida llega aproximadamente a $V_{OH} \approx V_{CC} - 1.5\text{ V}$.
   * Si alimentas el Pin 8 del LM358 con **5.0 V**, la salida se recortará en $\approx 3.5\text{ V}$ para los códigos digitales 12 a 15 ($3.75\text{ V}$ a $4.6875\text{ V}$).
   * Para ver los 16 escalones completos en TinkerCAD sin recorte (o en el laboratorio con fuente de dos canales), agrega un segundo suministro de energía en **`9.0 V`** para alimentar el **Pin 8 del LM358**, compartiendo la misma tierra (`GND`) y manteniendo el 555 a **`5.0 V`**.

### B. Etapa 1 y Etapa 2: Arduino + Red R–2R + OpAmp 1A (Req. C.3 y E.5)
1. **Resistencia de terminación ($20\text{ k}\Omega$):** Conecta desde el nodo de **Bit 0 ($V_1$)** hacia el riel de tierra (`-`).
2. **Resistencias verticales ($2R = 20\text{ k}\Omega$):**
   * Pin **D8** (LSB) $\rightarrow$ Nodo Bit 0 ($V_1$).
   * Pin **D9** $\rightarrow$ Nodo Bit 1 ($V_2$).
   * Pin **D10** $\rightarrow$ Nodo Bit 2 ($V_3$).
   * Pin **D11** (MSB) $\rightarrow$ Nodo Bit 3 ($V_4$).
3. **Resistencias horizontales ($R = 10\text{ k}\Omega$):**
   * Entre Nodo Bit 0 ($V_1$) y Nodo Bit 1 ($V_2$).
   * Entre Nodo Bit 1 ($V_2$) y Nodo Bit 2 ($V_3$).
   * Entre Nodo Bit 2 ($V_3$) y Nodo Bit 3 ($V_4$).
4. **Componente Activo del DAC (Mitad 1A del LM358):**
   * **Pin 8 (`Potencia`):** Al riel de alimentación del op-amp (preferiblemente $+9.0\text{ V}$ para evitar saturación superior a $3.5\text{ V}$, o $+5.0\text{ V}$ teniendo en cuenta el recorte en los últimos 4 códigos).
   * **Pin 4 (`Tierra`):** Al riel negativo (`- / GND`).
   * **Pin 3 (`Entrada no inversora 1`):** Al Nodo Bit 3 ($V_4$).
   * **Pin 2 (`Entrada inversora 1`):** Puente directo al **Pin 1 (`Salida 1`)**.

### C. Etapa 3: Interfaz con Amplificador Operacional (Mitad 1B del LM358 — Req. F.6)
* **Pin 5 (`Entrada no inversora 2`):** Conectar al **Pin 1 (`Salida 1`)** de la Etapa 2.
* **Pin 6 (`Entrada inversora 2`):** Puente directo al **Pin 7 (`Salida 2`)**.
* **Pin 7 (`Salida 2`):** Es la salida acondicionada y aislada de la Etapa 3 ($V_{interfaz}$).

### D. Etapa 4: Generador Óptico (Req. G.7 y H.8)
* Conecta la resistencia de **$150\ \Omega$** desde el **Pin 7 (`Salida 2`)** del LM358 hacia el **Ánodo** del **LED Rojo**.
* Conecta el **Cátodo** del LED Rojo al riel negativo (`- / GND`).

### E. Etapa 5 y Etapa 6: Receptor LDR y Temporizador 555 Astable (Req. J.10 y K.11)
* **Pin 1 (`Tierra`) del 555:** Al riel negativo (`- / GND`).
* **Pin 8 (`Potencia`) y Pin 4 (`Restablecer`) del 555:** Al riel positivo de **`+5.0 V`** de la fuente de laboratorio.
* **Pin 5 (`Voltaje de control`) del 555:** Condensador de **$10\text{ nF}$** a tierra (`-`).
* **Resistencia $R_1$ ($1\text{ k}\Omega$):** Entre el Pin 8 (`+`) y el Pin 7 (`Descarga`).
* **Fotorresistencia (LDR):** Entre el Pin 7 (`Descarga`) y el Pin 6 (`Umbral`).
* **Puente:** Une el Pin 6 (`Umbral`) con el Pin 2 (`Desencadenador`).
* **Condensador $C_1$ ($100\text{ nF}$):** Entre el Pin 2 y tierra (`-`).

### F. Etapa 7: Salida Audible con Buzzer Pasivo (Req. L.12)
* Conecta el terminal positivo del condensador de **$10\ \mu\text{F}$** al **Pin 3 (`Salida`)** del 555.
* Conecta el terminal negativo del condensador de $10\ \mu\text{F}$ al terminal positivo del **Piezo (Buzzer pasivo)**.
* Conecta el terminal negativo del Piezo a tierra (`- / GND`).
* **Nota sobre la impedancia del transductor:**
  * Un **buzzer piezoeléctrico pasivo** presenta una alta impedancia ($Z \approx 1\text{ k}\Omega$ a frecuencias de audio). Junto al condensador $C_{acoplo} = 10\ \mu\text{F}$, la frecuencia de corte pasa-altos es $f_c = \frac{1}{2\pi R C} \approx 16\text{ Hz}$, dejando pasar toda la banda del sistema ($177\text{ Hz}$ a $3.6\text{ kHz}$) sin atenuación.
  * Si se utilizara una bocina o parlante magnético de baja impedancia ($32\ \Omega$), la frecuencia de corte subiría a $\approx 497\text{ Hz}$, atenuando fuertemente los tonos graves entre 177 Hz y 500 Hz. En ese caso se debería usar un capacitor mayor ($100\ \mu\text{F}$ a $220\ \mu\text{F}$).

### G. Instrumentos de Medición Virtual
* **Multímetro 1:** Terminal rojo en Nodo Bit 3 ($V_4$) y negro a `GND` (verifica $V_{DAC}$ de $0.00\text{ V}$ a $4.69\text{ V}$).
* **Multímetro 2:** Terminal rojo en Pin 7 del LM358 ($V_{interfaz}$) y negro a `GND` (comprueba aislamiento sin caída por carga).
* **Osciloscopio:** Terminal positivo en Pin 3 del 555 y negativo a `GND` (visualiza la onda cuadrada cuya frecuencia varía al mover el control deslizante de luz de la LDR en TinkerCAD).

---

## 3. Ejecución y Comandos de Prueba en el Monitor en Serie

1. Haz clic en **Código** $\rightarrow$ **Texto** y pega el contenido íntegro de [`src/arduino/generador_patrones_tinkercad.ino`](file:///d:/Proyecto%20CC/src/arduino/generador_patrones_tinkercad.ino).
2. Presiona **Iniciar simulación** y abre el **Monitor en serie**:
   * Escribe `A` y presiona Enter: Recorre los 16 escalones (`0` a `15`).
   * Escribe `P` y presiona Enter: Reproduce el arreglo de longitud variable de la **señal periódica** generada en MATLAB ($N = 20$).
   * Escribe `S` y presiona Enter: Reproduce el arreglo de longitud variable de la **señal aperiódica** generada en MATLAB ($N = 18$).
   * Escribe `T500` o `T2000` y presiona Enter: Ajusta el tiempo de retención de cada palabra de 4 bits dentro del rango oficial $[1\text{ ms}, 10\,000\text{ ms}]$ (**Req. C.3**).
   * Haz clic sobre la **Fotorresistencia (LDR)** durante la simulación y desplaza el deslizador de luz de oscuridad a máxima luz para escuchar el cambio de tono en el Piezo y ver la variación de frecuencia en el osciloscopio.
