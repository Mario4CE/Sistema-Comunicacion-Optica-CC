# Sistema de Comunicación por Medios Ópticos (EL2113)

Repositorio oficial del proyecto de desarrollo para el curso **Circuitos y Comunicaciones (EL2113)**.

**Autor principal:** Mario Cerdas  
**Objetivo:** Transmitir un patrón digital de 4 bits mediante una señal luminosa guiada por fibra óptica plástica (30–60 cm), recuperarla mediante un receptor óptico y convertirla en una señal acústica audible.

---

## Arquitectura del Sistema

```
  [ MATLAB ]                                                 [ Buzzer Pasivo ]
      │                                                               ▲
      ▼                                                               │ (Tono AC)
 [ Arduino ] ──► [ DAC R–2R ] ──► [ Buffer OpAmp ] ──► [ Driver LED ]  │
   (4 bits)       (Voltaje)          (LM358)            (Luz roja)    │
                                                            │         │
                                                   (Fibra óptica)     │
                                                            │         │
                                                            ▼         │
                                                     [ Receptor LDR ] ┘
                                                            │ (Resistencia)
                                                            ▼
                                                     [ Astable 555 ]
```

---

## Estructura del Repositorio

* **`BITACORA.md`**: Registro cronológico de actividades, decisiones de diseño y cambios implementados.
* **`src/arduino/emisor_4bits/`**: Firmware para Arduino UNO/Nano con generación de datos y control serial.
* **`matlab/simulacion_dac_555.m`**: Script de simulación numérica completa y generación de gráficas de respuesta.
* **`docs/memoria_de_calculo.md`**: Análisis circuital matemático formal (Thévenin, Kirchhoff, cálculos de 555).
* **`docs/lista_de_materiales_BOM.md`**: Lista detallada de compra de componentes físicos y tolerancias.
* **`simulation/`**: Archivos y esquemáticos de simulación circuital (LTspice / TinkerCAD).

---

## Cómo Ejecutar el Código

### 1. Arduino
1. Abrir [emisor_4bits.ino](file:///d:/Proyecto%20CC/src/arduino/emisor_4bits/emisor_4bits.ino) en el Arduino IDE.
2. Seleccionar la placa (Arduino UNO o Nano) y el puerto COM asignado.
3. Presionar **Subir (Upload)**.
4. Abrir el **Monitor Serie** a 9600 baudios para visualizar la salida o enviar comandos ('A' para automático, 'C' para calibración, o números del 0 al 15).

### 2. MATLAB
1. Abrir MATLAB y navegar a la carpeta `matlab/`.
2. Ejecutar el script [simulacion_dac_555.m](file:///d:/Proyecto%20CC/matlab/simulacion_dac_555.m).
3. Se desplegará la tabla de los 16 estados en la consola y las 4 curvas características del sistema.
