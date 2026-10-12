# Guía Maestra Paso a Paso: Manual de Ejecución y Montaje del Proyecto
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos  
**Curso:** EL-2113 Circuitos Eléctricos en Corriente Continua  
**Institución:** Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR)  
**Autor:** Mario Cerdas y equipo de trabajo  
**Cumplimiento Integral:** Requerimientos A.1–Q.18, uso de laboratorios y formato IEEE Transactions  

---

## 1. Mapa General del Repositorio y Arquitectura del Sistema

Esta guía es el **documento central de operaciones**. Si vas a revisar, simular o empezar a montar físicamente el circuito en el protoboard de laboratorio, sigue este orden estructurado:

```
D:\Proyecto CC\
├── README.md                              -> Ficha técnica general del proyecto
├── BITACORA.md                            -> Bitácora cronológica de avances y decisiones
│
├── src\
│   ├── arduino\emisor_4bits\
│   │   └── emisor_4bits.ino               -> Firmware oficial Arduino (C.3, D.4 y Punto 8)
│   ├── empaquetar_entrega_oficial.py      -> Generador de la carpeta y .zip oficial (Sección 4.1)
│   └── test_validacion_sistema.py         -> Suite de pruebas automáticas (valida A.1 a Q.18)
│
├── matlab\
│   ├── generador_patrones.m               -> Genera arreglos periódicos y aperiódicos (C.3/D.4)
│   └── simulacion_dac_555.m               -> Modelado numérico completo (16 estados y 4 gráficas)
│
├── simulation\
│   ├── ltspice\
│   │   ├── dac_r2r_buffer.asc / .cir      -> Simulación Emisor (DAC R-2R + OpAmp 1A + OpAmp 1B + LED)
│   │   ├── receptor_555_ldr.asc / .cir    -> Simulación Receptor (LDR + NE555 astable + Buzzer)
│   │   └── sistema_completo_optico.asc/.cir -> Simulación INTEGRADA EN CONJUNTO (7 etapas)
│   └── tinkercad\
│       ├── codigo_tinkercad.ino           -> Firmware optimizado para simulación web
│       ├── guia_montaje_tinkercad.md      -> Guía de conexionado del sistema completo en TinkerCAD
│       └── enlace_simulacion_tinkercad.txt -> Archivo de enlace público requerido por Sección 4.1
│
├── prototipo_laboratorio\                 -> Archivos de fabricación en la Escuela de Electrónica (M.13)
│   ├── corte_laser\
│   │   └── base_soporte_optico.svg        -> Plano de corte láser con las 7 etapas grabadas (B.2)
│   └── impresion_3d\
│       └── acoplador_optico_led_ldr.scad  -> Modelo 3D de acopladores axiales opacos anti-luz ambiente
│
└── docs\
    ├── GUIA_PASO_A_PASO_GENERAL.md        -> Este manual operativo maestro
    ├── guia_de_conexion_protoboard.md     -> Conexionado físico pin a pin en protoboard
    ├── lista_de_materiales_BOM.md         -> Lista completa de componentes, tolerancias y pinouts
    ├── memoria_de_calculo.md              -> Análisis de Kirchhoff, Thévenin y Física 2/3 del LED
    ├── diseno_prototipado_3d_laser.md     -> Guía de corte láser, impresión 3D y propuesta de escáner 3D
    └── informe_ieee\
        └── informe_ieee_el2113.tex        -> Reporte formal en formato IEEE Transactions (<= 3 páginas)
```

---

## 2. FASE 0: Preparación de Materiales y Solicitud en la Bodega de Laboratorio

Antes de montar un solo cable en el protoboard, verifica que tienes todo preparado:

### A. Solicitar en la Bodega de la Escuela de Electrónica (Req. P.17 y Q.18)
1. **Fuente de Alimentación DC Regulada de Laboratorio:**  
   * **REGLA DE ORO:** Está terminantemente prohibido alimentar las etapas analógicas desde el pin `5V` de Arduino y usar pilas/baterías (Req. P.17 y Q.18).
   * Pide un par de cables banana a caimán (rojo y negro).
2. **Multímetro Digital de Banco:** Con puntas de prueba de aguja.
3. **Osciloscopio Digital:** Con al menos 1 punta de prueba $10\times$ compensada.

### B. Componentes Electrónicos (Consultar [lista_de_materiales_BOM.md](file:///d:/Proyecto%20CC/docs/lista_de_materiales_BOM.md))
* [ ] 1x Arduino UNO R3 (o Nano) con cable USB.
* [ ] 15x Resistencias de $10\text{ k}\Omega$ (1% tolerancia, película metálica).
* [ ] 8x Resistencias de $20\text{ k}\Omega$ (1% tolerancia, o pares de $10\text{ k}\Omega$ en serie).
* [ ] 1x Circuito integrado **LM358N** (DIP-8, doble amplificador operacional).
* [ ] 1x Resistencia de $150\ \Omega$ (1/4 W).
* [ ] 1x **LED Rojo de $5\text{ mm}$** ($\lambda \approx 650\text{ nm}$).
* [ ] 1x Tramo de fibra óptica plástica (POF) cortada limpia a **$45\text{ cm}$** ($30\text{--}60\text{ cm}$, Req. I.9).
* [ ] 1x Fotorresistencia (LDR GL5528 o similar de $5\text{ mm}$, componente pasivo, Req. J.10).
* [ ] 1x Circuito integrado **NE555P** (DIP-8).
* [ ] 1x Resistencia de $1\text{ k}\Omega$ (1/4 W).
* [ ] 1x Condensador cerámico de $100\text{ nF}$ (código `104`).
* [ ] 1x Condensador cerámico de $10\text{ nF}$ (código `103`).
* [ ] 1x Condensador electrolítico de $10\ \mu\text{F}$ ($16\text{ V}$ o $25\text{ V}$).
* [ ] 1x **Buzzer piezoeléctrico PASIVO** (verificar que al conectar 5V continuo solo haga un "clic" seco y no un pitido fijo).
* [ ] 1x Protoboard de 830 puntos y paquete de cables jumpers macho-macho.

### C. Laboratorio de Prototipado 3D y Corte Láser (Req. M.13)
* Solicita al asistente del laboratorio de prototipado de la Escuela de Electrónica:
  * **Corte Láser:** Cortar en acrílico o MDF de $3\text{ mm}$ el archivo [`prototipo_laboratorio/corte_laser/base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg). La base incluye el grabado de las 7 etapas y las ranuras para la fibra de $45\text{ cm}$.
  * **Impresión 3D:** Imprimir en PLA o PETG negro opaco (relleno $\ge 40\%$) 2 unidades de [`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad).

---

## 3. FASE 1: Generación y Simulación en MATLAB (Req. C.3 y D.4)

**Objetivo:** Generar las representaciones digitales de señales periódicas y aperiódicas cuantizadas a 4 bits y calcular las curvas de respuesta del sistema completo.

### Paso a paso:
1. Abre **MATLAB**.
2. Navega a la carpeta del proyecto: `D:\Proyecto CC\matlab\`.
3. **Paso 1.1 — Generador de Patrones de Longitud Variable:**
   * Abre y ejecuta [`generador_patrones.m`](file:///d:/Proyecto%20CC/matlab/generador_patrones.m) (`Run` o `F5`).
   * **Qué hace:**
     - Cuantiza a 4 bits una **señal periódica** de $N=20$ muestras (`PATRON_PERIODICO_MATLAB`) y una **señal aperiódica** exponencialmente amortiguada de $N=18$ muestras (`PATRON_APERIODICO_MATLAB`).
     - Calcula la resolución $\Delta V = 0.3125\text{ V}$ y la relación $SQNR = 25.84\text{ dB}$.
     - Imprime en la consola los arreglos listos en formato C++ para Arduino.
     - Abre una figura con 3 gráficas que muestran la señal analógica continua vs. la escalera digital cuantizada.
4. **Paso 1.2 — Simulación Integral del Sistema:**
   * Abre y ejecuta [`simulacion_dac_555.m`](file:///d:/Proyecto%20CC/matlab/simulacion_dac_555.m).
   * **Qué hace:**
     - Despliega en la consola la **tabla de los 16 estados** con $V_{DAC}$, $I_{LED}$, $R_{LDR}$, $f_{555}$ y Duty Cycle.
     - Genera una ventana con las **4 curvas características** del sistema:
       1. Escalera analógica del DAC R–2R ($0\text{ V}$ a $4.6875\text{ V}$).
       2. Corriente en el LED transmisor ($0\text{ mA}$ a $19.25\text{ mA}$).
       3. Resistencia fotoconductora de la LDR ($40\text{ k}\Omega$ a $1.5\text{ k}\Omega$).
       4. Frecuencia del sonido en el buzzer ($177.8\text{ Hz}$ a $3600.0\text{ Hz}$).
   * **Acción:** Exporta o toma capturas de estas gráficas para incluirlas en el análisis de resultados de tu informe.

---

## 4. FASE 2: Simulación Circuital en LTspice (Por Separado y en Conjunto)

**Objetivo:** Validar eléctricamente las etapas en el dominio del tiempo mediante modelos SPICE (cumpliendo el **Punto 7 de la página 6**: *"observar el comportamiento de las etapas, tanto separado como en conjunto"*).

### Simulación A — Etapa Emisora ([`dac_r2r_buffer.asc`](file:///d:/Proyecto%20CC/simulation/ltspice/dac_r2r_buffer.asc))
1. Abre **LTspice** y carga `simulation/ltspice/dac_r2r_buffer.asc`.
2. Presiona **Run** (icono de la persona corriendo).
3. Haz clic con la punta de prueba sobre:
   * **`V_R2R`:** Observarás los 16 escalones de la escalera pasiva R–2R ($0\text{ V}$ a $4.6875\text{ V}$).
   * **`V_DAC_ACTIVO`:** Tensión a la salida del OpAmp 1A (Etapa 2, Req. E.5).
   * **`V_INTERFAZ`:** Tensión a la salida del OpAmp 1B (Etapa 3, Req. F.6), intacta frente a la carga.
   * **`D_LED`:** Corriente creciente en el LED rojo a partir del estado 6 ($V > 1.8\text{ V}$).

### Simulación B — Etapa Receptora ([`receptor_555_ldr.asc`](file:///d:/Proyecto%20CC/simulation/ltspice/receptor_555_ldr.asc))
1. En LTspice, abre `simulation/ltspice/receptor_555_ldr.asc`.
2. Presiona **Run**. Ejecutará un barrido paramétrico de 5 resistencias de la LDR ($1.5\text{ k}\Omega, 5\text{ k}\Omega, 10\text{ k}\Omega, 20\text{ k}\Omega, 40\text{ k}\Omega$).
3. Mide en:
   * **`V_OUT_555`:** Tren de pulsos cuadrados cuya frecuencia varía desde $\approx 180\text{ Hz}$ (grave) hasta $\approx 3.6\text{ kHz}$ (agudo).
   * **`V_BUZZER`:** Onda centrada en $0\text{ V}$ (sin nivel DC gracias al condensador de $10\ \mu\text{F}$).

### Simulación C — Sistema Completo en Conjunto ([`sistema_completo_optico.asc`](file:///d:/Proyecto%20CC/simulation/ltspice/sistema_completo_optico.asc))
1. Abre `simulation/ltspice/sistema_completo_optico.asc` (o ejecuta el netlist `sistema_completo_optico.cir`).
2. Presiona **Run**: simula la cadena completa de extremo a extremo (Patrón digital 4 bits $\rightarrow$ DAC R–2R activo $\rightarrow$ Interfaz $\rightarrow$ Driver LED $\rightarrow$ Canal óptico $\rightarrow$ LDR $\rightarrow$ NE555 $\rightarrow$ Buzzer pasivo).
3. Comprueba que la variación en los 4 bits de entrada modula directamente la frecuencia de la señal de audio en el buzzer.

---

## 5. FASE 3: Simulación Virtual en TinkerCAD Circuits (Req. Sección 4.1 y 4.2)

**Objetivo:** Disponer del modelo virtual interactivo completo para los avances 1 y 2 según exige el instructivo.

### Paso a paso:
1. Abre tu navegador e inicia sesión en **[tinkercad.com](https://www.tinkercad.com)** $\rightarrow$ **Crear** $\rightarrow$ **Circuito**.
2. Consulta la guía detallada [guia_montaje_tinkercad.md](file:///d:/Proyecto%20CC/simulation/tinkercad/guia_montaje_tinkercad.md):
   * Agrega: Arduino UNO, Suministro de energía (ajustado en $5.0\text{ V}$, **sin conectar el 5V de Arduino al protoboard**, Req. P.17/Q.18), resistencias R-2R ($10\text{ k}\Omega / 20\text{ k}\Omega$), amplificador operacional LM358, resistencia $150\ \Omega$, LED rojo, fotorresistencia LDR, temporizador 555, condensadores ($100\text{ nF}$, $10\text{ nF}$, $10\ \mu\text{F}$), piezoeléctrico y multímetros.
3. Haz clic en **Código** $\rightarrow$ cambiar a **Texto**.
4. Pega todo el contenido de [`simulation/tinkercad/codigo_tinkercad.ino`](file:///d:/Proyecto%20CC/simulation/tinkercad/codigo_tinkercad.ino).
5. Haz clic en **Iniciar simulación**:
   * El multímetro en la salida del DAC irá subiendo en 16 pasos cada segundo.
   * Abre el **Monitor en serie** (abajo a la derecha):
     - Envía `'P'` para reproducir la señal periódica de MATLAB ($N=20$).
     - Envía `'S'` para reproducir la señal aperiódica de MATLAB ($N=18$).
     - Envía `'T500'` o `'T2000'` para cambiar el tiempo por dato entre $1\text{ ms}$ y $10\text{ s}$ (Req. C.3).
   * Haz clic sobre la **LDR** virtual y mueve el deslizador de luz: el osciloscopio mostrará la variación de frecuencia y el piezo cambiará de sonido.
6. **Guardar enlace público:** En TinkerCAD, haz clic en *Compartir* $\rightarrow$ *Invitar a personas* $\rightarrow$ Copia el enlace y pégalo dentro de [`simulation/tinkercad/enlace_simulacion_tinkercad.txt`](file:///d:/Proyecto%20CC/simulation/tinkercad/enlace_simulacion_tinkercad.txt).

---

## 6. FASE 4: Programación y Prueba del Firmware en el Arduino Físico

**Objetivo:** Flashear y validar el microcontrolador en la computadora antes de conectarlo a la electrónica sensible.

### Paso a paso:
1. Conecta el **Arduino UNO o Nano** a la computadora con el cable USB.
2. Abre el **Arduino IDE**.
3. Abre el archivo [`src/arduino/emisor_4bits/emisor_4bits.ino`](file:///d:/Proyecto%20CC/src/arduino/emisor_4bits/emisor_4bits.ino).
4. En el menú **Herramientas**:
   * Placa: **Arduino Uno** (o Nano).
   * Puerto: Selecciona el puerto COM correspondiente (ej. `COM3` o `COM4`).
5. Presiona **Subir (Upload)**.
6. Abre el **Monitor Serie** (Ctrl+Shift+M) y configura la velocidad estrictamente a **9600 baudios**.
7. **Prueba de comandos en la consola serial:**
   * Escribe `A` y Enter: Modo automático escalera (cuenta cíclica de 0 a 15).
   * Escribe `P` y Enter: Reproduce el arreglo de señal **periódica** de MATLAB ($N=20$).
   * Escribe `S` y Enter: Reproduce el arreglo de señal **aperiódica** de MATLAB ($N=18$).
   * Escribe `C` y Enter: Modo calibración (alterna 0 y 15 para calibrar los extremos de luz).
   * Escribe `T1000` y Enter: Ajusta el tiempo de permanencia a $1000\text{ ms}$ (valida el rango $[1, 10000]\text{ ms}$).
   * Escribe números del `0` al `15` y Enter: Fija ese código de 4 bits de forma estática.
8. **Prueba física con multímetro en los pines:**
   * Con el multímetro en voltios DC, mide entre cada pin digital y el pin `GND` de Arduino:
     - En código `'0'`: D8, D9, D10 y D11 deben marcar $0.00\text{ V}$.
     - En código `'15'`: D8, D9, D10 y D11 deben marcar $\approx 5.00\text{ V}$.
     - En código `'8'` ($1000_2$): D11 debe marcar $5.00\text{ V}$ y D8..D10 deben marcar $0.00\text{ V}$.
   * Una vez comprobado esto, el firmware está 100% verificado.

---

## 7. FASE 5: Montaje Físico Incremental en Protoboard (Paso a Paso)

Consulta el diagrama visual detallado en [guia_de_conexion_protoboard.md](file:///d:/Proyecto%20CC/docs/guia_de_conexion_protoboard.md). Sigue este orden para evitar errores:

### 1. Alimentación y Tierra Común (Req. P.17 y Q.18)
* Enciende la **Fuente de Laboratorio**, fija la salida en **$+5.00\text{ V}$** y limita la corriente a $0.25\text{ A}$.
* Conecta el caimán rojo de la fuente al riel rojo (`+VCC`) del protoboard.
* Conecta el caimán negro de la fuente al riel azul (`GND`) del protoboard.
* Conecta un cable desde el pin **`GND` del Arduino** al riel azul (`GND`) del protoboard.
* **RECORDATORIO:** Deja el pin `5V` del Arduino completamente desconectado.

### 2. Montaje del DAC R–2R (Etapa 2A)
* Conecta las 5 resistencias de $20\text{ k}\Omega$ (1 a tierra y 4 hacia D8, D9, D10, D11).
* Conecta las 3 resistencias horizontales de $10\text{ k}\Omega$ uniendo los nodos $V_1, V_2, V_3, V_4$.
* **Prueba:** Mide con el multímetro en el nodo $V_4$ enviando códigos por serial. Debe dar los 16 escalones ($0\text{ V}$ a $4.688\text{ V}$ en saltos de $0.3125\text{ V}$).

### 3. Montaje del Amplificador Operacional Dual LM358N (Etapa 2B y Etapa 3)
* Pin 8 a $+V_{CC}$ (Fuente de laboratorio) y Pin 4 a `GND`.
* **OpAmp 1A (DAC Activo):** Pin 3 a nodo $V_4$, Pin 2 puenteado a Pin 1.
* **OpAmp 1B (Interfaz Seguidora):** Pin 5 a Pin 1, Pin 6 puenteado a Pin 7.
* **Prueba:** Mide en Pin 7 del LM358. Debe entregar exactamente la misma tensión del DAC sin fluctuaciones.

### 4. Conexión del Generador Óptico (Etapa 4)
* Conecta la resistencia de $150\ \Omega$ desde el Pin 7 del LM358 al ánodo del LED rojo.
* Conecta el cátodo del LED rojo a `GND`.
* **Prueba:** Con código `'0'`, LED apagado. Con código `'15'`, LED encendido al máximo ($19\text{ mA}$).

### 5. Montaje del NE555 Astable y Receptor LDR (Etapas 5 y 6)
* Monta el NE555 en la mitad derecha del protoboard:
  - Pin 1 a `GND`, Pin 8 y Pin 4 a $+V_{CC}$ de laboratorio.
  - Pin 5 con condensador de $10\text{ nF}$ a `GND`.
  - Resistencia $R_1 = 1\text{ k}\Omega$ entre $+V_{CC}$ y Pin 7.
  - Puente entre Pin 2 y Pin 6. Condensador $C_1 = 100\text{ nF}$ entre Pin 2 y `GND`.
  - LDR conectada entre Pin 7 y Pin 6/2.
* Enfrenta el LED rojo y la LDR a través de la **fibra óptica plástica de $45\text{ cm}$** usando los acopladores 3D opacos para bloquear la luz del laboratorio.

### 6. Salida de Audio (Etapa 7)
* Conecta el condensador electrolítico de $10\ \mu\text{F}$ desde el Pin 3 del 555 hacia el polo positivo del **buzzer pasivo**.
* Conecta el polo negativo del buzzer a `GND`.
* **¡Momento de la verdad!** Pon el Arduino en modo `'A'` o `'P'`: escucharás cómo el buzzer reproduce las frecuencias audibles en sincronía con la luz transmitida.

---

## 8. FASE 6: Mediciones Experimentales y Validación de Error $< 10\%$ (Req. O.16)

Durante tus pruebas de laboratorio, llena esta tabla comparativa midiendo con multímetro y osciloscopio:

| Decimal ($D$) | $V_{te\acute{o}rico}$ (V) | $V_{simulado}$ SPICE (V) | $V_{medido}$ Lab (V) | Error Relativo ($E_{rel} \le 10\%$) | $f_{555}$ Medida (Hz) |
|:---:|:---:|:---:|:---:|:---:|:---:|
| **0** | $0.0000$ | $0.0000$ | ________ | Cumple | ________ |
| **1** | $0.3125$ | $0.3125$ | ________ | ________ % | ________ |
| **2** | $0.6250$ | $0.6250$ | ________ | ________ % | ________ |
| **4** | $1.2500$ | $1.2500$ | ________ | ________ % | ________ |
| **8** | $2.5000$ | $2.5000$ | ________ | ________ % | ________ |
| **12** | $3.7500$ | $3.7500$ | ________ | ________ % | ________ |
| **15** | $4.6875$ | $4.6875$ | ________ | ________ % | ________ |

Fórmula para calcular el error relativo:
$$E_{rel}(\%) = \left| \frac{V_{medido} - V_{te\acute{o}rico}}{V_{te\acute{o}rico}} \right| \times 100\%$$

---

## 9. FASE 7: Empaquetado Automático y Entrega Oficial (Sección 4.1 y 4.2)

Cuando vayas a entregar el proyecto:
1. **Ejecutar la Suite de Validación Integral:**
   ```bash
   python src/test_validacion_sistema.py
   ```
   Debe imprimir: `>>> RESULTADO FINAL: TODAS LAS PRUEBAS Y REQUERIMIENTOS (A.1–Q.18) 100% VALIDADOS <<<`.
2. **Generar la Carpeta Oficial y el Archivo `.zip` (Sección 4.1):**
   ```bash
   python src/empaquetar_entrega_oficial.py
   ```
   Creará automáticamente la carpeta `Investigacion_Dirigida/` con las 5 subcarpetas oficiales (`LTspice/`, `TinkerCAD/`, `Arduino/`, `Matlab/`, `Documentacion/`) y comprimirá `Investigacion_Dirigida.zip` listo para tecDigital.
3. **Compilar el Informe en Formato IEEE Transactions (Sección 4.2):**
   El archivo [`docs/informe_ieee/informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/docs/informe_ieee/informe_ieee_el2113.tex) contiene la estructura oficial completa ($\le 3$ páginas). Puedes compilarlo con `pdflatex` o subirlo a Overleaf para generar el PDF final.

---

## 10. Checklist Rápido de Verificación

- [ ] Alimentación conectada a la **Fuente de Laboratorio de la Bodega** ($5\text{ V}$, sin usar 5V de Arduino, Req. P.17/Q.18).
- [ ] Tierra `GND` de Arduino y Fuente de Laboratorio unidas en el protoboard.
- [ ] Red R–2R entrega los 16 niveles con error $< 10\%$ (Req. O.16).
- [ ] Amplificador operacional LM358N operando con OpAmp 1A (DAC activo) y OpAmp 1B (interfaz, Req. E.5 y F.6).
- [ ] LED rojo visible ($\lambda \approx 650\text{ nm}$, Req. G.7 y H.8) modula luz en función del voltaje.
- [ ] Fibra óptica plástica de $45\text{ cm}$ ($30\text{--}60\text{ cm}$, Req. I.9) acoplada herméticamente a la LDR pasiva (Req. J.10).
- [ ] Temporizador NE555 astable oscila en el rango audible ($178\text{ Hz}\text{--}3.6\text{ kHz}$, Req. K.11).
- [ ] Transductor acústico verificado como **buzzer piezoeléctrico pasivo** con condensador de desacoplo de $10\ \mu\text{F}$ (Req. L.12).
- [ ] Arreglos de longitud variable periódicos y aperiódicos probados desde MATLAB y Arduino (Req. C.3 y D.4).
- [ ] Piezas de corte láser e impresión 3D integradas en el soporte del prototipo (Req. M.13).
