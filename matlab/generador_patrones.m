%% ========================================================================
%  PROYECTO: Sistema de Comunicación por Medios Ópticos (EL2113)
%  SCRIPT: Generador de Arreglos de Longitud Variable (Periódicos y Aperiódicos)
%  AUTOR: Mario Cerdas y equipo de trabajo
%  CUMPLIMIENTO: Requerimientos C.3 y D.4 del instructivo oficial
% ========================================================================
%  Descripción:
%    Genera la representación digital de 4 bits (valores enteros de 0 a 15)
%    para cualquier señal analógica periódica o aperiódica utilizando
%    arreglos de longitud variable (N muestras configurables). Calcula la
%    relación señal a ruido de cuantización (SQNR), grafica las señales
%    continuas frente a las reconstruidas y exporta automáticamente los
%    arreglos en sintaxis C++ listos para cargarse en el Arduino.
%
%  Entrada:
%    - Parámetros de configuración en la Sección 1:
%      * N_bits (entero): Resolución del DAC (4 bits -> 16 niveles).
%      * V_min, V_max (reales): Rango de tensión de referencia (0.0 V a 5.0 V).
%      * N_periodica (entero): Longitud variable del arreglo periódico.
%      * N_aperiodica (entero): Longitud variable del arreglo aperiódico.
%      * duracion_muestra_ms (entero): Tiempo de retención entre 1 ms y 10000 ms.
%
%  Salida:
%    - Código C++ impreso en consola con arreglos de longitud variable:
%      `PATRON_PERIODICO_MATLAB[]` y `PATRON_APERIODICO_MATLAB[]`.
%    - Ventana gráfica con 3 subgráficas comparativas (Periódica, Aperiódica
%      y Escalera de Calibración).
%
%  Restricciones:
%    - N_bits debe ser exactamente 4 para el sistema de 4 pines digitales.
%    - Las longitudes N_periodica y N_aperiodica deben ser enteros >= 2.
%    - El tiempo por muestra en Arduino debe estar entre 1 ms y 10000 ms (10 s)
%      según el requerimiento C.3.
% ========================================================================
clear; clc; close all;

fprintf('======================================================================\n');
fprintf('  GENERADOR MATLAB DE ARREGLOS DE 4 BITS (PERIÓDICOS Y APERIÓDICOS)   \n');
fprintf('  Cumplimiento estricto de Requerimientos C.3 y D.4 (EL2113)          \n');
fprintf('======================================================================\n\n');

%% 1. PARÁMETROS GENERALES DE CUANTIZACIÓN Y LONGITUD VARIABLE
N_bits = 4;                          % Número de bits del sistema digital
niveles = 2^N_bits;                  % Total de estados posibles: 16 (0 a 15)
V_min = 0.0;                         % Nivel lógico bajo (0 V)
V_max = 5.0;                         % Nivel lógico alto de referencia (5 V)
delta_V = (V_max - V_min) / niveles; % Paso LSB = 5.0 / 16 = 0.3125 V

% Longitudes variables de los arreglos de prueba (Requerimiento C.3 y D.4)
N_periodica  = 20;                   % Longitud variable para señal periódica (ej. 20 muestras)
N_aperiodica = 18;                   % Longitud variable para señal aperiódica (ej. 18 muestras)
duracion_muestra_ms = 1000;          % Tiempo por dato en [1 ms, 10000 ms] (Req. C.3)

% Validación de restricciones del Requerimiento C.3
assert(duracion_muestra_ms >= 1 && duracion_muestra_ms <= 10000, ...
    'Restricción C.3 violada: El tiempo de permanencia debe estar entre 1 ms y 10000 ms (10 s).');

fprintf('Resolución del DAC (Delta V): %.4f V (%.2f mV)\n', delta_V, delta_V * 1000);
fprintf('Voltaje de escala completa (Estado 15): %.4f V\n', (niveles - 1) * delta_V);
fprintf('SQNR Teórico de cuantización (4 bits):  %.2f dB\n\n', 6.02 * N_bits + 1.76);

%% 2. GENERACIÓN DE SEÑAL PERIÓDICA (LONGITUD VARIABLE N_periodica)
% Vector de tiempo normalizado para N_periodica muestras
t_per = linspace(0, 1, N_periodica);

% Señal periódica continua: onda senoidal con armónico secundario dentro de [0, 4.6875 V]
senal_periodica_v = 2.34 + 2.10 * sin(2 * pi * 1.0 * t_per);

% Cuantización a palabras digitales de 4 bits (0 a 15) mediante función local
[arreglo_periodico_4b, periodica_reconstruida_v] = cuantizar_senal_4bits( ...
    senal_periodica_v, V_min, delta_V, niveles);

%% 3. GENERACIÓN DE SEÑAL APERIÓDICA (LONGITUD VARIABLE N_aperiodica)
% Vector de tiempo normalizado para N_aperiodica muestras
t_aper = linspace(0, 1, N_aperiodica);

% Señal aperiódica continua: pulso transitorio exponencialmente amortiguado (no se repite)
senal_aperiodica_v = 0.40 + 4.25 * exp(-3.2 * t_aper) .* abs(sin(2 * pi * 1.5 * t_aper + 0.3));

% Cuantización a palabras digitales de 4 bits (0 a 15) mediante función local
[arreglo_aperiodico_4b, aperiodica_reconstruida_v] = cuantizar_senal_4bits( ...
    senal_aperiodica_v, V_min, delta_V, niveles);

%% 4. GENERACIÓN DE PATRÓN EN ESCALERA COMPLETA (16 ESTADOS: 0 A 15)
arreglo_escalera_4b = 0:(niveles - 1);
escalera_reconstruida_v = arreglo_escalera_4b * delta_V;

%% 5. EXPORTACIÓN AUTOMÁTICA DE ARREGLOS A FORMATO C++ PARA ARDUINO
fprintf('// ===================================================================\n');
fprintf('// ARREGLOS EXPORTADOS DESDE MATLAB PARA ARDUINO (Req. C.3 y D.4)\n');
fprintf('// ===================================================================\n');
fprintf('const unsigned long TIEMPO_MUESTRA_MATLAB_MS = %dUL; // Rango [1, 10000] ms\n\n', duracion_muestra_ms);

% Exportar arreglo de señal periódica
imprimir_arreglo_cpp('PATRON_PERIODICO_MATLAB', arreglo_periodico_4b);

% Exportar arreglo de señal aperiódica
imprimir_arreglo_cpp('PATRON_APERIODICO_MATLAB', arreglo_aperiodico_4b);

%% 6. VISUALIZACIÓN GRÁFICA DE LAS REPRESENTACIONES DIGITALES
figure('Name', 'Generador MATLAB Periódico y Aperiódico - EL2113', ...
       'Color', [1 1 1], 'Position', [100 80 950 750]);

% Subgráfica 1: Señal Periódica Cuantizada
subplot(3, 1, 1);
plot(t_per, senal_periodica_v, 'k--', 'LineWidth', 1.4, 'DisplayName', 'Señal Continua Periódica');
hold on;
stairs(t_per, periodica_reconstruida_v, 'b-', 'LineWidth', 2.0, 'DisplayName', 'Cuantizada 4 bits (DAC)');
plot(t_per, periodica_reconstruida_v, 'ro', 'MarkerFaceColor', 'r', 'DisplayName', 'Muestras Digitales');
grid on;
title(sprintf('1. Señal Periódica en MATLAB (Longitud variable N = %d muestras)', N_periodica));
xlabel('Tiempo Normalizado (t/T)');
ylabel('Voltaje (V)');
legend('Location', 'best');
ylim([0 5.2]);

% Subgráfica 2: Señal Aperiódica Cuantizada
subplot(3, 1, 2);
plot(t_aper, senal_aperiodica_v, 'k--', 'LineWidth', 1.4, 'DisplayName', 'Señal Continua Aperiódica');
hold on;
stairs(t_aper, aperiodica_reconstruida_v, 'm-', 'LineWidth', 2.0, 'DisplayName', 'Cuantizada 4 bits (DAC)');
plot(t_aper, aperiodica_reconstruida_v, 'bo', 'MarkerFaceColor', 'b', 'DisplayName', 'Muestras Digitales');
grid on;
title(sprintf('2. Señal Aperiódica Transitoria en MATLAB (Longitud variable N = %d muestras)', N_aperiodica));
xlabel('Tiempo Normalizado (t)');
ylabel('Voltaje (V)');
legend('Location', 'best');
ylim([0 5.2]);

% Subgráfica 3: Palabras digitales (0 a 15) de la señal aperiódica
subplot(3, 1, 3);
stem(0:(N_aperiodica - 1), arreglo_aperiodico_4b, 'filled', 'MarkerSize', 6, 'Color', [0.1 0.5 0.2], 'LineWidth', 1.5);
grid on;
title('3. Palabras Digitales de 4 Bits Enviadas a los 4 Pines del Arduino (0 a 15)');
xlabel('Índice de Muestra (k)');
ylabel('Código Decimal (4 bits)');
ylim([0 16]);

fprintf('Script finalizado exitosamente. Gráficas y arreglos C++ generados.\n');

%% ========================================================================
%  FUNCIONES AUXILIARES CON ESTÁNDAR DE DOCUMENTACIÓN
% ========================================================================

% -------------------------------------------------------------------------
% Función: cuantizar_senal_4bits
% -------------------------------------------------------------------------
% Descripción:
%   Convierte un vector de voltajes analógicos continuos en su representación
%   digital cuantizada de N bits (valores enteros entre 0 y niveles-1) y
%   calcula la tensión escalonada ideal reconstruida a la salida del DAC.
%
% Entrada:
%   - senal_v (vector double): Muestras de tensión de la señal analógica (V).
%   - v_min (double): Voltaje mínimo de referencia del cuantizador (0.0 V).
%   - delta_v (double): Resolución de un bit menos significativo (LSB en V).
%   - niveles (int): Cantidad total de estados de cuantización (16 para 4 bits).
%
% Salida:
%   - codigo_digital (vector double): Enteros cuantizados en el rango [0, 15].
%   - senal_reconstruida (vector double): Voltaje analógico equivalente del DAC.
%
% Restricciones:
%   - delta_v debe ser estrictamente positivo (> 0).
%   - Cualquier valor que exceda el rango dinámico se satura a [0, niveles-1].
% -------------------------------------------------------------------------
function [codigo_digital, senal_reconstruida] = cuantizar_senal_4bits(senal_v, v_min, delta_v, niveles)
    % Dividir la excursión de tensión entre el paso LSB y redondear al entero más cercano
    codigo_sin_saturar = round((senal_v - v_min) / delta_v);

    % Aplicar saturación (clipping) al intervalo válido de 4 bits [0, 15]
    codigo_digital = max(0, min(niveles - 1, codigo_sin_saturar));

    % Calcular el voltaje analógico correspondiente a cada palabra digital
    senal_reconstruida = v_min + codigo_digital * delta_v;
end

% -------------------------------------------------------------------------
% Función: imprimir_arreglo_cpp
% -------------------------------------------------------------------------
% Descripción:
%   Formatea e imprime en la ventana de comandos de MATLAB un vector de datos
%   de 4 bits como un arreglo constante de C++ (`const uint8_t`) junto con
%   el cálculo automático de su longitud para Arduino.
%
% Entrada:
%   - nombre_var (char/string): Identificador de la constante en C++.
%   - datos (vector numérico): Arreglo de valores enteros entre 0 y 15.
%
% Salida:
%   Ninguna (imprime directamente el bloque de código C++ en consola).
%
% Restricciones:
%   - El vector 'datos' no debe estar vacío y sus elementos deben estar en [0, 15].
% -------------------------------------------------------------------------
function imprimir_arreglo_cpp(nombre_var, datos)
    % Obtener el número total de elementos del arreglo de longitud variable
    n_elem = length(datos);

    % Imprimir declaración del arreglo C++ y su longitud automática
    fprintf('const uint8_t %s[%d] = {\n  ', nombre_var, n_elem);
    for idx = 1:n_elem
        % Imprimir valor entero de 4 bits alineado a 2 caracteres
        fprintf('%2d', datos(idx));
        if idx < n_elem
            fprintf(', ');
        end
        % Insertar salto de línea cada 10 muestras para legibilidad
        if mod(idx, 10) == 0 && idx < n_elem
            fprintf('\n  ');
        end
    end
    fprintf('\n};\n');
    fprintf('const size_t LONGITUD_%s = sizeof(%s) / sizeof(%s[0]);\n\n', ...
        nombre_var, nombre_var, nombre_var);
end
