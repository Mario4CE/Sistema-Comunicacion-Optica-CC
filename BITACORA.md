# Bitácora de Desarrollo del Proyecto
**Proyecto:** Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos Eléctricos en Corriente Continua (EL2113)  
**Estudiante / Responsable:** Mario Cerdas  

---

## Registro de Actividades y Avances

### [2026-10-02] - Inicio del proyecto y estructura base
* **Configuración del repositorio:**
  * Se creó el repositorio local de Git y el archivo `.gitignore` para excluir archivos temporales de Arduino, MATLAB y LTspice.
* **Etapa 1 (Sistema Digital):**
  * Se programó la primera versión del código de Arduino para el control de los 4 pines digitales (`D8` a `D11`) hacia el DAC R-2R, con modo escalera, manual y calibración.
* **Cálculos y simulación en MATLAB:**
  * Se hizo el script `matlab/simulacion_dac_555.m` para calcular los niveles de tensión del DAC, la corriente del LED, el cambio de resistencia en la LDR y la frecuencia de salida del 555.
  * Se creó `matlab/generador_patrones.m` para cuantizar señales a 4 bits y obtener los arreglos de prueba.
* **Documentación inicial:**
  * Se redactaron `docs/memoria_de_calculo.md`, `docs/lista_de_materiales_BOM.md` y `docs/guia_de_conexion_protoboard.md`.
  * Se armaron las primeras simulaciones en LTspice (`dac_r2r_buffer.asc` y `receptor_555_ldr.asc`).

### [2026-10-10] - Rama de trabajo y documentación del código
* **Organización en Git:**
  * Se creó la rama `mario-dev` para ir subiendo los avances y revisiones.
* **Comentarios en el código:**
  * Se ordenaron los comentarios de las funciones en Arduino y MATLAB indicando descripción, entradas, salidas y restricciones de operación.

### [2026-10-11] - Arreglos desde MATLAB, diseño 3D/láser y memoria de cálculo
* **Señales periódicas y aperiódicas:**
  * Se ajustó `matlab/generador_patrones.m` para generar arreglos de longitud variable de una señal periódica (20 muestras) y una señal aperiódica amortiguada (18 muestras).
  * Se actualizó el código de Arduino con los arreglos exportados desde MATLAB (comandos `'P'` y `'S'`) y el ajuste de tiempo por muestra entre $1\text{ ms}$ y $10\,000\text{ ms}$ con el comando `'T<ms>'`.
* **Análisis teórico y separación de etapas:**
  * En `docs/memoria_de_calculo.md` se desarrolló el análisis nodal por Leyes de Kirchhoff (LCK en los nodos $V_1, V_2, V_3, V_4$) para demostrar la ecuación del DAC R-2R, dejando el OpAmp 1A del LM358 en la Etapa 2 y el OpAmp 1B como seguidor aislador en la Etapa 3.
  * Se agregó la justificación física del uso del LED rojo ($\lambda = 650\text{ nm}$) según la atenuación de la fibra plástica PMMA y la respuesta de la fotorresistencia.
* **Piezas de apoyo y avance del informe:**
  * Se diseñó la base para corte láser (`prototipo_laboratorio/corte_laser/base_soporte_optico.svg`) con las divisiones de las 7 etapas y los soportes en impresión 3D (`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad` y `.stl`) para alinear el LED y la LDR con la fibra.
  * Se armó la simulación del circuito completo en LTspice (`Simulacion/ltspice/sistema_completo_optico.asc`) y el borrador del informe en formato IEEE (`docs/informe_ieee/informe_ieee_el2113.tex`).

### [2026-10-12] - Simulación funcional en TinkerCAD y limpieza del repositorio
* **Simulación y pruebas en TinkerCAD:**
  * Se actualizó el código de Arduino en `src/arduino/generador_patrones_tinkercad.ino` y se verificó el funcionamiento del circuito en TinkerCAD.
  * Se agregaron las capturas del montaje (`CircuitoTinkercad.png`, `Componentes.png` y `Esquema.png`) junto con la guía `Simulacion/tinkercad/Montaje.md`.
* **Organización de carpetas y archivos:**
  * Se renombró la carpeta de simulaciones a `Simulacion/`.
  * Se eliminaron códigos y archivos repetidos dejando una única versión del programa de Arduino en `src/arduino/generador_patrones_tinkercad.ino` y limpiando archivos duplicados en `docs/` y `Simulacion/`.

---

## Registro de Commits
* Inicialización de la estructura del proyecto, código de Arduino, scripts de MATLAB y memoria de cálculo.
* Agregado de simulaciones en LTspice y guía de conexión en protoboard.
* Esquemático del DAC R-2R con buffer y etapa receptora con el 555 en LTspice.
* Generador de patrones de prueba en MATLAB con cuantización a 4 bits.
* Creación de la rama de trabajo y documentación de funciones en el código.
* Incorporación de arreglos de señales periódicas y aperiódicas, modelos para corte láser e impresión 3D, y borrador del informe IEEE.
* Actualización de las guías de montaje físico, lista de materiales y archivo STL de los acopladores.
* Actualización del código del generador de patrones de 4 bits.
* Simulación de TinkerCAD funcional con capturas del circuito, lista de componentes y esquema.
* Reorganización de la carpeta `Simulacion` y limpieza de archivos repetidos en el repositorio.
