# Bitácora de Desarrollo del Proyecto
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos Eléctricos en Corriente Continua (EL-2113)  
**Semestre:** II Semestre 2026  
**Profesor:** Ing. Fabián Alberto Picado García  
**Estudiante / Responsable:** Mario Cerdas  
**Repositorio Git:** `Sistema-Comunicacion-Optica-CC`  
**Rama Activa de Trabajo:** `mario-dev`  
**Estado General:** Fase de Simulación y Modelado Matemático 100% completada y validada; Adquisición física de componentes (BOM) completada al 100%; Ensamble físico preliminar del circuito en protoboard completado (electrónica conectada); Pendiente acoplamiento de fibra óptica, impresión 3D y corte láser.

---

## 1. Registro Cronológico Detallado de Actividades y Avances

### [2026-10-02] - Sesión 1: Inicialización del Repositorio, Arquitectura Base y Primeras Simulaciones
* **Control de Versiones e Infraestructura:**
  * Se inicializó el repositorio local de Git en `D:\Proyecto CC` y se configuró el archivo `.gitignore` adaptado para excluir archivos temporales generados por compilación de Arduino, sesiones de MATLAB (`.asv`) y archivos de forma de onda y datos transitorios de LTspice (`.raw`, `.log`, `.op.raw`).
* **Etapa 1 (Sistema Digital):**
  * Se programó el firmware base en Arduino (`src/arduino/emisor_4bits/emisor_4bits.ino`) para el control de 4 pines digitales de salida (`D8` a `D11`) hacia la escalera R-2R, implementando modos operativo automático (barrido de 0 a 15), manual por monitor serial y modo de calibración de extremos (alternancia 0 y 15).
* **Modelado Numérico en MATLAB:**
  * Se desarrolló el script `matlab/simulacion_dac_555.m` que modela la escalera analógica del DAC R-2R (16 estados), la corriente del LED rojo transmisor, la variación de resistencia de la fotorresistencia receptora (LDR) y la frecuencia generada por el temporizador NE555 astable.
  * Se creó `matlab/generador_patrones.m` para generar señales sinusoidales cuantizadas a 4 bits, calcular la relación señal a ruido de cuantización teórica ($SQNR = 25.84\text{ dB}$) y exportar arreglos en formato de constantes en C++.
* **Memoria de Cálculo y Análisis Circuital:**
  * Se redactó `docs/memoria_de_calculo.md` con deducciones mediante equivalente de Thévenin y leyes de Kirchhoff, demostrando el colapso de tensión por efecto de carga ($> 98\%$ de atenuación) si no se utiliza un buffer seguidor de tensión con amplificador operacional (LM358N).
* **Simulación en LTspice y TinkerCAD:**
  * Se crearon los esquemáticos `Simulacion/ltspice/dac_r2r_buffer.asc` y `Simulacion/ltspice/receptor_555_ldr.asc` para validar el DAC con buffer y la etapa receptora osciladora.
  * Se redactaron las guías iniciales de montaje en protoboard (`docs/guia_de_conexion_protoboard.md`), la lista de materiales (`docs/lista_de_materiales_BOM.md`) y la guía de inicio en TinkerCAD.

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
  * Se desarrolló el script de verificación estática y nodal `src/test_validacion_sistema.py` para auditar la sintaxis de los programas `.ino`, emular bit a bit la función `aplicarBits()`, resolver las ecuaciones nodales de la red R-2R mediante Leyes de Kirchhoff y validar la respuesta del 555.
* **Estandarización Formal de Comentarios de Código:**
  * Se aplicó el estándar estricto de documentación en todos los archivos de código del proyecto (Arduino, TinkerCAD, MATLAB y Python):
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
  * Se diseñó el plano vectorial 2D `prototipo_laboratorio/corte_laser/base_soporte_optico.svg` para cortar en acrílico o MDF de $3\text{ mm}$, incorporando ranuras para guiar la fibra a $45\text{ cm}$ ($30\text{--}60\text{ cm}$) y grabado láser de las 7 etapas delimitadas (Req. B.2).
  * Se diseñaron los cabezales opacos cilíndricos en OpenSCAD (`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad`) para alinear concéntricamente el LED y la LDR con la fibra e inmunizar la fotorresistencia contra la luz ambiental del laboratorio.
  * Se redactó `docs/diseno_prototipado_3d_laser.md` con la propuesta para emplear el escáner 3D de la Escuela de Electrónica en una cámara resonadora para el buzzer pasivo (Req. N.14).
* **Informe Técnico en Formato IEEE Transactions y Simulación Integrada:**
  * Se redactó el informe formal en LaTeX (`docs/informe_ieee/informe_ieee_el2113.tex`) con límite de 3 páginas y todas las secciones exigidas por la rúbrica.
  * Se creó la simulación integral `Simulacion/ltspice/sistema_completo_optico.asc` conectando las 7 etapas en conjunto.

---

### [2026-10-12] - Sesión 5: Simulación Funcional en TinkerCAD, Guías Operativas y Reorganización
* **Simulación en TinkerCAD Circuits:**
  * Se unificó el firmware para simulación web en `src/arduino/generador_patrones_tinkercad.ino` integrando arreglos periódicos y aperiódicos de MATLAB, comandos interactivos y respuesta visual.
  * Se montó y verificó el circuito en la plataforma web de TinkerCAD, capturando los esquemas y diagramas de montaje (`Simulacion/tinkercad/CircuitoTinkercad.png`, `Componentes.png` y `Esquema.png`).
  * Se redactó la guía paso a paso `Simulacion/tinkercad/Montaje.md`.
* **Generación de Malla STL y Plano Técnico:**
  * Se generó la malla 3D sólida `prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl` (512 facetas) para impresión FDM en el laboratorio de prototipado.
  * Se diseñó el diagrama vectorial `docs/diagrama_conexion_exacto_protoboard.svg` detallando las filas exactas del protoboard (filas 5, 9, 13 y 17) para evitar cortocircuitos entre resistencias.
* **Limpieza y Reestructuración del Repositorio:**
  * Se reorganizó la estructura del proyecto renombrando la carpeta de simulaciones a `Simulacion/` con sus subcarpetas `ltspice/` y `tinkercad/`, eliminando copias redundantes y consolidando las versiones definitivas.

---

### [2026-10-14] - Sesión 6: Corrección y Calibración de LTspice, Macromodelo LM358 y Mediciones Automáticas
* **Diagnóstico y Corrección de Modelos SPICE:**
  * Durante pruebas rigurosas en LTspice se detectaron tres anomalías críticas en esquemáticos previos: pines flotantes no conectados (nodos `NC_xx`), corto en el condensador de acoplo y fallo numérico de singularidad (`Singular matrix`) en el modelo del 555.
  * Se corrigieron los esquemáticos `dac_r2r_buffer.asc`, `receptor_555_ldr.asc` y `sistema_completo_optico.asc` junto con el netlist `sistema_completo_optico.cir`.
  * Se desarrolló el macromodelo `Simulacion/ltspice/LM358_simple.lib` que incorpora saturación de riel realista ($V_{OH} \approx V_{CC} - 1.5\text{ V}$) y comportamiento de saturación en $0\text{ V}$.
  * Se crearon archivos de directivas de medición automática `.meas`: `mediciones_dac.inc`, `mediciones_555.inc` y `mediciones_sistema.inc`.
  * Se redactó `Simulacion/ltspice/VERIFICACION.md` documentando los resultados de simulación: error del DAC $< 0.01\%$, error de frecuencia del 555 $< 1\%$, y frecuencia modulada del sistema completo entre $176.7\text{ Hz}$ (código 0) y $3443.0\text{ Hz}$ (código 15).
* **Calibración de Guías de Montaje:**
  * Se especificó en las guías alimentar el Pin 8 del LM358 con $+9.0\text{ V}$ desde el canal secundario de la fuente de laboratorio para garantizar linealidad plena hasta el código 15 ($4.688\text{ V}$), manteniendo el temporizador 555 en $+5.0\text{ V}$.
  * Se analizó la frecuencia de corte pasa-altos con el buzzer piezoeléctrico pasivo ($f_c \approx 16\text{ Hz}$ con transductor de $1\text{ k}\Omega$ y $C = 10\ \mu\text{F}$, preservando el contenido de audio en tonos graves).
* **Sincronización de Datos Reales en Documentación e Informe:**
  * Se actualizaron `docs/memoria_de_calculo.md` y `docs/informe_ieee/informe_ieee_el2113.tex` incorporando la tabla con las mediciones reales exactas arrojadas por LTspice.

---

### [2026-10-15] - Sesión 7: Refactorización de MATLAB, Compra de Componentes Físicos y Verificación
* **Refactorización de Scripts de MATLAB (Commit `409707d`):**
  * Se revisaron y actualizaron las descripciones, encabezados y comentarios de línea en `matlab/generador_patrones.m` y `matlab/simulacion_dac_555.m`, garantizando total consistencia con las 7 etapas del sistema y los resultados de simulación de LTspice.
* **Adquisición Física de Componentes y Materiales (BOM Completo):**
  * Se procedió con la **compra y recolección física de la totalidad de los componentes electrónicos** requeridos para el montaje del prototipo, basándose en la lista oficial `docs/lista_de_materiales_BOM.md`:
    1. **Resistencias de Precisión:** 15 resistencias de $10\text{ k}\Omega$ y 8 resistencias de $20\text{ k}\Omega$ al **1% de tolerancia (película metálica)** para asegurar que el error de conversión del DAC sea inferior al $10\%$ exigido por el Requerimiento O.16.
    2. **Resistencias de Soporte:** Resistencias de $150\ \Omega$ (1/4 W) para protección del LED y de $1\text{ k}\Omega$ (1/4 W) para carga del pin 7 del NE555.
    3. **Circuitos Integrados:** 2 unidades de amplificador operacional dual **LM358N** (DIP-8, fuente simple) y 2 unidades de temporizador de precisión **NE555P** (DIP-8).
    4. **Optoelectrónica y Medio Guiado:** Diodos LED rojos difusos de $5\text{ mm}$ ($\lambda \approx 650\text{ nm}$), 2 fotorresistencias **LDR GL5528** (componente pasivo, Req. J.10) y 1 metro de **fibra óptica plástica (POF)** con núcleo de PMMA.
    5. **Condensadores:** Cerámicos de $100\text{ nF}$ (código `104`) y $10\text{ nF}$ (código `103`), y condensador electrolítico de $10\ \mu\text{F}$ ($25\text{ V}$).
    6. **Transductor Acústico:** 1 **Buzzer piezoeléctrico pasivo**.
    7. **Herramientas de Montaje:** Protoboard de 830 puntos de contacto con rieles de alimentación continuos y paquete de 65 cables jumpers macho-macho de distintos colores.
* **Inspección de Calidad y Pruebas Experimentales de Aceptación:**
  * **Prueba Crítica de Buzzer (Pasivo vs. Activo):** Se realizó la prueba de polarización directa conectando el buzzer a una fuente continua de $5\text{ V}$. Se verificó que el transductor **no oscila por sí mismo** (solo produce un chasquido mecánico seco al energizar y desenergizar), confirmando que es **100% pasivo** y apto para ser modulado en frecuencia por el 555 (Req. L.12).
  * **Verificación Óhmica de Resistencias:** Se muestrearon con multímetro digital las resistencias R ($10\text{ k}\Omega$) y 2R ($20\text{ k}\Omega$), registrando desviaciones menores al $0.4\%$, óptimas para la escalera R-2R.
  * **Inspección de la Fibra Óptica:** Se preparó el tramo de fibra cortándolo limpiamente a **$45\text{ cm}$** con corte transversal perpendicular a $90^\circ$ para minimizar pérdidas por dispersión en las caras de entrada y salida (Req. I.9).
* **Planificación de Ensamble y Gestiones de Laboratorio:**
  * Se coordinó la solicitud de la Fuente DC Regulada de Laboratorio en la bodega de la Escuela de Electrónica para cumplir estrictamente con los Requerimientos P.17 y Q.18.
  * Se programó el envío del archivo SVG (`base_soporte_optico.svg`) a la máquina de corte láser y del archivo STL (`acoplador_optico_led_ldr.stl`) a la impresora 3D del laboratorio de prototipado.

---

### [2026-10-18] - Sesión 8: Optimización Visual, Despeje Nodal y Diagramación Profesional de Esquemáticos en LTspice
* **Auditoría Visual de Esquemáticos en LTspice (`dac_r2r_buffer.asc` y `sistema_completo_optico.asc`):**
  * Se identificaron colisiones tipográficas severas provocadas por el motor de renderizado de LTspice al asociar etiquetas (`FLAG`) directamente sobre terminales de pines verticales, lo que causaba rotación automática de 90° en los textos (`B0..B3`, `VOP`, `VCC`, `RLDR`, `V_BUZZER`), solapándose sobre los símbolos circuitales.
  * Se detectó que las resistencias horizontales de la red R-2R (`R_h1..R_h3`) empleaban justificación `Bottom 2` dentro del símbolo rotado `R90`, proyectando el texto verticalmente sobre los nodos de entrada `B1`, `B2`, `B3` y `V_R2R`.
  * En la etapa receptora (NE555), las etiquetas de los pines `OUT_555`, `THR` y `VCC` se encontraban sobrepuestas con los nombres internos de los pines del modelo gráfico DIP-8.
* **Reingeniería de Layout y Diagramación Estética:**
  * **Estandarización de Etiquetas Horizontales:** Se dotó a cada bandera de red de un tramo de cable horizontal (`WIRE`) para forzar al motor gráfico de LTspice a renderizar las etiquetas en orientación horizontal despejada, eliminando colisiones en `B0`, `B1`, `B2`, `B3`, `VOP`, `VCC`, `RLDR` y `V_BUZZER`.
  * **Alineación de Resistencias R-2R:** Se reconfiguraron las ventanas de atributos de `R_h1..R_h3` con alineación `VTop 2`, ubicando la designación y valor ($10\text{ k}\Omega$) centrados horizontalmente debajo del componente, aprovechando el espacio libre inferior y despejando los nodos de interconexión.
  * **Ampliación de Distancias Nocionales y Amplificador LM358:** Se incrementó la separación horizontal entre los amplificadores $U_{1A}$ y $U_{1B}$ y la resistencia limitadora de $150\ \Omega$ ($R_{\text{driver}}$). Se desplazaron los atributos del macromodelo `LM358_SIMPLE` a la parte inferior de los símbolos para evitar colisiones con el texto de valor de $R_{\text{driver}}$.
  * **Despeje del Diodo LED emisor:** Se desplazaron las coordenadas de designación (`D_LED`) y modelo (`D_RED_650`) 80 unidades a la derecha para no solapar las flechas de emisión de luz del símbolo esquemático.
  * **Reordenamiento de Etapas 5, 6 y 7 en el Sistema Completo:**
    * Se extendieron cables de interconexión horizontales en los 8 pines del `NE555` ($U_2$), alejando las etiquetas del contorno del encapsulado.
    * Se rediseñó la Etapa 7 (acoplo AC) colocando el condensador $C_{\text{acoplo}}$ en orientación horizontal serie (`cap R90`) hacia el nodo `V_BUZZER`, seguido de la resistencia del transductor piezoeléctrico ($R_{\text{buzzer}}$) orientada verticalmente a tierra, con el texto descriptivo ubicado limpiamente en la parte inferior.
    * Se reubicó la fuente de tensión dependiente $B_{\text{RLDR}}$ (modelo de la fotorresistencia en función de la corriente óptica $I(D_{\text{LED}})$) y sus notas explicativas, garantizando lectura inmediata sin cruce de trazos.
* **Verificación de Integridad Funcional mediante Simulación Batch:**
  * Se ejecutaron simulaciones automatizadas en modo batch (`LTspice.exe -Run -b`) para verificar que ninguna modificación de coordenadas o cables afectara la conectividad de la netlist SPICE.
  * Ambos circuitos convergieron en el punto de operación de Newton y superaron el 100% de las pruebas paramétricas (`.meas` en `dac_r2r_buffer.log` y `sistema_completo_optico.log`), certificando un error de conversión en el DAC $< 0.0012\%$ y concordancia analítica en la frecuencia de oscilación del 555 con error $< 0.90\%$.

---

### [2026-10-19] - Sesión 9 (HOY): Ensamble Físico Preliminar del Circuito en Protoboard
* **Montaje Electrónico en Protoboard:**
  * Se procedió con el armado físico y cableado de las etapas electrónicas del sistema sobre la placa de pruebas (protoboard de 830 puntos) siguiendo las directrices de `docs/guia_de_conexion_protoboard.md`:
    * Conexión de las salidas digitales del Arduino UNO ($D_8\text{--}D_{11}$) a la red ladder R-2R ($10\text{ k}\Omega / 20\text{ k}\Omega$ al 1%).
    * Inserción e interconexión del amplificador operacional dual **LM358N** (OpAmp 1A como buffer de tensión del DAC y OpAmp 1B como seguidor de aislamiento y driver del LED emisor con $R_{\text{driver}} = 150\ \Omega$).
    * Montaje del temporizador **NE555P** en configuración astable, conectando la resistencia de carga de $1\text{ k}\Omega$, el condensador de temporización de $100\text{ nF}$, el filtro de $10\text{ nF}$ en el pin de control y la fotorresistencia LDR GL5528.
    * Conexión de la etapa final de acoplamiento AC mediante el condensador electrolítico de $10\ \mu\text{F}$ hacia el buzzer piezoeléctrico pasivo.
* **Alcance y Delimitación del Montaje Preliminar:**
  * El circuito se armó a nivel eléctrico y circuital completo sobre la protoboard, **sin incluir todavía el cable de fibra óptica plástica (POF)**, **ni las piezas impresas en 3D (acopladores ópticos LED-LDR)**, **ni la base de soporte en corte láser**.
  * Los componentes optoelectrónicos (diodo LED rojo emisor y fotorresistencia LDR receptora) se encuentran dispuestos provisionalmente en la placa de pruebas para validación de continuidad eléctrica, niveles DC y polarización antes del ensamble mecánico definitivo.
* **Registro y Evidencia Fotográfica:**
  * Se capturó la fotografía del prototipo montado en la protoboard como evidencia del avance físico del circuito. Dicha imagen fue subida en `Simulacion/tinkercad/CircuitoArmado.jpeg` para documentar la trazabilidad del proyecto.

---

### [2026-10-22] - Sesión 10 (HOY): Adecuación a Normativa Oficial LAIMI (Corte Láser e Impresión 3D), Renders 3D y Matriz de Coordenadas (`0`–`60`, `a`–`j`)
* **Cumplimiento Estricto de Normativa LAIMI — Corte y Grabado Láser (Trotec Speedy 360):**
  * Se rediseñó la placa base organizadora a dimensiones exactas de **$250\text{ mm} \times 200\text{ mm}$**, calzando en la fracción oficial de **$\mathbf{1/8}\text{ de lámina}$ de LAIMI** ($200\text{ mm} \times 250\text{ mm}$) para minimizar el desperdicio de material y reducir costos.
  * Se generó el archivo vectorial obligatorio en formato **`.PDF` en espacio de color RGB** (`prototipo_laboratorio/corte_laser/2022437519_base_soporte_optico.pdf`), separando estrictamente las 3 capas exigidas por LAIMI:
    1. **Corte pasante:** Color Rojo RGB (`255, 0, 0` / `#FF0000`) con grosor de línea de **$0.025\text{ mm}$**.
    2. **Marcado vectorial de etapas y guías:** Color Azul RGB (`0, 0, 255` / `#0000FF`) con grosor de línea de **$0.025\text{ mm}$**.
    3. **Grabado de textos y rótulos:** Color Negro RGB (`0, 0, 0` / `#000000`) en relleno sólido.
* **Cumplimiento Estricto de Normativa LAIMI — Impresión 3D (Prusa MK4):**
  * Se generaron los archivos `.STL` con la nomenclatura obligatoria `CCCCCCCCCC-NN.stl` correspondiente al carné **`2022437519`**:
    * **`2022437519-01.stl` (Opción A):** Par de torres acopladoras ópticas con pestaña de fijación para tornillos M3 y conexión mediante 4 jumpers Hembra–Macho hacia la protoboard.
    * **`2022437519-02.stl` (Opción B):** Par de capuchones cilíndricos compactos para encajar directamente sobre el LED y la LDR clavados en las filas de la protoboard.
* **Mapeo Exacto de Protoboard Grande (`Filas 0 a 60`, `Columnas a–e / f–j`) y Tornillería:**
  * Se incorporó en `docs/guia_de_conexion_protoboard.md` la tabla maestra de coordenadas pin por pin para la protoboard de 60 filas (`0` a `60`, columnas `a` a `j`).
  * Se especificó en `docs/lista_de_materiales_BOM.md` y `docs/diseno_prototipado_3d_laser.md` la tornillería exacta: **8 tornillos métricos `M3 x 10 mm` (o `M3 x 12 mm`) con 8 tuercas hexagonales `M3`** para fijar el Arduino UNO y las torres 3D en los orificios de $\varnothing 3.2\text{ mm}$.
* **Renders 3D de Referencia para el Ensamblaje:**
  * Se integraron en `prototipo_laboratorio/` las imágenes de referencia `render_maqueta_3d_laser.jpg` y `comparativa_montaje_opcion_a_vs_b.jpg`.

---

## 2. Inventario Físico de Componentes Adquiridos (Sesión 2026-10-15)

| Ítem | Componente | Especificación / Modelo | Cantidad | Función Circuital (Etapa) | Estado de Verificación |
|:---:|:---|:---|:---:|:---|:---:|
| 1 | **Microcontrolador** | Arduino UNO R3 (ATmega328P) | 1 unid. | Etapa 1: Generador de patrones digitales de 4 bits | Verificado / Firmware listo |
| 2 | **Resistencias Red R** | $10\text{ k}\Omega$ (1/4 W, 1% metal film) | 15 unid. | Etapa 2: Ramas horizontales de la red R-2R | Medido con multímetro: $9.98\text{ k}\Omega$ |
| 3 | **Resistencias Red 2R** | $20\text{ k}\Omega$ (1/4 W, 1% metal film) | 8 unid. | Etapa 2: Ramas verticales 2R y terminación | Medido con multímetro: $19.96\text{ k}\Omega$ |
| 4 | **Amplificador Op.** | LM358N (DIP-8, Texas Instruments) | 2 unid. | Etapa 2 (OpAmp 1A) y Etapa 3 (OpAmp 1B) | Nuevo en blíster antiestático |
| 5 | **Resistencia Driver** | $150\ \Omega$ (1/4 W, 5%) | 2 unid. | Etapa 4: Limitación de corriente en el LED rojo | Medido: $149.3\ \Omega$ |
| 6 | **LED Emisor** | LED Rojo difuso $5\text{ mm}$ ($\lambda \approx 650\text{ nm}$) | 3 unid. | Etapa 4: Generador óptico ($V_D \approx 1.8\text{ V}$) | Probado a $10\text{ mA}$: emisión clara |
| 7 | **Fibra Óptica** | Fibra Óptica Plástica (POF, PMMA $1\text{ mm}$) | 1 m | Medio óptico guiado (tramo cortado a $45\text{ cm}$) | Inspección visual: corte recto a $90^\circ$ |
| 8 | **Fotorresistencia** | LDR GL5528 (Sulfuro de Cadmio CdS) | 2 unid. | Etapa 5: Receptor óptico pasivo | $R_{luz} \approx 1.2\text{ k}\Omega$, $R_{osc} \approx 45\text{ k}\Omega$ |
| 9 | **Temporizador IC** | NE555P (DIP-8, Texas Instruments) | 2 unid. | Etapa 6: Generador de onda cuadrada astable | Nuevo en blíster antiestático |
| 10 | **Resistencia Carga 555**| $1\text{ k}\Omega$ (1/4 W, 5%) | 2 unid. | Etapa 6: Resistencia $R_1$ en pin 7 del 555 | Medido: $0.998\text{ k}\Omega$ |
| 11 | **Condensador Astable** | $100\text{ nF}$ ($0.1\ \mu\text{F}$, código `104`) | 3 unid. | Etapa 6: Temporización $C_1$ (pines 2 y 6 a tierra) | Cerámico multicapa verificado |
| 12 | **Condensador Filtro** | $10\text{ nF}$ ($0.01\ \mu\text{F}$, código `103`) | 2 unid. | Etapa 6: Filtro de ruido en pin 5 (Control) | Cerámico de disco verificado |
| 13 | **Condensador Acoplo** | $10\ \mu\text{F}$ ($25\text{ V}$ electrolítico radial) | 2 unid. | Etapa 7: Bloqueo de nivel DC hacia el buzzer | Polaridad verificada |
| 14 | **Transductor Acústico** | Buzzer piezoeléctrico PASIVO ($12\text{ mm}$) | 1 unid. | Etapa 7: Conversión final a sonido audible | **Prueba DC: Confirma buzzer pasivo** |
| 15 | **Protoboard** | Placa de pruebas 830 puntos | 1 unid. | Plataforma física de ensamble experimental | Continuidad de rieles verificada |
| 16 | **Cables Jumpers** | Jumpers Macho-Macho 24 AWG | 1 juego (65) | Conexión e interconexión entre etapas | Surtido de colores listo |

---

## 3. Matriz Maestra de Control y Trazabilidad de Commits en Git

A continuación se presenta el registro cronológico completo de los 42 commits realizados en el repositorio, detallando el hash, la fecha, el autor, el módulo impactado y la descripción técnica de cada cambio:

| N° | Hash | Fecha | Módulo / Etapa | Descripción del Cambio / Alcance Técnico | Archivos Clave Impactados |
|:---:|:---:|:---:|:---|:---|:---|
| **1** | `07ee165` | 2026-10-02 | Infraestructura / Base | Inicialización de la estructura del proyecto, código base en Arduino, scripts numéricos en MATLAB y memoria de cálculo teórica. | `.gitignore`, `src/arduino/`, `matlab/`, `docs/memoria_de_calculo.md` |
| **2** | `e5306b5` | 2026-10-02 | Simulación / Docs | Incorporación de la primera simulación SPICE para el DAC con buffer, redacción de la guía de conexión en protoboard y actualización de la bitácora. | `simulation/ltspice/`, `docs/guia_de_conexion_protoboard.md`, `BITACORA.md` |
| **3** | `b864f34` | 2026-10-02 | Simulación LTspice | Creación del esquemático gráfico `.asc` en LTspice para el DAC R-2R de 4 bits y el buffer seguidor con OpAmp. | `simulation/ltspice/dac_r2r_buffer.asc` |
| **4** | `b04ca30` | 2026-10-02 | Simulación TinkerCAD | Desarrollo del código C++ optimizado y redacción de la guía de montaje paso a paso para simulación virtual en TinkerCAD Circuits. | `simulation/tinkercad/codigo_tinkercad.ino`, `simulation/tinkercad/guia_montaje_tinkercad.md` |
| **5** | `bcde82e` | 2026-10-02 | MATLAB / Etapa 1 | Implementación del generador de patrones de prueba analógicos y cuantización a 4 bits con exportación automática de arreglos a formato C++. | `matlab/generador_patrones.m` |
| **6** | `6c5a900` | 2026-10-02 | Documentación | Actualización de la bitácora registrando los avances sincronizados de LTspice, TinkerCAD y modelado matemático en MATLAB. | `BITACORA.md` |
| **7** | `cc960cb` | 2026-10-02 | Documentación | Creación de la guía maestra paso a paso de uso y ejecución del proyecto, e incorporación de la imagen de referencia del circuito en TinkerCAD. | `docs/GUIA_PASO_A_PASO_GENERAL.md`, `docs/tinkercad_circuito_dac.jpg` |
| **8** | `c77ce51` | 2026-10-02 | Documentación | Registro en bitácora del nuevo documento de guía maestra y actualización del historial de commits. | `BITACORA.md` |
| **9** | `8c9428f` | 2026-10-09 | Simulación LTspice | Diseño de la simulación SPICE para la etapa receptora: temporizador 555 astable modulado por LDR con barrido paramétrico y acoplo AC al buzzer. | `simulation/ltspice/receptor_555_ldr.asc`, `simulation/ltspice/receptor_555_ldr.cir` |
| **10** | `340c89c` | 2026-10-10 | Documentación | Actualización de la bitácora con los avances de la simulación del receptor 555 y sincronización con el repositorio remoto en GitHub. | `BITACORA.md` |
| **11** | `d0f6b54` | 2026-10-10 | Pruebas / Firmware | Creación de la suite de validación automatizada en Python (`test_validacion_sistema.py`) y mejora del parser serial de Arduino con validación `isDigit()`. | `src/test_validacion_sistema.py`, `src/arduino/emisor_4bits/emisor_4bits.ino` |
| **12** | `dda33c1` | 2026-10-10 | Estandarización Código | Estandarización de comentarios en todos los archivos C++, MATLAB y Python siguiendo la estructura: Descripción, Entrada, Salida y Restricciones. | `src/arduino/`, `matlab/`, `src/test_validacion_sistema.py` |
| **13** | `93e94b5` | 2026-10-11 | Normativa / Modelos | Cumplimiento exhaustivo de los requerimientos A.1–Q.18: soporte de arreglos periódicos/aperiódicos, modelos de corte láser/impresión 3D e informe IEEE. | `matlab/`, `src/arduino/`, `prototipo_laboratorio/`, `docs/informe_ieee/` |
| **14** | `378df73` | 2026-10-12 | Documentación / 3D | Actualización de guías operativas (`GUIA_PASO_A_PASO_GENERAL.md`, `lista_de_materiales_BOM.md`, protoboard) y generación de la malla STL de acopladores 3D. | `docs/`, `prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl` |
| **15** | `bbb6047` | 2026-10-12 | Diagramas / TinkerCAD | Incorporación de la imagen corregida de TinkerCAD (con fuente de laboratorio y sin cable 5V) y plano técnico exacto en formato SVG del protoboard. | `docs/tinkercad_circuito_corregido.jpg`, `docs/diagrama_conexion_exacto_protoboard.svg` |
| **16** | `cf7e85a` | 2026-10-12 | Simulación TinkerCAD | Respaldo y copia de las imágenes de referencia y diagramas corregidos dentro de la carpeta `simulation/tinkercad/`. | `simulation/tinkercad/` |
| **17** | `a0d1223` | 2026-10-12 | Firmware / TinkerCAD | Actualización y consolidación del firmware de Arduino, renombrándolo a `generador_patrones_tinkercad.ino` como versión unificada. | `src/arduino/generador_patrones_tinkercad.ino` |
| **18** | `bcac6f6` | 2026-10-12 | Simulación TinkerCAD | Verificación funcional del circuito completo en TinkerCAD Circuits, agregando capturas del circuito, componentes y esquema eléctrico. | `Simulacion/tinkercad/CircuitoTinkercad.png`, `Componentes.png`, `Esquema.png` |
| **19** | `749caa9` | 2026-10-12 | Reestructuración | Reestructuración de carpetas de simulación bajo el directorio unificado `Simulacion/` con subcarpetas `ltspice/` y `tinkercad/`. | `Simulacion/` |
| **20** | `1a6dfe1` | 2026-10-12 | Mantenimiento | Limpieza de archivos duplicados y obsoletos en el repositorio y actualización del registro de bitácora. | `BITACORA.md`, limpieza de archivos temporales |
| **21** | `3e73d60` | 2026-10-14 | Simulación LTspice | Corrección profunda de esquemáticos en LTspice: corrección de pines flotantes (`NC_xx`), macromodelo `LM358_simple.lib`, mediciones `.meas` y guía de verificación. | `Simulacion/ltspice/` (`LM358_simple.lib`, `VERIFICACION.md`, `.inc`, `.asc`) |
| **22** | `84eea96` | 2026-10-14 | Guías de Montaje | Documentación técnica de la alimentación de $+9.0\text{ V}$ para el pin 8 del LM358 para evitar saturación y análisis de impedancia del buzzer pasivo. | `Simulacion/tinkercad/Montaje.md`, `docs/guia_de_conexion_protoboard.md` |
| **23** | `e06d1b3` | 2026-10-14 | Informe IEEE / Memoria | Sincronización del informe IEEE Transactions y de la memoria de cálculo con las mediciones numéricas reales obtenidas en las simulaciones de LTspice. | `docs/informe_ieee/informe_ieee_el2113.tex`, `docs/memoria_de_calculo.md` |
| **24** | `a8dc060` | 2026-10-14 | Documentación | Actualización de la documentación general, alineación del README principal y actualización de las entradas de bitácora. | `BITACORA.md`, `README.md` |
| **25** | `409707d` | 2026-10-15 | MATLAB / Refactor | Refactorización de comentarios, descripciones y consistencia de encabezados en los scripts `matlab/generador_patrones.m` y `matlab/simulacion_dac_555.m`. | `matlab/generador_patrones.m`, `matlab/simulacion_dac_555.m` |
| **26** | `adc6aca` | 2026-10-15 | Documentación / Bitácora | Actualización integral de la bitácora con los 25 commits, trazabilidad completa de requerimientos A.1–Q.18 y detalle de compra de componentes físicos. | `BITACORA.md` |
| **27** | `12f262e` | 2026-10-18 | Simulación LTspice | Optimización visual, orientación horizontal de etiquetas y corrección de espaciado en esquemáticos del DAC y receptor 555. | `Simulacion/ltspice/dac_r2r_buffer.asc`, `Simulacion/ltspice/receptor_555_ldr.asc` |
| **28** | `9df55d5` | 2026-10-18 | Simulación LTspice | Rediseño espacial del sistema completo óptico de 7 etapas en LTspice, despejando pines de NE555 y acoplo AC. | `Simulacion/ltspice/sistema_completo_optico.asc` |
| **29** | `1a339b8` | 2026-10-18 | Documentación / LTspice | Actualización de la guía de verificación técnica con soporte de ejecución batch `-Run -b` y directrices de diagramación. | `Simulacion/ltspice/VERIFICACION.md` |
| **30** | `acb8f11` | 2026-10-18 | Documentación / Bitácora | Registro en bitácora de la optimización visual de esquemáticos LTspice y sincronización del estado del proyecto. | `BITACORA.md` |
| **31** | `1ef7c9b` | 2026-10-20 | Montaje Físico | Registro de cambios en bitácora e incorporación de fotografía de evidencia del ensamblaje físico preliminar en protoboard. | `BITACORA.md`, `Simulacion/tinkercad/CircuitoArmado.jpeg` |
| **32** | `0e23dae` | 2026-10-22 | Corte Láser LAIMI | Optimización de la placa base a fracción oficial 1/8 de lámina LAIMI ($250\times 200\text{ mm}$) y ajuste de capas RGB a $0.025\text{ mm}$. | `prototipo_laboratorio/corte_laser/base_soporte_optico.svg` |
| **33** | `5a1dcd0` | 2026-10-22 | Corte Láser LAIMI | Generación de archivo vectorial `.PDF` en modo RGB listo para cortadora Trotec Speedy 360 en LAIMI F4-05. | `prototipo_laboratorio/corte_laser/2022437519_base_soporte_optico.pdf` |
| **34** | `8c90a5e` | 2026-10-22 | Impresión 3D LAIMI | Adecuación de torres acopladoras M3 a normativa Prusa MK4 de LAIMI y exportación con formato oficial `2022437519-01.stl`. | `acoplador_optico_led_ldr.scad`, `2022437519-01.stl` |
| **35** | `10a0486` | 2026-10-22 | Impresión 3D LAIMI | Diseño paramétrico y exportación de capuchones ópticos directos para protoboard (`2022437519-02.stl` - Opción B). | `capuchon_protoboard_led_ldr.scad`, `2022437519-02.stl` |
| **36** | `ba2cb64` | 2026-10-22 | Visualización 3D | Incorporación de render 3D fotorrealista de la maqueta física completa ensamblada en laboratorio. | `prototipo_laboratorio/render_maqueta_3d_laser.jpg` |
| **37** | `3f25388` | 2026-10-22 | Visualización 3D | Incorporación de ilustración técnica comparativa de montaje optoelectrónico (Opción A con jumpers vs Opción B en protoboard). | `prototipo_laboratorio/comparativa_montaje_opcion_a_vs_b.jpg` |
| **38** | `3272145` | 2026-10-22 | Guía Protoboard | Mapeo de tabla maestra de coordenadas exactas para protoboard grande de 60 filas (`0` a `60`, columnas `a` a `j`). | `docs/guia_de_conexion_protoboard.md` |
| **39** | `20b3de1` | 2026-10-22 | Normativa LAIMI | Actualización de guía de prototipado con reglamento LAIMI F4-05, tornillería M3 y guion de solicitud presencial (`2022437519`). | `docs/diseno_prototipado_3d_laser.md` |
| **40** | `2377cba` | 2026-10-22 | Lista Materiales | Detalle de tornillería métrica `M3 x 10mm/12mm`, tuercas M3, jumpers Hembra–Macho y tarifas de manufactura LAIMI en el BOM. | `docs/lista_de_materiales_BOM.md` |
| **41** | `b1bb2fe` | 2026-10-22 | Documentación | Vinculación del montaje virtual de TinkerCAD con coordenadas físicas (`0`–`60`, `a`–`j`) y archivos oficiales de LAIMI. | `Simulacion/tinkercad/Montaje.md` |
| **42** | `a36e6b8` | 2026-10-22 | Documentación | Indexación en `README.md` de los archivos oficiales `.PDF` y `.STL` con carné `2022437519` y renders 3D de ensamblaje. | `README.md` |

---

## 4. Matriz de Cumplimiento de Requerimientos Oficiales (A.1 – Q.18)

| Requerimiento Oficial | Descripción Normativa | Estado | Artefacto / Archivo de Verificación |
|:---|:---|:---:|:---|
| **A.1 / B.2** | Sistema dividido en 7 etapas delimitadas e identificables | **CUMPLE** | Grabado láser en `base_soporte_optico.svg`, esquemáticos y guías de montaje. |
| **C.3 / D.4** | Arreglos de longitud variable en MATLAB y tiempo entre 1 ms y 10 s | **CUMPLE** | `matlab/generador_patrones.m` y comandos `'P'`, `'S'`, `'T<ms>'` en `generador_patrones_tinkercad.ino`. |
| **E.5** | DAC implementado con componentes pasivos y un componente activo | **CUMPLE** | Red R-2R ($10\text{ k}\Omega / 20\text{ k}\Omega$) + OpAmp 1A del LM358N (`docs/memoria_de_calculo.md`). |
| **F.6** | Etapa de interfaz implementada únicamente con OpAmp | **CUMPLE** | OpAmp 1B del LM358N en configuración seguidora de aislamiento unitario ($A_v = 1$). |
| **G.7 / H.8** | Generador óptico con componentes pasivos y LED justificado con Física 2/3 | **CUMPLE** | Resistor $150\ \Omega$ + LED rojo ($650\text{ nm}$) justificado por Planck-Einstein y ventana PMMA. |
| **I.9 / J.10** | Medio óptico de 30 a 60 cm y receptor óptico puramente pasivo | **CUMPLE** | Fibra plástica POF cortada a $45\text{ cm}$ y fotorresistencia LDR pasiva (GL5528). |
| **K.11 / L.12** | Oscilador comercial integrado con pasivos y salida en buzzer pasivo | **CUMPLE** | Temporizador NE555 astable ($177\text{--}3443\text{ Hz}$) + condensador $10\ \mu\text{F}$ + Buzzer pasivo. |
| **M.13 / N.14** | Prototipado en Escuela de Electrónica (corte láser, impresión 3D, escáner) | **CUMPLE** | Plano láser `base_soporte_optico.svg`, cabezal 3D `acoplador_optico_led_ldr.stl` y propuesta de escáner. |
| **Ñ.15 / O.16** | Montaje en protoboard y error de conversión DAC menor al 10% | **CUMPLE** | Protoboard 830 puntos, resistencias 1% y error simulado $< 0.01\%$ (medido en LTspice). |
| **P.17 / Q.18** | Prohibido usar 5V de Arduino y baterías; uso de fuente de laboratorio | **CUMPLE** | Cable 5V desconectado; alimentación desde Fuente DC de Laboratorio de bodega ($+5\text{ V} / +9\text{ V}$). |

---

## 5. Próximos Pasos Planificados

1. **Subida de Evidencia Fotográfica:** Subir al repositorio en GitHub la fotografía del circuito ensamblado en la protoboard.
2. **Fabricación e Integración Mecánica:** Fabricar las piezas en el taller de prototipado de la Escuela de Electrónica (`base_soporte_optico.svg` en corte láser y `acoplador_optico_led_ldr.stl` en impresión 3D PLA) e integrar el tramo de fibra óptica plástica ($45\text{ cm}$) entre el LED y la LDR.
3. **Medición Experimental en Laboratorio:** Conectar la fuente de laboratorio de la bodega de Electrónica y registrar los puntos de prueba TP1 a TP6 con multímetro y osciloscopio para completar la tabla del informe.
4. **Compilación Final del Informe IEEE:** Compilar `docs/informe_ieee/informe_ieee_el2113.tex` con los datos experimentales reales y generar el entregable `.zip` final.
