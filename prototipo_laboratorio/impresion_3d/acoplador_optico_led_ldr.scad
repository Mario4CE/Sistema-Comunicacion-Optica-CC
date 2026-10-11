// ============================================================================
// PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113 - ITCR)
// PIEZA DE IMPRESIÓN 3D (Req. M.13): Cabezal Acoplador Óptico LED / Fibra / LDR
// AUTOR: Mario Cerdas y equipo de trabajo
// LABORATORIO: Laboratorio de Prototipado 3D - Escuela de Ingeniería Electrónica
// ============================================================================
// Descripción:
//   Genera el modelo 3D paramétrico del manguito de acoplamiento óptico axial
//   que alinea concéntricamente el LED rojo (o la fotorresistencia LDR) de 5 mm
//   con la fibra óptica plástica (POF), aislando al 100% la luz ambiental del
//   laboratorio e incorporando una base con perforaciones M3 para fijarse sobre
//   la placa base cortada con láser.
//
// Entrada (Parámetros de diseño en milímetros):
//   - d_componente : Diámetro de inserción del LED/LDR (5.2 mm con tolerancia).
//   - d_fibra      : Diámetro del conducto para fibra óptica plástica (2.2 mm).
//   - largo_total  : Longitud axial total del cilindro acoplador (28.0 mm).
//
// Salida:
//   - Geometría sólida 3D exportable a formato .STL para impresión FDM (PLA Negro).
//
// Restricciones:
//   - Debe imprimirse en filamento opaco oscuro (PLA o PETG negro, relleno >= 40%)
//     para impedir la transmisión de luz parásita hacia la LDR.
// ============================================================================

$fn = 64; // Resolución circular de alta calidad (64 facetas por cilindro)

// Parámetros dimensionales en milímetros (mm)
d_exterior   = 12.0; // Diámetro exterior del barril opaco
d_componente = 5.2;  // Cavidad para LED de 5 mm o LDR GL5528 (con holgura 0.2 mm)
d_reborde    = 6.2;  // Alojamiento para el reborde inferior del LED de 5 mm
d_fibra      = 2.2;  // Conducto axial para la fibra óptica plástica (POF)
largo_total  = 28.0; // Longitud total del acoplador
largo_comp   = 12.0; // Profundidad de inserción del LED o LDR
ancho_base   = 26.0; // Ancho de la pestaña de anclaje a la base láser
espesor_base = 3.0;  // Espesor de la pestaña inferior
d_tornillo   = 3.2;  // Perforaciones para tornillos M3

/**
 * ----------------------------------------------------------------------------
 * Módulo: acoplador_optico_axial
 * ----------------------------------------------------------------------------
 * Descripción:
 *   Construye mediante geometría constructiva de sólidos (CSG) la unión del
 *   barril cilíndrico y la pestaña de montaje, restando las cavidades internas
 *   concéntricas para el componente optoelectrónico, la fibra y los tornillos.
 *
 * Entrada:
 *   Dimensiones globales parametrizadas en el encabezado del archivo.
 *
 * Salida:
 *   Sólido 3D listo para renderizado (F6) y exportación a archivo .STL.
 *
 * Restricciones:
 *   - d_exterior > d_reborde > d_componente > d_fibra > 0.
 * ----------------------------------------------------------------------------
 */
module acoplador_optico_axial() {
    difference() {
        // 1. Sólido positivo: Barril cilíndrico + Base plana de fijación
        union() {
            // Cilindro principal vertical
            cylinder(h = largo_total, d = d_exterior, center = false);
            // Base rectangular con extremos redondeados para fijar al chasis láser
            translate([-ancho_base/2, -d_exterior/2, 0])
                cube([ancho_base, d_exterior, espesor_base], center = false);
        }

        // 2. Cavidad inferior para el reborde del LED (2 mm de entrada)
        translate([0, 0, -0.1])
            cylinder(h = 2.1, d = d_reborde, center = false);

        // 3. Cavidad intermedia para el encapsulado de 5 mm (LED o LDR)
        translate([0, 0, 1.9])
            cylinder(h = largo_comp + 0.1, d = d_componente, center = false);

        // 4. Conducto superior de alineación axial para la fibra óptica plástica
        translate([0, 0, largo_comp + 1.8])
            cylinder(h = (largo_total - largo_comp) + 1.0, d = d_fibra, center = false);

        // 5. Perforaciones laterales M3 para anclaje a la base de corte láser
        translate([-ancho_base/2 + 3.5, 0, -0.1])
            cylinder(h = espesor_base + 0.2, d = d_tornillo, center = false);
        translate([ ancho_base/2 - 3.5, 0, -0.1])
            cylinder(h = espesor_base + 0.2, d = d_tornillo, center = false);
    }
}

// Instanciar par de acopladores: uno para el Emisor (Etapa 4) y otro para el Receptor (Etapa 5)
acoplador_optico_axial();
translate([0, 20, 0])
    acoplador_optico_axial();
