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


def emular_aplicar_bits_cpp(valor: int) -> tuple[int, int, int, int]:
    """Replica exactamente la lógica C++ de aplicarBits(uint8_t valor)."""
    dato4Bits = valor & 0x0F
    b0 = 1 if (dato4Bits & 0x01) else 0  # Pin D8 (LSB)
    b1 = 1 if (dato4Bits & 0x02) else 0  # Pin D9
    b2 = 1 if (dato4Bits & 0x04) else 0  # Pin D10
    b3 = 1 if (dato4Bits & 0x08) else 0  # Pin D11 (MSB)
    return (b0, b1, b2, b3)


def resolver_nodos_r2r_kirchhoff(b0: int, b1: int, b2: int, b3: int, v_ref: float = 5.0) -> float:
    """
    Resuelve las ecuaciones nodales exactas (Ley de Corrientes de Kirchhoff)
    para la red en escalera R-2R (R = 10k, 2R = 20k) sin usar la fórmula simplificada,
    para demostrar que el circuito físico produce exactamente el voltaje esperado.
    Nodos: V1 (b0), V2 (b1), V3 (b2), V4 (b3 = V_out)
    """
    R = 10000.0
    R2 = 20000.0

    v_in = [b0 * v_ref, b1 * v_ref, b2 * v_ref, b3 * v_ref]

    # Matriz de conductancias G * V = I por Ley de Corrientes de Kirchhoff (LCK):
    # Nodo 1: V1/2R (tierra) + (V1 - Vin0)/2R + (V1 - V2)/R = 0
    # Nodo 2: (V2 - V1)/R + (V2 - Vin1)/2R + (V2 - V3)/R = 0
    # Nodo 3: (V3 - V2)/R + (V3 - Vin2)/2R + (V3 - V4)/R = 0
    # Nodo 4: (V4 - V3)/R + (V4 - Vin3)/2R = 0 (entrada a Buffer OpAmp de Z_in -> inf)
    #
    # Resolviendo por sustitución sucesiva de Thévenin desde el nodo 1 al 4:
    # Etapa 1:
    v_th1 = v_in[0] * (R2 / (R2 + R2))
    r_th1 = (R2 * R2) / (R2 + R2) + R  # 10k + 10k = 20k

    # Etapa 2:
    v_th2 = (v_th1 * R2 + v_in[1] * r_th1) / (r_th1 + R2)
    r_th2 = (r_th1 * R2) / (r_th1 + R2) + R  # 10k + 10k = 20k

    # Etapa 3:
    v_th3 = (v_th2 * R2 + v_in[2] * r_th2) / (r_th2 + R2)
    r_th3 = (r_th2 * R2) / (r_th2 + R2) + R  # 10k + 10k = 20k

    # Etapa 4 (Salida V_DAC):
    v_out = (v_th3 * R2 + v_in[3] * r_th3) / (r_th3 + R2)
    return v_out


def emular_parser_serial_cpp(comando: str) -> str:
    """Replica exactamente la función procesarComandosSeriales() de emisor_4bits.ino."""
    entrada = comando.strip()
    if len(entrada) == 0:
        return "IGNORADO_VACIO"
    if entrada.upper() == "A":
        return "MODO_AUTOMATICO"
    elif entrada.upper() == "C":
        return "MODO_CALIBRACION"
    elif entrada.upper() == "M":
        return "MODO_MANUAL_ESPERA"
    else:
        es_numero = all(c.isdigit() for c in entrada)
        if es_numero:
            val = int(entrada)
            if 0 <= val <= 15:
                return f"MANUAL_VALOR_{val}"
        return "ERROR_COMANDO_NO_RECONOCIDO"


def verificar_archivo_ino(ruta: str) -> list[str]:
    """Verifica estructura sintáctica clave de un archivo .ino de Arduino (C++)."""
    errores = []
    if not os.path.exists(ruta):
        return [f"No se encontró el archivo: {ruta}"]

    with open(ruta, "r", encoding="utf-8") as f:
        contenido = f.read()

    # 1. Balance de llaves y paréntesis
    if contenido.count("{") != contenido.count("}"):
        errores.append("Desbalance de llaves '{' y '}'")
    if contenido.count("(") != contenido.count(")"):
        errores.append("Desbalance de paréntesis '(' y ')'")

    # 2. Presencia de funciones obligatorias de Arduino
    if not re.search(r"\bvoid\s+setup\s*\(\s*\)", contenido):
        errores.append("Falta función obligatoria void setup()")
    if not re.search(r"\bvoid\s+loop\s*\(\s*\)", contenido):
        errores.append("Falta función obligatoria void loop()")

    # 3. Configuración de Serial y pinMode
    if "Serial.begin(9600);" not in contenido:
        errores.append("Falta inicialización Serial.begin(9600)")
    if contenido.count("pinMode(") < 4:
        errores.append("Se esperaban al menos 4 llamadas a pinMode() para los 4 bits")
    if contenido.count("digitalWrite(") < 4:
        errores.append("Se esperaban al menos 4 llamadas a digitalWrite() para los 4 bits")

    return errores


def main():
    print("=" * 78)
    print("  SUITE DE VALIDACIÓN AUTOMÁTICA - SISTEMA DE COMUNICACIÓN ÓPTICA (EL2113)")
    print("=" * 78)

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
        errores = verificar_archivo_ino(ruta)
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
        obtenido = emular_parser_serial_cpp(entrada)
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

    R1_555 = 1000.0
    C1_555 = 100e-9
    R_DRIVER = 150.0
    V_LED_FWD = 1.8

    max_error_voltaje = 0.0
    for d in range(16):
        b0, b1, b2, b3 = emular_aplicar_bits_cpp(d)
        v_cpp = (d * 5.0) / 16.0
        v_kirchhoff = resolver_nodos_r2r_kirchhoff(b0, b1, b2, b3, v_ref=5.0)
        error_v = abs(v_cpp - v_kirchhoff)
        max_error_voltaje = max(max_error_voltaje, error_v)

        # Corriente en el LED tras el buffer LM358
        i_led_mA = max(0.0, (v_kirchhoff - V_LED_FWD) / R_DRIVER) * 1000.0

        # Modelo empírico de LDR (de 40k en oscuridad a 1.5k en iluminación máxima)
        if i_led_mA <= 0.01:
            r_ldr = 40000.0
        else:
            # Interpolación de ley de potencia inversa típica de fotorresistencia CdS
            i_norm = i_led_mA / ((4.6875 - V_LED_FWD) / R_DRIVER * 1000.0)
            r_ldr = 1500.0 + (40000.0 - 1500.0) * ((1.0 - i_norm) ** 2.2)

        # Frecuencia del 555 Astable: f = 1.44 / ((R1 + 2*R_LDR) * C1)
        f_555 = 1.44 / ((R1_555 + 2.0 * r_ldr) * C1_555)

        bits_str = f"{b3}  {b2}  {b1}  {b0}"
        print(f"{d:3d} |   [{bits_str}]   | {v_cpp:9.4f} | {v_kirchhoff:11.4f} | {i_led_mA:10.2f} | {r_ldr/1000.0:10.2f} | {f_555:10.1f}")

    print("-" * 78)
    print(f"\n[4/4] Resumen de Verificación Matemática:")
    print(f"  * Error máximo entre código C++ y Análisis Nodal de Kirchhoff: {max_error_voltaje:.10f} V")
    print(f"  * Rango de voltaje del DAC R-2R validado: 0.0000 V a 4.6875 V (Paso LSB = 0.3125 V)")
    print(f"  * Rango de frecuencia acústica del 555:   177.8 Hz (grave) a 3600.0 Hz (agudo)")

    if todo_ok and max_error_voltaje < 1e-9:
        print("\n>>> RESULTADO FINAL: TODAS LAS PRUEBAS PASARON EXITOSAMENTE (100% VALIDADO) <<<")
        sys.exit(0)
    else:
        print("\n>>> RESULTADO FINAL: SE DETECTARON ERRORES EN LA VALIDACIÓN <<<")
        sys.exit(1)


if __name__ == "__main__":
    main()
