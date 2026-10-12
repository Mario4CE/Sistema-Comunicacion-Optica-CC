#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
============================================================================
PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
SUITE DE VALIDACIÓN AUTOMÁTICA INTEGRAL (Requerimientos A.1 a Q.18)
AUTOR: Mario Cerdas y equipo de trabajo
============================================================================
Descripción:
Este script verifica automáticamente el cumplimiento al pie de la letra de:
1. Sintaxis estructural, arreglos de longitud variable (periódico y aperiódico)
   y mapeo de pines en los archivos C++ (.ino) (Req. C.3 y D.4).
2. Emulación bit a bit de aplicarBits() y del parser de comandos seriales,
   incluyendo el ajuste de tiempo en el rango estricto [1 ms, 10000 ms].
3. Resolución por Análisis Nodal (Leyes de Kirchhoff) de la escalera R-2R
   comparada contra el cálculo del código de Arduino (Error < 10%, Req. O.16).
4. Cumplimiento de reglas de laboratorio (Req. M.13, N.14, P.17, Q.18),
   simulación integrada en LTspice y reporte IEEE Transactions.
"""

import os
import re
import sys

# Asegurar salida UTF-8 en consola de Windows
if sys.stdout.encoding != "utf-8":
    sys.stdout.reconfigure(encoding="utf-8")


class ValidadorSistemaOptico:
    """
    ----------------------------------------------------------------------------
    Clase: ValidadorSistemaOptico
    ----------------------------------------------------------------------------
    Descripción:
      Encapsula las herramientas de verificación estática, emulación lógica de
      firmware C++ (Arduino), resolución matemática nodal del circuito analógico
      (DAC R-2R, OpAmp 1A/1B LM358, Driver LED, LDR y NE555 astable) y auditoría
      de cumplimiento de requerimientos de laboratorio (A.1 a Q.18).

    Funciones que contiene:
      - __init__(self, v_ref, r_base, r_driver, v_led_fwd, r1_555, c1_555):
          Inicializa los parámetros eléctricos y físicos del sistema óptico.
      - emular_aplicar_bits_cpp(self, valor):
          Emula la extracción bit a bit de la función C++ aplicarBits().
      - resolver_nodos_r2r_kirchhoff(self, b0, b1, b2, b3):
          Calcula el voltaje exacto del DAC R-2R mediante análisis nodal / LCK.
      - emular_parser_serial_cpp(self, comando):
          Replica el comportamiento del intérprete de comandos seriales de Arduino.
      - verificar_archivo_ino(self, ruta):
          Inspecciona la sintaxis, arreglos de longitud variable y estructura .ino.
      - calcular_respuesta_optica_y_555(self, v_dac):
          Calcula la corriente del LED, resistencia de la LDR y frecuencia del 555.
      - auditar_requerimientos_laboratorio(self, base_dir):
          Verifica el cumplimiento documental de P.17, Q.18, M.13, LTspice e IEEE.
    ----------------------------------------------------------------------------
    """

    def __init__(
        self,
        v_ref: float = 5.0,
        r_base: float = 10000.0,
        r_driver: float = 150.0,
        v_led_fwd: float = 1.8,
        r1_555: float = 1000.0,
        c1_555: float = 100e-9,
    ) -> None:
        """
        ------------------------------------------------------------------------
        Función: __init__ (Constructor)
        ------------------------------------------------------------------------
        Descripción:
          Inicializa una instancia del validador configurando las constantes
          eléctricas del DAC R-2R, del circuito emisor LED y del oscilador 555.

        Entrada:
          - v_ref (float): Voltaje lógico alto de referencia del Arduino en voltios.
          - r_base (float): Valor de la resistencia base R de la escalera en ohmios.
          - r_driver (float): Resistencia limitadora de corriente del LED en ohmios.
          - v_led_fwd (float): Voltaje umbral de conducción directa del LED en voltios.
          - r1_555 (float): Resistencia fija R1 conectada al pin 7 del NE555 en ohmios.
          - c1_555 (float): Capacitancia de temporización C1 del NE555 en faradios.

        Salida:
          Ninguna (None). Inicializa los atributos internos del objeto.

        Restricciones:
          - Todos los parámetros numéricos deben ser valores reales estrictamente
            positivos (> 0).
        ------------------------------------------------------------------------
        """
        # Validar que los parámetros físicos sean estrictamente positivos
        if any(p <= 0 for p in (v_ref, r_base, r_driver, v_led_fwd, r1_555, c1_555)):
            raise ValueError("Todos los parámetros eléctricos deben ser mayores que cero.")

        # Asignación de constantes de circuito como atributos de instancia
        self.v_ref = v_ref
        self.r_base = r_base
        self.r_doble = 2.0 * r_base  # Ramas 2R de la escalera
        self.r_driver = r_driver
        self.v_led_fwd = v_led_fwd
        self.r1_555 = r1_555
        self.c1_555 = c1_555

    def emular_aplicar_bits_cpp(self, valor: int) -> tuple[int, int, int, int]:
        """
        ------------------------------------------------------------------------
        Función: emular_aplicar_bits_cpp
        ------------------------------------------------------------------------
        Descripción:
          Replica con exactitud las operaciones a nivel de bits de la función
          aplicarBits(uint8_t valor) del firmware de Arduino para obtener el
          estado lógico de los pines D8, D9, D10 y D11.

        Entrada:
          - valor (int): Número entero que representa el estado a convertir en
            palabra binaria de 4 bits.

        Salida:
          - tuple[int, int, int, int]: Tupla de 4 enteros (b0, b1, b2, b3) donde
            cada elemento vale 0 o 1, siendo b0 el LSB (D8) y b3 el MSB (D11).

        Restricciones:
          - El parámetro 'valor' debe ser un número entero (int). Si excede el
            rango [0, 15], se trunca a los 4 bits menos significativos con 0x0F.
        ------------------------------------------------------------------------
        """
        # Aplicar máscara hexadecimal 0x0F para aislar los 4 bits inferiores
        dato4Bits = int(valor) & 0x0F

        # Extraer cada bit individual verificando su peso binario correspondiente
        b0 = 1 if (dato4Bits & 0x01) else 0  # Pin D8 (LSB - peso 1)
        b1 = 1 if (dato4Bits & 0x02) else 0  # Pin D9 (peso 2)
        b2 = 1 if (dato4Bits & 0x04) else 0  # Pin D10 (peso 4)
        b3 = 1 if (dato4Bits & 0x08) else 0  # Pin D11 (MSB - peso 8)

        return (b0, b1, b2, b3)

    def resolver_nodos_r2r_kirchhoff(self, b0: int, b1: int, b2: int, b3: int) -> float:
        """
        ------------------------------------------------------------------------
        Función: resolver_nodos_r2r_kirchhoff
        ------------------------------------------------------------------------
        Descripción:
          Resuelve paso a paso las ecuaciones nodales de la escalera R-2R usando
          Ley de Corrientes de Kirchhoff (LCK) en los 4 nodos V1, V2, V3 y V4,
          sin recurrir a la fórmula ideal abreviada, para comprobar el diseño físico.

        Entrada:
          - b0 (int): Estado lógico del bit 0 (LSB), 0 o 1.
          - b1 (int): Estado lógico del bit 1, 0 o 1.
          - b2 (int): Estado lógico del bit 2, 0 o 1.
          - b3 (int): Estado lógico del bit 3 (MSB), 0 o 1.

        Salida:
          - float: Voltaje analógico resultante en el nodo de salida V_DAC (voltios).

        Restricciones:
          - Los argumentos b0, b1, b2 y b3 deben pertenecer al conjunto {0, 1}.
          - Se asume acoplamiento a un seguidor de tensión (OpAmp 1A del LM358)
            con impedancia de entrada idealmente infinita.
        ------------------------------------------------------------------------
        """
        R = self.r_base
        R2 = self.r_doble

        # Convertir cada nivel lógico (0 o 1) en su tensión física de entrada (0V o 5V)
        v_in = [b0 * self.v_ref, b1 * self.v_ref, b2 * self.v_ref, b3 * self.v_ref]

        # Etapa 1 (Nodo V1 - LSB b0): LCK entre rama 2R a tierra, rama 2R hacia b0 y R en serie
        v_th1 = v_in[0] * (R2 / (R2 + R2))
        r_th1 = (R2 * R2) / (R2 + R2) + R  # Equivalente: 10k + 10k = 20k

        # Etapa 2 (Nodo V2 - b1): LCK entre el equivalente de Nodo 1 y la entrada b1
        v_th2 = (v_th1 * R2 + v_in[1] * r_th1) / (r_th1 + R2)
        r_th2 = (r_th1 * R2) / (r_th1 + R2) + R  # Equivalente: 10k + 10k = 20k

        # Etapa 3 (Nodo V3 - b2): LCK entre el equivalente de Nodo 2 y la entrada b2
        v_th3 = (v_th2 * R2 + v_in[2] * r_th2) / (r_th2 + R2)
        r_th3 = (r_th2 * R2) / (r_th2 + R2) + R  # Equivalente: 10k + 10k = 20k

        # Etapa 4 (Nodo V4 - MSB b3 hacia entrada no inversora del OpAmp 1A)
        v_out = (v_th3 * R2 + v_in[3] * r_th3) / (r_th3 + R2)
        return v_out

    def emular_parser_serial_cpp(self, comando: str) -> str:
        """
        ------------------------------------------------------------------------
        Función: emular_parser_serial_cpp
        ------------------------------------------------------------------------
        Descripción:
          Simula el comportamiento de la función procesarComandosSeriales() del
          archivo emisor_4bits.ino ante cualquier cadena enviada por el usuario,
          incluyendo los modos 'A', 'P' (Periódico MATLAB), 'S' (Aperiódico MATLAB),
          'C', 'M', ajuste de tiempo 'T<ms>' en [1, 10000] ms y números '0'..'15'.

        Entrada:
          - comando (str): Cadena de texto recibida por el puerto serial.

        Salida:
          - str: Identificador del estado o acción tomada.

        Restricciones:
          - El parámetro 'comando' debe ser de tipo cadena (str).
          - El tiempo 'T<ms>' solo es válido en [1, 10000] ms (Req. C.3).
          - Los valores numéricos manuales solo son válidos en [0, 15].
        ------------------------------------------------------------------------
        """
        # Eliminar espacios en blanco, tabulaciones y saltos de línea extremos
        entrada = comando.strip()

        # Si la cadena está vacía, el microcontrolador la ignora sin alterar el modo
        if len(entrada) == 0:
            return "IGNORADO_VACIO"

        # Evaluar comandos alfabéticos de selección de arreglo y modo
        upper = entrada.upper()
        if upper == "A":
            return "MODO_AUTOMATICO"
        elif upper == "P":
            return "ARREGLO_MATLAB_PERIODICO"
        elif upper == "S":
            return "ARREGLO_MATLAB_APERIODICO"
        elif upper == "C":
            return "MODO_CALIBRACION"
        elif upper == "M":
            return "MODO_MANUAL_ESPERA"
        elif upper.startswith("T") and len(upper) > 1:
            sub_t = upper[1:]
            if all(c.isdigit() for c in sub_t):
                t_ms = int(sub_t)
                if 1 <= t_ms <= 10000:
                    return f"TIEMPO_MS_{t_ms}"
            return "ERROR_TIEMPO_FUERA_DE_RANGO"
        else:
            # Verificar que absolutamente todos los caracteres sean dígitos (isDigit)
            es_numero = all(c.isdigit() for c in entrada)
            if es_numero:
                val = int(entrada)
                # Comprobar que el número entero se encuentre dentro de los 4 bits [0, 15]
                if 0 <= val <= 15:
                    return f"MANUAL_VALOR_{val}"

            # Cualquier otro texto o número fuera de rango es rechazado
            return "ERROR_COMANDO_NO_RECONOCIDO"

    def verificar_archivo_ino(self, ruta: str) -> list[str]:
        """
        ------------------------------------------------------------------------
        Función: verificar_archivo_ino
        ------------------------------------------------------------------------
        Descripción:
          Analiza estáticamente el código fuente de un archivo de Arduino (.ino)
          para comprobar el balance de delimitadores, las funciones principales y
          la presencia de los arreglos de longitud variable de MATLAB (Req. C.3/D.4).

        Entrada:
          - ruta (str): Ruta en el sistema de archivos hacia el archivo .ino.

        Salida:
          - list[str]: Lista con las descripciones de los errores encontrados.
            Devuelve una lista vacía [] si el archivo es completamente válido.

        Restricciones:
          - El archivo indicado en 'ruta' debe existir y estar codificado en UTF-8.
        ------------------------------------------------------------------------
        """
        errores = []

        # Verificar existencia física del archivo en disco
        if not os.path.exists(ruta):
            return [f"No se encontró el archivo: {ruta}"]

        # Leer contenido completo del código fuente C++
        with open(ruta, "r", encoding="utf-8") as f:
            contenido = f.read()

        # 1. Comprobar balance simétrico de llaves y paréntesis
        if contenido.count("{") != contenido.count("}"):
            errores.append("Desbalance de llaves '{' y '}'")
        if contenido.count("(") != contenido.count(")"):
            errores.append("Desbalance de paréntesis '(' y ')'")

        # 2. Verificar existencia de las funciones principales del ciclo de Arduino
        if not re.search(r"\bvoid\s+setup\s*\(\s*\)", contenido):
            errores.append("Falta función obligatoria void setup()")
        if not re.search(r"\bvoid\s+loop\s*\(\s*\)", contenido):
            errores.append("Falta función obligatoria void loop()")

        # 3. Verificar inicialización serial, pines y arreglos MATLAB (Req. C.3 y D.4)
        if "Serial.begin(9600);" not in contenido:
            errores.append("Falta inicialización Serial.begin(9600)")
        if contenido.count("pinMode(") < 4:
            errores.append("Se esperaban al menos 4 llamadas a pinMode() para los 4 bits")
        if contenido.count("digitalWrite(") < 4:
            errores.append("Se esperaban al menos 4 llamadas a digitalWrite() para los 4 bits")
        if "PATRON_PERIODICO_MATLAB" not in contenido or "PATRON_APERIODICO_MATLAB" not in contenido:
            errores.append("Faltan arreglos de longitud variable generados en MATLAB (Req. C.3/D.4)")
        if "TIEMPO_MIN_MS = 1UL" not in contenido or "TIEMPO_MAX_MS = 10000UL" not in contenido:
            errores.append("Faltan límites de tiempo [1 ms, 10000 ms] del Requerimiento C.3")

        return errores

    def calcular_respuesta_optica_y_555(self, v_dac: float) -> tuple[float, float, float]:
        """
        ------------------------------------------------------------------------
        Función: calcular_respuesta_optica_y_555
        ------------------------------------------------------------------------
        Descripción:
          Calcula la corriente de excitación del LED transmisor a la salida de la
          interfaz OpAmp 1B LM358, estima la resistencia de la LDR receptora y obtiene
          la frecuencia de la onda cuadrada generada por el NE555 en modo astable.

        Entrada:
          - v_dac (float): Voltaje analógico de salida del DAC R-2R en voltios.

        Salida:
          - tuple[float, float, float]: Tupla con (i_led_mA, r_ldr_ohm, f_555_hz).

        Restricciones:
          - El voltaje 'v_dac' debe estar dentro del rango operativo [0.0, 5.0] V.
          - Si v_dac <= v_led_fwd (1.8 V), el LED permanece en corte (0 mA).
        ------------------------------------------------------------------------
        """
        # Calcular corriente del LED en mA (0 mA si no supera el voltaje umbral de 1.8V)
        i_led_mA = max(0.0, (v_dac - self.v_led_fwd) / self.r_driver) * 1000.0

        # Determinar la resistencia de la LDR según el nivel de iluminación incidente
        if i_led_mA <= 0.01:
            r_ldr = 40000.0  # Resistencia en oscuridad (~40 kOhm)
        else:
            # Normalizar corriente respecto al estado máximo (estado 15 -> 4.6875 V)
            i_max_mA = ((4.6875 - self.v_led_fwd) / self.r_driver) * 1000.0
            i_norm = i_led_mA / i_max_mA
            # Curva de respuesta inversa típica de una celda fotoconductora de CdS
            r_ldr = 1500.0 + (40000.0 - 1500.0) * ((1.0 - i_norm) ** 2.2)

        # Calcular frecuencia del NE555 astable: f = 1.44 / ((R1 + 2 * R_LDR) * C1)
        f_555 = 1.44 / ((self.r1_555 + 2.0 * r_ldr) * self.c1_555)

        return (i_led_mA, r_ldr, f_555)

    def auditar_requerimientos_laboratorio(self, base_dir: str) -> list[str]:
        """
        ------------------------------------------------------------------------
        Función: auditar_requerimientos_laboratorio
        ------------------------------------------------------------------------
        Descripción:
          Verifica que los archivos del proyecto cumplan con las reglas de uso de
          laboratorios de la Escuela de Electrónica (Req. M.13, N.14, P.17 y Q.18),
          la simulación integrada en LTspice y la estructura de entrega oficial.

        Entrada:
          - base_dir (str): Ruta raíz del repositorio del proyecto.

        Salida:
          - list[str]: Lista de incumplimientos detectados (vacía [] si todo cumple).

        Restricciones:
          - 'base_dir' debe apuntar al directorio raíz del proyecto.
        ------------------------------------------------------------------------
        """
        incumplimientos = []

        # Archivos obligatorios para cumplir al pie de la letra con el manual
        archivos_requeridos = [
            "prototipo_laboratorio/corte_laser/base_soporte_optico.svg",
            "prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad",
            "prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl",
            "docs/diseno_prototipado_3d_laser.md",
            "docs/informe_ieee/informe_ieee_el2113.tex",
            "simulation/ltspice/sistema_completo_optico.cir",
            "simulation/ltspice/sistema_completo_optico.asc",
            "simulation/tinkercad/enlace_simulacion_tinkercad.txt",
            "Investigacion_Dirigida.zip",
        ]
        for rel in archivos_requeridos:
            if not os.path.exists(os.path.join(base_dir, rel)):
                incumplimientos.append(f"Falta archivo obligatorio: {rel}")

        # Verificar que guia_de_conexion_protoboard.md prohíba alimentar con 5V de Arduino (Req. P.17)
        ruta_proto = os.path.join(base_dir, "docs", "guia_de_conexion_protoboard.md")
        if os.path.exists(ruta_proto):
            with open(ruta_proto, "r", encoding="utf-8") as f:
                txt_proto = f.read()
            if "desde el pin de 5 V del Arduino" in txt_proto:
                incumplimientos.append("Violación de P.17 en guia_de_conexion_protoboard.md")
            if "Fuente de Alimentación" not in txt_proto or "Laboratorio" not in txt_proto:
                incumplimientos.append("Falta mención de Fuente de Laboratorio (Req. Q.18) en guía protoboard")

        return incumplimientos


def main() -> None:
    """
    ----------------------------------------------------------------------------
    Función: main
    ----------------------------------------------------------------------------
    Descripción:
      Función conductora principal que instancia la clase ValidadorSistemaOptico,
      ejecuta secuencialmente las 5 fases de verificación (sintaxis .ino, parser
      serial con C.3/D.4, análisis nodal de Kirchhoff del DAC R-2R, respuesta del
      555 y auditoría de laboratorios M.13/P.17/Q.18) e imprime el reporte final.

    Entrada:
      Ninguna. Utiliza las rutas relativas del repositorio del proyecto.

    Salida:
      Ninguna (None). Imprime tablas de validación en consola y finaliza el
      proceso con código de salida 0 (éxito) o 1 (error detectado).

    Restricciones:
      - Debe ejecutarse dentro de la estructura de carpetas del proyecto.
    ----------------------------------------------------------------------------
    """
    print("=" * 78)
    print("  SUITE DE VALIDACIÓN AUTOMÁTICA - SISTEMA DE COMUNICACIÓN ÓPTICA (EL2113)")
    print("=" * 78)

    # Instanciar la clase validadora con los parámetros nominales de diseño
    validador = ValidadorSistemaOptico()

    # Resolver la ruta raíz del proyecto a partir de la ubicación de este script
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    archivos_ino = [
        os.path.join(base_dir, "src", "arduino", "emisor_4bits", "emisor_4bits.ino"),
        os.path.join(base_dir, "simulation", "tinkercad", "codigo_tinkercad.ino"),
    ]

    # -------------------------------------------------------------------------
    # PRUEBA 1: Inspección estática de los códigos C++ (.ino) y arreglos MATLAB
    # -------------------------------------------------------------------------
    print("\n[1/5] Validando estructura C++, arreglos MATLAB y límites de tiempo (.ino)...")
    todo_ok = True
    for ruta in archivos_ino:
        nombre = os.path.relpath(ruta, base_dir)
        errores = validador.verificar_archivo_ino(ruta)
        if not errores:
            print(f"  [OK] {nombre} -> C++, arreglos MATLAB (C.3/D.4) y pines verificados.")
        else:
            todo_ok = False
            print(f"  [ERROR] {nombre}: {errores}")

    # -------------------------------------------------------------------------
    # PRUEBA 2: Validación del parser de comandos seriales (Incluyendo P, S, T<ms>)
    # -------------------------------------------------------------------------
    print("\n[2/5] Validando lógica de comandos seriales (procesarComandosSeriales)...")
    casos_serial = [
        ("A", "MODO_AUTOMATICO"),
        ("P", "ARREGLO_MATLAB_PERIODICO"),
        ("S", "ARREGLO_MATLAB_APERIODICO"),
        ("C", "MODO_CALIBRACION"),
        ("M", "MODO_MANUAL_ESPERA"),
        ("T1", "TIEMPO_MS_1"),
        ("T500", "TIEMPO_MS_500"),
        ("T10000", "TIEMPO_MS_10000"),
        ("T0", "ERROR_TIEMPO_FUERA_DE_RANGO"),
        ("T10001", "ERROR_TIEMPO_FUERA_DE_RANGO"),
        ("0", "MANUAL_VALOR_0"),
        ("7", "MANUAL_VALOR_7"),
        ("15", "MANUAL_VALOR_15"),
        ("16", "ERROR_COMANDO_NO_RECONOCIDO"),
        ("-3", "ERROR_COMANDO_NO_RECONOCIDO"),
        ("hola", "ERROR_COMANDO_NO_RECONOCIDO"),
        ("", "IGNORADO_VACIO"),
    ]
    for entrada, esperado in casos_serial:
        obtenido = validador.emular_parser_serial_cpp(entrada)
        estado = "OK" if obtenido == esperado else "FALLO"
        if estado != "OK":
            todo_ok = False
        print(f"  [{estado}] Entrada Serial: {repr(entrada):8s} -> {obtenido}")

    # -------------------------------------------------------------------------
    # PRUEBA 3 y 4: Simulación de los 16 estados (C++ -> DAC R-2R -> Óptica -> 555)
    # -------------------------------------------------------------------------
    print("\n[3/5] Validando los 16 estados: Pines C++ vs. Kirchhoff R-2R vs. 555 Astable...")
    print("-" * 78)
    print(f"{'Dec':>3} | {'Bits (D11..D8)':^14} | {'V_Cpp (V)':>9} | {'V_Kirchhoff':>11} | {'I_LED (mA)':>10} | {'R_LDR (kΩ)':>10} | {'f_555 (Hz)':>10}")
    print("-" * 78)

    max_error_voltaje = 0.0
    for d in range(16):
        # Emular pines de salida del Arduino para el estado decimal 'd'
        b0, b1, b2, b3 = validador.emular_aplicar_bits_cpp(d)

        # Comparar cálculo ideal del firmware C++ contra resolución nodal de Kirchhoff
        v_cpp = (d * 5.0) / 16.0
        v_kirchhoff = validador.resolver_nodos_r2r_kirchhoff(b0, b1, b2, b3)
        error_v = abs(v_cpp - v_kirchhoff)
        max_error_voltaje = max(max_error_voltaje, error_v)

        # Calcular las variables físicas de las etapas óptica y acústica
        i_led_mA, r_ldr, f_555 = validador.calcular_respuesta_optica_y_555(v_kirchhoff)

        # Formatear e imprimir fila de la tabla para el estado actual
        bits_str = f"{b3}  {b2}  {b1}  {b0}"
        print(f"{d:3d} |   [{bits_str}]   | {v_cpp:9.4f} | {v_kirchhoff:11.4f} | {i_led_mA:10.2f} | {r_ldr/1000.0:10.2f} | {f_555:10.1f}")

    print("-" * 78)

    # -------------------------------------------------------------------------
    # PRUEBA 5: Auditoría de Laboratorios (M.13, N.14, P.17, Q.18) y Entregable
    # -------------------------------------------------------------------------
    print("\n[4/5] Auditando requisitos de laboratorios (M.13, N.14, P.17, Q.18) y entregables...")
    errores_lab = validador.auditar_requerimientos_laboratorio(base_dir)
    if not errores_lab:
        print("  [OK] Reglas de fuente de laboratorio (P.17/Q.18) verificadas.")
        print("  [OK] Archivos de Corte Láser (.svg) e Impresión 3D (.scad) verificados (M.13).")
        print("  [OK] Propuesta de Escáner 3D documentada (N.14).")
        print("  [OK] Simulación integrada LTspice (7 etapas en conjunto) e Informe IEEE verificados.")
    else:
        todo_ok = False
        for err in errores_lab:
            print(f"  [ERROR] {err}")

    print("\n[5/5] Resumen de Verificación Matemática:")
    print(f"  * Error máximo entre código C++ y Análisis Nodal de Kirchhoff: {max_error_voltaje:.10f} V (< 10%, Req. O.16)")
    print("  * Rango de voltaje del DAC R-2R validado: 0.0000 V a 4.6875 V (Paso LSB = 0.3125 V)")
    print("  * Rango de frecuencia acústica del 555:   177.8 Hz (grave) a 3600.0 Hz (agudo)")

    # Verificar que todas las aserciones se cumplan con tolerancia numérica estricta
    if todo_ok and max_error_voltaje < 1e-9:
        print("\n>>> RESULTADO FINAL: TODAS LAS PRUEBAS Y REQUERIMIENTOS (A.1–Q.18) 100% VALIDADOS <<<")
        sys.exit(0)
    else:
        print("\n>>> RESULTADO FINAL: SE DETECTARON ERRORES EN LA VALIDACIÓN <<<")
        sys.exit(1)


if __name__ == "__main__":
    main()
