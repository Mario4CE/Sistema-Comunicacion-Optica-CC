# Guía de Simulación en TinkerCAD Circuits
**Proyecto:** Sistema de Comunicación por Medios Ópticos (EL2113)  
**Etapas validadas en TinkerCAD:** Etapa 1 (Arduino) y Etapa 2 (DAC R–2R) con Multímetro y Osciloscopio Virtual  

---

## 1. Componentes Requeridos en TinkerCAD

Busca estos componentes en el panel lateral derecho de TinkerCAD (categoría *"Todos"*):
* 1x **Arduino Uno R3**
* 1x **Placa de pruebas pequeña** (Breadboard)
* 4x **Resistencias de $10\text{ k}\Omega$** (Bandas: Marrón, Negro, Naranja, Dorado)
* 5x **Resistencias de $20\text{ k}\Omega$** (Bandas: Rojo, Negro, Naranja, Dorado)
* 1x **Multímetro** (configurado en modo *Voltaje*)
* 1x **Osciloscopio** (opcional, para ver la forma de onda de la escalera)

---

## 2. Instrucciones de Cableado en TinkerCAD

### A. Rieles de Alimentación y Tierra
1. Conecta el pin **GND** del Arduino al riel azul/negro ($-$) de la placa de pruebas.
2. *(Opcional)* Conecta el pin **5V** del Arduino al riel rojo ($+$).

### B. Montaje de la Red R–2R
Para mayor facilidad visual en la placa de pruebas:
1. **Resistencia de terminación ($20\text{ k}\Omega$):** Colócala conectando el nodo del Bit 0 hacia el riel de tierra ($-$).
2. **Resistencias verticales ($2R = 20\text{ k}\Omega$):**
   * Resistencia 1: Pin **D8** del Arduino $\rightarrow$ Nodo del Bit 0.
   * Resistencia 2: Pin **D9** del Arduino $\rightarrow$ Nodo del Bit 1.
   * Resistencia 3: Pin **D10** del Arduino $\rightarrow$ Nodo del Bit 2.
   * Resistencia 4: Pin **D11** del Arduino $\rightarrow$ Nodo del Bit 3.
3. **Resistencias horizontales ($R = 10\text{ k}\Omega$):**
   * Entre Nodo Bit 0 y Nodo Bit 1.
   * Entre Nodo Bit 1 y Nodo Bit 2.
   * Entre Nodo Bit 2 y Nodo Bit 3.

### C. Conexión de los Instrumentos de Medición Virtual
* **Multímetro Digital:**
  * Terminal positivo (rojo): Conectar al **Nodo Bit 3** (este nodo es la salida analógica $V_{out}$ del DAC).
  * Terminal negativo (negro): Conectar al riel de tierra ($-$).
* **Osciloscopio Virtual (si lo agregas):**
  * Cable positivo al Nodo Bit 3.
  * Cable negativo a tierra ($-$).
  * Configura la escala de tiempo del osciloscopio en `2 s/div` o `1 s/div`.

---

## 3. Cargar el Código y Simular

1. Haz clic en el botón superior **Código** de TinkerCAD.
2. Selecciona **Texto** en el selector desplegable.
3. Copia todo el contenido de [`codigo_tinkercad.ino`](file:///d:/Proyecto%20CC/simulation/tinkercad/codigo_tinkercad.ino) y pégalo en el editor.
4. Presiona **Iniciar simulación**.
5. Abre el **Monitor en serie** (en la parte inferior de la ventana de código).
6. **Resultado observable:**
   * El multímetro irá subiendo paso a paso cada segundo:  
     $0.00\text{ V} \rightarrow 0.31\text{ V} \rightarrow 0.62\text{ V} \rightarrow 0.94\text{ V} \dots \rightarrow 4.69\text{ V}$, y luego reiniciará en $0.00\text{ V}$.
   * El monitor serie mostrará simultáneamente el número decimal y los 4 bits correspondientes.
