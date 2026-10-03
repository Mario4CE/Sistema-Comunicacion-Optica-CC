# Guía Maestra Paso a Paso: Uso y Ejecución del Proyecto
**Proyecto:** Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos y Comunicaciones (EL2113)  
**Autor:** Mario Cerdas y equipo de trabajo  

---

## 1. Mapa de Archivos del Proyecto
Para saber exactamente qué tienes y dónde encontrar cada cosa:

```
D:\Proyecto CC\
├── README.md                              -> Resumen y especificaciones generales
├── BITACORA.md                            -> Diario de avances técnicos y commits
│
├── src\arduino\emisor_4bits\
│   └── emisor_4bits.ino                   -> Código principal para el Arduino físico
│
├── simulation\
│   ├── tinkercad\
│   │   ├── codigo_tinkercad.ino           -> Código para copiar y pegar en TinkerCAD
│   │   └── guia_montaje_tinkercad.md      -> Guía de conexionado para TinkerCAD
│   └── ltspice\
│       ├── dac_r2r_buffer.asc             -> Esquemático gráfico para abrir en LTspice
│       └── dac_r2r_buffer.cir             -> Netlist SPICE con directivas transitorias
│
├── matlab\
│   ├── simulacion_dac_555.m               -> Simulación integral (DAC + Óptica + 555)
│   └── generador_patrones.m               -> Generador de señales y cálculo de SQNR
│
└── docs\
    ├── GUIA_PASO_A_PASO_GENERAL.md        -> Este documento maestro
    ├── memoria_de_calculo.md              -> Demostraciones matemáticas y fórmulas
    ├── guia_de_conexion_protoboard.md     -> Cableado físico pin por pin
    ├── lista_de_materiales_BOM.md         -> Lista de compras y tolerancias
    └── tinkercad_circuito_dac.jpg         -> Imagen de referencia del circuito
```

---

## 2. FASE 1: Simulación Virtual en TinkerCAD Circuits
**Objetivo:** Verificar que el microcontrolador genera los 4 bits y que la red R–2R entrega los 16 escalones de voltaje esperados ($0.00\text{ V}$ a $4.69\text{ V}$).

```
[ Arduino Uno ] ──(D8 a D11)──► [ Red R-2R ] ──► [ Multímetro Digital ] (Mide 0V a 4.69V)
```

### Paso a paso:
1. Abre tu navegador web y entra a **[tinkercad.com](https://www.tinkercad.com)**.
2. Inicia sesión con tu cuenta y ve a la sección **Diseños** $\rightarrow$ **Crear** $\rightarrow$ **Circuito**.
3. En el panel de componentes (derecha), arrastra a la mesa de trabajo:
   * **1 Arduino Uno R3**
   * **1 Placa de pruebas pequeña** (Protoboard)
   * **4 Resistencias de $10\text{ k}\Omega$** (configúralas en $10\text{ k}\Omega$ en el cuadro de propiedades)
   * **5 Resistencias de $20\text{ k}\Omega$** (configúralas en $20\text{ k}\Omega$)
   * **1 Multímetro** (modo Voltaje)
4. Consulta la imagen de referencia guardada en [`docs/tinkercad_circuito_dac.jpg`](file:///d:/Proyecto%20CC/docs/tinkercad_circuito_dac.jpg):
   * Conecta el **GND** de Arduino al riel negativo ($-$) de la placa de pruebas.
   * Conecta la resistencia de terminación de $20\text{ k}\Omega$ entre el nodo del Bit 0 y tierra.
   * Conecta las 4 resistencias verticales de $20\text{ k}\Omega$ desde los pines **D8**, **D9**, **D10** y **D11** de Arduino.
   * Conecta las 3 resistencias horizontales de $10\text{ k}\Omega$ uniendo los nodos entre sí.
   * Conecta el cable **rojo (+)** del multímetro al nodo de salida (Bit 3).
   * Conecta el cable **negro (-)** del multímetro a tierra.
5. Haz clic en el botón superior **Código**, cambia el selector de "Bloques" a **"Texto"**.
6. Abre el archivo [`simulation/tinkercad/codigo_tinkercad.ino`](file:///d:/Proyecto%20CC/simulation/tinkercad/codigo_tinkercad.ino), copia todo su contenido y pégalo en TinkerCAD.
7. Haz clic en **Iniciar simulación**:
   * Observa la pantalla del multímetro: subirá escalón por escalón cada segundo ($0.00\text{ V} \rightarrow 0.31\text{ V} \rightarrow 0.62\text{ V} \dots \rightarrow 4.69\text{ V}$).
   * Abre el botón inferior **Monitor en serie** para ver el conteo simultáneo en binario y decimal.

---

## 3. FASE 2: Simulación Circuital en LTspice
**Objetivo:** Validar la forma de onda de la escalera en el dominio del tiempo, comprobar que el buffer LM358 aísla la carga y verificar la corriente en el LED.

### Paso a paso:
1. Abre el programa **LTspice** en tu computadora.
2. Ve a **File** $\rightarrow$ **Open** y navega hasta:
   `D:\Proyecto CC\simulation\ltspice\dac_r2r_buffer.asc`
3. Verás en pantalla el circuito dibujado con la red de resistencias, las fuentes de pulsos binarios, el buffer operacional y el LED.
4. Presiona el botón del **muñequito corriendo ("Run")** en la barra superior.
5. Para ver las gráficas:
   * Pasa el cursor sobre el cable etiquetado como `V_DAC` (el cursor cambiará a una punta de prueba roja) y haz clic. Aparecerá la **escalera analógica perfecta de 16 escalones**.
   * Haz clic sobre el nodo `V_BUFFER`: verás que la señal es exactamente idéntica, lo que demuestra que el amplificador operacional eliminó el efecto de carga.
   * Pasa el cursor sobre el diodo `D_LED` (el cursor cambiará a un amperímetro) y haz clic: verás la corriente pulsante que fluirá por la fibra óptica.

---

## 4. FASE 3: Simulación y Modelado Matemático en MATLAB
**Objetivo:** Obtener las curvas completas de transferencia del sistema completo (desde el dato digital hasta la frecuencia del sonido).

### Paso a paso:
1. Abre **MATLAB**.
2. En la barra de dirección de MATLAB, navega a la carpeta: `D:\Proyecto CC\matlab\`.
3. **Simulación 1 (Sistema Completo):**
   * Ejecuta en la consola o abre el script [`simulacion_dac_555.m`](file:///d:/Proyecto%20CC/matlab/simulacion_dac_555.m) y presiona **Run (F5)**.
   * **Qué observar:**
     - En la Command Window aparecerá la **tabla completa de los 16 estados** con voltajes, corrientes del LED, resistencia estimada de la LDR y frecuencia en Hz.
     - Se abrirá una ventana con 4 gráficas simultáneas que puedes exportar o incluir directamente en tu informe final.
4. **Simulación 2 (Generador de Patrones y Cuantización):**
   * Abre [`generador_patrones.m`](file:///d:/Proyecto%20CC/matlab/generador_patrones.m) y presiona **Run (F5)**.
   * **Qué observar:**
     - Muestra la señal senoidal original vs. la señal reconstruida de 4 bits.
     - Calcula la relación señal a ruido ($SQNR = 25.84\text{ dB}$).
     - Te entrega en consola un arreglo de datos en C++ por si deseas transmitir tonos musicales pregrabados.

---

## 5. FASE 4: Montaje Físico y Carga en el Arduino Real
**Objetivo:** Llevar el circuito al mundo real sobre el protoboard de laboratorio.

### Paso a paso:
1. Conecta tu **Arduino UNO o Nano** a tu computadora mediante el cable USB.
2. Abre el **Arduino IDE**.
3. Abre el archivo [`src/arduino/emisor_4bits/emisor_4bits.ino`](file:///d:/Proyecto%20CC/src/arduino/emisor_4bits/emisor_4bits.ino).
4. En el menú **Herramientas**:
   * Selecciona tu placa: **Arduino Uno** o **Arduino Nano**.
   * Selecciona el **Puerto COM** correspondiente (ej. COM3, COM4).
5. Presiona el botón de la flecha hacia la derecha: **Subir (Upload)**.
6. Abre el **Monitor Serie** (icono de lupa arriba a la derecha) y asegúrate de fijar la velocidad en **9600 baudios**.
7. Prueba los modos de trabajo:
   * Escribe `A` y presiona Enter: el Arduino recorrerá automáticamente de 0 a 15 cada 1.5 segundos.
   * Escribe cualquier número (ej. `7` o `12`) y presiona Enter: el Arduino mantendrá ese número fijo en los pines para que puedas medir con calma con el multímetro.
   * Escribe `C` y presiona Enter: modo de calibración (alterna entre 0 y 15).

---

## 6. FASE 5: Enlace Óptico, 555 y Salida por Buzzer
**Objetivo:** Transmitir por luz y escuchar los tonos en el buzzer piezoeléctrico.

### Paso a paso:
1. **Montaje del DAC y Buffer en el protoboard:**
   * Sigue la distribución de cables explicada en [`docs/guia_de_conexion_protoboard.md`](file:///d:/Proyecto%20CC/docs/guia_de_conexion_protoboard.md).
   * Mide con el multímetro en el **Pin 1 del LM358**: debes registrar entre $0\text{ V}$ y $4.68\text{ V}$.
2. **Acoplamiento de la Fibra Óptica:**
   * Corta entre **30 cm y 60 cm** de la fibra óptica plástica con un corte recto y limpio.
   * Enfrenta un extremo al LED rojo y el otro a la fotorresistencia (LDR).
   * **Truco de laboratorio:** Cubre ambas uniones con un trozo de termoencogible negro o cinta aislante para que la luz del techo del laboratorio no altere las mediciones.
3. **Temporizador 555 Astable y Buzzer Pasivo:**
   * Conecta el 555 con la LDR según el diagrama.
   * Conecta en serie el condensador de $10\ \mu\text{F}$ hacia el terminal positivo del **buzzer pasivo**.
   * Conforme el Arduino transmita valores mayores (más luz $\rightarrow$ menor resistencia en la LDR), la frecuencia del 555 aumentará y escucharás tonos progresivamente más agudos en el buzzer.

---

## 7. Checklist Rápido antes de la Entrega
- [ ] Simulación de TinkerCAD guardada y capturas tomadas para el informe.
- [ ] Simulación de LTspice ejecutada y gráficas guardadas.
- [ ] Gráficas de MATLAB exportadas.
- [ ] Código de Arduino cargado y probado en modo manual (`0` a `15`).
- [ ] Medición de voltajes reales con multímetro anotada en la tabla comparativa.
- [ ] Extremos de la fibra óptica cubiertos para evitar ruido de luz ambiental.
- [ ] Buzzer verificado como **pasivo** (emite notas diferentes, no un pitido único).
