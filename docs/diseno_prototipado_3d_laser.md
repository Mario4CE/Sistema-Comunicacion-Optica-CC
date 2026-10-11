# Guía de Uso de Equipos de Laboratorio y Prototipado (Escuela de Electrónica)
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos (EL2113)  
**Autor:** Mario Cerdas y equipo de trabajo  
**Cumplimiento:** Requerimientos B.2, M.13, N.14, Ñ.15, P.17 y Q.18 del instructivo y presentación oficial  

---

## 1. Uso de las Fuentes de Alimentación de la Bodega (Req. P.17 y Q.18)

El instructivo oficial establece dos restricciones estrictas sobre la energía eléctrica del prototipo:
* **Requerimiento P.17:** *No es permitido utilizar el Arduino como fuente de alimentación para las etapas globales de transmisión y recepción.*
* **Requerimiento Q.18:** *No se permite el uso de baterías AA, AAA o cuadrada como fuente de alimentación. En lugar de ello, el grupo de trabajo deberá utilizar las **fuentes de alimentación de laboratorio provistas por la bodega de la Escuela de Electrónica**.*

### Protocolo en el Laboratorio:
1. Solicitar en la bodega de la Escuela de Electrónica una **Fuente de Alimentación DC Regulada de Laboratorio** (con cables banana-lagarto).
2. Antes de conectar al protoboard, encender la fuente en vacío, fijar la tensión en **$+5.00\text{ V}$** (o usar dos canales: Canal 1 en $+6.5\text{ V}\text{--}9.0\text{ V}$ para el pin 8 del LM358 para evitar saturación del riel superior, y Canal 2 en $+5.0\text{ V}$ para el temporizador NE555) y limitar la corriente de protección a **$0.25\text{ A}$** ($250\text{ mA}$).
3. Apagar la salida (`OUTPUT OFF`), conectar el terminal positivo (`+`) al riel de alimentación del protoboard y el terminal negativo (`- / GND`) al riel de tierra del protoboard.
4. Conectar un cable jumper desde el pin **`GND` del Arduino UNO/Nano** hacia el riel de tierra del protoboard (**dejando el pin `5V` del Arduino totalmente desconectado**).

---

## 2. Pieza 1: Base Organizadora con Corte Láser (Req. M.13 y B.2)

Tanto el manual (**Req. M.13**) como la presentación oficial (**Diapositiva 13, Comentario 5**) requieren integrar piezas elaboradas con los equipos de **corte láser** e **impresión 3D** de la Escuela de Electrónica.

* **Archivo de fabricación:** [`prototipo_laboratorio/corte_laser/base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg)
* **Material recomendado:** Lámina de MDF o acrílico de **$3.0\text{ mm}$ de espesor** ($380\text{ mm} \times 160\text{ mm}$).
* **Funciones de ingeniería que cumple la pieza:**
  1. **Delimitación física e identificación visual de las 7 etapas (Req. B.2):** Mediante grabado láser vectorial (capa azul `#0000FF`), la placa base presenta recuadros punteados y rótulos grabados para cada una de las 7 etapas (*Etapa 1: Patrón 4 Bits*, *Etapa 2: DAC R–2R + OpAmp 1A*, *Etapa 3: Interfaz OpAmp 1B*, *Etapa 4: Generador Óptico*, *Etapa 5: Receptor LDR*, *Etapa 6: Oscilador 555*, *Etapa 7: Buzzer Pasivo*). Durante la defensa presencial en semana 18/19, el profesor puede identificar cada etapa de inmediato.
  2. **Fijación geométrica de la longitud de la fibra óptica (Req. I.9):** Incorpora ranuras para sujetar el arco de fibra óptica plástica (POF) de **$45\text{ cm}$** (dentro del rango exigido de $30\text{ cm}$ a $60\text{ cm}$) sin radios de curvatura bruscos que causen pérdidas por macrocurvatura.
  3. **Anclaje mecánico:** Incluye perforaciones M3 ($3.2\text{ mm}$) para atornillar el Arduino UNO y los dos cabezales acopladores impresos en 3D.

---

## 3. Pieza 2: Cabezales de Acoplamiento Óptico en Impresión 3D (Req. M.13)

* **Archivo de fabricación:** [`prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad)
* **Parámetros de impresión 3D para el asistente del laboratorio de la Escuela de Electrónica:**
  * **Material:** Filamento **PLA Negro** o **PETG Negro** (estrictamente opaco para bloquear la luz ambiente).
  * **Altura de capa:** $0.20\text{ mm}$.
  * **Relleno (Infill):** $40\%$ a $100\%$ (evita translucidez de las paredes).
  * **Soportes:** No requiere soportes (diseñado para imprimirse verticalmente sobre su pestaña plana).
* **Funciones de ingeniería que cumple la pieza:**
  1. **Alineación coaxial precisa ($\text{Apertura Numérica } NA \approx 0.51$):** Garantiza que el eje óptico del domo del LED rojo de $5\text{ mm}$ (Etapa 4) y el núcleo de la fibra óptica plástica ($1.0\text{--}2.2\text{ mm}$) queden enfrentados a $0^\circ$, maximizando la potencia acoplada según la Ley de Snell.
  2. **Blindaje hermético contra luz ambiental en la LDR (Etapa 5):** Elimina la interferencia de las lámparas fluorescentes/LED del techo del laboratorio sobre la fotorresistencia pasiva.

---

## 4. Propuesta de Uso del Escáner 3D para Puntos Extra (Req. N.14)

El **Requerimiento N.14** indica:
> *"También existe la posibilidad de utilizar un escáner 3D en la Escuela de Electrónica. Si así lo desean, pueden plantear sus propuestas de cómo se podría utilizar para el prototipado de la propuesta. Si están interesados, por favor contactar al profesor a cargo. **Se otorgarán puntos extra a los grupos de trabajo que logren emplear este equipo.**"*

### Propuesta Técnica Lista para Presentar al Profesor (`fapicado@itcr.ac.cr`):
1. **Objeto a escanear:** Escanear en 3D el conjunto físico del **transductor piezoeléctrico pasivo (Etapa 7) y el conector/portafibra junto con el extremo del protoboard**.
2. **Aplicación en el prototipo:**
   * A partir de la nube de puntos / malla `.stl` obtenida con el **Escáner 3D de la Escuela de Electrónica**, realizar una operación booleana inversa (molde negativo exacto) para imprimir una **cámara acústica resonadora (caja de resonancia Helmholtz) y cubierta ergonómica a medida** que encaje con tolerancia submilimétrica sobre el buzzer pasivo y el protoboard.
   * **Beneficio técnico demostrable:** Incrementa la presión sonora (dB SPL) del buzzer pasivo en el rango de frecuencias bajas ($180\text{ Hz}\text{--}800\text{ Hz}$) y asegura la sujeción mecánica sin adhesivos.
