# Verificación de las simulaciones en LTspice

Los tres esquemáticos traen **mediciones automáticas (`.meas`)**: después de correr la simulación, los resultados aparecen en **View → SPICE Error Log** (`Ctrl + L`). Así no hace falta medir a mano con cursores para saber si el circuito funciona.

| Archivo | Etapas | Mediciones (`.include`) |
|:---|:---|:---|
| [`dac_r2r_buffer.asc`](dac_r2r_buffer.asc) | 1 a 4 (bits, DAC R-2R + LM358 1A, interfaz LM358 1B, LED) | [`mediciones_dac.inc`](mediciones_dac.inc) |
| [`receptor_555_ldr.asc`](receptor_555_ldr.asc) | 5 a 7 (LDR, NE555 astable, buzzer) | [`mediciones_555.inc`](mediciones_555.inc) |
| [`sistema_completo_optico.asc`](sistema_completo_optico.asc) / [`.cir`](sistema_completo_optico.cir) | Las 7 etapas juntas | [`mediciones_sistema.inc`](mediciones_sistema.inc) |
| [`LM358_simple.lib`](LM358_simple.lib) | Modelo del LM358 (LTspice no lo incluye) | — |

> [!IMPORTANT]
> Todos los archivos `.asc`, `.cir`, `.inc` y `.lib` tienen que quedar **en la misma carpeta**, porque los esquemáticos los cargan con rutas relativas.

---

## 1. Revisión de conexiones (antes de ver resultados)

1. **Pines sin conectar:** en el esquemático no debe haber cuadritos vacíos en las puntas de los componentes.
2. **View → SPICE Netlist:** no debe aparecer ningún nodo `NC_01`, `NC_02`, … (son pines al aire), ni componentes con el mismo nodo en ambos terminales (por ejemplo, `R1 B0 B0 10k`).
3. **SPICE Error Log:** no deben aparecer `Singular matrix`, `Node ... is floating` ni `Timestep too small`.

---

## 2. Qué se debe obtener en cada simulación

### 2.1 `dac_r2r_buffer.asc` (T = 1 ms por código, LM358 con VOP = 9 V)

* `V00` a `V15` deben valer `k × 0.3125 V`; `E01` a `E15` dan el error en %, que debe ser **< 10 %** (Req. O.16).
* `V00` da ≈ 5 mV, no 0 V: es el límite inferior de salida del LM358 y está bien.
* Señales para graficar: `V(V_R2R)`, `V(V_DAC)` y `V(V_INTERFAZ)` (las tres forman la misma escalera de 16 escalones), además de `I(D_LED)`.

| Medición | Teórico | Simulado |
|:---|:---:|:---:|
| `V01` | 0.3125 V | 0.3125 V |
| `V08` | 2.5000 V | 2.5000 V |
| `V15` | 4.6875 V | 4.6875 V |
| Error máximo (`E01`..`E15`) | < 10 % | 0.0012 % |
| `ILED05` (código 5) | ≈ 0 (LED apagado) | 0.03 mA |
| `ILED06` (código 6) | empieza a conducir | 1.04 mA |
| `ILED15` (código 15) | ≈ 19 mA | 18.66 mA |

> [!TIP]
> Si cambias `.param VOP=9` por `VOP=5`, la escalera se recorta en ≈ 3.5 V a partir del código 12. Esto confirma por qué el LM358 se alimenta con más de 5 V (ver la memoria de cálculo).

### 2.2 `receptor_555_ldr.asc` (`.step` de R_LDR: 1.5k, 5k, 10k, 20k, 40k)

* `FREQ` es la frecuencia simulada, `F_TEO = 1.443 / ((R1 + 2·R_LDR)·C1)` la teórica y `ERR_F` el error en %.
* `DUTY` es el ciclo de trabajo porcentual ($T_{\text{alto}} / T \times 100$).
* `VTHR_MAX` ≈ 3.33 V y `VTHR_MIN` ≈ 1.67 V (umbrales 2/3 y 1/3 de Vcc del 555).
* Señales para graficar: `V(OUT_555)`, `V(THR)` y `V(V_BUZZER)`. Con clic derecho → *Select Steps* se ve un valor de R_LDR a la vez.

| R_LDR | F_TEO | FREQ simulada | Error | Ciclo de Trabajo (Duty) | VTHR_MAX | VTHR_MIN |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 1.5 kΩ | 3607.5 Hz | 3574.6 Hz | 0.91 % | 62.08 % | 3.335 V | 1.665 V |
| 5 kΩ | 1311.8 Hz | 1300.8 Hz | 0.84 % | 54.19 % | 3.335 V | 1.665 V |
| 10 kΩ | 687.1 Hz | 681.5 Hz | 0.82 % | 52.04 % | 3.335 V | 1.666 V |
| 20 kΩ | 352.0 Hz | 349.1 Hz | 0.81 % | 50.89 % | 3.334 V | 1.666 V |
| 40 kΩ | 178.1 Hz | 176.7 Hz | 0.81 % | 50.29 % | 3.334 V | 1.666 V |

### 2.3 `sistema_completo_optico.asc` (las 7 etapas, T = 20 ms por código)

La LDR se modela con `B_LDR`, una resistencia controlada por la corriente del LED: `V(RLDR)` tiene su valor en ohm (40 kΩ a oscuras y ≈ 1.6 kΩ con el LED a 19 mA).

* Para cada código `kk`: `VINTkk`, `ILEDkk`, `RLDRkk`, `Fkk` (frecuencia simulada), `FTEOkk` y `ERRkk`.
* Señales para graficar: `V(V_INTERFAZ)`, `I(D_LED)`, `V(RLDR)` y `V(OUT_555)`. La onda cuadrada debe ir más rápido conforme sube la escalera.

| Código | V_INTERFAZ | I_LED | R_LDR | f simulada | Error vs. teoría |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 0 | 0.005 V | 0 mA | 40.0 kΩ | 176.7 Hz | 0.81 % |
| 5 | 1.562 V | 0.03 mA | 39.6 kΩ | 178.6 Hz | 0.81 % |
| 6 | 1.875 V | 1.04 mA | 28.7 kΩ | 244.9 Hz | 0.81 % |
| 8 | 2.500 V | 4.69 mA | 9.56 kΩ | 710.9 Hz | 0.82 % |
| 10 | 3.125 V | 8.61 mA | 3.68 kΩ | 1711.6 Hz | 0.85 % |
| 12 | 3.750 V | 12.61 mA | 2.08 kΩ | 2776.7 Hz | 0.88 % |
| 15 | 4.687 V | 18.66 mA | 1.58 kΩ | 3443.0 Hz | 0.90 % |

> [!NOTE]
> La curva de la LDR (`1500 + 38500·exp(−I_LED/3 mA)`) es una aproximación. Para que la simulación se parezca al montaje real, conviene medir la resistencia de la LDR del laboratorio con el LED en varios códigos y ajustar esa expresión en `B_RLDR`.

---

## 3. Correr las simulaciones desde la terminal (opcional)

```powershell
$lt = "$env:LOCALAPPDATA\Programs\ADI\LTspice\LTspice.exe"
& $lt -Run -b dac_r2r_buffer.asc | Out-Null
& $lt -Run -b receptor_555_ldr.asc | Out-Null
& $lt -Run -b sistema_completo_optico.asc | Out-Null
Get-Content sistema_completo_optico.log
```

Para depurar y dejar la carpeta libre de archivos pesados de formas de onda transitorias (`.raw`), bases de datos (`.db`) y registros temporales:
```powershell
Get-ChildItem -Include *.raw, *.op.raw, *.log.raw, *.db, *.log, *.net -Recurse | Remove-Item -Force
```

---

## 4. Legibilidad y estética de los esquemáticos

Los esquemáticos han sido diagramados para facilitar su inspección visual y exportación a documentación técnica:
* **Etiquetas de red horizontales:** Todos los nodos principales (`B0..B3`, `V_R2R`, `V_DAC`, `V_INTERFAZ`, `OUT_555`, `THR`, `V_BUZZER`, `VCC`, `VOP`, `RLDR`) cuentan con segmentos de cable horizontales para evitar que LTspice rote los textos 90° sobre pines verticales.
* **Resistencias horizontales R-2R:** Los textos de `R_h1..R_h3` (`10k`) se alinean horizontalmente bajo el componente con `VTop`, liberando los nodos de interconexión con las ramas verticales.
* **Separación de etapas e integrados:** Los circuitos integrados (`LM358`, `NE555`) disponen de holgura en sus pines para no superponer los nombres de los pines del modelo con las etiquetas de red del usuario.

