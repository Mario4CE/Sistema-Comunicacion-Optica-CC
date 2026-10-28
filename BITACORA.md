# Bitácora de Desarrollo del Proyecto
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos Eléctricos en Corriente Continua (EL-2113)  
**Semestre:** II Semestre 2026  
**Profesor:** Ing. Fabián Alberto Picado García  
**Estudiante / Responsable:** Mario Cerdas  
**Repositorio Git:** `Sistema-Comunicacion-Optica-CC`  
**Rama Activa de Trabajo:** `mario-dev`  
**Estado General:** Fase de Simulación y Modelado Matemático 100% completada y validada; Adquisición física de componentes electrónicos y tornillería mecánica completada al 100%; Placa base organizadora en corte láser fabricada y recibida de LAIMI; Ensamble físico e integración en el chasis completado (Arduino UNO fijado con tornillos y tuercas M3, protoboard con circuito de 7 etapas montada sobre la base con evidencia fotográfica en [`Simulacion/tinkercad/CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoConBase.jpeg)); Kit 3D unificado de 8 piezas desarrollado y exportado ([`prototipo_laboratorio/impresion_3d/Atornillada.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/Atornillada.scad) / [`2022437519-01.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-01.stl)) con 2 torres acopladoras coaxiales M3 y 6 clips de fijación para la fibra de $45\text{ cm}$; Informe IEEE actualizado con plantilla para mediciones de laboratorio ([`docs/informe_ieee/informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/docs/informe_ieee/informe_ieee_el2113.tex)).

---

## 1. Registro Cronológico Detallado de Actividades y Avances

### [2026-10-02] - Sesión 1: Inicialización del Repositorio, Arquitectura Base y Primeras Simulaciones
* **Control de Versiones e Infraestructura:**
  * Se inicializó el repositorio local de Git en `D:\Proyecto CC` y se configuró el archivo `.gitignore` adaptado para excluir archivos temporales generados por compilación de Arduino, sesiones de MATLAB (`.asv`) y archivos de forma de onda y datos transitorios de LTspice (`.raw`, `.log`, `.op.raw`).
* **Etapa 1 (Sistema Digital):**
  * Se programó el firmware base en Arduino para el control de 4 pines digitales de salida (`D8` a `D11`) hacia la escalera R-2R, implementando modos operativo automático (barrido de 0 a 15), manual por monitor serial y modo de calibración de extremos (alternancia 0 y 15).
* **Modelado Numérico en MATLAB:**
  * Se desarrolló el script `matlab/simulacion_dac_555.m` que modela la escalera analógica del DAC R-2R (16 estados), la corriente del LED rojo transmisor, la variación de resistencia de la fotorresistencia receptora (LDR) y la frecuencia generada por el temporizador NE555 astable.
  * Se creó `matlab/generador_patrones.m` para generar señales sinusoidales cuantizadas a 4 bits, calcular la relación señal a ruido de cuantización teórica ($SQNR = 25.84\text{ dB}$) y exportar arreglos en formato de constantes en C++.
* **Memoria de Cálculo y Análisis Circuital:**
  * Se redactó `docs/memoria_de_calculo.md` con deducciones mediante equivalente de Thévenin y leyes de Kirchhoff, demostrando el colapso de tensión por efecto de carga ($> 98\%$ de atenuación) si no se utiliza un buffer seguidor de tensión con amplificador operacional (LM358N).
* **Simulación en LTspice y TinkerCAD:**
  * Se crearon los esquemáticos de simulación iniciales para validar el DAC con buffer y la etapa receptora osciladora.

---

### [2026-10-09] - Sesión 2: Simulación de Etapa Receptora (LDR + 555 Astable + Buzzer) en LTspice
* **Modelado de la Etapa Receptora:**
  * Se refinó el circuito `Simulacion/ltspice/receptor_555_ldr.asc` y su netlist `.cir` modelando el temporizador NE555 astable modulado por la resistencia óptica de la LDR.
  * Se implementó una directiva de barrido paramétrico `.step param R_LDR list 1.5k 5k 10k 20k 40k` para validar todo el rango de frecuencias audibles ($180\text{ Hz}$ a $3.6\text{ kHz}$) ante distintas intensidades de luz recibidas.
  * Se incorporó el modelo de acoplamiento AC mediante condensador de $10\ \mu\text{F}$ en serie con el buzzer pasivo para filtrar el nivel de corriente continua (DC) y proteger el transductor piezoeléctrico.

---

### [2026-10-10] - Sesión 3: Rama de Trabajo Colaborativa y Estandarización de Documentación de Código
* **Gestión de Ramas en Git:**
  * Se creó y publicó la rama de trabajo independiente `mario-dev` a partir de `main` para aislar el desarrollo de nuevas características y revisiones técnicas.
* **Suite de Validación Automatizada:**
  * Se desarrollaron pruebas de verificación estática y nodal para auditar la sintaxis de los programas `.ino`, emular bit a bit la función `aplicarBits()`, resolver las ecuaciones nodales de la red R-2R mediante Leyes de Kirchhoff y validar la respuesta del 555.
* **Estandarización Formal de Comentarios de Código:**
  * Se aplicó el estándar estricto de documentación en todos los archivos de código del proyecto (Arduino, TinkerCAD y MATLAB):
    * **Clases:** Encabezado con descripción del propósito y lista detallada de métodos y atributos.
    * **Funciones:** Bloque estructurado con campos obligatorios: **Descripción**, **Entrada**, **Salida** y **Restricciones**, complementado con comentarios de línea explicativos en operaciones críticas.

---

### [2026-10-11] - Sesión 4: Cumplimiento Normativo Integral (Req. A.1–Q.18), Modelos 3D/Láser e Informe IEEE
* **Arreglos de Longitud Variable en MATLAB y Firmware (Req. C.3 y D.4):**
  * Se reestructuró `matlab/generador_patrones.m` con las funciones documentadas `cuantizar_senal_4bits` e `imprimir_arreglo_cpp`, generando arreglos de longitud variable tanto para señales **periódicas** ($N=20$) como **aperiódicas** ($N=18$).
  * Se actualizó el firmware de Arduino para recorrer arreglos exportados desde MATLAB mediante los comandos `'P'` (periódico) y `'S'` (aperiódico), y se implementó la función `configurarTiempoPermanencia()` con el comando `'T<ms>'` que valida que el tiempo por palabra digital se mantenga en el rango estricto de $[1\text{ ms}, 10\,000\text{ ms}]$ ($1\text{ ms}$ a $10\text{ s}$).
* **Análisis Nodal de Kirchhoff y Separación de OpAmps (Req. E.5 y F.6):**
  * En `docs/memoria_de_calculo.md` se dedujo formalmente la función de transferencia del DAC R-2R resolviendo el sistema de 4 ecuaciones nodales (LCK) en los nodos $V_1, V_2, V_3, V_4$.
  * Se delimitó el uso de los dos amplificadores operacionales del encapsulado DIP-8 del **LM358N**: el **OpAmp 1A** conforma el componente activo de la Etapa 2 (DAC activo, Req. E.5), y el **OpAmp 1B** conforma la Etapa 3 (interfaz seguidora de aislamiento, Req. F.6).
* **Justificación Física del Color de la Fuente Luminosa (Req. H.8):**
  * Se fundamentó la selección del LED rojo visible ($\lambda \approx 650\text{ nm}$) mediante conceptos de Física 2 y Física 3: relación cuántica de Planck-Einstein ($E = hc/\lambda = 1.91\text{ eV} \implies V_D \approx 1.8\text{ V}$ para maximizar el rango dinámico útil), ley de Snell y ventana de mínima atenuación de la fibra plástica de PMMA en $650\text{ nm}$, y acoplamiento espectral con el sulfuro de cadmio (CdS) de la LDR.
* **Diseño Mecánico y Prototipado en la Escuela de Electrónica (Req. M.13 y N.14):**
  * Se diseñó el plano vectorial 2D para cortar en acrílico o MDF de $3\text{ mm}$, incorporando ranuras para guiar la fibra a $45\text{ cm}$ ($30\text{--}60\text{ cm}$) y grabado láser de las 7 etapas delimitadas (Req. B.2).
  * Se diseñaron los cabezales opacos cilíndricos para alinear concéntricamente el LED y la LDR con la fibra e inmunizar la fotorresistencia contra la luz ambiental del laboratorio.
* **Informe Técnico en Formato IEEE Transactions y Simulación Integrada:**
  * Se redactó el informe formal en LaTeX (`docs/informe_ieee/informe_ieee_el2113.tex`) con límite de 3 páginas y todas las secciones exigidas por la rúbrica.
  * Se creó la simulación integral `Simulacion/ltspice/sistema_completo_optico.asc` conectando las 7 etapas en conjunto.

---

### [2026-10-12] - Sesión 5: Simulación Funcional en TinkerCAD, Guías Operativas y Reorganización
* **Simulación en TinkerCAD Circuits:**
  * Se unificó el firmware para simulación web en `src/arduino/generador_patrones_tinkercad/generador_patrones_tinkercad.ino` integrando arreglos periódicos y aperiódicos de MATLAB, comandos interactivos y respuesta visual.
  * Se montó y verificó el circuito en la plataforma web de TinkerCAD, capturando los esquemas y diagramas de montaje (`Simulacion/tinkercad/CircuitoTinkercad.png`, `Componentes.png` y `Esquema.png`).
* **Limpieza y Reestructuración del Repositorio:**
  * Se reorganizó la estructura del proyecto renombrando la carpeta de simulaciones a `Simulacion/` con sus subcarpetas `ltspice/` y `tinkercad/`.

---

### [2026-10-14] - Sesión 6: Corrección y Calibración de LTspice, Macromodelo LM358 y Mediciones Automáticas
* **Diagnóstico y Corrección de Modelos SPICE:**
  * Se corrigieron los esquemáticos `dac_r2r_buffer.asc`, `receptor_555_ldr.asc` y `sistema_completo_optico.asc` junto con el netlist `sistema_completo_optico.cir`.
  * Se desarrolló el macromodelo `Simulacion/ltspice/LM358_simple.lib` que incorpora saturación de riel realista ($V_{OH} \approx V_{CC} - 1.5\text{ V}$) y comportamiento de saturación en $0\text{ V}$.
  * Se crearon archivos de directivas de medición automática `.meas`: `mediciones_dac.inc`, `mediciones_555.inc` y `mediciones_sistema.inc`.
  * Se redactó `Simulacion/ltspice/VERIFICACION.md` documentando los resultados de simulación: error del DAC $< 0.01\%$, error de frecuencia del 555 $< 1\%$, y frecuencia modulada del sistema completo entre $176.7\text{ Hz}$ (código 0) y $3443.0\text{ Hz}$ (código 15).
* **Calibración de Guías de Montaje:**
  * Se especificó alimentar el Pin 8 del LM358 con $+6.5\text{ V}\text{ a }+9.0\text{ V}$ desde el canal secundario de la fuente de laboratorio para garantizar linealidad plena hasta el código 15 ($4.688\text{ V}$), manteniendo el temporizador 555 en $+5.0\text{ V}$.
  * Se analizó la frecuencia de corte pasa-altos con el buzzer piezoeléctrico pasivo ($f_c \approx 16\text{ Hz}$ con transductor de $1\text{ k}\Omega$ y $C = 10\ \mu\text{F}$, preservando el contenido de audio en tonos graves).

---

### [2026-10-15] - Sesión 7: Refactorización de MATLAB, Compra de Componentes Físicos y Verificación
* **Refactorización de Scripts de MATLAB:**
  * Se revisaron descripciones, encabezados y comentarios de línea en `matlab/generador_patrones.m` y `matlab/simulacion_dac_555.m`, garantizando total consistencia con las 7 etapas del sistema y los resultados de simulación de LTspice.
* **Adquisición Física de Componentes y Materiales:**
  * Se procedió con la **compra y recolección física de la totalidad de los componentes electrónicos** requeridos para el montaje del prototipo: resistencias de $10\text{ k}\Omega$ y $20\text{ k}\Omega$ al 1%, LM358N, NE555P, LEDs rojos $5\text{ mm}$, fotorresistencias LDR GL5528, fibra POF de $45\text{ cm}$, condensadores y buzzer pasivo.
* **Inspección de Calidad y Pruebas Experimentales de Aceptación:**
  * **Prueba Crítica de Buzzer (Pasivo vs. Activo):** Se realizó la prueba de polarización directa conectando el buzzer a una fuente continua de $5\text{ V}$. Se verificó que el transductor **no oscila por sí mismo** (solo produce un chasquido mecánico seco al energizar y desenergizar), confirmando que es **100% pasivo** (Req. L.12).
  * **Verificación Óhmica de Resistencias:** Se muestrearon con multímetro digital las resistencias R ($10\text{ k}\Omega$) y 2R ($20\text{ k}\Omega$), registrando desviaciones menores al $0.4\%$.
  * **Inspección de la Fibra Óptica:** Se preparó el tramo de fibra cortándolo limpiamente a **$45\text{ cm}$** con corte transversal perpendicular a $90^\circ$ (Req. I.9).

---

### [2026-10-18] - Sesión 8: Optimización Visual, Despeje Nodal y Diagramación Profesional de Esquemáticos en LTspice
* **Auditoría Visual y Diagramación en LTspice:**
  * Se eliminaron colisiones tipográficas en etiquetas verticales (`B0..B3`, `VOP`, `VCC`, `RLDR`, `V_BUZZER`), extendiendo tramos de cables horizontales para forzar lectura limpia.
  * Se reubicaron componentes pasivos y etiquetas de pines del integrado NE555 y de la Etapa 7 de acoplo AC.
  * Se ejecutaron simulaciones automatizadas en modo batch (`LTspice.exe -Run -b`) validando la total convergencia y ausencia de errores de sintaxis en las netlists.

---

### [2026-10-19] - Sesión 9: Ensamble Físico Preliminar del Circuito en Protoboard
* **Montaje Electrónico en Protoboard:**
  * Se cablearon las etapas electrónicas del sistema sobre la placa de pruebas (protoboard de 830 puntos): salidas del Arduino ($D_8\text{--}D_{11}$), red ladder R-2R, LM358N (OpAmp 1A y OpAmp 1B), NE555P astable, condensadores y buzzer piezoeléctrico pasivo.
  * Se capturó la fotografía del prototipo preliminar (`Simulacion/tinkercad/CircuitoArmado.jpeg`).

---

### [2026-10-22] - Sesión 10: Adecuación a Normativa Oficial LAIMI (Corte Láser e Impresión 3D) y Visor 3D
* **Normativa LAIMI — Corte y Grabado Láser (Trotec Speedy 360):**
  * Se optimizó la placa base organizadora a dimensiones exactas de **$250\text{ mm} \times 200\text{ mm}$** (fracción de $1/8$ de lámina) con archivos oficiales en espacio RGB y grosor $0.025\text{ mm}$ (`prototipo_laboratorio/corte_laser/base_soporte_optico.pdf` y `.svg`).
* **Visor 3D Interactivo:**
  * Se desarrolló el visor interactivo Three.js (`prototipo_laboratorio/impresion_3d/visor_interactivo_3d.html`) para previsualizar piezas 3D en el navegador sin software externo.

---

### [2026-10-23] - Sesión 11: Tornillería M3, Recepción de Base Láser LAIMI y Ensamble en el Chasis
* **Fabricación y Recepción de la Base de Soporte en Corte Láser (LAIMI F4-05):**
  * La cortadora láser Trotec Speedy 360 procesó con éxito la lámina de $3.0\text{ mm}$ con contorno exterior a escuadra, orificios pasantes de $\varnothing 3.2\text{ mm}$, ranuras de sujeción para la fibra y grabado de las 7 etapas (Req. B.2 y M.13).
* **Montaje Físico y Ensamble Estructural sobre la Base Láser:**
  * **Fijación del Arduino UNO:** Se fijó la placa Arduino UNO rígidamente a la placa base mediante tornillos y tuercas M3.
  * **Instalación de Protoboard en Chasis:** Se montó la protoboard de 830 puntos con el circuito completo de las 7 etapas sobre el chasis delimitado.
  * **Preparación de Tornillos M3 para Torres 3D:** Se insertaron y ajustaron los 4 tornillos M3 con tuercas en la posición de las torres TX y RX de la base láser.
* **Evidencia Fotográfica del Chasis:**
  * Se capturó y subió al repositorio la fotografía [`Simulacion/tinkercad/CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoConBase.jpeg) evidenciando la integración del Arduino atornillado, la protoboard delimitada y la base de MDF.

---

### [2026-10-24] - Sesión 12 (HOY): Unificación del Kit 3D (8 Piezas), Limpieza del Repositorio y Plantilla IEEE
* **Diseño y Unificación del Kit 3D de 8 Piezas (`Atornillada.scad` / `2022437519-01.stl`):**
  * Se integró en un único archivo OpenSCAD paramétrico (`prototipo_laboratorio/impresion_3d/Atornillada.scad`) el conjunto completo de 8 piezas optimizado para manufactura en cama plana de $54.9\text{ mm} \times 43.6\text{ mm}$ sin soportes:
    1. 1x Torre Horizontal Atornillada M3 para Emisor TX (LED Rojo $5\text{ mm}$, cámara $\varnothing 6.2\text{ mm}$).
    2. 1x Torre Horizontal Atornillada M3 para Receptor RX (LDR GL5528, cámara $\varnothing 6.4\text{ mm}$).
    3. 3x Grapas/clips de retención transversales para las ranuras frente a las torres ($Z=6.0\text{ mm}$).
    4. 3x Grapas/clips de retención longitudinales para ranuras de $7\times 3\text{ mm}$ del arco exterior de la base láser, asegurando el tramo de fibra sin necesidad de gazas plásticas.
  * Se exportó la malla STL sólida oficial `prototipo_laboratorio/impresion_3d/2022437519-01.stl` cumpliendo con la normativa LAIMI Prusa MK4.
* **Reorganización de Firmware y Depuración del Repositorio:**
  * Se ubicó el sketch de Arduino en la estructura oficial `src/arduino/generador_patrones_tinkercad/generador_patrones_tinkercad.ino` para compatibilidad directa con Arduino IDE.
  * Se eliminaron enlaces rotos y referencias a documentos obsoletos en `README.md`.
* **Actualización del Informe Final IEEE (`informe_ieee_el2113.tex`):**
  * Se incorporó la columna experimental $V_{\text{exp}}$ en el Cuadro I de resultados para registrar las mediciones reales de laboratorio con multímetro.
  * Se dejó lista la estructura de autores con soporte para integrar los nombres completos de los miembros del grupo.

---

## 2. Inventario Físico de Componentes Adquiridos

| Ítem | Componente | Especificación / Modelo | Cantidad | Función Circuital (Etapa) | Estado de Verificación |
|:---:|:---|:---|:---:|:---|:---:|
| 1 | **Microcontrolador** | Arduino UNO R3 (ATmega328P) | 1 unid. | Etapa 1: Generador de patrones digitales de 4 bits | Verificado / Atornillado en base |
| 2 | **Resistencias Red R** | $10\text{ k}\Omega$ (1/4 W, 1% metal film) | 15 unid. | Etapa 2: Ramas horizontales de la red R-2R | Medido con multímetro: $9.98\text{ k}\Omega$ |
| 3 | **Resistencias Red 2R** | $20\text{ k}\Omega$ (1/4 W, 1% metal film) | 8 unid. | Etapa 2: Ramas verticales 2R y terminación | Medido con multímetro: $19.96\text{ k}\Omega$ |
| 4 | **Amplificador Op.** | LM358N (DIP-8, Texas Instruments) | 2 unid. | Etapa 2 (OpAmp 1A) y Etapa 3 (OpAmp 1B) | Nuevo en protoboard montada |
| 5 | **Resistencia Driver** | $150\ \Omega$ (1/4 W, 5%) | 2 unid. | Etapa 4: Limitación de corriente en el LED rojo | Medido: $149.3\ \Omega$ |
| 6 | **LED Emisor** | LED Rojo difuso $5\text{ mm}$ ($\lambda \approx 650\text{ nm}$) | 3 unid. | Etapa 4: Generador óptico ($V_D \approx 1.8\text{ V}$) | Probado a $10\text{ mA}$: emisión clara |
| 7 | **Fibra Óptica** | Fibra Óptica Plástica (POF, PMMA $1\text{ mm}$) | 1 m | Medio óptico guiado (tramo cortado a $45\text{ cm}$) | Inspección visual: corte recto a $90^\circ$ |
| 8 | **Fotorresistencia** | LDR GL5528 (Sulfuro de Cadmio CdS) | 2 unid. | Etapa 5: Receptor óptico pasivo | $R_{luz} \approx 1.2\text{ k}\Omega$, $R_{osc} \approx 45\text{ k}\Omega$ |
| 9 | **Temporizador IC** | NE555P (DIP-8, Texas Instruments) | 2 unid. | Etapa 6: Generador de onda cuadrada astable | Nuevo en protoboard montada |
| 10 | **Resistencia Carga 555**| $1\text{ k}\Omega$ (1/4 W, 5%) | 2 unid. | Etapa 6: Resistencia $R_1$ en pin 7 del 555 | Medido: $0.998\text{ k}\Omega$ |
| 11 | **Condensador Astable** | $100\text{ nF}$ ($0.1\ \mu\text{F}$, código `104`) | 3 unid. | Etapa 6: Temporización $C_1$ (pines 2 y 6 a tierra) | Cerámico multicapa verificado |
| 12 | **Condensador Filtro** | $10\text{ nF}$ ($0.01\ \mu\text{F}$, código `103`) | 2 unid. | Etapa 6: Filtro de ruido en pin 5 (Control) | Cerámico de disco verificado |
| 13 | **Condensador Acoplo** | $10\ \mu\text{F}$ ($25\text{ V}$ electrolítico radial) | 2 unid. | Etapa 7: Bloqueo de nivel DC hacia el buzzer | Polaridad verificada |
| 14 | **Transductor Acústico** | Buzzer piezoeléctrico PASIVO ($12\text{ mm}$) | 1 unid. | Etapa 7: Conversión final a sonido audible | **Prueba DC: Confirma buzzer pasivo** |
| 15 | **Protoboard** | Placa de pruebas 830 puntos | 1 unid. | Plataforma física de ensamble experimental | Montada en el chasis de MDF |
| 16 | **Cables Jumpers** | Jumpers Macho-Macho 24 AWG | 1 juego (65) | Conexión e interconexión entre etapas | Surtido de colores listo |
| 17 | **Tornillos y Tuercas M3** | Métricos `M3 x 10/12 mm` con tuercas | 8 juegos | Fijación de Arduino y torres 3D | 4 fijando Arduino / 4 listos para torres |
| 18 | **Placa Base Láser** | Base $250\times 200\text{ mm}$ en corte/grabado LAIMI | 1 unid. | Chasis soporte con 7 etapas delimitadas (Req. B.2/M.13) | **Entregada por LAIMI** / Totalmente integrada |
| 19 | **Kit Unificado 3D (8 pcs)**| 2 Torres M3 + 6 Clips de fibra (`2022437519-01.stl`) | 1 juego | Inmunización coaxial y retención de fibra ($45\text{ cm}$) | Malla y código paramétrico listos en `Atornillada.scad` |

---

## 3. Matriz Maestra de Control y Trazabilidad de Commits en Git

| N° | Hash | Fecha | Módulo / Etapa | Descripción del Cambio / Alcance Técnico | Archivos Clave Impactados |
|:---:|:---:|:---:|:---|:---|:---|
| **1** | `07ee165` | 2026-10-02 | Infraestructura / Base | Inicialización de la estructura del proyecto, código base en Arduino, scripts numéricos en MATLAB y memoria de cálculo teórica. | `.gitignore`, `src/arduino/`, `matlab/`, `docs/memoria_de_calculo.md` |
| **2** | `e5306b5` | 2026-10-02 | Simulación / Docs | Incorporación de la primera simulación SPICE para el DAC con buffer y actualización de la bitácora. | `Simulacion/ltspice/`, `BITACORA.md` |
| **3** | `b864f34` | 2026-10-02 | Simulación LTspice | Creación del esquemático gráfico `.asc` en LTspice para el DAC R-2R de 4 bits y el buffer seguidor con OpAmp. | `Simulacion/ltspice/dac_r2r_buffer.asc` |
| **4** | `b04ca30` | 2026-10-02 | Simulación TinkerCAD | Desarrollo del código C++ optimizado para simulación virtual en TinkerCAD Circuits. | `src/arduino/`, `Simulacion/tinkercad/` |
| **5** | `bcde82e` | 2026-10-02 | MATLAB / Etapa 1 | Implementación del generador de patrones de prueba analógicos y cuantización a 4 bits con exportación automática de arreglos a C++. | `matlab/generador_patrones.m` |
| **6** | `6c5a900` | 2026-10-02 | Documentación | Actualización de la bitácora registrando los avances sincronizados de LTspice, TinkerCAD y modelado matemático en MATLAB. | `BITACORA.md` |
| **7** | `cc960cb` | 2026-10-02 | Documentación | Creación de la guía de ejecución del proyecto e incorporación de imagen de referencia del circuito. | `docs/`, `Simulacion/` |
| **8** | `c77ce51` | 2026-10-02 | Documentación | Registro en bitácora de la guía maestra y actualización del historial de commits. | `BITACORA.md` |
| **9** | `8c9428f` | 2026-10-09 | Simulación LTspice | Diseño de la simulación SPICE para la etapa receptora: 555 astable modulado por LDR con barrido paramétrico y acoplo AC al buzzer. | `Simulacion/ltspice/receptor_555_ldr.asc`, `.cir` |
| **10** | `340c89c` | 2026-10-10 | Documentación | Actualización de la bitácora con los avances de la simulación del receptor 555 y sincronización remota en GitHub. | `BITACORA.md` |
| **11** | `d0f6b54` | 2026-10-10 | Pruebas / Firmware | Creación de la suite de validación automatizada y mejora del parser serial de Arduino con validación `isDigit()`. | `src/`, `src/arduino/` |
| **12** | `dda33c1` | 2026-10-10 | Estandarización Código | Estandarización de comentarios en todos los archivos C++, MATLAB y Python: Descripción, Entrada, Salida y Restricciones. | `src/arduino/`, `matlab/` |
| **13** | `93e94b5` | 2026-10-11 | Normativa / Modelos | Cumplimiento exhaustivo de requerimientos A.1–Q.18: soporte de arreglos periódicos/aperiódicos, modelos 3D/corte láser e informe IEEE. | `matlab/`, `src/arduino/`, `prototipo_laboratorio/`, `docs/informe_ieee/` |
| **14** | `378df73` | 2026-10-12 | Documentación / 3D | Actualización de guías operativas y generación inicial de la malla STL de acopladores 3D. | `docs/`, `prototipo_laboratorio/impresion_3d/` |
| **15** | `bbb6047` | 2026-10-12 | Diagramas / TinkerCAD | Incorporación de imagen corregida de TinkerCAD (con fuente de laboratorio y sin cable 5V de Arduino). | `docs/`, `Simulacion/tinkercad/` |
| **16** | `cf7e85a` | 2026-10-12 | Simulación TinkerCAD | Respaldo y ordenamiento de imágenes de referencia en la carpeta de TinkerCAD. | `Simulacion/tinkercad/` |
| **17** | `a0d1223` | 2026-10-12 | Firmware / TinkerCAD | Actualización y consolidación del firmware de Arduino como versión unificada para hardware y simulador. | `src/arduino/generador_patrones_tinkercad/` |
| **18** | `bcac6f6` | 2026-10-12 | Simulación TinkerCAD | Verificación funcional del circuito completo en TinkerCAD Circuits, agregando capturas del circuito, componentes y esquema. | `Simulacion/tinkercad/CircuitoTinkercad.png`, `Componentes.png`, `Esquema.png` |
| **19** | `749caa9` | 2026-10-12 | Reestructuración | Reestructuración de carpetas de simulación bajo el directorio unificado `Simulacion/` con subcarpetas `ltspice/` y `tinkercad/`. | `Simulacion/` |
| **20** | `1a6dfe1` | 2026-10-12 | Mantenimiento | Limpieza de archivos duplicados y obsoletos en el repositorio y actualización de la bitácora. | `BITACORA.md` |
| **21** | `3e73d60` | 2026-10-14 | Simulación LTspice | Corrección profunda de esquemáticos en LTspice: corrección de pines flotantes (`NC_xx`), macromodelo `LM358_simple.lib` y directivas `.meas`. | `Simulacion/ltspice/` (`LM358_simple.lib`, `VERIFICACION.md`, `.inc`, `.asc`) |
| **22** | `84eea96` | 2026-10-14 | Guías de Montaje | Documentación técnica de la alimentación de $+9.0\text{ V}$ para el pin 8 del LM358 para evitar saturación y análisis del buzzer pasivo. | `docs/memoria_de_calculo.md` |
| **23** | `e06d1b3` | 2026-10-14 | Informe IEEE / Memoria | Sincronización del informe IEEE Transactions y de la memoria de cálculo con las mediciones numéricas exactas obtenidas en LTspice. | `docs/informe_ieee/informe_ieee_el2113.tex`, `docs/memoria_de_calculo.md` |
| **24** | `a8dc060` | 2026-10-14 | Documentación | Actualización de la documentación general, alineación del README principal y actualización de las entradas de bitácora. | `BITACORA.md`, `README.md` |
| **25** | `409707d` | 2026-10-15 | MATLAB / Refactor | Refactorización de comentarios, descripciones y consistencia de encabezados en los scripts de MATLAB. | `matlab/generador_patrones.m`, `matlab/simulacion_dac_555.m` |
| **26** | `adc6aca` | 2026-10-15 | Documentación / Bitácora | Actualización integral de la bitácora con los 25 commits, trazabilidad completa A.1–Q.18 y detalle de compra de componentes físicos. | `BITACORA.md` |
| **27** | `12f262e` | 2026-10-18 | Simulación LTspice | Optimización visual, orientación horizontal de etiquetas y corrección de espaciado en esquemáticos del DAC y receptor 555. | `Simulacion/ltspice/dac_r2r_buffer.asc`, `Simulacion/ltspice/receptor_555_ldr.asc` |
| **28** | `9df55d5` | 2026-10-18 | Simulación LTspice | Rediseño espacial del sistema completo óptico de 7 etapas en LTspice, despejando pines de NE555 y acoplo AC. | `Simulacion/ltspice/sistema_completo_optico.asc` |
| **29** | `1a339b8` | 2026-10-18 | Documentación / LTspice | Actualización de la guía de verificación técnica con soporte de ejecución batch `-Run -b` y directrices de diagramación. | `Simulacion/ltspice/VERIFICACION.md` |
| **30** | `acb8f11` | 2026-10-18 | Documentación / Bitácora | Registro en bitácora de la optimización visual de esquemáticos LTspice y sincronización del estado del proyecto. | `BITACORA.md` |
| **31** | `1ef7c9b` | 2026-10-20 | Montaje Físico | Incorporación de fotografía de evidencia del ensamblaje físico preliminar en protoboard. | `BITACORA.md`, `Simulacion/tinkercad/CircuitoArmado.jpeg` |
| **32** | `0e23dae` | 2026-10-22 | Corte Láser LAIMI | Optimización de la placa base a fracción oficial 1/8 de lámina LAIMI ($250\times 200\text{ mm}$) y ajuste de capas RGB a $0.025\text{ mm}$. | `prototipo_laboratorio/corte_laser/base_soporte_optico.svg` |
| **33** | `5a1dcd0` | 2026-10-22 | Corte Láser LAIMI | Generación de archivo vectorial `.PDF` en modo RGB listo para cortadora Trotec Speedy 360 en LAIMI F4-05. | `prototipo_laboratorio/corte_laser/base_soporte_optico.pdf` |
| **34** | `8c90a5e` | 2026-10-22 | Impresión 3D LAIMI | Adecuación de torres acopladoras M3 a normativa Prusa MK4 de LAIMI y exportación con formato oficial `2022437519-01.stl`. | `prototipo_laboratorio/impresion_3d/` |
| **35** | `10a0486` | 2026-10-22 | Impresión 3D LAIMI | Diseño preliminar de capuchones ópticos directos para protoboard. | `prototipo_laboratorio/impresion_3d/` |
| **36** | `ba2cb64` | 2026-10-22 | Visualización 3D | Incorporación de render 3D de la maqueta física completa en laboratorio. | `prototipo_laboratorio/` |
| **37** | `3f25388` | 2026-10-22 | Visualización 3D | Incorporación de ilustración técnica comparativa de montaje optoelectrónico. | `prototipo_laboratorio/` |
| **38** | `3272145` | 2026-10-22 | Guía Protoboard | Mapeo de coordenadas exactas para protoboard de 60 filas (`0` a `60`, columnas `a` a `j`). | `docs/` |
| **39** | `20b3de1` | 2026-10-22 | Normativa LAIMI | Actualización de directrices con reglamento LAIMI F4-05 y tornillería M3 (`2022437519`). | `docs/` |
| **40** | `2377cba` | 2026-10-22 | Lista Materiales | Detalle de tornillería métrica `M3 x 10mm/12mm`, tuercas M3 y jumpers Hembra–Macho. | `docs/` |
| **41** | `b1bb2fe` | 2026-10-22 | Documentación | Vinculación del montaje virtual de TinkerCAD con coordenadas físicas y archivos de LAIMI. | `Simulacion/tinkercad/` |
| **42** | `a36e6b8` | 2026-10-22 | Documentación | Indexación en `README.md` de los archivos oficiales `.PDF` y `.STL` con carné `2022437519`. | `README.md` |
| **43** | `b47597b` | 2026-10-22 | Documentación / Bitácora | Registro en bitácora de la Sesión 10 (adecuación normativa LAIMI y visor 3D). | `BITACORA.md` |
| **44** | `8a1f05a` | 2026-10-22 | Corte Láser LAIMI | Limpieza del cajetín de identificación y reubicación de rótulos de etapas fuera de la huella del protoboard. | `prototipo_laboratorio/corte_laser/base_soporte_optico.pdf`, `.svg` |
| **45** | `5dafb14` | 2026-10-22 | Impresión 3D | Incorporación de visor 3D interactivo en HTML5 (`visor_interactivo_3d.html`) para previsualización de piezas STL sin software externo. | `prototipo_laboratorio/impresion_3d/visor_interactivo_3d.html` |
| **46** | `dc0a80a` | 2026-10-23 | Ensamble Mecánico | Adquisición de tornillería M3, recepción de base láser LAIMI, atornillado de Arduino y montaje de protoboard en chasis. | `BITACORA.md`, `README.md` |
| **47** | `c4f9474` | 2026-10-24 | Documentación | Sincronización del hash de commit de la sesión 11 en la bitácora. | `BITACORA.md` |
| **48** | `f91bec0` | 2026-10-24 | Limpieza / Ensamble | Limpieza de archivos temporales/obsoletos e incorporación de evidencia fotográfica del circuito con base láser (`CircuitoConBase.jpeg`). | `Simulacion/tinkercad/CircuitoConBase.jpeg`, limpieza de archivos |
| **49** | `bdfdcbc` | 2026-10-24 | Documentación | Registro de actividades de limpieza y mantenimiento en bitácora. | `BITACORA.md` |
| **50** | *(Local)* | 2026-10-24 | Kit 3D / IEEE / Docs | Unificación del kit 3D de 8 piezas (`Atornillada.scad` / `2022437519-01.stl`), reorganización del firmware Arduino, actualización de `README.md` y plantilla experimental en `informe_ieee_el2113.tex`. | `Atornillada.scad`, `2022437519-01.stl`, `README.md`, `informe_ieee_el2113.tex`, `BITACORA.md` |

---

## 4. Matriz de Cumplimiento de Requerimientos Oficiales (A.1 – Q.18)

| Requerimiento Oficial | Descripción Normativa | Estado | Artefacto / Archivo de Verificación |
|:---|:---|:---:|:---|
| **A.1 / B.2** | Sistema dividido en 7 etapas delimitadas e identificables | **CUMPLE** | Grabado láser en base física entregada ([`base_soporte_optico.pdf`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.pdf)), Arduino atornillado y protoboard montada en el chasis ([`CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoConBase.jpeg)). |
| **C.3 / D.4** | Arreglos de longitud variable en MATLAB y tiempo entre 1 ms y 10 s | **CUMPLE** | [`matlab/generador_patrones.m`](file:///d:/Proyecto%20CC/matlab/generador_patrones.m) y comandos `'P'`, `'S'`, `'T<ms>'` en [`generador_patrones_tinkercad.ino`](file:///d:/Proyecto%20CC/src/arduino/generador_patrones_tinkercad/generador_patrones_tinkercad.ino). |
| **E.5** | DAC implementado con componentes pasivos y un componente activo | **CUMPLE** | Red R-2R ($10\text{ k}\Omega / 20\text{ k}\Omega$ al 1%) + OpAmp 1A del LM358N ([`docs/memoria_de_calculo.md`](file:///d:/Proyecto%20CC/docs/memoria_de_calculo.md)). |
| **F.6** | Etapa de interfaz implementada únicamente con OpAmp | **CUMPLE** | OpAmp 1B del LM358N en configuración seguidora de aislamiento unitario ($A_v = 1$, $Z_{in} \to \infty$, $Z_{out} \approx 0\ \Omega$). |
| **G.7 / H.8** | Generador óptico con componentes pasivos y LED justificado con Física 2/3 | **CUMPLE** | Resistor $150\ \Omega$ + LED rojo ($650\text{ nm}$) justificado por Planck-Einstein ($1.91\text{ eV}$), ventana PMMA y fotoconducción CdS. |
| **I.9 / J.10** | Medio óptico de 30 a 60 cm y receptor óptico puramente pasivo | **CUMPLE** | Fibra plástica POF cortada a $45\text{ cm}$ y fotorresistencia LDR pasiva (GL5528). |
| **K.11 / L.12** | Oscilador comercial integrado con pasivos y salida en buzzer pasivo | **CUMPLE** | Temporizador NE555 astable ($177\text{--}3443\text{ Hz}$) + condensador $10\ \mu\text{F}$ + Buzzer pasivo verificado con prueba DC. |
| **M.13 / N.14** | Prototipado en Escuela de Electrónica (corte láser, impresión 3D, escáner) | **CUMPLE** | Base láser recibida de LAIMI; Arduino atornillado con M3; protoboard montada; kit 3D de 8 piezas diseñado y unificado ([`Atornillada.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/Atornillada.scad) / [`2022437519-01.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-01.stl)). |
| **Ñ.15 / O.16** | Montaje en protoboard y error de conversión DAC menor al 10% | **CUMPLE** | Protoboard montada en el chasis organizador, circuito completo de 7 etapas cableado y error simulado $< 0.01\%$ (plantilla experimental en [`informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/docs/informe_ieee/informe_ieee_el2113.tex)). |
| **P.17 / Q.18** | Prohibido usar 5V de Arduino y baterías; uso de fuente de laboratorio | **CUMPLE** | Cable 5V de Arduino desconectado ([`CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoConBase.jpeg)); alimentación desde Fuente DC de Laboratorio de bodega ($+5\text{ V} / +9\text{ V}$, tierra común). |

---

## 5. Próximos Pasos Planificados

1. **Retiro y Ensamble del Kit 3D Unificado de 8 Piezas:** Retirar en LAIMI las piezas impresas (`2022437519-01.stl`), atornillar las 2 torres coaxiales M3 a la placa base, insertar los 6 clips en las ranuras de $7\times 3\text{ mm}$ y acoplar el LED, la LDR y el tramo de fibra de $45\text{ cm}$.
2. **Medición Experimental en Laboratorio (Req. P.17/Q.18 y O.16):** Conectar la maqueta a la Fuente DC Regulada de Laboratorio de la Escuela de Electrónica ($+5.0\text{ V} / +9.0\text{ V}$) y registrar con multímetro digital los 16 voltajes de salida del DAC ($V_{\text{exp}}$) para llenar la columna de la tabla del informe IEEE.
3. **Compilación Final del Informe IEEE y Entrega:** Completar los autores, insertar las mediciones reales y compilar `Informe.pdf` ($\le 3$ páginas) para adjuntarlo en la estructura de entrega requerida.
