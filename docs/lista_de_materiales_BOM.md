# Lista de Materiales (Bill of Materials - BOM) y Equipos de Laboratorio
**Proyecto:** Sistema de Comunicación por Medios Ópticos (EL2113)  
**Institución:** Escuela de Ingeniería Electrónica, Instituto Tecnológico de Costa Rica (ITCR)  
**Autor:** Mario Cerdas y equipo de trabajo  
**Cumplimiento:** Requerimientos A.1–Q.18 (incluyendo P.17, Q.18, M.13, N.14)

---

## 1. Componentes Electrónicos para Montaje en Protoboard

| Ítem | Componente | Modelo / Valor Nominal | Encapsulado / Tipo | Cant. Sugerida | Función en el Circuito | Notas de Adquisición / Tolerancias |
|:---:|:---|:---|:---|:---:|:---|:---|
| **1** | **Microcontrolador** | Arduino UNO R3 (o Arduino Nano) | Placa de desarrollo USB | 1 unid. | **Etapa 1:** Generación del patrón de datos de 4 bits ($D_8\text{--}D_{11}$) | Incluir cable USB tipo A-B o mini-USB. |
| **2** | **Resistencias Red R** | $10\text{ k}\Omega$ (1/4 W, 1%) | Through-Hole (Película metálica) | 15 unid. | **Etapa 2:** Ramas horizontales de la escalera R–2R | **1% de tolerancia** (bandas: Marrón, Negro, Negro, Rojo, Marrón) para asegurar error $< 10\%$. |
| **3** | **Resistencias Red 2R** | $20\text{ k}\Omega$ (1/4 W, 1%) | Through-Hole (Película metálica) | 8 unid. | **Etapa 2:** Ramas verticales $2R$ y terminación a tierra | Si no hay de $20\text{ k}\Omega$, usar dos de $10\text{ k}\Omega$ en serie. |
| **4** | **Amplificador Operacional Dual** | **LM358N** (o LM324N) | DIP-8 (Through-Hole) | 2 unid. (1 repuesto) | **Etapa 2:** OpAmp 1A (DAC Activo)<br>**Etapa 3:** OpAmp 1B (Interfaz Seguidora) | Opera con fuente simple ($0\text{--}5\text{ V}$). Su rango de entrada incluye tierra ($0\text{ V}$). |
| **5** | **Resistencia limitadora del LED** | $150\ \Omega$ (o $180\ \Omega$, 1/4 W) | Through-Hole (Película de carbón) | 2 unid. | **Etapa 4:** Resistor pasivo protector de corriente del LED | Fija $I_{LED(max)} \approx 19.25\text{ mA}$ para $V_{DAC} = 4.69\text{ V}$. |
| **6** | **Diodo Emisor de Luz (LED)** | **LED Rojo difuso o transparente** ($5\text{ mm}$) | Radial $5\text{ mm}$ | 3 unid. | **Etapa 4:** Generador óptico ($\lambda \approx 650\text{ nm}$, $V_D \approx 1.8\text{ V}$) | **Rojo obligatorio por justificación de Física 2/3** (coincide con ventana PMMA y CdS). |
| **7** | **Medio Óptico Guiado** | **Fibra Óptica Plástica (POF)** | Núcleo PMMA ($d = 1.0\text{--}2.2\text{ mm}$) | $1\text{ m}$ (cortar a $45\text{ cm}$) | Enlace físico guiado entre Etapa 4 y Etapa 5 | Longitud obligatoria entre **$30\text{ cm}$ y $60\text{ cm}$** (Req. I.9). Corte limpio a $90^\circ$. |
| **8** | **Receptor Óptico Pasivo** | **Fotorresistencia (LDR)** mod. GL5528 o GL5516 | Radial $5\text{ mm}$ | 2 unid. | **Etapa 5:** Receptor óptico pasivo ($R_{LDR} \approx 1.5\text{ k}\Omega\text{--}40\text{ k}\Omega$) | Componente puramente pasivo (Req. J.10). Sensibilidad en el visible ($540\text{--}650\text{ nm}$). |
| **9** | **Temporizador IC** | **NE555P** (o LM555CN / TLC555) | DIP-8 (Through-Hole) | 2 unid. (1 repuesto) | **Etapa 6:** Generador de onda cuadrada astable | Frecuencia modulada por la LDR en el rango audible ($178\text{ Hz}\text{--}3.6\text{ kHz}$). |
| **10** | **Resistencia fija de carga 555** | $1\text{ k}\Omega$ (1/4 W, 5%) | Through-Hole | 2 unid. | **Etapa 6:** Resistencia $R_1$ entre Pin 8 y Pin 7 del 555 | Limita la corriente del transistor interno de descarga (Pin 7). |
| **11** | **Condensador de tiempo 555** | **$100\text{ nF}$** ($0.1\ \mu\text{F}$, código `104`) | Cerámico multicapa o poliéster | 3 unid. | **Etapa 6:** Condensador $C_1$ de oscilación hacia tierra | Define la constante de tiempo $RC$ del astable. |
| **12** | **Condensador de control 555** | **$10\text{ nF}$** ($0.01\ \mu\text{F}$, código `103`) | Cerámico | 2 unid. | **Etapa 6:** Filtro de ruido en Pin 5 (Control) del 555 | Conectar entre Pin 5 y GND para inmunidad al ruido. |
| **13** | **Condensador de acoplo AC** | **$10\ \mu\text{F}$** ($16\text{ V}$ o $25\text{ V}$) | Electrolítico radial | 2 unid. | **Etapa 7:** Bloqueo de componente DC hacia el buzzer | Filtro pasa-altos; deja pasar solo la componente oscilatoria AC. |
| **14** | **Transductor acústico** | **Buzzer piezoeléctrico PASIVO** | Radial 2 pines ($12\text{ mm}$) | 1 unid. | **Etapa 7:** Conversión de la onda cuadrada a sonido audible | **¡OBLIGATORIO PASIVO!** Un buzzer activo no varía su tono acústico. |
| **15** | **Placa de pruebas (Protoboard)** | Protoboard estándar de 830 puntos | 830 puntos con rieles de alimentación | 1 o 2 unid. | Montaje físico del circuito experimental | Dos rieles continuos superiores e inferiores para $+V_{CC}$ y `GND`. |
| **16** | **Cables jumpers de conexión** | Jumpers Macho-Macho surtidos | Calibre 24-26 AWG | 1 juego (60+ cables) | Cableado de pines y buses del protoboard | Colores sugeridos: Rojo ($+V_{CC}$), Negro (`GND`), Amarillo (4 bits), Azul (analógico). |

---

## 2. Equipos e Instrumentos a Solicitar en la Bodega de la Escuela de Electrónica (Req. P.17 y Q.18)

| Ítem | Instrumento / Equipo | Modelo / Especificación | Propósito en el Proyecto | Restricción Oficial Asociada |
|:---:|:---|:---|:---|:---|
| **B1** | **Fuente de Alimentación DC Regulada de Laboratorio** | Doble o triple salida con display digital ($0\text{--}30\text{ V}$, $0\text{--}3\text{ A}$) | Alimentar las etapas analógicas de transmisión y recepción ($+5.0\text{ V}$ y `GND`) | **Obligatorio (Req. P.17 y Q.18).** Prohibido usar el pin 5V de Arduino y prohibido usar baterías. |
| **B2** | **Cables de alimentación de laboratorio** | Par de cables Banana a Caimán / Lagarto (Rojo y Negro) | Conectar los bornes de la fuente de laboratorio a los rieles del protoboard | Inspeccionar que los caimanes tengan buen agarre y aislamiento intacto. |
| **B3** | **Multímetro Digital de Banco o Portátil** | True RMS con puntas de aguja | Medir tensiones continuas ($V_{DAC}$, $V_{interfaz}$), corriente del LED y resistencia de la LDR | Validación de error $< 10\%$ (Req. O.16) en los puntos de prueba TP1 a TP5. |
| **B4** | **Osciloscopio Digital con Puntas $10\times$** | $50\text{ MHz}\text{--}100\text{ MHz}$ (2 canales) | Visualizar la onda cuadrada en el Pin 3 del 555 y la señal acústica en el buzzer | Medir frecuencia real en Hz y ciclo de trabajo en TP6. |

---

## 3. Piezas de Prototipado y Montaje Mecánico (Escuela de Electrónica — Req. M.13 y N.14)

| Ítem | Pieza / Proceso | Archivo de Diseño en el Repositorio | Material / Parámetros de Fabricación | Función Mecánica / Óptica |
|:---:|:---|:---|:---|:---|
| **P1** | **Base Organizadora con Corte Láser** | [`prototipo_laboratorio/corte_laser/base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg) | MDF o Acrílico de $3.0\text{ mm}$ ($380\text{ mm} \times 160\text{ mm}$) | Fija el protoboard, el Arduino y el canal de fibra óptica ($45\text{ cm}$). **Graba las 7 etapas delimitadas (Req. B.2 y M.13).** |
| **P2** | **Cabezales Acopladores Ópticos (Impresión 3D)** | [`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad) | PLA o PETG Negro opaco (relleno $\ge 40\%$, 2 unidades) | Alineación concéntrica LED-Fibra y Fibra-LDR con **blindaje total contra luz ambiental**. |
| **P3** | **Tornillería de fijación mecánica** | Tornillos M3 ($3\text{ mm} \times 10\text{ mm}$) con tuercas | Acero o nylon | 6 juegos | Atornillar los acopladores 3D y la placa Arduino a la base láser. |
| **P4** | **Cámara Acústica con Escáner 3D** | Propuesta técnica en [`docs/diseno_prototipado_3d_laser.md`](file:///d:/Proyecto%20CC/docs/diseno_prototipado_3d_laser.md) | Escaneo 3D en el laboratorio de prototipado | Resonador acústico para el buzzer piezoeléctrico pasivo (**Puntos extra - Req. N.14**). |

---

## 4. Diagrama Rápido de Pines de los Circuitos Integrados (DIP-8)

Para facilitar el conexionado en la mesa de trabajo sin cometer errores:

### A. Amplificador Operacional Dual LM358N (Vista Superior)
```
          ┌───┬───┐
  OUT 1  ─┤ 1 └── 8 ├─  VCC (+5V a +9V Fuente Laboratorio)
   IN 1- ─┤ 2     7 ├─  OUT 2 (Salida Etapa 3 Interfaz)
   IN 1+ ─┤ 3     6 ├─  IN 2- (Puente con Pin 7)
    GND  ─┤ 4     5 ├─  IN 2+ (Entrada desde Pin 1)
          └───────┘
  [ OpAmp 1A: Etapa 2 DAC ]       [ OpAmp 1B: Etapa 3 Interfaz ]
  Pin 3: Entrada desde R-2R        Pin 5: Entrada desde Pin 1
  Pin 2: Puente a Pin 1            Pin 6: Puente a Pin 7
  Pin 1: Salida analógica DAC      Pin 7: Salida hacia resistor 150 Ohm
```

### B. Temporizador NE555 (Vista Superior)
```
          ┌───┬───┐
    GND  ─┤ 1 └── 8 ├─  VCC (+5V Fuente Laboratorio)
TRIGGER  ─┤ 2     7 ├─  DISCHARGE (Entre R1=1k y LDR)
 OUTPUT  ─┤ 3     6 ├─  THRESHOLD (Puente con Pin 2 y a LDR)
  RESET  ─┤ 4     5 ├─  CONTROL (Condensador 10nF a GND)
          └───────┘
  Pin 1: A GND
  Pin 2 & Pin 6: Puenteados juntos, conectados a la LDR y a C1 (100nF a GND)
  Pin 3: Salida de onda cuadrada hacia condensador de 10 uF y Buzzer
  Pin 4 & Pin 8: Conectados juntos a +5V de la Fuente de Laboratorio
  Pin 5: Condensador de 10 nF hacia GND
  Pin 7: Conectado a R1 (1k a +5V) y al otro extremo de la LDR
```

---

## 5. Verificación Fundamental antes de Armar: Buzzer Pasivo vs. Activo

> [!CAUTION]
> **¿Cómo comprobar si tu buzzer es pasivo o activo con un multímetro o fuente?**
> 1. Conecta el buzzer directamente a una salida de corriente continua de $5\text{ V}$ (o prueba de diodos del multímetro):
>    * Si emite un **pitido continuo inmediato y monótono**: **Es ACTIVO y NO SIRVE** para este proyecto.
>    * Si solo hace un pequeño **"clic" o "chasquido" seco al conectar y desconectar**: **Es PASIVO y ES EL CORRECTO**.
> 2. El temporizador 555 genera frecuencias variables ($178\text{ Hz}\text{--}3.6\text{ kHz}$). Solo un buzzer pasivo responderá cambiando la nota acústica según la intensidad luminosa.
