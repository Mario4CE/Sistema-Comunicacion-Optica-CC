%% ========================================================================
% DESCRIPCION:
% Este script digitaliza cualquier senal analogica a 4 bits (convirtiendola 
% en numeros del 0 al 15) usando la cantidad de muestras que se le indique. 
% Ademas de graficar la senal original contra la reconstruida para ver las 
% diferencias y calcular su margen de error (SQNR), el programa hace el 
% trabajo pesado: exporta los datos ya formateados en sintaxis de C++ para 
% que solo tengas que pegarlos en tu codigo de Arduino.
%
% Entrada:
%   - Parametros de configuracion en la Seccion 1:
%     * N_bits (entero): Resolucion del DAC (4 bits -> 16 niveles).
%     * V_min, V_max (reales): Rango de tension (0.0 V a 5.0 V).
%     * N_periodica (entero): Cantidad de datos del arreglo periodico.
%     * N_aperiodica (entero): Cantidad de datos del arreglo aperiodico.
%     * duracion_muestra_ms (entero): Tiempo entre datos (1 ms a 10000 ms).
%
% Salida:
%   - Codigo C++ en consola listo para copiar:
%     PATRON_PERIODICO_MATLAB[] y PATRON_APERIODICO_MATLAB[].
%   - Graficas comparativas (Periodica, Aperiodica y Escalera).
% ========================================================================

clear; clc; close all;

fprintf('======================================================================\n');
fprintf('  GENERADOR MATLAB DE ARREGLOS DE 4 BITS (PERIODICOS Y APERIODICOS)   \n');
fprintf('  Sistema de Comunicacion por Medios Opticos (EL2113)                 \n');
fprintf('======================================================================\n\n');

%% 1. PARAMETROS GENERALES
N_bits = 4;                          % Numero de bits del sistema
niveles = 2^N_bits;                  % Total de estados: 16 (0 a 15)
V_min = 0.0;                         % Nivel bajo (0 V)
V_max = 5.0;                         % Nivel alto (5 V)
delta_V = (V_max - V_min) / niveles; % Paso LSB = 5.0 / 16 = 0.3125 V

% Longitudes de los arreglos
N_periodica  = 20;                   % Cantidad de muestras para senal periodica
N_aperiodica = 18;                   % Cantidad de muestras para senal aperiodica
duracion_muestra_ms = 1000;          % Tiempo por dato en milisegundos

% Validacion basica del tiempo
assert(duracion_muestra_ms >= 1 && duracion_muestra_ms <= 10000, ...
    'Error: El tiempo de permanencia debe estar entre 1 ms y 10000 ms.');

fprintf('Resolucion del DAC (Delta V): %.4f V (%.2f mV)\n', delta_V, delta_V * 1000);
fprintf('Voltaje maximo (Estado 15): %.4f V\n', (niveles - 1) * delta_V);
fprintf('SQNR Teorico (4 bits):  %.2f dB\n\n', 6.02 * N_bits + 1.76);

%% 2. GENERACION DE SENAL PERIODICA
% Vector de tiempo para N_periodica muestras
t_per = linspace(0, 1, N_periodica);

% Senal periodica (onda senoidal)
senal_periodica_v = 2.34 + 2.10 * sin(2 * pi * 1.0 * t_per);

% Cuantizacion a datos de 4 bits (0 a 15)
[arreglo_periodico_4b, periodica_reconstruida_v] = cuantizar_senal_4bits( ...
    senal_periodica_v, V_min, delta_V, niveles);

%% 3. GENERACION DE SENAL APERIODICA
% Vector de tiempo para N_aperiodica muestras
t_aper = linspace(0, 1, N_aperiodica);

% Senal aperiodica (pulso que se apaga poco a poco)
senal_aperiodica_v = 0.40 + 4.25 * exp(-3.2 * t_aper) .* abs(sin(2 * pi * 1.5 * t_aper + 0.3));

% Cuantizacion a datos de 4 bits (0 a 15)
[arreglo_aperiodico_4b, aperiodica_reconstruida_v] = cuantizar_senal_4bits( ...
    senal_aperiodica_v, V_min, delta_V, niveles);

%% 4. PATRON EN ESCALERA
arreglo_escalera_4b = 0:(niveles - 1);
escalera_reconstruida_v = arreglo_escalera_4b * delta_V;

%% 5. EXPORTACION A C++ PARA ARDUINO
fprintf('// ===================================================================\n');
fprintf('// ARREGLOS EXPORTADOS DESDE MATLAB PARA ARDUINO\n');
fprintf('// ===================================================================\n');
fprintf('const unsigned long TIEMPO_MUESTRA_MATLAB_MS = %d; // Rango [1, 10000] ms\n\n', duracion_muestra_ms);

% Imprime los arreglos ya en sintaxis de C++
imprimir_arreglo_cpp('PATRON_PERIODICO_MATLAB', arreglo_periodico_4b);
imprimir_arreglo_cpp('PATRON_APERIODICO_MATLAB', arreglo_aperiodico_4b);

%% 6. GRAFICAS
figure('Name', 'Generador MATLAB - EL2113', 'Color', [1 1 1], 'Position', [100 80 950 750]);

% Subgrafica 1: Senal Periodica
subplot(3, 1, 1);
plot(t_per, senal_periodica_v, 'k--', 'LineWidth', 1.4, 'DisplayName', 'Senal Continua');
hold on;
stairs(t_per, periodica_reconstruida_v, 'b-', 'LineWidth', 2.0, 'DisplayName', 'Cuantizada (4 bits)');
plot(t_per, periodica_reconstruida_v, 'ro', 'MarkerFaceColor', 'r', 'DisplayName', 'Muestras');
grid on;
title(sprintf('1. Senal Periodica (N = %d muestras)', N_periodica));
xlabel('Tiempo Normalizado');
ylabel('Voltaje (V)');
legend('Location', 'best');
ylim([0 5.2]);

% Subgrafica 2: Senal Aperiodica
subplot(3, 1, 2);
plot(t_aper, senal_aperiodica_v, 'k--', 'LineWidth', 1.4, 'DisplayName', 'Senal Continua');
hold on;
stairs(t_aper, aperiodica_reconstruida_v, 'm-', 'LineWidth', 2.0, 'DisplayName', 'Cuantizada (4 bits)');
plot(t_aper, aperiodica_reconstruida_v, 'bo', 'MarkerFaceColor', 'b', 'DisplayName', 'Muestras');
grid on;
title(sprintf('2. Senal Aperiodica Transitoria (N = %d muestras)', N_aperiodica));
xlabel('Tiempo Normalizado');
ylabel('Voltaje (V)');
legend('Location', 'best');
ylim([0 5.2]);

% Subgrafica 3: Valores digitales puros
subplot(3, 1, 3);
stem(0:(N_aperiodica - 1), arreglo_aperiodico_4b, 'filled', 'MarkerSize', 6, 'Color', [0.1 0.5 0.2], 'LineWidth', 1.5);
grid on;
title('3. Valores enviados a los pines de Arduino (0 a 15)');
xlabel('Indice de Muestra (k)');
ylabel('Codigo Decimal');
ylim([0 16]);

fprintf('Script finalizado. Graficas y arreglos C++ generados.\n');

%% ========================================================================
%  FUNCIONES AUXILIARES
% ========================================================================

% -------------------------------------------------------------------------
% Funcion: cuantizar_senal_4bits
% -------------------------------------------------------------------------
% Descripcion:
%   Convierte voltajes a digital (0 a niveles-1) y calcula la
%   tension teorica de salida del DAC.
function [codigo_digital, senal_reconstruida] = cuantizar_senal_4bits(senal_v, v_min, delta_v, niveles)
    codigo_sin_saturar = round((senal_v - v_min) / delta_v);
    codigo_digital = max(0, min(niveles - 1, codigo_sin_saturar));
    senal_reconstruida = v_min + codigo_digital * delta_v;
end

% -------------------------------------------------------------------------
% Funcion: imprimir_arreglo_cpp
% -------------------------------------------------------------------------
% Descripcion:
%   Imprime en la consola el arreglo de C++ listo para copiar a Arduino.
function imprimir_arreglo_cpp(nombre_var, datos)
    n_elem = length(datos);

    fprintf('const uint8_t %s[%d] = {\n  ', nombre_var, n_elem);
    for idx = 1:n_elem
        fprintf('%2d', datos(idx));
        if idx < n_elem
            fprintf(', ');
        end
        if mod(idx, 10) == 0 && idx < n_elem
            fprintf('\n  ');
        end
    end
    fprintf('\n};\n');
    fprintf('const size_t LONGITUD_%s = sizeof(%s) / sizeof(%s[0]);\n\n', ...
        nombre_var, nombre_var, nombre_var);
end