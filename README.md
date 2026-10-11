# Sistema de Comunicación por Medios Ópticos (EL2113)

Repositorio oficial del proyecto de Investigación Dirigida para el curso **Circuitos Eléctricos en Corriente Continua (EL2113)**, Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR).

**Autor principal:** Mario Cerdas  
**Objetivo:** Transmitir arreglos de longitud variable de datos digitales de 4 bits (de señales periódicas y aperiódicas generadas en MATLAB) mediante una señal luminosa guiada por fibra óptica plástica ($30\text{--}60\text{ cm}$), recuperarla mediante un receptor óptico pasivo y convertirla en una señal acústica audible mediante un buzzer piezoeléctrico pasivo.

---

## Arquitectura de las 7 Etapas Delimitadas (Req. A.1 y B.2)

```
 [ Fuente DC de Laboratorio — Bodega Escuela de Electrónica (Req. P.17 y Q.18) ]
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

## Estructura del Repositorio y Cumplimiento del Instructivo

* **[`BITACORA.md`](file:///d:/Proyecto%20CC/BITACORA.md)**: Registro cronológico de actividades, decisiones de diseño y cambios implementados.
* **[`src/arduino/emisor_4bits/emisor_4bits.ino`](file:///d:/Proyecto%20CC/src/arduino/emisor_4bits/emisor_4bits.ino)**: Firmware de Arduino UNO/Nano con arreglos de longitud variable (periódico $N=20$, aperiódico $N=18$ y escalera $N=16$) y tiempo configurable entre $1\text{ ms}$ y $10\text{ s}$ (**Req. C.3, D.4 y Punto 8**).
* **[`matlab/generador_patrones.m`](file:///d:/Proyecto%20CC/matlab/generador_patrones.m)** y **[`matlab/simulacion_dac_555.m`](file:///d:/Proyecto%20CC/matlab/simulacion_dac_555.m)**: Generación de señales periódicas y aperiódicas cuantizadas a 4 bits y simulación numérica integral.
* **[`docs/memoria_de_calculo.md`](file:///d:/Proyecto%20CC/docs/memoria_de_calculo.md)**: Análisis nodal de Kirchhoff (LCK/LVK), separación de OpAmp 1A (Etapa 2, **Req. E.5**) y OpAmp 1B (Etapa 3, **Req. F.6**), y justificación física (Física 2 y 3) del LED rojo de $650\text{ nm}$ (**Req. H.8**).
* **[`docs/diseno_prototipado_3d_laser.md`](file:///d:/Proyecto%20CC/docs/diseno_prototipado_3d_laser.md)** y **[`prototipo_laboratorio/`](file:///d:/Proyecto%20CC/prototipo_laboratorio)**: Diseños listos para los equipos de **Corte Láser** ([`base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg)), **Impresión 3D** ([`acoplador_optico_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad)) y propuesta de **Escáner 3D** de la Escuela de Electrónica (**Req. M.13 y N.14**).
* **[`docs/informe_ieee/informe_ieee_el2113.tex`](file:///d:/Proyecto%20CC/docs/informe_ieee/informe_ieee_el2113.tex)**: Informe técnico en formato **IEEE Transactions** ($\le 3$ páginas, **Sección 4.2 y Rúbrica**).
* **[`simulation/ltspice/`](file:///d:/Proyecto%20CC/simulation/ltspice)**: Simulaciones SPICE por separado ([`dac_r2r_buffer.asc`](file:///d:/Proyecto%20CC/simulation/ltspice/dac_r2r_buffer.asc), [`receptor_555_ldr.asc`](file:///d:/Proyecto%20CC/simulation/ltspice/receptor_555_ldr.asc)) y en conjunto ([`sistema_completo_optico.asc`](file:///d:/Proyecto%20CC/simulation/ltspice/sistema_completo_optico.asc)).
* **[`src/empaquetar_entrega_oficial.py`](file:///d:/Proyecto%20CC/src/empaquetar_entrega_oficial.py)**: Genera automáticamente la carpeta `Investigacion_Dirigida/` y el archivo `Investigacion_Dirigida.zip` con la estructura oficial de la **Sección 4.1**.
