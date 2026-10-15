# Sistema de Comunicación por Medios Ópticos (EL2113)

Repositorio del proyecto de Investigación Dirigida para el curso **Circuitos Eléctricos en Corriente Continua (EL2113)**, Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR).

**Responsable:** Mario Cerdas  
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
                                              Soportes 3D y Láser)             │
                                                       ║                       │
                                                       ▼                       │
                                            [ Etapa 5: Receptor LDR ] ─────────┤ (Resistencia)
                                                                               ▼
                                                                   [ Etapa 6: Astable 555 ]
```

---

## Estructura del Repositorio

* **[`BITACORA.md`](file:///d:/Proyecto%20CC/BITACORA.md)**: Registro de actividades y avances del proyecto.
* **[`src/arduino/generador_patrones_tinkercad.ino`](file:///d:/Proyecto%20CC/src/arduino/generador_patrones_tinkercad.ino)**: Código de Arduino UNO con los arreglos de 4 bits (escalera de 16 niveles, señal periódica de 20 muestras y señal aperiódica de 18 muestras) y tiempo de permanencia configurable entre $1\text{ ms}$ y $10\text{ s}$. Se usa tanto en el montaje físico como en TinkerCAD.
* **[`matlab/generador_patrones.m`](file:///d:/Proyecto%20CC/matlab/generador_patrones.m)** y **[`matlab/simulacion_dac_555.m`](file:///d:/Proyecto%20CC/matlab/simulacion_dac_555.m)**: Scripts en MATLAB para cuantizar las señales a 4 bits y graficar la respuesta teórica de las etapas.
* **[`Simulacion/tinkercad/`](file:///d:/Proyecto%20CC/Simulacion/tinkercad)**: Guía de montaje en TinkerCAD ([`Montaje.md`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/Montaje.md)) e imágenes del circuito simulado ([`CircuitoTinkercad.png`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/CircuitoTinkercad.png), [`Componentes.png`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/Componentes.png), [`Esquema.png`](file:///d:/Proyecto%20CC/Simulacion/tinkercad/Esquema.png)).
* **[`Simulacion/ltspice/`](file:///d:/Proyecto%20CC/Simulacion/ltspice)**: Simulaciones en LTspice de la etapa emisora ([`dac_r2r_buffer.asc`](file:///d:/Proyecto%20CC/Simulacion/ltspice/dac_r2r_buffer.asc)), etapa receptora ([`receptor_555_ldr.asc`](file:///d:/Proyecto%20CC/Simulacion/ltspice/receptor_555_ldr.asc)) y sistema completo de las 7 etapas ([`sistema_completo_optico.asc`](file:///d:/Proyecto%20CC/Simulacion/ltspice/sistema_completo_optico.asc) y su netlist equivalente [`.cir`](file:///d:/Proyecto%20CC/Simulacion/ltspice/sistema_completo_optico.cir)). Incluye el modelo [`LM358_simple.lib`](file:///d:/Proyecto%20CC/Simulacion/ltspice/LM358_simple.lib), mediciones automáticas (`mediciones_*.inc`) y la guía [`VERIFICACION.md`](file:///d:/Proyecto%20CC/Simulacion/ltspice/VERIFICACION.md) con los resultados esperados.
* **[`prototipo_laboratorio/`](file:///d:/Proyecto%20CC/prototipo_laboratorio)**: Archivos para corte láser ([`base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg)) e impresión 3D ([`acoplador_optico_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad) y [`.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl)).
* **[`docs/`](file:///d:/Proyecto%20CC/docs)**:
  * [`memoria_de_calculo.md`](file:///d:/Proyecto%20CC/docs/memoria_de_calculo.md): Desarrollo de ecuaciones de nodos (Kirchhoff), diseño del DAC R-2R, etapa de interfaz y cálculos del 555.
  * [`lista_de_materiales_BOM.md`](file:///d:/Proyecto%20CC/docs/lista_de_materiales_BOM.md): Lista de componentes electrónicos y equipo de laboratorio.
  * [`guia_de_conexion_protoboard.md`](file:///d:/Proyecto%20CC/docs/guia_de_conexion_protoboard.md): Guía de armado físico en protoboard.
  * [`diseno_prototipado_3d_laser.md`](file:///d:/Proyecto%20CC/docs/diseno_prototipado_3d_laser.md): Detalles de las piezas en corte láser e impresión 3D.
  * [`informe_ieee/informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/docs/informe_ieee/informe_ieee_el2113.tex): Informe en plantilla IEEE.
