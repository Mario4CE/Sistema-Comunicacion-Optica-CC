%% ========================================================================
%  PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
%  SCRIPT: Generador de Patrones de Prueba y Cuantización de 4 Bits
%  AUTOR: Mario Cerdas y equipo de trabajo
% ========================================================================
%  DESCRIPCIÓN:
%  Este script genera diferentes tipos de señales de prueba, las cuantiza
%  a una resolución de 4 bits (16 niveles entre 0 y 5V) y genera arreglos
%  en formato C++ listos para ser copiados al código de Arduino.
% ========================================================================
clear; clc; close all;

fprintf('===============================================================\n');
fprintf('  GENERADOR DE PATRONES DE PRUEBA Y CUANTIZACIÓN (MATLAB)      \n');
fprintf('===============================================================\n\n');

%% 1. PARÁMETROS DE CUANTIZACIÓN
N_bits = 4;
niveles = 2^N_bits;        % 16 niveles
V_min = 0.0;               % Voltaje mínimo (V)
V_max = 5.0;               % Voltaje máximo (V)
delta_V = (V_max - V_min) / niveles; % Resolución = 0.3125 V

fprintf('Resolución del DAC (Delta V): %.4f V (%.2f mV)\n', delta_V, delta_V*1000);
fprintf('Relación Señal a Ruido de Cuantización Teórica (SQNR): %.2f dB\n\n', 6.02 * N_bits + 1.76);

%% 2. GENERACIÓN DE SEÑALES DE PRUEBA
num_muestras = 32;
t = linspace(0, 1, num_muestras);

% A. Señal Senoidal analógica normalizada (0V a 5V)
f_senal = 1; % 1 ciclo
senoidal_continua = 2.5 + 2.3 * sin(2 * pi * f_senal * t);

% Cuantización a 4 bits (enteros de 0 a 15)
senoidal_digital_4bits = round((senoidal_continua - V_min) / delta_V);
senoidal_digital_4bits = max(0, min(niveles - 1, senoidal_digital_4bits)); % Saturar 0-15
senoidal_reconstruida = senoidal_digital_4bits * delta_V;

% B. Escalera ascendente clásica (0 a 15 y retorno)
escalera_digital = [0:15, 14:-1:0];
escalera_voltaje = escalera_digital * delta_V;

%% 3. EXPORTACIÓN AUTOMÁTICA DE VECTORES A FORMATO C++ PARA ARDUINO
fprintf('// --- VECTOR DE PATRÓN SENOIDAL PARA ARDUINO (32 muestras) ---\n');
fprintf('const uint8_t PATRON_SENOIDAL[32] = {\n  ');
for i = 1:num_muestras
    fprintf('%2d', senoidal_digital_4bits(i));
    if i < num_muestras
        fprintf(', ');
    end
    if mod(i, 8) == 0 && i < num_muestras
        fprintf('\n  ');
    end
end
fprintf('\n};\n\n');

%% 4. VISUALIZACIÓN GRÁFICA
figure('Name', 'Generador de Patrones MATLAB - EL2113', 'Color', [1 1 1], 'Position', [150 150 900 600]);

% Gráfica 1: Señal continua vs Cuantizada
subplot(2, 1, 1);
plot(t, senoidal_continua, 'k--', 'LineWidth', 1.5, 'DisplayName', 'Señal Analógica Original');
hold on;
stairs(t, senoidal_reconstruida, 'b', 'LineWidth', 2.0, 'DisplayName', 'Señal Cuantizada (4 bits)');
plot(t, senoidal_reconstruida, 'ro', 'MarkerFaceColor', 'r', 'DisplayName', 'Puntos de Muestreo');
grid on;
title('Cuantización de Señal Senoidal en 4 Bits (16 Niveles)');
xlabel('Tiempo Normalizado');
ylabel('Voltaje (V)');
legend('Location', 'best');
ylim([0 5.2]);

% Gráfica 2: Escalera simétrica
subplot(2, 1, 2);
stem(0:length(escalera_digital)-1, escalera_digital, 'filled', 'MarkerSize', 6, 'Color', [0 0.5 0]);
grid on;
title('Patrón de Prueba: Rampa Ascendente y Descendente');
xlabel('Índice de Muestra');
ylabel('Código Digital (0 a 15)');
ylim([0 16]);

fprintf('Script ejecutado. Arreglo C++ generado y gráficas creadas exitosamente.\n');
