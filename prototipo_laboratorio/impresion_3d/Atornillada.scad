
// Parámetros de Solicitud 
//   - Material: PLA (o PETG)
//   - Resolución eje Z: 0.2 mm (óptimo balance tiempo/calidad en Prusa MK4)
//   - Relleno (Infill): 20% a 30%
//   - Color: Preferible color negro (u oscuro opaco) para bloqueo lumínico;
//            si no hay disponible, imprimir en otro color y pintar con acrílico.
// ============================================================================

$fn = 64; // Resolución circular de alta calidad (64 facetas por cilindro)

// Parámetros dimensionales en milímetros (mm) - Escala 1:1 verificada para STL
d_exterior   = 12.0; // Diámetro exterior del barril opaco
d_componente = 5.2;  // Cavidad para LED de 5 mm o LDR GL5528 (con holgura 0.2 mm)
d_reborde    = 6.2;  // Alojamiento para el reborde inferior del LED de 5 mm
d_fibra      = 2.2;  // Conducto axial superior para fibra óptica plástica (POF)
largo_total  = 28.0; // Longitud axial total de la torre acopladora
largo_comp   = 12.0; // Profundidad de inserción del LED o LDR desde la base
ancho_base   = 26.0; // Ancho de la pestaña de anclaje a la base láser (centro a centro M3 = 19 mm)
espesor_base = 3.0;  // Espesor de la pestaña inferior para tornillos M3 x 10mm/12mm
d_tornillo   = 3.2;  // Perforaciones pasantes para tornillos métricos M3

module acoplador_optico_axial() {
    difference() {
        // 1. Sólido positivo: Barril cilíndrico + Pestaña plana de fijación M3
        union() {
            cylinder(h = largo_total, d = d_exterior, center = false);
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

        // 5. Perforaciones laterales M3 (separadas 19.0 mm entre centros: -9.5 y +9.5)
        translate([-ancho_base/2 + 3.5, 0, -0.1])
            cylinder(h = espesor_base + 0.2, d = d_tornillo, center = false);
        translate([ ancho_base/2 - 3.5, 0, -0.1])
            cylinder(h = espesor_base + 0.2, d = d_tornillo, center = false);
    }
}

// Instanciar par de torres acopladoras: Emisor TX (Etapa 4) y Receptor RX (Etapa 5)
acoplador_optico_axial();
translate([0, 20, 0])
    acoplador_optico_axial();
