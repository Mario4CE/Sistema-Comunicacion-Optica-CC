# Lista de Materiales (Bill of Materials - BOM)
**Proyecto:** Sistema de Comunicación por Medios Ópticos (EL2113)  
**Institución:** Escuela de Ingeniería Electrónica  

---

| Item | Componente | Valor / Modelo | Cant. Sugerida | Función en el Circuito | Notas de Adquisición |
|:---:|:---|:---|:---:|:---|:---|
| 1 | **Microcontrolador** | Arduino UNO o Arduino Nano | 1 | Generador digital de patrones de 4 bits | Cable USB de programación incluido |
| 2 | **Resistencias Red R** | $10\text{ k}\Omega$ (1/4 W, 1%) | 15 unid. | Red R–2R y divisores | Preferiblemente película metálica (1% tolerancia) para precisión |
| 3 | **Resistencias Red 2R** | $20\text{ k}\Omega$ (1/4 W, 1%) | 8 unid. | Ramas de la red R–2R | Si no hay de $20\text{ k}\Omega$, usar dos de $10\text{ k}\Omega$ en serie |
| 4 | **Resistencia limitadora** | $150\ \Omega$ o $180\ \Omega$ (1/4 W) | 2 unid. | Protección del LED emisor | Evita sobrecorriente en el LED |
| 5 | **Resistencia fija 555** | $1\text{ k}\Omega$ (1/4 W) | 2 unid. | Resistencia $R_1$ del pin 7 del 555 | Carga de temporización |
| 6 | **Amplificador Operacional** | LM358N (o LM324N) | 1 unid. | Buffer seguidor de voltaje (Etapa 3) | DIP-8, opera con fuente simple $0-5\text{ V}$ |
| 7 | **Diodo Emisor de Luz** | LED Rojo difuso o ultrabrillante | 2 unid. | Fuente de luz transmisora (Etapa 4) | $5\text{ mm}$, longitud de onda $\approx 630-660\text{ nm}$ |
| 8 | **Fibra Óptica** | Fibra óptica plástica (POF) | $1\text{ m}$ (cortar a $30-60\text{ cm}$) | Medio de transmisión guiado | Diámetro típico $1\text{ mm}$ a $2\text{ mm}$ |
| 9 | **Fotorresistencia (LDR)** | LDR estándar (ej. GL5528 o GL5516) | 2 unid. | Receptor óptico de luz | $5\text{ mm}$ de diámetro, respuesta en el visible |
| 10 | **Temporizador IC** | NE555P o LM555 | 1 unid. | Generador de onda cuadrada astable | DIP-8 |
| 11 | **Condensador de tiempo** | $100\text{ nF}$ ($0.1\ \mu\text{F}$) cerámico/poliéster | 3 unid. | Temporización $C_1$ del 555 y desacoplo | Código `104` |
| 12 | **Condensador de control**| $10\text{ nF}$ ($0.01\ \mu\text{F}$) cerámico | 2 unid. | Pin 5 (Control) del 555 a tierra | Código `103` (evita ruido) |
| 13 | **Condensador desacoplo**| $10\ \mu\text{F}$ electrolítico (16V o 25V) | 2 unid. | Desacoplo DC hacia el buzzer | Bloquea continua |
| 14 | **Transductor acústico** | Buzzer piezoeléctrico **PASIVO** | 1 unid. | Salida audible de frecuencias | **OJO:** Debe ser PASIVO (no activo) para reproducir tonos |
| 15 | **Termoencogible / Cinta** | Tubo termoencogible negro o cinta | 1 rollo / tramo | Aislamiento lumínico en acoplamiento | Para evitar interferencia de luz ambiental |
| 16 | **Protoboard y Cables** | Protoboard estándar + jumpers M-M | 1 kit | Ensamble físico del circuito | Cables de varios colores para orden |

---

### Verificación Crítica antes de comprar:
> [!IMPORTANT]
> **Buzzer Activo vs. Buzzer Pasivo:**
> El proyecto exige variar el tono acústico con un oscilador 555. Para ello, el buzzer **debe ser PASIVO**.
> * Un buzzer **activo** contiene su propio oscilador interno y solo suena a un pitido fijo cuando le aplicas voltaje.
> * Un buzzer **pasivo** no tiene oscilador interno y necesita una onda cuadrada externa (como la del 555) para sonar a diferentes notas/frecuencias.
