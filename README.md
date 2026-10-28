# Sistema de Comunicación por Medios Ópticos (EL2113)

Repositorio del proyecto de Investigación Dirigida para el curso **Circuitos Eléctricos en Corriente Continua (EL2113)**, Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR).

**Responsable:** Mario Cerdas  
**Estado Actual del Prototipo:** Base de soporte en corte láser recibida de LAIMI e integrada con éxito; Arduino UNO fijado rígidamente con tornillería métrica M3 y protoboard con el circuito completo de 7 etapas montada sobre el chasis delimitado ([`CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoConBase.jpeg)). Kit 3D unificado de 8 piezas ([`Atornillada.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/Atornillada.scad) / [`2022437519-01.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-01.stl)) que integra 2 torres coaxiales con fijación M3 y 6 clips de retención para fijar la fibra óptica plástica ($45\text{ cm}$) en las ranuras de la base sin necesidad de gazas plásticas.  
**Objetivo:** Transmitir arreglos de longitud variable de datos digitales de 4 bits (de señales periódicas y aperiódicas generadas en MATLAB) mediante una señal luminosa guiada por fibra óptica plástica ($30\text{--}60\text{ cm}$), recuperarla con un receptor óptico pasivo (LDR) y convertirla en una señal audible mediante un oscilador 555 y un buzzer pasivo.

---

## Diagrama de Bloques de las 7 Etapas

```
 [ Fuente DC de Laboratorio (Escuela de Electrónica) ]
                                       │
  [ MATLAB ]                           ▼                           [ Etapa 7: Buzzer Pasivo ]
      │ (Arreglos 4 bits)  [ Etapa 2: DAC R-2R + OpAmp 1A ]                    ▲
      ▼                                │ (Voltaje)                             │ (Onda Cuadrada AC)
 [ Etapa 1: Arduino ] ─────────────────┴──► [ Etapa 3: Interfaz OpAmp 1B ]     │
   (Pines D8..D11)                                     │                       │
   (1 ms a 10 s)                                       ▼                       │
                                            [ Etapa 4: R_drv + LED Rojo ]      │
                                                       ║                       │
                                             (Fibra POF 45 cm +                │
                                              Torres 3D y Base Láser)          │
                                                       ║                       │
                                                       ▼                       │
                                            [ Etapa 5: Receptor LDR ] ─────────┤ (Resistencia)
                                                                               ▼
                                                                   [ Etapa 6: Astable 555 ]
```

---

## Estructura del Repositorio

* **[`BITACORA.md`](file:///d:/Proyecto%20CC/BITACORA.md)**: Registro exhaustivo de actividades, avances, historial de commits y cumplimiento de requerimientos oficiales (A.1--Q.18).
* **[`src/arduino/generador_patrones_tinkercad/generador_patrones_tinkercad.ino`](file:///d:/Proyecto%20CC/src/arduino/generador_patrones_tinkercad/generador_patrones_tinkercad.ino)**: Código de Arduino UNO con los arreglos de 4 bits (escalera de 16 niveles, señal periódica de 20 muestras y señal aperiódica de 18 muestras) y tiempo de permanencia configurable entre $1\text{ ms}$ y $10\text{ s}$. Se usa tanto en el montaje físico como en TinkerCAD.
* **[`matlab/generador_patrones.m`](file:///d:/Proyecto%20CC/matlab/generador_patrones.m)** y **[`matlab/simulacion_dac_555.m`](file:///d:/Proyecto%20CC/matlab/simulacion_dac_555.m)**: Scripts en MATLAB para cuantizar las señales periódicas/aperiódicas a 4 bits, calcular el SQNR ($25.84\text{ dB}$) y modelar la respuesta teórica de las etapas.
* **[`Simulacion/tinkercad/`](file:///d:/Proyecto%20CC/Simulacion/tinkercad)**: Evidencias de simulación virtual y fotográficas del prototipo físico:
  * [`CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoConBase.jpeg): Fotografía de alta resolución de la placa base de MDF en corte láser con el Arduino UNO atornillado con tornillería M3 y el circuito de 7 etapas montado en protoboard.
  * [`CircuitoArmado.jpeg`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoArmado.jpeg): Fotografía de la verificación preliminar de conexionado en protoboard.
  * [`CircuitoTinkercad.png`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoTinkercad.png), [`Componentes.png`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/Componentes.png) y [`Esquema.png`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/Esquema.png): Simulación funcional en TinkerCAD Circuits.
* **[`Simulacion/ltspice/`](file:///d:/Proyecto%20CC/Simulacion/ltspice)**: Simulaciones en LTspice de la etapa emisora ([`dac_r2r_buffer.asc`](file:///d:/Proyecto%20CC/Simulacion/ltspice/dac_r2r_buffer.asc)), etapa receptora ([`receptor_555_ldr.asc`](file:///d:/Proyecto%20CC/Simulacion/ltspice/receptor_555_ldr.asc)) y sistema completo de las 7 etapas ([`sistema_completo_optico.asc`](file:///d:/Proyecto%20CC/Simulacion/ltspice/sistema_completo_optico.asc) y su netlist equivalente [`.cir`](file:///d:/Proyecto%20CC/Simulacion/ltspice/sistema_completo_optico.cir)). Incluye el macromodelo [`LM358_simple.lib`](file:///d:/Proyecto%20CC/Simulacion/ltspice/LM358_simple.lib), mediciones automáticas (`mediciones_*.inc`) y la guía [`VERIFICACION.md`](file:///d:/Proyecto%20CC/Simulacion/ltspice/VERIFICACION.md) con los resultados esperados.
* **[`prototipo_laboratorio/`](file:///d:/Proyecto%20CC/prototipo_laboratorio)**:
  * **Corte y Grabado Láser (Normativa LAIMI F4-05):** [`base_soporte_optico.pdf`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.pdf) (vectorial en modo RGB para Trotec Speedy 360 en fracción oficial de $1/8$ de lámina de $250\text{ mm} \times 200\text{ mm}$) y su archivo fuente [`base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg).
  * **Impresión 3D (Normativa LAIMI Prusa MK4 - Carné 2022437519):**
    * [`2022437519-01.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-01.stl): Malla sólida del **Kit Unificado de 8 piezas** optimizado para impresión plana directa (cama de $54.9\text{ mm} \times 43.6\text{ mm}$, sin soportes).
    * [`Atornillada.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/Atornillada.scad): Código OpenSCAD paramétrico que genera el kit completo:
      1. 1x Torre Horizontal Atornillada M3 para Emisor TX (LED Rojo $5\text{ mm}$, cámara $\varnothing 6.2\text{ mm}$).
      2. 1x Torre Horizontal Atornillada M3 para Receptor RX (LDR GL5528, cámara $\varnothing 6.4\text{ mm}$).
      3. 3x Grapas/clips de retención transversales para fijar la fibra frente a las torres ($Z=6.0\text{ mm}$).
      4. 3x Grapas/clips de retención longitudinales para ranuras de $7\times 3\text{ mm}$ del arco exterior de la base láser.
    * [`visor_interactivo_3d.html`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/visor_interactivo_3d.html): Visor web 3D interactivo en HTML5/Three.js para inspeccionar las piezas impresas.
* **[`docs/`](file:///d:/Proyecto%20CC/docs)**:
  * [`memoria_de_calculo.md`](file:///d:/Proyecto%20CC/docs/memoria_de_calculo.md): Desarrollo formal de ecuaciones de nodos de Kirchhoff (LCK/LVK), diseño de la red R--2R ($10\text{ k}\Omega / 20\text{ k}\Omega$), interfaz de aislamiento con OpAmp 1B, justificación cuántica/óptica del LED rojo mediante Física 2 y 3, ecuaciones de temporización del astable 555 y acoplo AC al buzzer pasivo.
  * [`informe_ieee/informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/docs/informe_ieee/informe_ieee_el2113.tex): Informe final del proyecto estructurado en plantilla oficial IEEE Transactions (máximo 3 páginas), con tabla comparativa de resultados teóricos, simulados y plantilla para mediciones experimentales de laboratorio.
