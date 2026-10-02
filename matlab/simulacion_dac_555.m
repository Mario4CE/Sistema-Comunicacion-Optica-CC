%% ========================================================================
%  PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
%  SCRIPT: Simulación y Modelado Integral (DAC R-2R + Óptica + NE555)
%  AUTOR: Mario Cerdas y equipo de trabajo
% ========================================================================
clear; clc; close all;

fprintf('===============================================================\n');
fprintf('  SIMULACIÓN DEL SISTEMA DE COMUNICACIÓN POR MEDIOS ÓPTICOS   \n');
fprintf('===============================================================\n\n');

%% 1. PARÁMETROS DEL SISTEMA
% --- Etapa 1 & 2: DAC R-2R ---
V_ref = 5.0;            % Voltaje de referencia de salida de Arduino (V)
N_bits = 4;             % Número de bits del sistema
num_niveles = 2^N_bits; % 16 niveles (0 a 15)
R_dac = 10e3;           % Resistencia base R = 10 kOhm
R_2dac = 20e3;          % Resistencia 2R = 20 kOhm

% --- Etapa 4: Transmisor Óptico (Driver LED) ---
V_led_drop = 1.8;       % Caída típica de LED rojo (V)
R_driver = 150;         % Resistencia limitadora del LED (Ohms)

% --- Etapa 5: Receptor Óptico (LDR) ---
% Modelo aproximado de LDR según intensidad luminosa recibida
% R_LDR oscila típicamente entre ~50 kOhm en oscuridad total hasta ~1 kOhm a máxima luz
R_oscuridad = 50e3;     % Resistencia en ausencia de luz (Ohms)
R_luz_max = 1.2e3;      % Resistencia a máxima iluminación por fibra (Ohms)

% --- Etapa 6: Generador de Onda Cuadrada (NE555 Astable) ---
% Ecuación de frecuencia astable: f = 1.44 / ((R1 + 2*R_LDR) * C)
R1_555 = 1.0e3;         % Resistencia mínima de carga pin 7 (1 kOhm)
C_555 = 100e-9;         % Condensador de temporización (100 nF)

%% 2. CÁLCULO DE NIVELES TEÓRICOS
datos_decimales = (0:num_niveles-1)';
bits_matriz = de2bi(datos_decimales, N_bits, 'right-msb'); % [b3 b2 b1 b0]

% Voltaje de salida de la red R-2R
% V_out = V_ref * sum(b_i * 2^(i - N)) = V_ref * (D / 2^N)
voltajes_dac = V_ref * (datos_decimales / num_niveles);

% Corriente en el LED (zona activa cuando V_dac > V_led_drop)
corriente_led_mA = max(0, (voltajes_dac - V_led_drop) / R_driver) * 1000;

% Resistencia estimada de la LDR según la corriente lumínica recibida
% Se modela un comportamiento inverso característico de fotorresistencias
factor_atenuacion = corriente_led_mA / max(corriente_led_mA + 1e-6);
R_ldr_estimada = R_oscuridad ./ (1 + 40 * factor_atenuacion);
R_ldr_estimada = max(R_ldr_estimada, R_luz_max); % Límite de saturación física

% Frecuencia de oscilación del temporizador 555 astable (Hz)
frecuencia_555_Hz = 1.44 ./ ((R1_555 + 2 * R_ldr_estimada) * C_555);

% Ciclo de trabajo (Duty Cycle) del 555 estándar:
% DC = (R1 + R_LDR) / (R1 + 2*R_LDR) * 100 %
duty_cycle_pct = ((R1_555 + R_ldr_estimada) ./ (R1_555 + 2 * R_ldr_estimada)) * 100;

%% 3. TABLA CONSOLA DE RESULTADOS
fprintf('Dec | Binario [b3 b2 b1 b0] | V_DAC (V) | I_LED (mA) | R_LDR (kOhm) | Frecuencia (Hz) | Duty Cycle (%%)\n');
fprintf('--------------------------------------------------------------------------------------------------\n');
for i = 1:num_niveles
    fprintf(' %2d |       %d  %d  %d  %d       |  %6.4f   |   %5.2f    |    %6.2f    |     %7.1f     |     %5.1f\n', ...
        datos_decimales(i), ...
        bits_matriz(i,1), bits_matriz(i,2), bits_matriz(i,3), bits_matriz(i,4), ...
        voltajes_dac(i), corriente_led_mA(i), R_ldr_estimada(i)/1e3, ...
        frecuencia_555_Hz(i), duty_cycle_pct(i));
end
fprintf('--------------------------------------------------------------------------------------------------\n\n');

%% 4. GENERACIÓN DE GRÁFICAS DE VALIDACIÓN
figure('Name', 'Sistema de Comunicación Óptica - EL2113', 'Color', [1 1 1], 'Position', [100 100 1000 700]);

% Gráfica 1: Escalera DAC de 4 bits
subplot(2, 2, 1);
stairs(datos_decimales, voltajes_dac, 'b', 'LineWidth', 1.8);
hold on;
plot(datos_decimales, voltajes_dac, 'ro', 'MarkerFaceColor', 'r');
grid on;
title('1. Respuesta del DAC R-2R (4 bits)');
xlabel('Valor Decimal (Entrada)');
ylabel('Voltaje de Salida V_{DAC} (V)');
ylim([0 5.2]);
xlim([0 15]);

% Gráfica 2: Corriente del LED emisor
subplot(2, 2, 2);
plot(datos_decimales, corriente_led_mA, 'r-s', 'LineWidth', 1.5, 'MarkerFaceColor', [1 0.3 0.3]);
grid on;
title('2. Corriente en LED Emisor');
xlabel('Valor Decimal');
ylabel('Corriente I_{LED} (mA)');
xlim([0 15]);

% Gráfica 3: Resistencia del receptor LDR
subplot(2, 2, 3);
plot(datos_decimales, R_ldr_estimada / 1e3, 'm-^', 'LineWidth', 1.5, 'MarkerFaceColor', 'm');
grid on;
title('3. Resistencia de la Fotorresistencia (LDR)');
xlabel('Valor Decimal');
ylabel('Resistencia R_{LDR} (k\Omega)');
xlim([0 15]);

% Gráfica 4: Frecuencia de salida hacia el Buzzer
subplot(2, 2, 4);
plot(datos_decimales, frecuencia_555_Hz, 'g-d', 'LineWidth', 1.8, 'MarkerFaceColor', [0.2 0.7 0.2]);
grid on;
title('4. Frecuencia del Buzzer (Astable 555)');
xlabel('Valor Decimal');
ylabel('Frecuencia f (Hz)');
xlim([0 15]);

fprintf('Simulación completada con éxito. Gráficas generadas.\n');
