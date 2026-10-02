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
* **Guía de Montaje Físico y Puntos de Prueba:**
  * Se redactó `docs/guia_de_conexion_protoboard.md` con instrucciones pin a pin para el armado en protoboard y tabla de puntos de prueba (TP1 a TP5) con multímetro/osciloscopio.

---

## Próximos Pasos Planificados
1. Vincular el repositorio a GitHub cuando el usuario proporcione la URL del repositorio remoto.
2. Añadir la plantilla del informe formal en LaTeX / Markdown según el formato exigido por la cátedra.
3. Pruebas y registro de mediciones experimentales en laboratorio.
