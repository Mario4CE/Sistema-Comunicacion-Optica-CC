# Sistema de Comunicación por Medios Ópticos (EL2113)

Repositorio del proyecto de Investigación Dirigida para el curso **Circuitos Eléctricos en Corriente Continua (EL2113)**, Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR).

**Equipo de Trabajo:**
* **Mario Cerdas** (Carné: 2022437519)
* **Sebastián Quirós Fernández** (Carné: 2024073526, `s.quiros.4@estudiantec.cr`)
* **Ian Fuentes Núñez** (Carné: 2025100912, `I.fuentes.1@estudiantec.cr`)

**Profesor:** Ing. Fabián Alberto Picado García  
**Semestre:** II Semestre 2026  

**Estado Actual del Prototipo:** El circuito físico de 7 etapas se encuentra completamente ensamblado sobre la base de corte láser LAIMI ($250\times 200\text{ mm}$) con el Arduino UNO fijado con tornillería M3 y la fibra óptica plástica ($45\text{ cm}$) alineada. Durante una primera sesión de pruebas experimentales en el laboratorio realizada por Mario Cerdas e Ian Fuentes, las mediciones no pudieron concluirse debido a discrepancias detectadas en el voltaje de salida del circuito. Posteriormente, Mario realizó una exhaustiva depuración tanto en la simulación virtual de TinkerCAD como en la protoboard física, detectando y corrigiendo la configuración de los puentes de realimentación del amplificador operacional dual LM358N (Pin 1 a 2 para el buffer DAC, Pin 6 a 7 para el buffer de interfaz y Pin 1 a 5 para el acople directo, documentados en [`puentes_lm358.svg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.svg)). Pruebas preliminares posteriores validaron de forma exitosa que el circuito ahora entrega con precisión los niveles de voltaje correctos. Se tiene programada una sesión conjunta de trabajo para el próximo **lunes 2 de noviembre** en el laboratorio para completar las mediciones cuantitativas finales ($V_{\text{exp}}$ y respuesta frecuencial con osciloscopio) y concluir el proyecto.

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

## Estructura del Repositorio (Alineada con el Formato Oficial de Entrega)

* **[`BITACORA.md`](file:///d:/Proyecto%20CC/BITACORA.md)**: Registro exhaustivo de actividades cronológicas, estado del ensamble físico, pruebas de banco, historial de commits y matriz de cumplimiento de requerimientos oficiales (A.1--Q.18).
* **[`Arduino/Logica/Logica.ino`](file:///d:/Proyecto%20CC/Arduino/Logica/Logica.ino)**: Firmware para Arduino UNO con los arreglos de 4 bits (escalera de 16 niveles, señal periódica de 20 muestras y señal aperiódica de 18 muestras exportadas de MATLAB) y tiempo de permanencia configurable entre $1\text{ ms}$ y $10\text{ s}$ (`'T<ms>'`).
* **[`Matlab/Primero.m`](file:///d:/Proyecto%20CC/Matlab/Primero.m)** y **[`Matlab/Final.m`](file:///d:/Proyecto%20CC/Matlab/Final.m)**: Scripts en MATLAB para digitalizar/cuantizar señales a 4 bits, calcular el SQNR ($25.84\text{ dB}$), generar código C++ y simular la respuesta analógica y acústica del sistema completo.
* **[`TinkerCAD/`](file:///d:/Proyecto%20CC/TinkerCAD)**: Evidencias de simulación virtual y fotográficas del prototipo físico:
  * [`CircuitoConBase.jpeg`](file:///d:/Proyecto%20CC/TinkerCAD/CircuitoConBase.jpeg): Fotografía de la placa base de MDF en corte láser con el Arduino UNO atornillado con tornillería M3 y el circuito de 7 etapas montado en protoboard.
  * [`Circuito Completo.jpeg`](file:///d:/Proyecto%20CC/TinkerCAD/Circuito%20Completo.jpeg) y [`CircuitoArmado.jpeg`](file:///d:/Proyecto%20CC/TinkerCAD/CircuitoArmado.jpeg): Evidencias del circuito completamente ensamblado y cableado.
  * [`CircuitoTinkercad.png`](file:///d:/Proyecto%20CC/TinkerCAD/CircuitoTinkercad.png), [`Componentes.png`](file:///d:/Proyecto%20CC/TinkerCAD/Componentes.png) y [`Esquema.png`](file:///d:/Proyecto%20CC/TinkerCAD/Esquema.png): Simulación funcional en TinkerCAD Circuits.
  * [`puentes_lm358.svg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.svg) y [`puentes_lm358.jpg`](file:///d:/Proyecto%20CC/TinkerCAD/puentes_lm358.jpg): Diagrama técnico de corrección de puentes y lazos de realimentación para los seguidores de tensión del amplificador operacional dual LM358N (Pin 1 a 2 para DAC, Pin 6 a 7 para interfaz y Pin 1 a 5 para acople inter-etapas).
  * [`Enlace.txt`](file:///d:/Proyecto%20CC/TinkerCAD/Enlace.txt): Enlace interactivo al diseño y simulación en línea de TinkerCAD.
* **[`LTspice/`](file:///d:/Proyecto%20CC/LTspice)**: Simulaciones en LTspice de la etapa emisora ([`dac_r2r_buffer.asc`](file:///d:/Proyecto%20CC/LTspice/dac_r2r_buffer.asc)), etapa receptora ([`receptor_555_ldr.asc`](file:///d:/Proyecto%20CC/LTspice/receptor_555_ldr.asc)) y sistema completo de las 7 etapas ([`sistema_completo_optico.asc`](file:///d:/Proyecto%20CC/LTspice/sistema_completo_optico.asc) y su netlist [`.cir`](file:///d:/Proyecto%20CC/LTspice/sistema_completo_optico.cir)). Incluye macromodelo [`LM358_simple.lib`](file:///d:/Proyecto%20CC/LTspice/LM358_simple.lib), mediciones automáticas (`mediciones_*.inc`) y la guía técnica [`VERIFICACION.md`](file:///d:/Proyecto%20CC/LTspice/VERIFICACION.md).
* **[`Prototipos/`](file:///d:/Proyecto%20CC/Prototipos)**:
  * **Corte y Grabado Láser (Normativa LAIMI F4-05):** [`base_soporte_optico.pdf`](file:///d:/Proyecto%20CC/Prototipos/Corte_laser/base_soporte_optico.pdf) (vectorial RGB para Trotec Speedy 360 en formato oficial de $1/8$ de lámina de $250\text{ mm} \times 200\text{ mm}$) y archivo fuente [`base_soporte_optico.svg`](file:///d:/Proyecto%20CC/Prototipos/Corte_laser/base_soporte_optico.svg).
  * **Impresión 3D (Normativa LAIMI Prusa MK4 - Carné 2022437519):**
    * [`2022437519-01.stl`](file:///d:/Proyecto%20CC/Prototipos/Impresion_3D/2022437519-01.stl): Malla sólida del **Kit Unificado de 8 piezas** optimizado para impresión directa en cama sin soportes.
    * [`Atornillada.scad`](file:///d:/Proyecto%20CC/Prototipos/Impresion_3D/Atornillada.scad): Código OpenSCAD paramétrico que genera el kit (torres horizontales coaxiales M3 para TX y RX y 6 clips de retención para la fibra de $45\text{ cm}$).
    * [`visor_interactivo_3d.html`](file:///d:/Proyecto%20CC/Prototipos/Impresion_3D/visor_interactivo_3d.html): Visor web 3D interactivo en Three.js para visualizar las piezas sin software externo.
* **[`Documentacion/`](file:///d:/Proyecto%20CC/Documentacion)**:
  * [`memoria_de_calculo.md`](file:///d:/Proyecto%20CC/Documentacion/memoria_de_calculo.md): Análisis teórico completo con ecuaciones de nodos (LCK/LVK), diseño de la red R--2R ($10\text{ k}\Omega / 20\text{ k}\Omega$), interfaz de aislamiento con OpAmp 1B, justificación físico-cuántica del LED rojo mediante Física 2 y 3, ecuaciones de oscilación astable del 555, acoplo AC al buzzer pasivo y registro de pruebas preliminares.
  * [`informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/Documentacion/informe_ieee_el2113.tex): Informe final del proyecto estructurado en plantilla oficial IEEE Transactions (máximo 3 páginas), con los integrantes del grupo de trabajo, comparación teórico-simulada y plantilla para mediciones experimentales de laboratorio.
