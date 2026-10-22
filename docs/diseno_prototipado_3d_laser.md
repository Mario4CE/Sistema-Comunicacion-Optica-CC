# Guía Oficial de Prototipado en LAIMI (Corte Láser e Impresión 3D) y Tornillería
**Proyecto:** Investigación Dirigida — Sistema de Comunicación por Medios Ópticos (EL2113)  
**Estudiante Solicitante:** Mario Cerdas — **Carné:** `2022437519`  
**Ubicación del Servicio en el TEC:** LAIMI 2 (Edificio F4, Aula F4-05, Sede Central Cartago)  
**Cumplimiento:** Requerimientos B.2, I.9, M.13, N.14, Ñ.15, P.17 y Q.18 + Normativa Oficial LAIMI  

---

## 1. ¿Qué Tornillos Comprar para Fijar el Arduino y las Impresiones 3D?

Tanto los 4 agujeros de montaje del **Arduino UNO** como los 4 agujeros de las **torres acopladoras 3D** están diseñados con un diámetro pasante de **$3.2\text{ mm}$**, mientras que la lámina de corte láser tiene **$3.0\text{ mm}$ de espesor** y la base de las piezas 3D mide **$3.0\text{ mm}$ de espesor**.

### Lista de Compra en Ferretería o Tienda de Electrónica:
* **Medida exacta a pedir:** **Tornillos métricos `M3 x 10 mm` o `M3 x 12 mm`** (de $3\text{ mm}$ de grosor por $10\text{ mm}$ o $12\text{ mm}$ de largo, cabeza Phillips o Allen cilíndrica) con su respectiva **tuerca hexagonal `M3`**.
  * *(Alternativa en pulgadas si vas a una ferretería tradicional):* Tornillos de **$1/8\text{''} \times 1/2\text{''}$** con tuerca (también entran en los orificios de $3.2\text{ mm}$, aunque el **M3** calza más holgado en los bordes del Arduino).
* **Cantidad necesaria:**
  * **4 tornillos M3 + 4 tuercas M3** para fijar el Arduino UNO a la base.
  * **4 tornillos M3 + 4 tuercas M3** para fijar las 2 torres 3D (`2022437519-01.stl`) a la base (2 tornillos por cada torre).
  * **Total a comprar:** **8 tornillos M3 x 10mm (o 12mm) y 8 tuercas M3** *(se recomienda comprar 10 unidades de cada uno por respaldo)*.
* **¿Cómo se fija la Protoboard Grande (filas `0` a `60`, columnas `a`–`j`)?**
  * La protoboard NO lleva tornillos; trae una lámina autoadhesiva amarilla en su parte trasera, o bien puedes pegarla dentro del recuadro grabado central usando dos tiras de **cinta doble faz**.

---

## 2. Pieza 1: Base Organizadora en Corte y Grabado Láser (Normativa LAIMI)

El archivo ha sido preparado cumpliendo al 100% el reglamento oficial de **Corte y Grabado Láser de LAIMI** para las cortadoras **Trotec Speedy 360** ($\text{CO}_2$ sellado de $60\text{ W}$):

* **Archivo obligatorio para entregar en LAIMI (Formato `.PDF`):**  
  [`prototipo_laboratorio/corte_laser/2022437519_base_soporte_optico.pdf`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/2022437519_base_soporte_optico.pdf)  
  *(También disponible la fuente vectorial en [`base_soporte_optico.svg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/base_soporte_optico.svg)).*
* **Optimización de Material (Fracción Oficial LAIMI):**
  * La lámina completa de LAIMI mide $800\text{ mm} \times 500\text{ mm}$.
  * Nuestro diseño mide exactamente **$250\text{ mm} \times 200\text{ mm}$**, lo cual calza de forma exacta en **$\mathbf{1/8}\text{ de lámina}$ ($200\text{ mm} \times 250\text{ mm}$)**, cumpliendo la regla de reducción de residuos (Sección 4 del manual LAIMI) y reduciendo el costo a la mitad respecto a $1/4$ de lámina.
* **Cumplimiento de Paleta de Colores RGB y Grosores LAIMI (Secciones 5, 6 y 7):**
  1. **Líneas de CORTE:** Color **ROJO RGB (`255, 0, 0` / `#FF0000`)** con grosor exacto de **$0.025\text{ mm}$** (contorno exterior de $244\text{ mm} \times 194\text{ mm}$, 8 perforaciones M3 de $\varnothing 3.2\text{ mm}$ y 5 ranuras pasantes para amarrar el arco de fibra óptica de $45\text{ cm}$).
  2. **Líneas de MARCADO Vectorial:** Color **AZUL RGB (`0, 0, 255` / `#0000FF`)** con grosor exacto de **$0.025\text{ mm}$** (recuadros de ubicación de la protoboard de filas `0` a `60`, subdivisiones de las 7 etapas según **Req. B.2**, silueta del Arduino y trayectoria guía de la fibra óptica).
  3. **Textos de GRABADO:** Color **NEGRO RGB (`0, 0, 0` / `#000000`)** en relleno sólido (rótulos de cada etapa, numeración de filas `0` a `60` de la protoboard, carné `2022437519` y reglas de laboratorio P.17/Q.18).

---

## 3. Pieza 2: Acopladores Ópticos en Impresión 3D (Normativa LAIMI Prusa MK4)

Cumpliendo la **Sección 1 de Condiciones del Servicio de Impresión 3D de LAIMI**, los archivos `.STL` llevan el formato oficial **`CCCCCCCCCC-NN.stl`** con tu número de carné (`2022437519`):

1. **Opción A (Recomendada — Torres con pestaña para tornillos M3 y conexión con 4 jumpers):**
   * **Archivo oficial a entregar:** [`prototipo_laboratorio/impresion_3d/2022437519-01.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-01.stl)
   * **Código fuente paramétrico:** [`acoplador_optico_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad)
   * Contiene las 2 torres (Emisor LED y Receptor LDR) listas en una sola cama de impresión.

2. **Opción B (Alternativa — Capuchones compactos para pinchar el LED y la LDR directo en la protoboard):**
   * **Archivo oficial a entregar:** [`prototipo_laboratorio/impresion_3d/2022437519-02.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-02.stl)
   * **Código fuente paramétrico:** [`capuchon_protoboard_led_ldr.scad`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/capuchon_protoboard_led_ldr.scad)
   * Contiene los 2 capuchones cilíndricos sin pestañas laterales para encajar sobre el LED y la LDR clavados en la protoboard.

---

## 4. Guion Exacto: ¿Qué Decir y Qué Datos Dar al Ir a LAIMI (Aula F4-05)?

Lleva en una llave maya (USB) los archivos [`2022437519_base_soporte_optico.pdf`](file:///d:/Proyecto%20CC/prototipo_laboratorio/corte_laser/2022437519_base_soporte_optico.pdf) y [`2022437519-01.stl`](file:///d:/Proyecto%20CC/prototipo_laboratorio/impresion_3d/2022437519-01.stl) (o `2022437519-02.stl`), preséntate en el **Edificio F4, Aula F4-05 (LAIMI 2)** con tu carné estudiantil físico y toma ficha de atención.

### A. Lo que debes decir para el CORTE Y GRABADO LÁSER:
> *"Hola, vengo a solicitar un trabajo de corte, marcado y grabado láser para un proyecto de curso. Ya traigo el archivo en formato `.PDF` en modo RGB optimizado a $250\text{ mm} \times 200\text{ mm}$ con líneas de corte rojas (`#FF0000`) y de marcado azules (`#0000FF`) en $0.025\text{ mm}$, y textos de grabado en negro (`#000000`)."*

* **Material a solicitar al operador (proporcionado por LAIMI):**
  * **Tamaño:** **$1/8\text{ de lámina}$** ($200\text{ mm} \times 250\text{ mm}$).
  * **Tipo de material:** **MDF de $3\text{ mm}$** (el más económico y de excelente contraste en grabado) o **Acrílico de $3\text{ mm}$** (según disponibilidad en LAIMI).
  * **Costo oficial LAIMI:** $\mathbb{C}100$ por minuto de máquina + costo de la fracción de $1/8$ de lámina (se carga al Sistema de Solicitudes de Impresión; no se paga en efectivo).

### B. Lo que debes decir para la IMPRESIÓN 3D:
> *"También vengo a dejar en cola este archivo `.STL` nombrado con mi carné (`2022437519-01.stl`) para impresión 3D de uso académico de curso."*

* **Datos que te pedirá el asistente para llenar la boleta (Sección 1 y 6 del manual):**
  * **Carné:** `2022437519`
  * **Correo institucional:** Tu correo `@estudiantec.cr`
  * **Uso:** **`Curso`** *(Importante decir que es de **Curso** — EL2113 Circuitos Eléctricos en Corriente Continua — porque el reglamento de LAIMI en la página 5 le da **prioridad en la fila** a los trabajos académicos de curso sobre los personales).*
  * **Material:** **`PLA`** (o `PETG` si no hubiera PLA).
  * **Resolución (eje Z):** **`0.2 mm`** (resolución estándar recomendada por LAIMI para las Prusa MK4).
  * **Relleno (Infill):** **`20%` a `30%`**.
  * **Indicación de Color (Página 7 del manual):** Dile al asistente:  
    *"Preferible imprimir en color **negro** (o gris/azul oscuro) porque es un aislante óptico para una fotorresistencia, pero si se agotó el negro, usar cualquier otro color disponible."* *(Recuerda que si lo imprimen en un color claro, puedes pintarlo por fuera con pintura acrílica negra escolar o marcador permanente negro).*
* **¡Paso Crítico Después de Ir a LAIMI!**
  * Revisa tu correo `@estudiantec.cr` en las horas siguientes: LAIMI te enviará un correo con el tiempo estimado y el costo (a **$\mathbb{C}50$ por gramo**). **Debes responder ese correo diciendo que AUTORIZAS la impresión**, de lo contrario no la pondrán en las impresoras Prusa MK4.
  * Para retirar las piezas cuando te llegue el correo de finalización, lleva tu **carné estudiantil** para firmar la boleta.

---

## 5. Referencias Visuales del Ensamblaje Físico

* **Vista de la maqueta completa armada:** [`prototipo_laboratorio/render_maqueta_3d_laser.jpg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/render_maqueta_3d_laser.jpg)
* **Comparativa visual Opción A (Torres M3 + Jumpers) vs Opción B (Capuchón en Protoboard):** [`prototipo_laboratorio/comparativa_montaje_opcion_a_vs_b.jpg`](file:///d:/Proyecto%20CC/prototipo_laboratorio/comparativa_montaje_opcion_a_vs_b.jpg)

---

## 6. Propuesta de Uso del Escáner 3D para Puntos Extra (Req. N.14)

Si desean optar por los puntos extra del **Requerimiento N.14** utilizando el escáner 3D de la Escuela de Electrónica:
1. **Objeto a escanear:** Escanear en 3D el conjunto físico del **transductor piezoeléctrico pasivo (Etapa 7) junto con el extremo de la protoboard (filas 50 a 60)**.
2. **Aplicación en el prototipo:** A partir de la malla `.stl` escaneada, generar una **campana acústica resonadora (caja Helmholtz) a medida** que encaje sobre las filas 50–60 de la protoboard para amplificar acústicamente el tono del buzzer pasivo en el rango de bajas frecuencias ($178\text{ Hz}\text{--}800\text{ Hz}$).
