#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
============================================================================
PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113 - ITCR)
EMPAQUETADOR DE ENTREGA OFICIAL (Cumplimiento Sección 4.1 del Instructivo)
AUTOR: Mario Cerdas y equipo de trabajo
============================================================================
Descripción:
Construye automáticamente el directorio 'Investigacion_Dirigida/' y el archivo
comprimido 'Investigacion_Dirigida.zip' respetando al pie de la letra la
jerarquía oficial exigida en la Sección 4.1 y Tabla 1 (Rúbrica) del manual:
  Investigacion_Dirigida/
  ├── LTspice/
  ├── TinkerCAD/
  ├── Arduino/
  ├── Matlab/
  └── Documentacion/
"""

import os
import shutil
import sys
import zipfile

# Configurar codificación UTF-8 para la salida estándar en Windows
if sys.stdout.encoding != "utf-8":
    sys.stdout.reconfigure(encoding="utf-8")


class EmpaquetadorEntregaOficial:
    """
    ----------------------------------------------------------------------------
    Clase: EmpaquetadorEntregaOficial
    ----------------------------------------------------------------------------
    Descripción:
      Automatiza la creación de la estructura oficial de carpetas para la entrega
      de la Investigación Dirigida (EL2113) y genera el paquete comprimido .zip
      listo para subir al tecDigital.

    Funciones que contiene:
      - __init__(self, directorio_raiz):
          Inicializa las rutas de origen del repositorio y de destino del entregable.
      - crear_estructura_carpetas(self):
          Crea las subcarpetas obligatorias (LTspice, TinkerCAD, Arduino, Matlab,
          Documentacion) dentro de 'Investigacion_Dirigida/'.
      - copiar_archivos_entregables(self):
          Copia los archivos de simulación, código fuente y documentación en sus
          carpetas oficiales correspondientes.
      - comprimir_zip(self):
          Empaqueta la carpeta 'Investigacion_Dirigida/' en 'Investigacion_Dirigida.zip'.
    ----------------------------------------------------------------------------
    """

    def __init__(self, directorio_raiz: str) -> None:
        """
        ------------------------------------------------------------------------
        Función: __init__ (Constructor)
        ------------------------------------------------------------------------
        Descripción:
          Inicializa el empaquetador verificando que el directorio raíz del
          proyecto exista y definiendo las rutas de salida.

        Entrada:
          - directorio_raiz (str): Ruta absoluta a la carpeta raíz del proyecto.

        Salida:
          Ninguna (None). Configura los atributos de instancia.

        Restricciones:
          - 'directorio_raiz' debe ser un directorio existente en el sistema.
        ------------------------------------------------------------------------
        """
        # Verificar que la ruta proporcionada exista en disco
        if not os.path.isdir(directorio_raiz):
            raise FileNotFoundError(f"El directorio raíz no existe: {directorio_raiz}")

        # Definir atributos de rutas principales
        self.raiz = os.path.abspath(directorio_raiz)
        self.carpeta_entrega = os.path.join(self.raiz, "Investigacion_Dirigida")
        self.archivo_zip = os.path.join(self.raiz, "Investigacion_Dirigida.zip")
        self.subcarpetas = ["LTspice", "TinkerCAD", "Arduino", "Matlab", "Documentacion"]

    def crear_estructura_carpetas(self) -> None:
        """
        ------------------------------------------------------------------------
        Función: crear_estructura_carpetas
        ------------------------------------------------------------------------
        Descripción:
          Crea desde cero el árbol de directorios oficial 'Investigacion_Dirigida/'
          con las cinco subcarpetas estipuladas en la Sección 4.1 del manual.

        Entrada:
          Ninguna. Utiliza los atributos 'self.carpeta_entrega' y 'self.subcarpetas'.

        Salida:
          Ninguna (None). Crea los directorios físicos en el sistema de archivos.

        Restricciones:
          - Si existe una carpeta previa 'Investigacion_Dirigida/', se actualiza
            preservando cualquier archivo 'Informe.pdf' compilado previamente.
        ------------------------------------------------------------------------
        """
        # Crear el directorio principal de entrega y cada una de las 5 subcarpetas
        for sub in self.subcarpetas:
            ruta_sub = os.path.join(self.carpeta_entrega, sub)
            os.makedirs(ruta_sub, exist_ok=True)

    def copiar_archivos_entregables(self) -> int:
        """
        ------------------------------------------------------------------------
        Función: copiar_archivos_entregables
        ------------------------------------------------------------------------
        Descripción:
          Copia los archivos de LTspice, TinkerCAD, Arduino, MATLAB y Documentación
          hacia la estructura oficial 'Investigacion_Dirigida/'.

        Entrada:
          Ninguna. Lee desde las carpetas de desarrollo del repositorio.

        Salida:
          - int: Cantidad total de archivos copiados exitosamente.

        Restricciones:
          - Los archivos fuente deben existir en sus ubicaciones del repositorio.
        ------------------------------------------------------------------------
        """
        # Mapa de archivos (origen relativo -> subcarpeta oficial de destino)
        mapa_copias = [
            # 1. LTspice (Etapas separadas y sistema completo en conjunto)
            ("simulation/ltspice/dac_r2r_buffer.asc", "LTspice"),
            ("simulation/ltspice/dac_r2r_buffer.cir", "LTspice"),
            ("simulation/ltspice/receptor_555_ldr.asc", "LTspice"),
            ("simulation/ltspice/receptor_555_ldr.cir", "LTspice"),
            ("simulation/ltspice/sistema_completo_optico.asc", "LTspice"),
            ("simulation/ltspice/sistema_completo_optico.cir", "LTspice"),
            # 2. TinkerCAD (Archivo de simulación y archivo de enlace)
            ("simulation/tinkercad/codigo_tinkercad.ino", "TinkerCAD"),
            ("simulation/tinkercad/enlace_simulacion_tinkercad.txt", "TinkerCAD"),
            ("simulation/tinkercad/guia_montaje_tinkercad.md", "TinkerCAD"),
            # 3. Arduino (Código fuente oficial)
            ("src/arduino/emisor_4bits/emisor_4bits.ino", "Arduino"),
            # 4. Matlab (Código para generar representaciones digitales y simulación)
            ("matlab/generador_patrones.m", "Matlab"),
            ("matlab/simulacion_dac_555.m", "Matlab"),
            # 5. Documentacion (Informe IEEE, memoria de cálculo y planos 3D/Láser)
            ("docs/informe_ieee/informe_ieee_el2113.tex", "Documentacion"),
            ("docs/memoria_de_calculo.md", "Documentacion"),
            ("docs/diseno_prototipado_3d_laser.md", "Documentacion"),
            ("prototipo_laboratorio/corte_laser/base_soporte_optico.svg", "Documentacion"),
            ("prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.scad", "Documentacion"),
            ("prototipo_laboratorio/impresion_3d/acoplador_optico_led_ldr.stl", "Documentacion"),
        ]

        total_copiados = 0
        # Recorrer la lista y copiar cada archivo a su carpeta oficial
        for rel_origen, carpeta_dest in mapa_copias:
            abs_origen = os.path.join(self.raiz, rel_origen)
            dir_destino = os.path.join(self.carpeta_entrega, carpeta_dest)
            if os.path.exists(abs_origen):
                shutil.copy2(abs_origen, dir_destino)
                total_copiados += 1

        return total_copiados

    def comprimir_zip(self) -> str:
        """
        ------------------------------------------------------------------------
        Función: comprimir_zip
        ------------------------------------------------------------------------
        Descripción:
          Comprime recursivamente la carpeta 'Investigacion_Dirigida/' en un
          archivo 'Investigacion_Dirigida.zip' manteniendo las rutas relativas.

        Entrada:
          Ninguna. Utiliza 'self.carpeta_entrega' y 'self.archivo_zip'.

        Salida:
          - str: Ruta absoluta del archivo .zip generado.

        Restricciones:
          - La carpeta 'Investigacion_Dirigida/' debe haber sido poblada antes
            de invocar este método.
        ------------------------------------------------------------------------
        """
        # Abrir archivo ZIP en modo escritura con compresión estándar DEFLATE
        with zipfile.ZipFile(self.archivo_zip, "w", zipfile.ZIP_DEFLATED) as zf:
            # Recorrer recursivamente todas las subcarpetas de Investigacion_Dirigida
            for root, _, files in os.walk(self.carpeta_entrega):
                for file in files:
                    ruta_completa = os.path.join(root, file)
                    ruta_relativa = os.path.relpath(ruta_completa, self.raiz)
                    zf.write(ruta_completa, ruta_relativa)

        return self.archivo_zip


def main() -> None:
    """
    ----------------------------------------------------------------------------
    Función: main
    ----------------------------------------------------------------------------
    Descripción:
      Punto de entrada que ejecuta la creación de la carpeta oficial de entrega
      'Investigacion_Dirigida/' y genera el paquete comprimido '.zip'.

    Entrada:
      Ninguna.

    Salida:
      Ninguna (None). Muestra resumen en consola.

    Restricciones:
      - Debe ejecutarse desde el entorno del proyecto con permisos de escritura.
    ----------------------------------------------------------------------------
    """
    # Determinar el directorio raíz del proyecto
    base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    empaquetador = EmpaquetadorEntregaOficial(base_dir)

    # Crear directorios, copiar archivos y generar el archivo .zip
    empaquetador.crear_estructura_carpetas()
    num_archivos = empaquetador.copiar_archivos_entregables()
    ruta_zip = empaquetador.comprimir_zip()

    print("=" * 72)
    print("  ESTRUCTURA OFICIAL DE ENTREGA GENERADA (Sección 4.1 - EL2113)")
    print("=" * 72)
    print(f"  * Carpeta oficial : {empaquetador.carpeta_entrega}")
    print(f"  * Archivos copiados: {num_archivos}")
    print(f"  * Archivo ZIP     : {ruta_zip}")
    print("=" * 72)


if __name__ == "__main__":
    main()
