#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Genera el archivo acoplador_optico_led_ldr.stl (STL ASCII) para impresión 3D
en la Escuela de Electrónica (Req. M.13).
"""

import math
import os


def generar_cilindro_triangulos(r_ext, r_int, h, n_seg=32, z0=0.0):
    triangulos = []
    d_theta = 2 * math.pi / n_seg
    for i in range(n_seg):
        th1 = i * d_theta
        th2 = (i + 1) * d_theta
        cos1, sin1 = math.cos(th1), math.sin(th1)
        cos2, sin2 = math.cos(th2), math.sin(th2)

        # Exterior
        p1 = (r_ext * cos1, r_ext * sin1, z0)
        p2 = (r_ext * cos2, r_ext * sin2, z0)
        p3 = (r_ext * cos2, r_ext * sin2, z0 + h)
        p4 = (r_ext * cos1, r_ext * sin1, z0 + h)
        triangulos.append((p1, p2, p3))
        triangulos.append((p1, p3, p4))

        # Interior
        q1 = (r_int * cos1, r_int * sin1, z0)
        q2 = (r_int * cos2, r_int * sin2, z0)
        q3 = (r_int * cos2, r_int * sin2, z0 + h)
        q4 = (r_int * cos1, r_int * sin1, z0 + h)
        triangulos.append((q1, q3, q2))
        triangulos.append((q1, q4, q3))

        # Tapa inferior (z0)
        triangulos.append((p1, q1, q2))
        triangulos.append((p1, q2, p2))

        # Tapa superior (z0 + h)
        triangulos.append((p4, p3, q3))
        triangulos.append((p4, q3, q4))

    return triangulos


def main():
    directorio = os.path.dirname(os.path.abspath(__file__))
    ruta_stl = os.path.join(directorio, "acoplador_optico_led_ldr.stl")

    # Sección 1: Cavidad de 5.2 mm (radio 2.6 mm) para LED/LDR (h = 12 mm)
    t1 = generar_cilindro_triangulos(
        r_ext=6.0, r_int=2.6, h=12.0, n_seg=32, z0=0.0
    )
    # Sección 2: Conducto de 2.2 mm (radio 1.1 mm) para fibra POF (h = 16 mm)
    t2 = generar_cilindro_triangulos(
        r_ext=6.0, r_int=1.1, h=16.0, n_seg=32, z0=12.0
    )

    todos = t1 + t2

    with open(ruta_stl, "w", encoding="utf-8") as f:
        f.write("solid acoplador_optico_led_ldr\n")
        for p1, p2, p3 in todos:
            # Calcular normal aproximada
            u = (p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2])
            v = (p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2])
            nx = u[1] * v[2] - u[2] * v[1]
            ny = u[2] * v[0] - u[0] * v[2]
            nz = u[0] * v[1] - u[1] * v[0]
            mag = math.sqrt(nx * nx + ny * ny + nz * nz) or 1.0
            nx, ny, nz = nx / mag, ny / mag, nz / mag

            f.write(f"  facet normal {nx:.6f} {ny:.6f} {nz:.6f}\n")
            f.write("    outer loop\n")
            f.write(f"      vertex {p1[0]:.4f} {p1[1]:.4f} {p1[2]:.4f}\n")
            f.write(f"      vertex {p2[0]:.4f} {p2[1]:.4f} {p2[2]:.4f}\n")
            f.write(f"      vertex {p3[0]:.4f} {p3[1]:.4f} {p3[2]:.4f}\n")
            f.write("    endloop\n")
            f.write("  endfacet\n")
        f.write("endsolid acoplador_optico_led_ldr\n")

    print(
        f"Archivo STL generado exitosamente con {len(todos)} facetas: {ruta_stl}"
    )


if __name__ == "__main__":
    main()
