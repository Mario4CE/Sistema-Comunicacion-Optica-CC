// ============================================================================
// PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113 - ITCR)
// ARCHIVO STL OFICIAL UNIFICADO LAIMI: 2022437519-01.stl
//
// CONTENIDO DEL KIT EN ESTE ARCHIVO ÚNICO (8 piezas en cama de 54.9 x 43.6 mm):
//   1) 1x Torre Horizontal Atornillada M3 — Emisor TX (LED Rojo 5 mm)
//   2) 1x Torre Horizontal Atornillada M3 — Receptor RX (Fotorreceptor LDR GL5528)
//   3) 3x Grapas/Clips de Presión Transversales (encajan en las ranuras de 7x3 mm
//         ubicadas frente a las torres TX y RX para sujetar la fibra a Z=6.0 mm sin gazas)
//   4) 3x Grapas/Clips de Presión Longitudinales (encajan en las 3 ranuras de 7x3 mm
//         del arco exterior de la base láser para sujetar el cable de fibra sin gazas)
//
// Parámetros de Solicitud en LAIMI F4-05 (Prusa MK4):
//   - Carné: 2022437519
//   - Uso: Curso (EL2113 - Circuitos Eléctricos en Corriente Continua)
//   - Material: PLA (o PETG)
//   - Resolución eje Z: 0.2 mm
//   - Relleno (Infill): 20% a 30%
//   - Soportes: NO requiere (todas las piezas imprimen planas en Z=0)
// ============================================================================

$fn = 64;

// --- 1. Parámetros de las Torres Acopladoras Horizontales M3 ---
ancho_base     = 26.0; // Ancho total eje X (-13.0 a +13.0 mm, calza en silueta de 26 mm)
largo_axial    = 18.0; // Largo axial eje Y (-9.0 a +9.0 mm)
espesor_base   = 3.0;  // Espesor de las alas planas para tornillos M3 x 10mm/12mm
dist_tornillos = 19.0; // Separación exacta entre centros M3 en la base láser (-9.5 y +9.5 mm)
ancho_ojal_m3  = 3.8;  // Ancho holgado para paso libre de tornillo M3
ancho_cuerpo   = 11.0; // Ancho central (-5.5 a +5.5 mm); deja 4.0 mm libres al centro del tornillo
alto_cuerpo    = 12.0; // Altura total eje Z (0.0 a 12.0 mm)
z_eje_optico   = 6.0;  // Altura del eje óptico sobre la base de madera

d_reborde_in   = 7.2;  // Entrada amplia (Ø 7.2 mm, prof. 3.5 mm) para reborde de LED y LDR
prof_reborde   = 3.5;
prof_cuerpo    = 6.5;  // Profundidad total componente = 10.0 mm (> 8.6 mm del LED de 5 mm)
d_collar_fibra = 2.7;  // Boquilla interna de retención suave para cable de fibra (~2.2 mm)
d_fibra_pof    = 3.2;  // Conducto guía para cable de fibra óptica con forro negro (~2.2 mm)
d_embudo_fibra = 4.8;  // Cono guía exterior (1.5 mm) para inserción suave de la fibra

module ojal_m3(x_centro) {
    hull() {
        translate([x_centro - 0.5, 0, -0.1])
            cylinder(h = espesor_base + 0.2, d = ancho_ojal_m3);
        translate([x_centro + 0.5, 0, -0.1])
            cylinder(h = espesor_base + 0.2, d = ancho_ojal_m3);
    }
}

module tunel_teardrop(d_nom, longitud) {
    rotate([-90, 0, 0])
    union() {
        cylinder(h = longitud, d = d_nom);
        translate([0, -d_nom * 0.18, 0])
            rotate([0, 0, 45])
                cube([d_nom * 0.38, d_nom * 0.38, longitud]);
    }
}

module acoplador_horizontal_atornillado_m3(d_cuerpo_comp = 6.2) {
    difference() {
        union() {
            linear_extrude(height = espesor_base)
                polygon(points = [
                    [-11.5, -9.0], [ 11.5, -9.0], [ 13.0, -7.5],
                    [ 13.0,  7.5], [ 11.5,  9.0], [-11.5,  9.0],
                    [-13.0,  7.5], [-13.0, -7.5]
                ]);

            rotate([90, 0, 0])
                translate([0, 0, -largo_axial/2])
                linear_extrude(height = largo_axial)
                    polygon(points = [
                        [-5.5, 0.0], [ 5.5, 0.0], [ 5.5, 10.5],
                        [ 4.0, 12.0], [-4.0, 12.0], [-5.5, 10.5]
                    ]);
        }

        // 1. Entrada para reborde de LED / boca de LDR (Y = -9.0 a -5.5 mm, Ø 7.2 mm)
        translate([0, -largo_axial/2 - 0.1, z_eje_optico])
            tunel_teardrop(d_reborde_in, prof_reborde + 0.15);

        // 2. Cámara profunda para LED (Ø 6.2 mm) o LDR (Ø 6.4 mm) (prof. total = 10.0 mm)
        translate([0, -largo_axial/2 + prof_reborde, z_eje_optico])
            tunel_teardrop(d_cuerpo_comp, prof_cuerpo + 0.05);

        // 3. Boquilla interna cónica de retención suave para fibra (Y = +1.0 a +3.0 mm)
        translate([0, 1.0, z_eje_optico])
            rotate([-90, 0, 0])
                cylinder(h = 2.05, d1 = d_collar_fibra, d2 = d_fibra_pof);

        // 4. Conducto coaxial para cable de fibra óptica (Y = +3.0 a +7.5 mm, Ø 3.2 mm)
        translate([0, 3.0, z_eje_optico])
            tunel_teardrop(d_fibra_pof, 4.6);

        // 5. Embudo cónico de entrada de fibra (Y = +7.5 a +9.0 mm, Ø 3.2 -> 4.8 mm)
        translate([0, largo_axial/2 - 1.5, z_eje_optico])
            rotate([-90, 0, 0])
                cylinder(h = 1.6, d1 = d_fibra_pof, d2 = d_embudo_fibra);

        // 6. Ojales pasantes M3 separados exactamente 19.0 mm entre centros (-9.5 y +9.5 mm)
        ojal_m3(-dist_tornillos / 2);
        ojal_m3( dist_tornillos / 2);
    }
}

// --- 2. Módulo de Grapa/Clip Sujetador de Fibra a Presión (Reemplaza las gazas) ---
module clip_sujetador_fibra(ancho_espiga, ancho_hombro, y_fibra, hendidura, espesor_z) {
    linear_extrude(height = espesor_z)
    difference() {
        union() {
            // Hombro plano sobre la base MDF
            translate([-ancho_hombro/2, 0])
                square([ancho_hombro, max(1.8, y_fibra - 2.6)]);
            // Cuerpo exterior de la pinza elástica en C
            translate([-2.95, max(1.8, y_fibra - 2.6)])
                square([5.9, (y_fibra + 2.55) - max(1.8, y_fibra - 2.6)]);
            // Espiga inferior con retención para ranura de 7x3 mm en MDF de 3 mm
            polygon(points = [
                [-ancho_espiga/2, 0.0],
                [-ancho_espiga/2 - 0.12, -1.5],
                [-ancho_espiga/2 + 0.22, -3.1],
                [ ancho_espiga/2 - 0.22, -3.1],
                [ ancho_espiga/2 + 0.12, -1.5],
                [ ancho_espiga/2, 0.0]
            ]);
        }
        // Asiento circular Ø 2.36 mm para el cable de fibra óptica negro (~2.2 mm)
        translate([0, y_fibra])
            circle(d = 2.36);
        // Cuello de retención a presión (1.72 mm) y boca superior en V (3.3 mm)
        polygon(points = [
            [-0.86, y_fibra + 0.8],
            [ 0.86, y_fibra + 0.8],
            [ 1.65, y_fibra + 2.65],
            [-1.65, y_fibra + 2.65]
        ]);
        // Hendidura elástica central en la espiga inferior
        translate([-hendidura/2, -3.2])
            square([hendidura, 2.8]);
    }
}

// --- Instanciación del Kit Unificado en un solo archivo ---
// 1. Torre TX (LED 5 mm, cámara Ø 6.2 mm) y Torre RX (LDR, cámara Ø 6.4 mm)
translate([-15.0, -11.0, 0])
    acoplador_horizontal_atornillado_m3(d_cuerpo_comp = 6.2);
translate([-15.0,  11.0, 0])
    acoplador_horizontal_atornillado_m3(d_cuerpo_comp = 6.4);

// 2. 3x Clips Transversales (para las ranuras de 7x3 mm frente a las torres TX y RX)
for (cy = [-15.0, 0.0, 15.0]) {
    translate([10.0, cy, 0])
        clip_sujetador_fibra(6.85, 10.4, 6.0, 3.2, 2.75);
}

// 3. 3x Clips Longitudinales (para las 3 ranuras de 7x3 mm del arco de fibra)
for (cy = [-15.0, 0.0, 15.0]) {
    translate([23.0, cy, 0])
        clip_sujetador_fibra(2.90, 7.8, 4.6, 1.0, 5.60);
}
