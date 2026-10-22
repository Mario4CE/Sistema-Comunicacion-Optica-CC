// ============================================================================
// PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113 - ITCR)
// SERVICIO: LAIMI 2 – Impresión 3D (Edificio F4, Aula F4-05, Sede Cartago)
// EQUIPO DESTINO: Impresora 3D Prusa MK4
// ARCHIVO STL OFICIAL LAIMI: 2022437519-02.stl (Carné: 2022437519 - Pieza 02)
//
// Descripción:
//   Genera el modelo 3D paramétrico del par de capuchones cilíndricos compactos
//   (Opción B: Montaje directo sobre protoboard). Permite clavar las patas del
//   LED rojo de 5 mm y de la LDR GL5528 directamente en las filas de la
//   protoboard (a-j, 0-60) y encajar este capuchón negro por encima para
//   sujetar la fibra óptica plástica (POF) de 2.2 mm y bloquear la luz ambiental.
//
// Parámetros de Solicitud en LAIMI F4-05:
//   - Carné: 2022437519
//   - Uso: Curso (EL2113 - Circuitos Eléctricos en Corriente Continua)
//   - Material: PLA (o PETG)
//   - Resolución eje Z: 0.2 mm
//   - Relleno (Infill): 20% a 30%
//   - Color: Preferible negro u oscuro opaco (o pintar con acrílico negro).
// ============================================================================

$fn = 48;

d_ext_cap    = 9.6;  // Diámetro exterior compacto (< 4 filas de protoboard)
d_comp_cap   = 5.25; // Cavidad inferior para domo de LED 5 mm o LDR GL5528
d_reb_cap    = 6.1;  // Alojamiento inferior para el reborde del LED
d_fibra_cap  = 2.2;  // Conducto superior para fibra óptica plástica (POF)
h_total_cap  = 22.0; // Altura total del capuchón directo
h_comp_cap   = 10.0; // Profundidad de encaje sobre el LED/LDR

module capuchon_directo_protoboard() {
    difference() {
        // Cuerpo cilíndrico exterior sin pestañas laterales
        cylinder(h = h_total_cap, d = d_ext_cap, center = false);

        // Reborde inferior del LED (1.8 mm)
        translate([0, 0, -0.1])
            cylinder(h = 1.9, d = d_reb_cap, center = false);

        // Cavidad para el cuerpo del LED / LDR
        translate([0, 0, 1.7])
            cylinder(h = h_comp_cap - 1.6, d = d_comp_cap, center = false);

        // Conducto coaxial superior para la fibra óptica (12 mm de guía)
        translate([0, 0, h_comp_cap])
            cylinder(h = (h_total_cap - h_comp_cap) + 0.2, d = d_fibra_cap, center = false);
    }
}

// Instanciar par de capuchones (TX para LED y RX para LDR)
capuchon_directo_protoboard();
translate([0, 14, 0])
    capuchon_directo_protoboard();
