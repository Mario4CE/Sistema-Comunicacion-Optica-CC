# Bitácora de Desarrollo del Proyecto
**Proyecto:** Sistema de Comunicación por Medios Ópticos  
**Curso:** Circuitos y Comunicaciones (EL2113)  
**Estudiante / Responsable:** Mario Cerdas  

---

## Registro de Actividades y Avances

### [2026-10-02] - Sesión de Inicio e Infraestructura Base
* **Inicialización del Control de Versiones:**
  * Se inicializó el repositorio local de Git en `D:\Proyecto CC`.
  * Se configuró el archivo `.gitignore` adaptado a proyectos de Arduino, MATLAB y LTspice.
* **Etapa 1 (Sistema Digital):**
  * Se desarrolló el firmware para Arduino en `src/arduino/emisor_4bits/emisor_4bits.ino`.
  * Se implementó el control de 4 pines digitales (D8 a D11) con tres modos operativos:
    1. *Modo Automático*: Recorrido ascendente 0 a 15 con retardo no bloqueante (`millis()`) para dar estabilidad a la LDR.
    2. *Modo Manual*: Fijación de valores específicos por monitor serial.
    3. *Modo Calibración*: Alternancia periódica entre 0 y 15 para calibrar los extremos de luz.
* **Modelado y Simulación en MATLAB:**
  * Se creó el script `matlab/simulacion_dac_555.m` que modela numéricamente:
    1. La escalera de voltajes teórica de la red R-2R (16 estados).
    2. La corriente del driver del LED.
    3. La curva de resistencia de la LDR.
    4. La frecuencia resultante en el temporizador NE555 astable.
* **Memoria de Cálculo y Análisis Circuital:**
  * Se redactó `docs/memoria_de_calculo.md` con las deducciones formales mediante el equivalente de Thévenin y leyes de Kirchhoff.
  * Se demostró matemáticamente la necesidad del buffer seguidor de voltaje (LM358) para evitar el colapso por efecto de carga (>98% de pérdida si no se usa buffer).
  * Se calcularon las frecuencias teóricas del 555 para el rango audible ($180\text{ Hz}$ a $3.6\text{ kHz}$).
* **Lista de Materiales y Guía de Compras:**
  * Se generó `docs/lista_de_materiales_BOM.md` especificando componentes, tolerancias y la advertencia clave de adquirir un buzzer pasivo y no activo.

* **Simulación Circuital (LTspice):**
  * Se creó el netlist SPICE `simulation/ltspice/dac_r2r_buffer.cir` para validar la respuesta analógica y el aislamiento del buffer.
  * Se generó el esquemático visual gráfico `simulation/ltspice/dac_r2r_buffer.asc` con generadores de pulsos y directivas de simulación transitoria.
* **Entorno de Simulación Virtual (TinkerCAD):**
  * Se creó `simulation/tinkercad/codigo_tinkercad.ino` optimizado para el editor web de Arduino en TinkerCAD.
  * Se redactó `simulation/tinkercad/guia_montaje_tinkercad.md` con la guía de cableado, selección de componentes y conexión de instrumentos virtuales.
* **Modelado Avanzado en MATLAB:**
  * Se creó `matlab/generador_patrones.m` para generar señales sinusoidales cuantizadas a 4 bits, calcular la relación señal a ruido de cuantización ($SQNR = 25.84\text{ dB}$) y exportar automáticamente arreglos en formato C++ para Arduino.
* **Guía de Montaje Físico y Puntos de Prueba:**
  * Se redactó `docs/guia_de_conexion_protoboard.md` con instrucciones pin a pin para el armado en protoboard y tabla de puntos de prueba (TP1 a TP5) con multímetro/osciloscopio.
* **Simulación de Etapa Receptora (LTspice - 555 Astable + LDR + Buzzer):**
  * Se diseñó el netlist SPICE `simulation/ltspice/receptor_555_ldr.cir` modelando el temporizador NE555 astable modulado por la resistencia óptica de la LDR.
  * Se implementó el barrido paramétrico `.step param R_LDR list 1.5k 5k 10k 20k 40k` para validar todo el rango audible ($180\text{ Hz}$ a $3.6\text{ kHz}$).
  * Se incluyó la red de acoplamiento AC ($C_{acoplo} = 10\ \mu\text{F}$) y el modelo de transductor acústico (buzzer pasivo de $32\ \Omega$).
  * Se generó el esquemático gráfico interactivo `simulation/ltspice/receptor_555_ldr.asc` para visualización y edición en la interfaz gráfica de LTspice.
* **Validación Automatizada de Código C++ y Modelos Físicos:**
  * Se mejoró la validación de entrada serial en `src/arduino/emisor_4bits/emisor_4bits.ino` (`isDigit`) para evitar que `toInt()` convierta texto no numérico en el estado `0` y soportar el comando `'M'` explícito.
  * Se desarrolló y ejecutó la suite de pruebas `src/test_validacion_sistema.py` que verifica la sintaxis de los archivos `.ino`, emula la lógica bit a bit, resuelve las ecuaciones nodales de Kirchhoff de la red R-2R (error $0.0000\text{ V}$) y valida la respuesta del 555 astable.
  * Se actualizó `docs/GUIA_PASO_A_PASO_GENERAL.md` con los pasos de simulación del circuito receptor en LTspice.

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

---

## Próximos Pasos Planificados
1. Preparar la estructura del informe final escrito formal según las guías del curso.
2. Pruebas experimentales en protoboard y validación con osciloscopio / multímetro.
