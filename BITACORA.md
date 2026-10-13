# Bitácora de Desarrollo del Proyecto
**Proyecto:** Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos Eléctricos en Corriente Continua (EL2113)  
**Estudiante / Responsable:** Mario Cerdas  

---

## Registro de Actividades y Avances

### [2026-10-02] - Sesión de Inicio e Infraestructura Base
* **Inicialización del Control de Versiones:**
  * Se inicializó el repositorio local de Git en `D:\Proyecto CC`.
  * Se configuró el archivo `.gitignore` adaptado a proyectos de Arduino, MATLAB y LTspice.
* **Etapa 1 (Sistema Digital):**
  * Se desarrolló el firmware inicial para Arduino en `src/arduino/emisor_4bits/emisor_4bits.ino`.
  * Se implementó el control de 4 pines digitales (D8 a D11) con modos automático, manual y calibración.
* **Modelado y Simulación en MATLAB:**
  * Se creó el script `matlab/simulacion_dac_555.m` que modela numéricamente las curvas características del sistema.
  * Se creó `matlab/generador_patrones.m` para cuantización a 4 bits y cálculo de SQNR ($25.84\text{ dB}$).
* **Memoria de Cálculo, Guías y Simulaciones:**
  * Se redactó `docs/memoria_de_calculo.md`, `docs/lista_de_materiales_BOM.md` y `docs/guia_de_conexion_protoboard.md`.
  * Se crearon las simulaciones en LTspice (`dac_r2r_buffer.asc`/`.cir` y `receptor_555_ldr.asc`/`.cir`) y TinkerCAD (`generador_patrones_tinkercad.ino` y `guia_montaje_tinkercad.md`).
  * Se implementó la suite de validación `src/test_validacion_sistema.py`.

### [2026-10-10] - Creación de Rama de Trabajo y Estandarización de Comentarios
* **Flujo de Trabajo Colaborativo en Git:**
  * Se creó y activó la rama independiente `mario-dev` a partir de `main`.
* **Estandarización de Documentación Interna de Código:**
  * Se aplicó el estándar formal de comentarios en todo el código del proyecto:
    * **Clases:** Encabezado con breve descripción y listado de funciones/métodos que contiene.
    * **Funciones:** Bloques estructurados con **Descripción**, **Entrada**, **Salida** y **Restricciones**, además de comentarios de línea explicativos.

### [2026-10-11] - Cumplimiento Integral al Pie de la Letra (Requerimientos A.1–Q.18 y Laboratorios)
* **Arreglos de Longitud Variable y Señales Periódicas/Aperiódicas (Req. C.3 y D.4):**
  * Se amplió `matlab/generador_patrones.m` con funciones documentadas (`cuantizar_senal_4bits`, `imprimir_arreglo_cpp`) que generan arreglos de longitud variable tanto para señales **periódicas** ($N=20$) como **aperiódicas** ($N=18$).
  * Se actualizaron `src/arduino/emisor_4bits/emisor_4bits.ino` y `simulation/tinkercad/generador_patrones_tinkercad.ino` para recorrer arreglos de longitud variable exportados desde MATLAB (comandos `'P'` y `'S'`) y configurar el tiempo de estabilidad en el rango estricto $[1\text{ ms}, 10\,000\text{ ms}]$ (comando `'T<ms>'`).
* **Separación Activa Etapa 2 y Etapa 3 + Análisis Nodal de Kirchhoff + Física 2/3 (Req. E.5, F.6 y H.8):**
  * Se actualizó `docs/memoria_de_calculo.md` desarrollando las ecuaciones explícitas de nodos de Kirchhoff (LCK en $V_1, V_2, V_3, V_4$), asignando el **OpAmp 1A del LM358** como componente activo del DAC (Etapa 2) y el **OpAmp 1B del LM358** como etapa de interfaz aisladora (Etapa 3), e incorporando la justificación física del LED rojo ($\lambda = 650\text{ nm}$) basada en Física 2 y 3 (ecuación de Planck-Einstein, Ley de Snell y ventana de mínima atenuación de PMMA).
* **Cumplimiento de Laboratorios de la Escuela de Electrónica (Req. M.13, N.14, P.17 y Q.18):**
  * Se corrigieron `docs/guia_de_conexion_protoboard.md` y `simulation/tinkercad/guia_montaje_tinkercad.md` prohibiendo alimentar las etapas desde el pin `5V` del Arduino y especificando el uso exclusivo de las **Fuentes DC de Laboratorio de la bodega de la Escuela de Electrónica**.
  * Se diseñó la base organizadora para **Corte Láser** (`prototipo_laboratorio/corte_laser/base_soporte_optico.svg`) con las 7 etapas grabadas y delimitadas (**Req. B.2 y M.13**).
  * Se diseñó el par de cabezales acopladores coaxiales opacos para **Impresión 3D** (`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad`) y se documentó el protocolo de **Escáner 3D** para puntos extra (`docs/diseno_prototipado_3d_laser.md`, **Req. N.14**).
* **Simulación Integrada en Conjunto, Reporte IEEE Transactions y Empaquetador Oficial (Sección 4.1 y 4.2):**
  * Se crearon `simulation/ltspice/sistema_completo_optico.cir` y `.asc` para simular las 7 etapas en conjunto.
  * Se redactó el informe en formato **IEEE Transactions** (`docs/informe_ieee/informe_ieee_el2113.tex`).
  * Se creó `src/empaquetar_entrega_oficial.py` para generar automáticamente la estructura oficial `Investigacion_Dirigida/` y `Investigacion_Dirigida.zip`.

### [2026-10-12] - Actualización Integral de Guías Operativas (.md) y Preparación de Montaje Físico
* **Revisión y Expansión de Guías de Montaje y Laboratorio:**
  * Se reestructuró [`docs/GUIA_PASO_A_PASO_GENERAL.md`](file:///d:/Proyecto%20CC/docs/GUIA_PASO_A_PASO_GENERAL.md) como el manual operativo maestro que guía al usuario desde la Fase 0 (materiales y bodega) hasta la Fase 7 (entrega oficial), detallando el montaje paso a paso, comandos seriales y checklist de laboratorio.
  * Se actualizó [`docs/lista_de_materiales_BOM.md`](file:///d:/Proyecto%20CC/docs/lista_de_materiales_BOM.md) con los equipos de bodega de la Escuela de Electrónica (fuente DC, multímetro, osciloscopio), piezas de corte láser/impresión 3D, pinouts de circuitos integrados (LM358N y NE555) y la prueba de descarte de buzzer pasivo vs activo.
  * Se actualizó [`docs/guia_de_conexion_protoboard.md`](file:///d:/Proyecto%20CC/docs/guia_de_conexion_protoboard.md) con un mapa visual de distribución en el protoboard (zona transmisora izquierda y zona receptora derecha), procedimiento incremental de 6 pasos y sección de troubleshooting para laboratorio.
  * Se generó la malla 3D ASCII [`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl) (512 facetas) lista para corte en slicer (Cura/PrusaSlicer) y se integró en el empaquetador oficial.

---

## Historial de Commits en Git
* `07ee165` - *Inicializar estructura de proyecto, codigo Arduino, simulacion MATLAB y documentacion de calculo*
* `e5306b5` - *Agregar simulacion SPICE, guia detallada de conexion en protoboard y actualizar bitacora*
* `b864f34` - *feat(simulation): agregar esquematico grafico .asc de LTspice para DAC R-2R y buffer*
* `b04ca30` - *feat(simulation): agregar codigo y guia de montaje paso a paso para TinkerCAD*
* `bcde82e` - *feat(matlab): agregar generador de patrones de prueba y cuantizacion con exportacion a C++*
* `cc960cb` - *docs: agregar guia maestra paso a paso e imagen de referencia para TinkerCAD*
* `8c9428f` - *feat(simulation): agregar simulacion de etapa receptora 555 astable modulado por LDR en LTspice*
* `340c89c` - *docs: actualizar bitacora con commit 8c9428f y sincronizacion remota*
* `d0f6b54` - *feat(test): agregar suite de validacion automatica de C++ y circuitos y mejorar parser serial*
* `ef1578c` - *docs(comments): estandarizar documentacion de clases y funciones en todo el codigo*
* `93e94b5` - *feat(compliance): cumplir al 100% requerimientos A.1-Q.18, arreglos MATLAB, laboratorios 3D/laser e informe IEEE*

