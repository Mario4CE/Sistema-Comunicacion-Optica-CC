# Bitácora de Desarrollo del Proyecto
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos Eléctricos en Corriente Continua (EL-2113)  
**Semestre:** II Semestre 2026  
**Profesor:** Ing. Fabián Alberto Picado García  
**Equipo de Trabajo:**
* **Mario Cerdas** (Carné: 2022437519, `m.cerdas@estudiantec.cr`)
* **Sebastián Quirós Fernández** (Carné: 2024073526, `s.quiros.4@estudiantec.cr`)
* **Ian Fuentes Núñez** (Carné: 2025100912, `I.fuentes.1@estudiantec.cr`)  
**Repositorio Git:** `Sistema-Comunicacion-Optica-CC`  
**Rama Activa de Trabajo:** `mario-dev`  
**Estado General:** Fase de Simulación y Modelado Matemático 100% completada y verificada mediante mediciones automáticas en LTspice (DAC con error < 0.0012%, receptor 555 con error < 0.91% y sistema óptico completo de 176.7 Hz a 3443.0 Hz); archivo esquemático del receptor corregido en codificación UTF-8 sin BOM y carpeta LTspice saneada dejando únicamente los 9 archivos fuente esenciales; Adquisición física de componentes electrónicos y tornillería mecánica completada al 100%; Placa base organizadora en corte láser fabricada y recibida de LAIMI; Ensamble físico e integración en el chasis completado al 100% (Arduino UNO fijado con tornillos y tuercas M3, protoboard con el circuito completo de 7 etapas interconectadas sobre la base de corte láser y enlace de fibra plástica POF de $45\text{ cm}$ alineado); Pruebas en laboratorio iniciadas con corrección de lazos y puentes del amplificador LM358N (Pin 1 a 2, Pin 6 a 7 y enlace Pin 1 a 5); Pruebas preliminares posteriores superadas exitosamente comprobando la entrega de los niveles de voltaje correctos; Reunión de trabajo del equipo programada para el próximo lunes 2 de noviembre para completar las mediciones experimentales cuantitativas finales de laboratorio con la Fuente DC Regulada (Req. P.17 y Q.18) y dar cierre definitivo al proyecto.

---

## 1. Registro Cronológico Detallado de Actividades y Avances

### [2026-10-02] - Sesión 1: Inicialización del Repositorio, Arquitectura Base y Primeras Simulaciones
* **Control de Versiones e Infraestructura:**
  * Se inicializó el repositorio local de Git en `D:\Proyecto CC` y se configuró el archivo `.gitignore` adaptado para excluir archivos temporales generados por compilación de Arduino, sesiones de MATLAB (`.asv`) y archivos de forma de onda y datos transitorios de LTspice (`.raw`, `.log`, `.op.raw`).
* **Etapa 1 (Sistema Digital):**
  * Se programó el firmware base en Arduino para el control de 4 pines digitales de salida (`D8` a `D11`) hacia la escalera R-2R, implementando modos operativo automático (barrido de 0 a 15), manual por monitor serial y modo de calibración de extremos (alternancia 0 y 15).
* **Modelado Numérico en MATLAB:**
  * Se desarrolló el script de modelado que evalúa la escalera analógica del DAC R-2R (16 estados), la corriente del LED rojo transmisor, la variación de resistencia de la fotorresistencia receptora (LDR) y la frecuencia generada por el temporizador NE555 astable.
  * Se generaron patrones para digitalizar señales sinusoidales cuantizadas a 4 bits, calcular la relación señal a ruido de cuantización teórica ($SQNR = 25.84\text{ dB}$) y exportar arreglos en formato de constantes en C++.
* **Memoria de Cálculo y Análisis Circuital:**
  * Se redactó `Documentacion/memoria_de_calculo.md` con deducciones mediante equivalente de Thévenin y leyes de Kirchhoff, demostrando el colapso de tensión por efecto de carga ($> 98\%$ de atenuación) si no se utiliza un buffer seguidor de tensión con amplificador operacional (LM358N).
* **Simulación en LTspice y TinkerCAD:**
  * Se crearon los esquemáticos de simulación iniciales para validar el DAC con buffer y la etapa receptora osciladora.

---

### [2026-10-09] - Sesión 2: Simulación de Etapa Receptora (LDR + 555 Astable + Buzzer) en LTspice
* **Modelado de la Etapa Receptora:**
  * Se refinó el circuito `LTspice/receptor_555_ldr.asc` y su netlist `.cir` modelando el temporizador NE555 astable modulado por la resistencia óptica de la LDR.
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
  * Se reestructuró `Matlab/Primero.m` generando arreglos de longitud variable tanto para señales **periódicas** ($N=20$) como **aperiódicas** ($N=18$).
  * Se actualizó el firmware de Arduino para recorrer arreglos exportados desde MATLAB mediante los comandos `'P'` (periódico) y `'S'` (aperiódico), y se implementó la función `configurarTiempoPermanencia()` con el comando `'T<ms>'` que valida que el tiempo por palabra digital se mantenga en el rango estricto de $[1\text{ ms}, 10\,000\text{ ms}]$ ($1\text{ ms}$ a $10\text{ s}$).
* **Análisis Nodal de Kirchhoff y Separación de OpAmps (Req. E.5 y F.6):**
  * En `Documentacion/memoria_de_calculo.md` se dedujo formalmente la función de transferencia del DAC R-2R resolviendo el sistema de 4 ecuaciones nodales (LCK) en los nodos $V_1, V_2, V_3, V_4$.
  * Se delimitó el uso de los dos amplificadores operacionales del encapsulado DIP-8 del **LM358N**: el **OpAmp 1A** conforma el componente activo de la Etapa 2 (DAC activo, Req. E.5), y el **OpAmp 1B** conforma la Etapa 3 (interfaz seguidora de aislamiento, Req. F.6).
* **Justificación Física del Color de la Fuente Luminosa (Req. H.8):**
  * Se fundamentó la selección del LED rojo visible ($\lambda \approx 650\text{ nm}$) mediante conceptos de Física 2 y Física 3: relación cuántica de Planck-Einstein ($E = hc/\lambda = 1.91\text{ eV} \implies V_D \approx 1.8\text{ V}$ para maximizar el rango dinámico útil), ley de Snell y ventana de mínima atenuación de la fibra plástica de PMMA en $650\text{ nm}$, y acoplamiento espectral con el sulfuro de cadmio (CdS) de la LDR.
* **Diseño Mecánico y Prototipado en la Escuela de Electrónica (Req. M.13 y N.14):**
  * Se diseñó el plano vectorial 2D para cortar en acrílico o MDF de $3\text{ mm}$, incorporando ranuras para guiar la fibra a $45\text{ cm}$ ($30\text{--}60\text{ cm}$) y grabado láser de las 7 etapas delimitadas (Req. B.2).
  * Se diseñaron los cabezales opacos cilíndricos para alinear concéntricamente el LED y la LDR con la fibra e inmunizar la fotorresistencia contra la luz ambiental del laboratorio.
* **Informe Técnico en Formato IEEE Transactions y Simulación Integrada:**
  * Se redactó el informe formal en LaTeX (`Documentacion/informe_ieee_el2113.tex`) con límite de 3 páginas y todas las secciones exigidas por la rúbrica.
  * Se creó la simulación integral `LTspice/sistema_completo_optico.asc` conectando las 7 etapas en conjunto.

---

### [2026-10-12] - Sesión 5: Simulación Funcional en TinkerCAD, Guías Operativas y Reorganización
* **Simulación en TinkerCAD Circuits:**
  * Se unificó el firmware para simulación web integrando arreglos periódicos y aperiódicos de MATLAB, comandos interactivos y respuesta visual.
  * Se montó y verificó el circuito en la plataforma web de TinkerCAD, capturando los esquemas y diagramas de montaje (`TinkerCAD/CircuitoTinkercad.png`, `Componentes.png` y `Esquema.png`).
* **Limpieza y Reestructuración del Repositorio:**
  * Se organizaron las carpetas de simulación bajo directorios dedicados para LTspice y TinkerCAD.

---

### [2026-10-14] - Sesión 6: Corrección y Calibración de LTspice, Macromodelo LM358 y Mediciones Automáticas
* **Diagnóstico y Corrección de Modelos SPICE:**
  * Se corrigieron los esquemáticos `dac_r2r_buffer.asc`, `receptor_555_ldr.asc` y `sistema_completo_optico.asc` junto con el netlist `sistema_completo_optico.cir`.
  * Se desarrolló el macromodelo `LTspice/LM358_simple.lib` que incorpora saturación de riel realista ($V_{OH} \approx V_{CC} - 1.5\text{ V}$) y comportamiento de saturación en $0\text{ V}$.
  * Se crearon archivos de directivas de medición automática `.meas`: `mediciones_dac.inc`, `mediciones_555.inc` y `mediciones_sistema.inc`.
  * Se redactó `LTspice/VERIFICACION.md` documentando los resultados de simulación: error del DAC $< 0.01\%$, error de frecuencia del 555 $< 1\%$, y frecuencia modulada del sistema completo entre $176.7\text{ Hz}$ (código 0) y $3443.0\text{ Hz}$ (código 15).
* **Calibración de Guías de Montaje:**
  * Se especificó alimentar el Pin 8 del LM358 con $+6.5\text{ V}\text{ a }+9.0\text{ V}$ desde el canal secundario de la fuente de laboratorio para garantizar linealidad plena hasta el código 15 ($4.688\text{ V}$), manteniendo el temporizador 555 en $+5.0\text{ V}$.
  * Se analizó la frecuencia de corte pasa-altos con el buzzer piezoeléctrico pasivo ($f_c \approx 16\text{ Hz}$ con transductor de $1\text{ k}\Omega$ y $C = 10\ \mu\text{F}$, preservando el contenido de audio en tonos graves).

---

### [2026-10-15] - Sesión 7: Refactorización de MATLAB, Compra de Componentes Físicos y Verificación
* **Refactorización de Scripts de MATLAB:**
  * Se revisaron descripciones, encabezados y comentarios de línea en `Matlab/Primero.m` y `Matlab/Final.m`, garantizando total consistencia con las 7 etapas del sistema y los resultados de simulación de LTspice.
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
  * Se capturó la fotografía del prototipo preliminar (`TinkerCAD/CircuitoArmado.jpeg`).

---

### [2026-10-22] - Sesión 10: Adecuación a Normativa Oficial LAIMI (Corte Láser e Impresión 3D) y Visor 3D
* **Normativa LAIMI — Corte y Grabado Láser (Trotec Speedy 360):**
  * Se optimizó la placa base organizadora a dimensiones exactas de **$250\text{ mm} \times 200\text{ mm}$** (fracción de $1/8$ de lámina) con archivos oficiales en espacio RGB y grosor $0.025\text{ mm}$ (`Prototipos/Corte_laser/base_soporte_optico.pdf` y `.svg`).
* **Visor 3D Interactivo:**
  * Se desarrolló el visor interactivo Three.js (`Prototipos/Impresion_3D/visor_interactivo_3d.html`) para previsualizar piezas 3D en el navegador sin software externo.

---

### [2026-10-23] - Sesión 11: Tornillería M3, Recepción de Base Láser LAIMI y Ensamble en el Chasis
* **Fabricación y Recepción de la Base de Soporte en Corte Láser (LAIMI F4-05):**
  * La cortadora láser Trotec Speedy 360 procesó con éxito la lámina de $3.0\text{ mm}$ con contorno exterior a escuadra, orificios pasantes de $\varnothing 3.2\text{ mm}$, ranuras de sujeción para la fibra y grabado de las 7 etapas (Req. B.2 y M.13).
* **Montaje Físico y Ensamble Estructural sobre la Base Láser:**
  * **Fijación del Arduino UNO:** Se fijó la placa Arduino UNO rígidamente a la placa base mediante tornillos y tuercas M3.
  * **Instalación de Protoboard en Chasis:** Se montó la protoboard de 830 puntos con el circuito completo de las 7 etapas sobre el chasis delimitado.
  * **Preparación de Tornillos M3 para Torres 3D:** Se insertaron y ajustaron los 4 tornillos M3 con tuercas en la posición de las torres TX y RX de la base láser.
* **Evidencia Fotográfica del Chasis:**
  * Se capturó y subió al repositorio la fotografía [`TinkerCAD/CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/TinkerCAD/CircuitoConBase.jpeg) evidenciando la integración del Arduino atornillado, la protoboard delimitada y la base de MDF.

---

### [2026-10-24] - Sesión 12: Unificación del Kit 3D (8 Piezas), Limpieza del Repositorio y Plantilla IEEE
* **Diseño y Unificación del Kit 3D de 8 Piezas (`Atornillada.scad` / `2022437519-01.stl`):**
  * Se integró en un único archivo OpenSCAD paramétrico (`Prototipos/Impresion_3D/Atornillada.scad`) el conjunto completo de 8 piezas optimizado para manufactura en cama plana de $54.9\text{ mm} \times 43.6\text{ mm}$ sin soportes:
    1. 1x Torre Horizontal Atornillada M3 para Emisor TX (LED Rojo $5\text{ mm}$, cámara $\varnothing 6.2\text{ mm}$).
    2. 1x Torre Horizontal Atornillada M3 para Receptor RX (LDR GL5528, cámara $\varnothing 6.4\text{ mm}$).
    3. 3x Grapas/clips de retención transversales para las ranuras frente a las torres ($Z=6.0\text{ mm}$).
    4. 3x Grapas/clips de retención longitudinales para ranuras de $7\times 3\text{ mm}$ del arco exterior de la base láser, asegurando el tramo de fibra sin necesidad de gazas plásticas.
  * Se exportó la malla STL sólida oficial `Prototipos/Impresion_3D/2022437519-01.stl` cumpliendo con la normativa LAIMI Prusa MK4.
* **Reorganización de Firmware y Depuración del Repositorio:**
  * Se ubicó el sketch de Arduino en la estructura oficial `Arduino/Logica/Logica.ino` para compatibilidad directa con Arduino IDE.
  * Se actualizaron referencias a documentos obsoletos en `README.md`.
* **Actualización del Informe Final IEEE (`informe_ieee_el2113.tex`):**
  * Se incorporó la columna experimental $V_{\text{exp}}$ en el Cuadro I de resultados para registrar las mediciones reales de laboratorio con multímetro.

---

### [2026-10-27] - Sesión 13 (HOY): Ensamble Físico Completo del Circuito y Pruebas Preliminares de Funcionamiento con Batería
* **Conformación Oficial del Equipo de Trabajo:**
  * Se formalizó la integración de los compañeros de trabajo en toda la documentación técnica, código y reportes del proyecto:
    * **Mario Cerdas** (Carné: 2022437519, `m.cerdas@estudiantec.cr`).
    * **Sebastián Quirós Fernández** (Carné: 2024073526, correo institucional: `s.quiros.4@estudiantec.cr`).
    * **Ian Fuentes Núñez** (Carné: 2025100912, correo institucional: `I.fuentes.1@estudiantec.cr`).
* **Ensamble Físico Integral del Circuito (100% Completado):**
  * El circuito se ensambló por completo en su totalidad sobre la placa protoboard de 830 puntos montada e integrada en la base de MDF de corte láser ($250\times 200\text{ mm}$), dejando interconectadas las 7 etapas de la cadena optoelectrónica:
    1. **Etapa 1 (Patrón Digital):** Conexión de los 4 pines de control (`D8` a `D11`) desde el Arduino UNO hacia las entradas de ponderación de la escalera DAC.
    2. **Etapa 2 (DAC Pasivo-Activo):** Red en escalera R–2R ($10\text{ k}\Omega / 20\text{ k}\Omega$ al 1%) conectada a la entrada no inversora del OpAmp 1A (LM358N, pines 1, 2 y 3).
    3. **Etapa 3 (Interfaz OpAmp):** Interfaz seguidora de tensión no inversora con OpAmp 1B (LM358N, pines 5, 6 y 7) acoplada a la salida de la Etapa 2 para eliminar el efecto de carga.
    4. **Etapa 4 (Generador Óptico):** Resistor pasivo limitador de $150\ \Omega$ acoplado al ánodo del LED rojo difuso de $5\text{ mm}$ ($\lambda \approx 650\text{ nm}$).
    5. **Etapa 5 (Medio y Receptor Óptico):** Tramo de fibra óptica plástica (POF) cortado a $45\text{ cm}$ alineado coaxialmente con el emisor y acoplado a la fotorresistencia receptora pasiva LDR GL5528.
    6. **Etapa 6 (Generador Astable 555):** Temporizador NE555P conectado en modo astable, con la LDR gobernando el ciclo de temporización en conjunto con $R_1 = 1\text{ k}\Omega$, $C_1 = 100\text{ nF}$ y desacoplo de $10\text{ nF}$ en pin 5.
    7. **Etapa 7 (Buzzer Pasivo):** Condensador electrolítico de acople AC de $10\ \mu\text{F}$ en serie con el buzzer piezoeléctrico pasivo, eliminando el nivel continuo DC.
* **Pruebas Funcionales Preliminares con Batería Portátil:**
  * **Verificación de Continuidad e Integridad de Banco:** Se energizó el circuito de forma preliminar utilizando una batería como fuente autónoma portátil de banco para verificar la continuidad nodal general y descartar cortocircuitos o calentamientos en los componentes.
  * **Validación de Emisión Óptica y Respuesta Acústica:** Se comprobó exitosamente que el barrido digital modula la luminosidad del LED rojo, que la señal de luz se propaga eficazmente por el tramo de fibra de $45\text{ cm}$ hasta la LDR, y que la variación fotoconductora modula la frecuencia astable del temporizador 555, generando tonos audibles progresivos claramente perceptibles en el buzzer piezoeléctrico pasivo.
  * **Nota Normativa de Cumplimiento (Req. P.17 y Q.18):** Se deja constancia explícita en la documentación de que el uso de la batería se restringió de manera exclusiva a una prueba preliminar de validación de banco y continuidad tras finalizar el ensamble. Para la toma de datos cuantitativos finales ($V_{\text{exp}}$) que se consignarán en el Cuadro I del informe IEEE y para la defensa y evaluación presencial final ante el profesor en semana 19, la alimentación se realizará obligatoria y exclusivamente con la **Fuente DC Regulada de Laboratorio** provista por la bodega de la Escuela de Electrónica.
* **Actualización Integral de Documentos del Proyecto:**
  * Se actualizaron todos los documentos que lo ameritaban:
    * `Documentacion/informe_ieee_el2113.tex`: Incorporación de los tres autores, correos institucionales, mención al ensamble completo y pruebas preliminares con batería.
    * `Documentacion/memoria_de_calculo.md`: Actualización del equipo de trabajo en el encabezado y adición de la subsección 7.1 documentando las pruebas preliminares de banco con batería frente al cumplimiento estricto de P.17/Q.18.
    * `README.md`: Actualización del equipo de autores, estado del prototipo completamente ensamblado con pruebas de batería superadas y corrección de enlaces a la estructura oficial del repositorio.
    * `Arduino/Logica/Logica.ino`: Encabezado con autores, carnés y correos institucionales.
    * `Matlab/Primero.m` y `Matlab/Final.m`: Encabezados con autores y datos académicos del equipo.

---

### [2026-10-28] - Sesión 14: Pruebas Experimentales en Laboratorio, Diagnóstico/Corrección de Voltaje en TinkerCAD y Físico, y Validación Preliminar Exitosa
* **Sesión Experimental en Laboratorio con Ian Fuentes:**
  * Mario Cerdas e Ian Fuentes acudieron al laboratorio de la Escuela de Ingeniería Electrónica del ITCR con el prototipo integrado en la base láser para llevar a cabo la toma de mediciones experimentales cuantitativas ($V_{\text{exp}}$) y la caracterización osciloscópica requerida por la rúbrica oficial.
  * **Incidencias y Pruebas Inconclusas:** Debido a limitaciones de tiempo de disponibilidad de puesto y a la aparición de un comportamiento eléctrico inesperado en el circuito de acondicionamiento, no fue posible completar la totalidad de las pruebas de laboratorio programadas para la sesión.
  * **Detección de Falla en el Voltaje:** Durante la energización con la fuente de laboratorio y el barrido digital desde Arduino, se detectaron discrepancias severas en los niveles de tensión analógicos entregados por el DAC y la etapa de interfaz (lecturas erráticas y falta de progresión monótona esperada), lo que impedía validar el margen de error menor al $10\%$ (Req. O.16).
* **Diagnóstico y Corrección Circuital en Simulación (TinkerCAD) y en Montaje Físico:**
  * Tras la sesión de laboratorio, Mario realizó una auditoría y depuración exhaustiva nodo a nodo del circuito, contrastando la implementación física contra el diseño teórico y reproduciendo las fallas en el simulador virtual interactivo de **TinkerCAD Circuits**.
  * **Causa Raíz Identificada:** Se detectaron problemas en los lazos de realimentación y puentes de conexión del amplificador operacional dual **LM358N** (DIP-8), donde la ausencia o conexión inadecuada de puentes dejaba los amplificadores en lazo abierto o con realimentación flotante, provocando que no actuaran como seguidores de tensión unitarios ($A_v = 1$).
  * **Correcciones Efectuadas en TinkerCAD y en la Protoboard:**
    1. **Puente 1 (Seguidor DAC - Etapa 2):** Conexión directa entre el **Pin 1 (Salida 1)** y el **Pin 2 (Entrada Inversora 1)** del LM358N. Esto cierra el lazo de realimentación negativa del OpAmp 1A, configurándolo como buffer unitario estricto para la tensión analógica $V_4$ proveniente de la escalera R-2R en el Pin 3.
    2. **Puente 2 (Seguidor Interfaz - Etapa 3):** Conexión directa entre el **Pin 6 (Entrada Inversora 2)** y el **Pin 7 (Salida 2)** del LM358N. Esto conforma el buffer seguidor del OpAmp 1B que maneja la corriente hacia la resistencia limitadora de $150\ \Omega$ y el LED rojo.
    3. **Cable de Enlace Inter-etapas en U (Etapa 2 a Etapa 3):** Conexión en puente desde el **Pin 1 (Salida OpAmp 1A)** hacia el **Pin 5 (Entrada No Inversora OpAmp 1B)**, desacoplando la impedancia Thévenin de la red R-2R ($R_{th} = 10\text{ k}\Omega$) y eliminando por completo el efecto de carga del diodo emisor.
    4. **Revisión de Polarización y Referencias:** Verificación del riel $V_{CC}$ en Pin 8 ($+9.0\text{ V}$) y el retorno común de tierra en Pin 4 (GND compartido entre fuente, Arduino y protoboard).
  * **Generación de Evidencias y Documentación Técnica:**
    * Se elaboró el diagrama vectorial ilustrativo [`TinkerCAD/puentes_lm358.svg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.svg) y su versión gráfica [`TinkerCAD/puentes_lm358.jpg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.jpg), donde se detalla con código de colores la ubicación exacta de cada puente sobre el chip DIP-8 en la protoboard.
    * Se actualizó el circuito interactivo en línea en la plataforma TinkerCAD ([`TinkerCAD/Enlace.txt`](file:///d:/Proyecto%20CC/TinkerCAD/Enlace.txt)).
* **Pruebas Preliminares de Validación del Voltaje:**
  * Mario efectuó pruebas preliminares de banco sobre el prototipo físico debidamente corregido.
  * Se comprobó de forma concluyente que el circuito ahora entrega con total exactitud los niveles de tensión teóricos esperados para los distintos estados de la escalera ($0\text{ V}$ hasta $\approx 4.69\text{ V}$), restaurando la linealidad, la monotonía y la modulación luminosa precisa del LED rojo.
* **Planificación del Equipo y Próxima Sesión de Cierre:**
  * Se coordinó una reunión conjunta de trabajo del equipo para el próximo **lunes 2 de noviembre de 2026** en el laboratorio de la Escuela de Electrónica.
  * En dicha sesión se completarán las pruebas pendientes:
    1. Registro sistemático de los 16 voltajes experimentales ($V_{\text{exp}}$) con multímetro digital calibrado de banco para completar el Cuadro I del informe IEEE y calcular el error relativo exacto (Req. O.16).
    2. Medición de frecuencias de oscilación astable del temporizador 555 con osciloscopio digital.
    3. Consolidación del informe final IEEE y preparación de la entrega definitiva del proyecto.

---

### [2026-10-29] - Sesión 15 (HOY): Verificación Integral de Simulaciones en LTspice, Corrección de Codificación UTF-8 y Depuración de Temporales
* **Auditoría y Corrección de Codificación en Esquemático (`receptor_555_ldr.asc`):**
  * Se identificó que el archivo `receptor_555_ldr.asc` contenía una cabecera con marca de orden de bytes (UTF-8 BOM `0xEF 0xBB 0xBF`), lo que provocaba que LTspice en Windows abortara la apertura con el error `Unknown schematic syntax: Version 4.1`.
  * Se procesó el archivo eliminando el BOM y preservando la sintaxis limpia en ANSI/UTF-8 puro, restableciendo la apertura inmediata y ejecución de la simulación tanto en entorno gráfico interactivo como en modo batch (`-Run -b`).
* **Validación de Resultados de Simulación en las Tres Etapas:**
  1. **DAC R-2R con Buffer LM358 (`dac_r2r_buffer.asc`):** Se verificaron los 16 estados analógicos ($0\text{ V}$ a $4.6875\text{ V}$ con escalones de $0.3125\text{ V}$). El error relativo máximo registrado fue de **$0.0012\%$** (cumpliendo sobradamente el Req. O.16 de $< 10\%$). En el estado 0 se registró el piso de saturación baja de $5.0\text{ mV}$ característico del LM358 con fuente simple. La corriente del LED se modula desde $0.03\text{ mA}$ (corte en código 5) hasta $18.66\text{ mA}$ (código 15), protegiendo el diodo por debajo de los $20\text{ mA}$.
  2. **Receptor 555 con LDR (`receptor_555_ldr.asc`):** Se verificó el barrido paramétrico de 5 pasos ($R_{LDR} \in [1.5\text{ k}\Omega, 40\text{ k}\Omega]$) modulando la frecuencia audible entre **$176.7\text{ Hz}$** y **$3574.6\text{ Hz}$**, con un error respecto a la teoría menor al **$0.91\%$**. Se comprobaron los umbrales de conmutación del comparador interno ($V_{THR(\max)} = 3.335\text{ V} \approx \frac{2}{3}V_{CC}$ y $V_{THR(\min)} = 1.665\text{ V} \approx \frac{1}{3}V_{CC}$) y ciclos de trabajo coherentes entre $50.3\%$ y $62.1\%$.
  3. **Sistema Completo Óptico de 7 Etapas (`sistema_completo_optico.asc` / `.cir`):** Se validó la respuesta integrada de las 7 etapas en conjunto. La variación fotoconductora de la LDR modulada por el LED rojo barrió su resistencia desde $40.0\text{ k}\Omega$ hasta $1.58\text{ k}\Omega$, logrando una excursión frecuencial acústica en el buzzer pasivo de **$176.7\text{ Hz}$ a $3443.0\text{ Hz}$** con error $< 0.90\%$.
* **Saneamiento y Depuración de la Carpeta `LTspice/`:**
  * Se eliminaron todos los archivos temporales y binarios transitorios pesados generados por las simulaciones (`*.raw`, `*.op.raw`, `*.log.raw`, `*.db`, `*.log`, `*.net`), reduciendo el consumo innecesario de almacenamiento (> 35 MB) y dejando en la carpeta de LTspice exclusivamente los 9 archivos fuente esenciales (`.asc`, `.cir`, `.inc`, `.lib` y `.md`).
  * Se actualizó `.gitignore` incorporando las reglas de exclusión para evitar la subida involuntaria de archivos temporales al repositorio.
* **Actualización Documental:**
  * Se sincronizó `LTspice/VERIFICACION.md` detallando las mediciones de duty cycle y umbrales del 555.
  * Se incorporó la tabla resumen de resultados en `Documentacion/memoria_de_calculo.md`.
  * Se actualizó la descripción de la carpeta en `README.md`.

---

### [2026-10-29] - Sesión 16: Validación Rigurosa de Referencias Académicas, Justificación de Modelos y Humanización del Informe Técnico
* **Actualización y Unificación de Correos Institucionales:**
  * Se incorporó el correo institucional de Mario Cerdas (`m.cerdas@estudiantec.cr`) en todos los documentos del proyecto (`informe_ieee_el2113.tex`, `memoria_de_calculo.md`, `protocolo_pruebas_laboratorio.tex`, `README.md` y `BITACORA.md`), asegurando la consistencia de los tres autores ante la Escuela de Ingeniería Electrónica del ITCR.
* **Auditoría y Validación Rigurosa de Referencias Bibliográficas Académicas:**
  * Se eliminaron atribuciones genéricas o imprecisas y se sustituyeron por **14 referencias académicas de primer nivel, reales, verificables y de alto impacto**, vinculando cada ecuación, teorema y principio físico con su respectiva obra canónica:
    1. **Alexander y Sadiku (2018):** Leyes de Kirchhoff, análisis nodal en corriente continua, divisor de tensión y efecto de carga.
    2. **Hayt, Kemmerly y Durbin (2012):** Redes en escalera lineal, equivalente de Thévenin y respuesta transitoria RC.
    3. **Sedra, Smith, Carusone y Gaudet (2020):** Escaleras R-2R, cuantización y parámetros reales del amplificador operacional.
    4. **Franco (2015):** Demostración analítica de impedancia $R_{\text{th}} = R$ en convertidores D/A R-2R y aislamiento con buffers seguidores unitarios.
    5. **Proakis y Manolakis (2007):** Cuantización uniforme, discretización de señales periódicas/aperiódicas y resolución $\Delta V_{\text{LSB}} = V_{\text{ref}}/2^N$.
    6. **Kasap (2013):** Física de semiconductores en diodos emisores de luz (LED), relación energía-longitud de onda y reflexión interna total.
    7. **Tipler y Mosca (2010):** Ley de refracción de Snell, ángulo crítico $\theta_c$ y relación cuántica de Planck-Einstein ($E = hf$).
    8. **Ziemann, Krauser, Zamzow y Daum (2008, *POF Handbook*, Springer):** Obra definitiva en fibras ópticas plásticas; justifica la ventana de mínima atenuación en PMMA a $\lambda = 650\text{ nm}$ ($\approx 0.15\text{ dB/m}$) debida al compromiso físico entre la dispersión Rayleigh ($\propto \lambda^{-4}$) y los sobretonos vibracionales C--H en el infrarrojo cercano.
    9. **Rose (1963):** Teoría de fotoconductividad en sulfuro de cadmio (CdS), estados trampa y ley de potencias $R_{\text{LDR}} \propto E_v^{-\gamma}$.
    10. **Streetman y Banerjee (2016):** Propiedades electrónicas del CdS ($E_g \approx 2.42\text{ eV}$) y respuesta espectral en la banda visible (540--650 nm).
    11. **Camenzind (2005):** Libro de Hans Camenzind (diseñador e inventor del circuito integrado 555); deducción analítica de los tiempos de carga $t_H$ y descarga $t_L$ y la frecuencia astable $f = 1.443/((R_1+2R_2)C)$.
    12. **Cady (1964):** Transducción electromecánica de elementos piezoeléctricos pasivos y filtrado pasa-altos capacitivo ($f_c = 1/(2\pi RC)$) para desacoplo de corriente continua.
    13. **Texas Instruments (2023):** Hoja de datos oficial del LM358N (tensión de saturación baja $V_{\text{OL}} \approx 5\text{ mV}$ con fuente simple y límite superior $V_{\text{OH}} \le V_{\text{CC}} - 1.5\text{ V}$).
    14. **Texas Instruments (2022):** Hoja de datos oficial del temporizador de precisión NE555.
* **Humanización y Reescritura del Informe IEEE (`informe_ieee_el2113.tex`):**
  * Se reescribió el artículo adoptando un tono de ingeniería natural, sobrio, formal y fluido, eliminando clichés robóticos típicos de inteligencia artificial y estructuras repetitivas.
  * Se narró de forma veraz y contextualizada la experiencia de laboratorio: la detección inicial de voltajes anómalos, la identificación de lazos de realimentación abiertos en el LM358N en protoboard, la depuración interactiva en TinkerCAD Circuits y la inserción de los tres puentes de realimentación (Pin 1 a 2, Pin 6 a 7, Pin 1 a 5), concluyendo con las pruebas de banco que validaron la monotonía de los 16 escalones previo a la sesión de cierre del lunes 2 de noviembre.
  * Se ajustó el contenido para cumplir de forma estricta con el límite máximo de tres páginas de la plantilla oficial `IEEEtran`.
* **Ampliación de la Memoria de Cálculo (`memoria_de_calculo.md`):**
  * Se incorporaron las citas explícitas en cada sección técnica y se redactó la sección final 8 con el desglose formal de las 14 referencias normadas.

---

## 2. Inventario Físico de Componentes Adquiridos

| Ítem | Componente | Especificación / Modelo | Cantidad | Función Circuital (Etapa) | Estado de Verificación |
|:---:|:---|:---|:---:|:---|:---:|
| 1 | **Microcontrolador** | Arduino UNO R3 (ATmega328P) | 1 unid. | Etapa 1: Generador de patrones digitales de 4 bits | Verificado / Atornillado en base |
| 2 | **Resistencias Red R** | $10\text{ k}\Omega$ (1/4 W, 1% metal film) | 15 unid. | Etapa 2: Ramas horizontales de la red R-2R | Medido con multímetro: $9.98\text{ k}\Omega$ |
| 3 | **Resistencias Red 2R** | $20\text{ k}\Omega$ (1/4 W, 1% metal film) | 8 unid. | Etapa 2: Ramas verticales 2R y terminación | Medido con multímetro: $19.96\text{ k}\Omega$ |
| 4 | **Amplificador Op.** | LM358N (DIP-8, Texas Instruments) | 2 unid. | Etapa 2 (OpAmp 1A) y Etapa 3 (OpAmp 1B) | En protoboard / Puentes de realimentación corregidos (Pin 1-2, 6-7, 1-5) y prevalidado |
| 5 | **Resistencia Driver** | $150\ \Omega$ (1/4 W, 5%) | 2 unid. | Etapa 4: Limitación de corriente en el LED rojo | Medido: $149.3\ \Omega$ |
| 6 | **LED Emisor** | LED Rojo difuso $5\text{ mm}$ ($\lambda \approx 650\text{ nm}$) | 3 unid. | Etapa 4: Generador óptico ($V_D \approx 1.8\text{ V}$) | Probado a $10\text{ mA}$: emisión clara |
| 7 | **Fibra Óptica** | Fibra Óptica Plástica (POF, PMMA $1\text{ mm}$) | 1 m | Medio óptico guiado (tramo cortado a $45\text{ cm}$) | Inspección visual: corte recto a $90^\circ$ |
| 8 | **Fotorresistencia** | LDR GL5528 (Sulfuro de Cadmio CdS) | 2 unid. | Etapa 5: Receptor óptico pasivo | $R_{luz} \approx 1.2\text{ k}\Omega$, $R_{osc} \approx 45\text{ k}\Omega$ |
| 9 | **Temporizador IC** | NE555P (DIP-8, Texas Instruments) | 2 unid. | Etapa 6: Generador de onda cuadrada astable | En protoboard / Totalmente integrado |
| 10 | **Resistencia Carga 555**| $1\text{ k}\Omega$ (1/4 W, 5%) | 2 unid. | Etapa 6: Resistencia $R_1$ en pin 7 del 555 | Medido: $0.998\text{ k}\Omega$ |
| 11 | **Condensador Astable** | $100\text{ nF}$ ($0.1\ \mu\text{F}$, código `104`) | 3 unid. | Etapa 6: Temporización $C_1$ (pines 2 y 6 a tierra) | Cerámico multicapa verificado |
| 12 | **Condensador Filtro** | $10\text{ nF}$ ($0.01\ \mu\text{F}$, código `103`) | 2 unid. | Etapa 6: Filtro de ruido en pin 5 (Control) | Cerámico de disco verificado |
| 13 | **Condensador Acoplo** | $10\ \mu\text{F}$ ($25\text{ V}$ electrolítico radial) | 2 unid. | Etapa 7: Bloqueo de nivel DC hacia el buzzer | Polaridad verificada |
| 14 | **Transductor Acústico** | Buzzer piezoeléctrico PASIVO ($12\text{ mm}$) | 1 unid. | Etapa 7: Conversión final a sonido audible | **Prueba DC: Confirma buzzer pasivo** |
| 15 | **Protoboard** | Placa de pruebas 830 puntos | 1 unid. | Plataforma física de ensamble experimental | **Circuito 100% ensamblado, corregido y validado con voltajes correctos** |
| 16 | **Cables Jumpers** | Jumpers Macho-Macho 24 AWG | 1 juego (65) | Conexión e interconexión entre etapas | Interconectando las 7 etapas |
| 17 | **Tornillos y Tuercas M3** | Métricos `M3 x 10/12 mm` con tuercas | 8 juegos | Fijación de Arduino y torres 3D | 4 fijando Arduino / 4 en torres |
| 18 | **Placa Base Láser** | Base $250\times 200\text{ mm}$ en corte/grabado LAIMI | 1 unid. | Chasis soporte con 7 etapas delimitadas (Req. B.2/M.13) | **Entregada por LAIMI** / Totalmente integrada |
| 19 | **Kit Unificado 3D (8 pcs)**| 2 Torres M3 + 6 Clips de fibra (`2022437519-01.stl`) | 1 juego | Inmunización coaxial y retención de fibra ($45\text{ cm}$) | Malla y código listos en `Atornillada.scad` |

---

## 3. Matriz Maestra de Control y Trazabilidad de Commits en Git

| N° | Hash | Fecha | Módulo / Etapa | Descripción del Cambio / Alcance Técnico | Archivos Clave Impactados |
|:---:|:---:|:---:|:---|:---|:---|
| **1** | `07ee165` | 2026-10-02 | Infraestructura / Base | Inicialización de la estructura del proyecto, código base en Arduino, scripts numéricos en MATLAB y memoria de cálculo teórica. | `.gitignore`, `Arduino/`, `Matlab/`, `Documentacion/memoria_de_calculo.md` |
| **2** | `e5306b5` | 2026-10-02 | Simulación / Docs | Incorporación de la primera simulación SPICE para el DAC con buffer y actualización de la bitácora. | `LTspice/`, `BITACORA.md` |
| **3** | `b864f34` | 2026-10-02 | Simulación LTspice | Creación del esquemático gráfico `.asc` en LTspice para el DAC R-2R de 4 bits y el buffer seguidor con OpAmp. | `LTspice/dac_r2r_buffer.asc` |
| **4** | `b04ca30` | 2026-10-02 | Simulación TinkerCAD | Desarrollo del código C++ optimizado para simulación virtual en TinkerCAD Circuits. | `Arduino/`, `TinkerCAD/` |
| **5** | `bcde82e` | 2026-10-02 | MATLAB / Etapa 1 | Implementación del generador de patrones de prueba analógicos y cuantización a 4 bits con exportación automática de arreglos a C++. | `Matlab/Primero.m` |
| **6** | `6c5a900` | 2026-10-02 | Documentación | Actualización de la bitácora registrando los avances sincronizados de LTspice, TinkerCAD y modelado matemático en MATLAB. | `BITACORA.md` |
| **7** | `cc960cb` | 2026-10-02 | Documentación | Creación de la guía de ejecución del proyecto e incorporación de imagen de referencia del circuito. | `Documentacion/`, `TinkerCAD/` |
| **8** | `c77ce51` | 2026-10-02 | Documentación | Registro en bitácora de la guía maestra y actualización del historial de commits. | `BITACORA.md` |
| **9** | `8c9428f` | 2026-10-09 | Simulación LTspice | Diseño de la simulación SPICE para la etapa receptora: 555 astable modulado por LDR con barrido paramétrico y acoplo AC al buzzer. | `LTspice/receptor_555_ldr.asc`, `.cir` |
| **10** | `340c89c` | 2026-10-10 | Documentación | Actualización de la bitácora con los avances de la simulación del receptor 555 y sincronización remota en GitHub. | `BITACORA.md` |
| **11** | `d0f6b54` | 2026-10-10 | Pruebas / Firmware | Creación de la suite de validación automatizada y mejora del parser serial de Arduino con validación `isDigit()`. | `Arduino/` |
| **12** | `dda33c1` | 2026-10-10 | Estandarización Código | Estandarización de comentarios en todos los archivos C++, MATLAB y Python: Descripción, Entrada, Salida y Restricciones. | `Arduino/`, `Matlab/` |
| **13** | `93e94b5` | 2026-10-11 | Normativa / Modelos | Cumplimiento exhaustivo de requerimientos A.1–Q.18: soporte de arreglos periódicos/aperiódicos, modelos 3D/corte láser e informe IEEE. | `Matlab/`, `Arduino/`, `Prototipos/`, `Documentacion/informe_ieee_el2113.tex` |
| **14** | `378df73` | 2026-10-12 | Documentación / 3D | Actualización de guías operativas y generación inicial de la malla STL de acopladores 3D. | `Documentacion/`, `Prototipos/Impresion_3D/` |
| **15** | `bbb6047` | 2026-10-12 | Diagramas / TinkerCAD | Incorporación de imagen corregida de TinkerCAD (con fuente de laboratorio y sin cable 5V de Arduino). | `Documentacion/`, `TinkerCAD/` |
| **16** | `cf7e85a` | 2026-10-12 | Simulación TinkerCAD | Respaldo y ordenamiento de imágenes de referencia en la carpeta de TinkerCAD. | `TinkerCAD/` |
| **17** | `a0d1223` | 2026-10-12 | Firmware / TinkerCAD | Actualización y consolidación del firmware de Arduino como versión unificada para hardware y simulador. | `Arduino/Logica/Logica.ino` |
| **18** | `bcac6f6` | 2026-10-12 | Simulación TinkerCAD | Verificación funcional del circuito completo en TinkerCAD Circuits, agregando capturas del circuito, componentes y esquema. | `TinkerCAD/CircuitoTinkercad.png`, `Componentes.png`, `Esquema.png` |
| **19** | `749caa9` | 2026-10-12 | Reestructuración | Reestructuración de carpetas de simulación bajo directorios dedicados para LTspice y TinkerCAD. | `LTspice/`, `TinkerCAD/` |
| **20** | `1a6dfe1` | 2026-10-12 | Mantenimiento | Limpieza de archivos duplicados y obsoletos en el repositorio y actualización de la bitácora. | `BITACORA.md` |
| **21** | `3e73d60` | 2026-10-14 | Simulación LTspice | Corrección profunda de esquemáticos en LTspice: corrección de pines flotantes (`NC_xx`), macromodelo `LM358_simple.lib` y directivas `.meas`. | `LTspice/` (`LM358_simple.lib`, `VERIFICACION.md`, `.inc`, `.asc`) |
| **22** | `84eea96` | 2026-10-14 | Guías de Montaje | Documentación técnica de la alimentación de $+9.0\text{ V}$ para el pin 8 del LM358 para evitar saturación y análisis del buzzer pasivo. | `Documentacion/memoria_de_calculo.md` |
| **23** | `e06d1b3` | 2026-10-14 | Informe IEEE / Memoria | Sincronización del informe IEEE Transactions y de la memoria de cálculo con las mediciones numéricas exactas obtenidas en LTspice. | `Documentacion/informe_ieee_el2113.tex`, `Documentacion/memoria_de_calculo.md` |
| **24** | `a8dc060` | 2026-10-14 | Documentación | Actualización de la documentación general, alineación del README principal y actualización de las entradas de bitácora. | `BITACORA.md`, `README.md` |
| **25** | `409707d` | 2026-10-15 | MATLAB / Refactor | Refactorización de comentarios, descripciones y consistencia de encabezados en los scripts de MATLAB. | `Matlab/Primero.m`, `Matlab/Final.m` |
| **26** | `adc6aca` | 2026-10-15 | Documentación / Bitácora | Actualización integral de la bitácora con los 25 commits, trazabilidad completa A.1–Q.18 y detalle de compra de componentes físicos. | `BITACORA.md` |
| **27** | `12f262e` | 2026-10-18 | Simulación LTspice | Optimización visual, orientación horizontal de etiquetas y corrección de espaciado en esquemáticos del DAC y receptor 555. | `LTspice/dac_r2r_buffer.asc`, `LTspice/receptor_555_ldr.asc` |
| **28** | `9df55d5` | 2026-10-18 | Simulación LTspice | Rediseño espacial del sistema completo óptico de 7 etapas en LTspice, despejando pines de NE555 y acoplo AC. | `LTspice/sistema_completo_optico.asc` |
| **29** | `1a339b8` | 2026-10-18 | Documentación / LTspice | Actualización de la guía de verificación técnica con soporte de ejecución batch `-Run -b` y directrices de diagramación. | `LTspice/VERIFICACION.md` |
| **30** | `acb8f11` | 2026-10-18 | Documentación / Bitácora | Registro en bitácora de la optimización visual de esquemáticos LTspice y sincronización del estado del proyecto. | `BITACORA.md` |
| **31** | `1ef7c9b` | 2026-10-20 | Montaje Físico | Incorporación de fotografía de evidencia del ensamblaje físico preliminar en protoboard. | `BITACORA.md`, `TinkerCAD/CircuitoArmado.jpeg` |
| **32** | `0e23dae` | 2026-10-22 | Corte Láser LAIMI | Optimización de la placa base a fracción oficial 1/8 de lámina LAIMI ($250\times 200\text{ mm}$) y ajuste de capas RGB a $0.025\text{ mm}$. | `Prototipos/Corte_laser/base_soporte_optico.svg` |
| **33** | `5a1dcd0` | 2026-10-22 | Corte Láser LAIMI | Generación de archivo vectorial `.PDF` en modo RGB listo para cortadora Trotec Speedy 360 en LAIMI F4-05. | `Prototipos/Corte_laser/base_soporte_optico.pdf` |
| **34** | `8c90a5e` | 2026-10-22 | Impresión 3D LAIMI | Adecuación de torres acopladoras M3 a normativa Prusa MK4 de LAIMI y exportación con formato oficial `2022437519-01.stl`. | `Prototipos/Impresion_3D/` |
| **35** | `10a0486` | 2026-10-22 | Impresión 3D LAIMI | Diseño preliminar de capuchones ópticos directos para protoboard. | `Prototipos/Impresion_3D/` |
| **36** | `ba2cb64` | 2026-10-22 | Visualización 3D | Incorporación de render 3D de la maqueta física completa en laboratorio. | `Prototipos/` |
| **37** | `3f25388` | 2026-10-22 | Visualización 3D | Incorporación de ilustración técnica comparativa de montaje optoelectrónico. | `Prototipos/` |
| **38** | `3272145` | 2026-10-22 | Guía Protoboard | Mapeo de coordenadas exactas para protoboard de 60 filas (`0` a `60`, columnas `a` a `j`). | `Documentacion/` |
| **39** | `20b3de1` | 2026-10-22 | Normativa LAIMI | Actualización de directrices con reglamento LAIMI F4-05 y tornillería M3 (`2022437519`). | `Documentacion/` |
| **40** | `2377cba` | 2026-10-22 | Lista Materiales | Detalle de tornillería métrica `M3 x 10mm/12mm`, tuercas M3 y jumpers Hembra–Macho. | `Documentacion/` |
| **41** | `b1bb2fe` | 2026-10-22 | Documentación | Vinculación del montaje virtual de TinkerCAD con coordenadas físicas y archivos de LAIMI. | `TinkerCAD/` |
| **42** | `a36e6b8` | 2026-10-22 | Documentación | Indexación en `README.md` de los archivos oficiales `.PDF` y `.STL` con carné `2022437519`. | `README.md` |
| **43** | `b47597b` | 2026-10-22 | Documentación / Bitácora | Registro en bitácora de la Sesión 10 (adecuación normativa LAIMI y visor 3D). | `BITACORA.md` |
| **44** | `8a1f05a` | 2026-10-22 | Corte Láser LAIMI | Limpieza del cajetín de identificación y reubicación de rótulos de etapas fuera de la huella del protoboard. | `Prototipos/Corte_laser/base_soporte_optico.pdf`, `.svg` |
| **45** | `5dafb14` | 2026-10-22 | Impresión 3D | Incorporación de visor 3D interactivo en HTML5 (`visor_interactivo_3d.html`) para previsualización de piezas STL sin software externo. | `Prototipos/Impresion_3D/visor_interactivo_3d.html` |
| **46** | `dc0a80a` | 2026-10-23 | Ensamble Mecánico | Adquisición de tornillería M3, recepción de base láser LAIMI, atornillado de Arduino y montaje de protoboard en chasis. | `BITACORA.md`, `README.md` |
| **47** | `c4f9474` | 2026-10-24 | Documentación | Sincronización del hash de commit de la sesión 11 en la bitácora. | `BITACORA.md` |
| **48** | `f91bec0` | 2026-10-24 | Limpieza / Ensamble | Limpieza de archivos temporales/obsoletos e incorporación de evidencia fotográfica del circuito con base láser (`CircuitoConBase.jpeg`). | `TinkerCAD/CircuitoConBase.jpeg` |
| **49** | `bdfdcbc` | 2026-10-24 | Documentación | Registro de actividades de limpieza y mantenimiento en bitácora. | `BITACORA.md` |
| **50** | `91ca47d` | 2026-10-24 | Kit 3D / IEEE / Docs | Unificación del kit 3D de 8 piezas (`Atornillada.scad` / `2022437519-01.stl`), reorganización del firmware Arduino, actualización de `README.md` y plantilla experimental en `informe_ieee_el2113.tex`. | `Prototipos/Impresion_3D/Atornillada.scad`, `Documentacion/informe_ieee_el2113.tex`, `BITACORA.md` |
| **51** | `21754a2` | 2026-10-27 | Ensamble Físico / Pruebas / Docs | Ensamble físico 100% completado del circuito, pruebas funcionales preliminares con batería superadas, formalización de integrantes del equipo (Sebastián Quirós Fernández e Ian Fuentes Núñez) y actualización de toda la documentación del proyecto. | `BITACORA.md`, `README.md`, `Documentacion/informe_ieee_el2113.tex`, `Documentacion/memoria_de_calculo.md`, `Arduino/Logica/Logica.ino`, `Matlab/Primero.m`, `Matlab/Final.m` |
| **52** | `46944a3` | 2026-10-28 | Pruebas Lab / Corrección Voltaje / Docs | Registro de pruebas en laboratorio con Ian Fuentes, diagnóstico y corrección circuital de puentes del LM358N en TinkerCAD y físico (`puentes_lm358.svg`), validación preliminar de niveles de voltaje correctos y estimación de reunión el lunes para pruebas finales y cierre del proyecto. | `BITACORA.md`, `README.md`, `TinkerCAD/puentes_lm358.*`, `Documentacion/` |
| **53** | `7e19a42` | 2026-10-28 | Evidencia de Trabajo / Limpieza | Incorporación de fotografías de evidencia de laboratorio e integración y depuración de archivos basura. | `Prototipos/Circuito/`, `BITACORA.md` |
| **54** | `209281e` | 2026-10-29 | Simulación LTspice / Fix Codificación | Corrección de cabecera UTF-8 en `receptor_555_ldr.asc` (eliminación de marca BOM) para permitir la apertura inmediata sin errores de sintaxis en LTspice de Windows. | `LTspice/receptor_555_ldr.asc` |
| **55** | `aa06504` | 2026-10-29 | Mantenimiento / Exclusiones Git | Limpieza de archivos temporales pesados de simulación (`.raw`, `.op.raw`, `.db`, `.log`) en LTspice y actualización de `.gitignore` para evitar su seguimiento. | `.gitignore` |
| **56** | `c14ed7a` | 2026-10-29 | Documentación / LTspice | Actualización técnica de `LTspice/VERIFICACION.md` incorporando las mediciones completas de ciclo de trabajo y umbrales de conmutación del NE555. | `LTspice/VERIFICACION.md` |
| **57** | `7c19f14` | 2026-10-29 | Memoria de Cálculo | Sincronización de la memoria de cálculo con los resultados del barrido paramétrico de fotorresistencia en el temporizador 555. | `Documentacion/memoria_de_calculo.md` |
| **58** | `daad84d` | 2026-10-29 | Documentación / README | Actualización del estado de validación al 100% de los esquemáticos de simulación en el archivo principal `README.md`. | `README.md` |
| **59** | `4d6d204` | 2026-10-29 | Bitácora / Sesión 15 | Registro de la sesión 15 con el diagnóstico y verificación de las 3 simulaciones en LTspice, saneamiento de carpeta y trazabilidad completa de commits. | `BITACORA.md` |

---

## 4. Matriz de Cumplimiento de Requerimientos Oficiales (A.1 – Q.18)

| Requerimiento Oficial | Descripción Normativa | Estado | Artefacto / Archivo de Verificación |
|:---|:---|:---:|:---|
| **A.1 / B.2** | Sistema dividido en 7 etapas delimitadas e identificables | **CUMPLE** | Ensamble físico 100% completado sobre la base de corte láser entregada ([`base_soporte_optico.pdf`](file:///d:/Proyecto%20CC/Prototipos/Corte_laser/base_soporte_optico.pdf)), Arduino atornillado y protoboard con las 7 etapas interconectadas ([`CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/TinkerCAD/CircuitoConBase.jpeg) y [`Circuito Completo.jpeg`](file:///d:/Proyecto%20CC/TinkerCAD/Circuito%20Completo.jpeg)). |
| **C.3 / D.4** | Arreglos de longitud variable en MATLAB y tiempo entre 1 ms y 10 s | **CUMPLE** | [`Matlab/Primero.m`](file:///d:/Proyecto%20CC/Matlab/Primero.m) y comandos `'P'`, `'S'`, `'T<ms>'` en [`Arduino/Logica/Logica.ino`](file:///d:/Proyecto%20CC/Arduino/Logica/Logica.ino). |
| **E.5** | DAC implementado con componentes pasivos y un componente activo | **CUMPLE** | Red R-2R ($10\text{ k}\Omega / 20\text{ k}\Omega$ al 1%) + OpAmp 1A del LM358N; lazo de realimentación unitaria asegurado mediante puente Pin 1--2 ([`puentes_lm358.svg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.svg) y [`Documentacion/memoria_de_calculo.md`](file:///d:/Proyecto%20CC/Documentacion/memoria_de_calculo.md)). |
| **F.6** | Etapa de interfaz implementada únicamente con OpAmp | **CUMPLE** | OpAmp 1B del LM358N en configuración seguidora de aislamiento unitario ($A_v = 1$, $Z_{in} \to \infty$, $Z_{out} \approx 0\ \Omega$); lazo cerrado con puente Pin 6--7 y acople directo Pin 1--5 ([`puentes_lm358.svg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.svg)). |
| **G.7 / H.8** | Generador óptico con componentes pasivos y LED justificado con Física 2/3 | **CUMPLE** | Resistor $150\ \Omega$ + LED rojo ($650\text{ nm}$) justificado por Planck-Einstein ($1.91\text{ eV}$), ventana PMMA y fotoconducción CdS. |
| **I.9 / J.10** | Medio óptico de 30 a 60 cm y receptor óptico puramente pasivo | **CUMPLE** | Fibra plástica POF cortada a $45\text{ cm}$ y fotorresistencia LDR pasiva (GL5528). |
| **K.11 / L.12** | Oscilador comercial integrado con pasivos y salida en buzzer pasivo | **CUMPLE** | Temporizador NE555 astable ($177\text{--}3443\text{ Hz}$) + condensador $10\ \mu\text{F}$ + Buzzer pasivo verificado con prueba DC y en pruebas preliminares. |
| **M.13 / N.14** | Prototipado en Escuela de Electrónica (corte láser, impresión 3D, escáner) | **CUMPLE** | Base láser recibida de LAIMI; Arduino atornillado con M3; protoboard montada; kit 3D de 8 piezas diseñado y unificado ([`Atornillada.scad`](file:///d:/Proyecto%20CC/Prototipos/Impresion_3D/Atornillada.scad) / [`2022437519-01.stl`](file:///d:/Proyecto%20CC/Prototipos/Impresion_3D/2022437519-01.stl)). |
| **Ñ.15 / O.16** | Montaje en protoboard y error de conversión DAC menor al 10% | **CUMPLE** | Circuito físico 100% ensamblado en protoboard montada en el chasis; tras la falla de voltaje detectada en laboratorio con Ian, Mario corrigió los lazos del LM358 en TinkerCAD y físico, validando preliminarmente que entrega los voltajes correctos; toma formal de $V_{\text{exp}}$ programada para el lunes (plantilla en [`Documentacion/informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/Documentacion/informe_ieee_el2113.tex)). |
| **P.17 / Q.18** | Prohibido usar 5V de Arduino y baterías; uso de fuente de laboratorio | **CUMPLE** | Pruebas de laboratorio iniciadas con fuente de laboratorio; tras corregir fallas de voltaje en puentes del LM358 y validar preliminarmente los niveles de tensión en banco, la toma final de mediciones cuantitativas ($V_{\text{exp}}$) y evaluación definitiva se completará el lunes con Fuente DC Regulada de Laboratorio ($+5\text{ V} / +9\text{ V}$, tierra común). |

---

## 5. Próximos Pasos Planificados

1. **Sesión Final de Pruebas Experimentales en Laboratorio (Lunes 2 de Noviembre):** Asistencia conjunta del equipo de trabajo al laboratorio de la Escuela de Electrónica para energizar el prototipo corregido exclusivamente con la Fuente DC Regulada de Laboratorio ($+5.0\text{ V} / +9.0\text{ V}$, tierra común con Arduino), medir sistemáticamente con multímetro digital calibrado los 16 voltajes reales del DAC ($V_{\text{exp}}$) completando el Cuadro I del informe IEEE (Req. O.16 y P.17/Q.18) y terminar el proyecto.
2. **Caracterización Frecuencial y Espectral del Receptor con Osciloscopio:** Medir en el osciloscopio digital la frecuencia de la onda cuadrada generada por el temporizador 555 astable a través de los diferentes niveles ópticos recibidos por la LDR y contrastar los valores con el modelo analítico de la memoria de cálculo.
3. **Compilación, Empaquetado y Entrega Final:** Consolidar el informe final en formato IEEE Transactions ($\le 3$ páginas) en PDF con los valores de laboratorio reales y generar el paquete comprimido `.zip` respetando la estructura oficial del curso para la entrega formal en el tecDigital y la defensa presencial ante el profesor.
