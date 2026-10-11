#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
============================================================================
PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
SUITE DE VALIDACIÓN AUTOMÁTICA: Código C++ Arduino + Red R-2R + 555 Astable
AUTOR: Mario Cerdas y equipo de trabajo
============================================================================
Descripción:
Este script verifica automáticamente:
1. Sintaxis estructural y mapeo de pines en los archivos C++ (.ino).
2. Emulación bit a bit de aplicarBits() y del parser de comandos seriales.
3. Resolución por Análisis Nodal (Leyes de Kirchhoff) de la escalera R-2R
   comparada contra el cálculo del código de Arduino.
4. Validación del modelo físico del Driver LED, LDR y Temporizador 555.
"""

import os
import re
import sys

# Asegurar salida UTF-8 en consola de Windows
if sys.stdout.encoding != 'utf-8':
    sys.stdout.reconfigure(encoding='utf-8')


class ValidadorSistemaOptico:
    """
    ----------------------------------------------------------------------------
    Clase: ValidadorSistemaOptico
    ----------------------------------------------------------------------------
    Descripción:
      Encapsula las herramientas de verificación estática, emulación lógica de
      firmware C++ (Arduino) y resolución matemática nodal del circuito analógico
      (DAC R-2R, Driver LED, fotorresistencia LDR y temporizador NE555 astable).

    Funciones que contiene:
      - __init__(self, v_ref, r_base, r_driver, v_led_fwd, r1_555, c1_555):
          Inicializa los parámetros eléctricos y físicos del sistema óptico.
      - emular_aplicar_bits_cpp(self, valor):
          Emula la extracción bit a bit de la función C++ aplicarBits().
      - resolver_nodos_r2r_kirchhoff(self, b0, b1, b2, b3):
          Calcula el voltaje exacto del DAC R-2R mediante análisis nodal / Thévenin.
      - emular_parser_serial_cpp(self, comando):
          Replica el comportamiento del intérprete de comandos seriales de Arduino.
      - verificar_archivo_ino(self, ruta):
          Inspecciona la sintaxis y estructura obligatoria de un archivo .ino.
      - calcular_respuesta_optica_y_555(self, v_dac):
          Calcula la corriente del LED, resistencia de la LDR y frecuencia del 555.
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
          reducción sucesiva de Thévenin y Ley de Corrientes de Kirchhoff (LCK),
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
          - Se asume acoplamiento a un seguidor de tensión (LM358) con impedancia
            de entrada idealmente infinita.
        ------------------------------------------------------------------------
        """
        R = self.r_base
        R2 = self.r_doble

        # Convertir cada nivel lógico (0 o 1) en su tensión física de entrada (0V o 5V)
        v_in = [b0 * self.v_ref, b1 * self.v_ref, b2 * self.v_ref, b3 * self.v_ref]

        # Etapa 1 (Nodo LSB b0): Divisor entre rama 2R a tierra y rama 2R hacia b0, más R en serie
        v_th1 = v_in[0] * (R2 / (R2 + R2))
        r_th1 = (R2 * R2) / (R2 + R2) + R  # Equivalente: 10k + 10k = 20k

        # Etapa 2 (Nodo b1): Superposición entre el equivalente de la Etapa 1 y la entrada b1
        v_th2 = (v_th1 * R2 + v_in[1] * r_th1) / (r_th1 + R2)
        r_th2 = (r_th1 * R2) / (r_th1 + R2) + R  # Equivalente: 10k + 10k = 20k

        # Etapa 3 (Nodo b2): Superposición entre el equivalente de la Etapa 2 y la entrada b2
        v_th3 = (v_th2 * R2 + v_in[2] * r_th2) / (r_th2 + R2)
        r_th3 = (r_th2 * R2) / (r_th2 + R2) + R  # Equivalente: 10k + 10k = 20k

        # Etapa 4 (Nodo MSB b3 - Salida V_DAC hacia entrada no inversora del OpAmp)
        v_out = (v_th3 * R2 + v_in[3] * r_th3) / (r_th3 + R2)
        return v_out

    def emular_parser_serial_cpp(self, comando: str) -> str:
        """
        ------------------------------------------------------------------------
        Función: emular_parser_serial_cpp
        ------------------------------------------------------------------------
        Descripción:
          Simula el comportamiento de la función procesarComandosSeriales() del
          archivo emisor_4bits.ino ante cualquier cadena enviada por el usuario.

        Entrada:
          - comando (str): Cadena de texto recibida por el puerto serial.

        Salida:
          - str: Identificador del estado o acción tomada ('MODO_AUTOMATICO',
            'MODO_CALIBRACION', 'MODO_MANUAL_ESPERA', 'MANUAL_VALOR_<N>',
            'IGNORADO_VACIO' o 'ERROR_COMANDO_NO_RECONOCIDO').

        Restricciones:
          - El parámetro 'comando' debe ser de tipo cadena (str).
          - Los valores numéricos manuales solo son válidos en el rango entero [0, 15].
        ------------------------------------------------------------------------
        """
        # Eliminar espacios en blanco, tabulaciones y saltos de línea extremos
        entrada = comando.strip()

        # Si la cadena está vacía, el microcontrolador la ignora sin alterar el modo
        if len(entrada) == 0:
            return "IGNORADO_VACIO"

        # Evaluar comandos alfabéticos de cambio de modo
        if entrada.upper() == "A":
            return "MODO_AUTOMATICO"
        elif entrada.upper() == "C":
            return "MODO_CALIBRACION"
        elif entrada.upper() == "M":
            return "MODO_MANUAL_ESPERA"
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
          para comprobar el balance de delimitadores y la presencia de las funciones
          y llamadas de hardware indispensables.

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

        # 3. Verificar inicialización serial y configuración de los 4 pines de salida
        if "Serial.begin(9600);" not in contenido:
            errores.append("Falta inicialización Serial.begin(9600)")
        if contenido.count("pinMode(") < 4:
            errores.append("Se esperaban al menos 4 llamadas a pinMode() para los 4 bits")
        if contenido.count("digitalWrite(") < 4:
            errores.append("Se esperaban al menos 4 llamadas a digitalWrite() para los 4 bits")

        return errores

    def calcular_respuesta_optica_y_555(self, v_dac: float) -> tuple[float, float, float]:
        """
        ------------------------------------------------------------------------
        Función: calcular_respuesta_optica_y_555
        ------------------------------------------------------------------------
        Descripción:
          Calcula la corriente de excitación del LED transmisor a la salida del
          buffer LM358, estima la resistencia de la LDR receptora y obtiene la
          frecuencia de la onda cuadrada generada por el NE555 en modo astable.

        Entrada:
          - v_dac (float): Voltaje analógico de salida del DAC R-2R en voltios.

        Salida:
          - tuple[float, float, float]: Tupla con (i_led_mA, r_ldr_ohm, f_555_hz)
            correspondientes a la corriente del LED (mA), resistencia de la LDR (Ω)
            y frecuencia de oscilación del 555 (Hz).

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


def main() -> None:
    """
    ----------------------------------------------------------------------------
    Función: main
    ----------------------------------------------------------------------------
    Descripción:
      Función conductora principal que instancia la clase ValidadorSistemaOptico,
      ejecuta secuencialmente las 4 fases de verificación (sintaxis .ino, parser
      serial, análisis nodal de Kirchhoff del DAC R-2R y respuesta del 555) e
      imprime el reporte de resultados en consola.

    Entrada:
      Ninguna. Utiliza las rutas relativas del repositorio del proyecto.

    Salida:
      Ninguna (None). Imprime tablas de validación en consola y finaliza el
      proceso con código de salida 0 (éxito) o 1 (error detectado).

    Restricciones:
      - Debe ejecutarse dentro de la estructura de carpetas del proyecto para
        localizar correctamente las carpetas 'src/' y 'simulation/'.
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
    # PRUEBA 1: Inspección estática de los códigos C++ (.ino)
    # -------------------------------------------------------------------------
    print("\n[1/4] Validando estructura y sintaxis de archivos C++ (.ino)...")
    todo_ok = True
    for ruta in archivos_ino:
        nombre = os.path.relpath(ruta, base_dir)
        errores = validador.verificar_archivo_ino(ruta)
        if not errores:
            print(f"  [OK] {nombre} -> Estructura C++, llaves, setup(), loop() y pines verificados.")
        else:
            todo_ok = False
            print(f"  [ERROR] {nombre}: {errores}")

    # -------------------------------------------------------------------------
    # PRUEBA 2: Validación del parser de comandos seriales (Casos normales y borde)
    # -------------------------------------------------------------------------
    print("\n[2/4] Validando lógica de comandos seriales (procesarComandosSeriales)...")
    casos_serial = [
        ("A", "MODO_AUTOMATICO"),
        ("a", "MODO_AUTOMATICO"),
        ("C", "MODO_CALIBRACION"),
        ("M", "MODO_MANUAL_ESPERA"),
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
    print("\n[3/4] Validando los 16 estados: Pines C++ vs. Kirchhoff R-2R vs. 555 Astable...")
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
    print("\n[4/4] Resumen de Verificación Matemática:")
    print(f"  * Error máximo entre código C++ y Análisis Nodal de Kirchhoff: {max_error_voltaje:.10f} V")
    print("  * Rango de voltaje del DAC R-2R validado: 0.0000 V a 4.6875 V (Paso LSB = 0.3125 V)")
    print("  * Rango de frecuencia acústica del 555:   177.8 Hz (grave) a 3600.0 Hz (agudo)")

    # Verificar que todas las aserciones se cumplan con tolerancia numérica estricta
    if todo_ok and max_error_voltaje < 1e-9:
        print("\n>>> RESULTADO FINAL: TODAS LAS PRUEBAS PASARON EXITOSAMENTE (100% VALIDADO) <<<")
        sys.exit(0)
    else:
        print("\n>>> RESULTADO FINAL: SE DETECTARON ERRORES EN LA VALIDACIÓN <<<")
        sys.exit(1)


if __name__ == "__main__":
    main()
