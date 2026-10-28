%
% DESCRIPCION:
% Este script es un simulador completo de nuestro sistema de comunicacion 
% optica. Lo que hace es calcular matematicamente que deberia pasar en 
% cada etapa del circuito a medida que el Arduino envia los valores del 0 al 15.
%
% El programa calcula y grafica paso a paso:
% 1. El voltaje que sale del convertidor (DAC R-2R).
% 2. La corriente que pasa por el LED transmisor al encenderse.
% 3. La resistencia que asume la fotoresistencia (LDR) al recibir esa luz.
% 4. La frecuencia (el tono) que generara el temporizador 555 en el buzzer.
%
% Restricciones y notas:
%   - Ocupas tener instalado el toolbox "Communications" de MATLAB para  
%     poder usar la funcion 'de2bi'.
%   - El codigo asume que hay un amplificador operacional (LM358) trabajando  
%     como seguidor de voltaje entre el DAC y el LED para que el voltaje no
%     se caiga al conectar la carga.

clear; clc; close all;

fprintf('  SIMULACION DEL SISTEMA DE COMUNICACION POR MEDIOS OPTICOS   \n');

%% 1. PARAMETROS DEL SISTEMA
% --- Etapa 1 y 2: DAC R-2R ---
V_ref = 5.0;            % Voltaje maximo de salida del Arduino (V)
N_bits = 4;             % Numero de bits
num_niveles = 2^N_bits; % 16 niveles de escalera (0 a 15)
R_dac = 10e3;           % Resistencia base R (10 kOhm)
R_2dac = 20e3;          % Resistencia 2R (20 kOhm)

% --- Etapa 4: Transmisor Optico (Driver LED) ---
V_led_drop = 1.8;       % Voltaje que consume el LED rojo (V)
R_driver = 150;         % Resistencia para proteger el LED (Ohms)

% --- Etapa 5: Receptor Optico (LDR) ---
% Modelo de la fotoresistencia (LDR) segun la luz que recibe.
% Oscila entre ~50 kOhm a oscuras y ~1.2 kOhm con luz maxima.
R_oscuridad = 50e3;     % Resistencia sin luz (Ohms)
R_luz_max = 1.2e3;      % Resistencia con mucha luz (Ohms)

% --- Etapa 6: Generador de Tono (NE555 en modo Astable) ---
% Formula del NE555: Frecuencia = 1.44 / ((R1 + 2*R_LDR) * C)
R1_555 = 1.0e3;         % Resistencia fija del pin 7 (1 kOhm)
C_555 = 100e-9;         % Condensador (100 nF)

%% 2. CALCULO DE NIVELES TEORICOS
datos_decimales = (0:num_niveles-1)';
bits_matriz = de2bi(datos_decimales, N_bits, 'left-msb'); % Binario estandar: [b3 b2 b1 b0] con MSB a la izquierda

% 1. Voltaje teorico a la salida de la escalera R-2R
voltajes_dac = V_ref * (datos_decimales / num_niveles);

% 2. Corriente en el LED (solo hay corriente si el voltaje supera la caida del LED)
corriente_led_mA = max(0, (voltajes_dac - V_led_drop) / R_driver) * 1000;

% 3. Resistencia de la LDR estimada segun cuanta corriente tiene el LED
factor_atenuacion = corriente_led_mA / max(corriente_led_mA + 1e-6);
R_ldr_estimada = R_oscuridad ./ (1 + 40 * factor_atenuacion);
R_ldr_estimada = max(R_ldr_estimada, R_luz_max); % Pone el limite minimo fisico

% 4. Frecuencia del sonido generado por el 555 (Hz)
frecuencia_555_Hz = 1.44 ./ ((R1_555 + 2 * R_ldr_estimada) * C_555);

% Calculo extra: Ciclo de trabajo (Duty Cycle) del 555
duty_cycle_pct = ((R1_555 + R_ldr_estimada) ./ (R1_555 + 2 * R_ldr_estimada)) * 100;

%% 3. TABLA DE RESULTADOS EN CONSOLA
fprintf('Dec | Binario [b3 b2 b1 b0] | V_DAC (V) | I_LED (mA) | R_LDR (kOhm) | Frecuencia (Hz) | Duty Cycle (%%)\n');
for i = 1:num_niveles
    fprintf(' %2d |       %d  %d  %d  %d       |  %6.4f   |   %5.2f    |    %6.2f    |     %7.1f     |     %5.1f\n', ...
        datos_decimales(i), ...
        bits_matriz(i,1), bits_matriz(i,2), bits_matriz(i,3), bits_matriz(i,4), ...
        voltajes_dac(i), corriente_led_mA(i), R_ldr_estimada(i)/1e3, ...
        frecuencia_555_Hz(i), duty_cycle_pct(i));
end

%% 4. GENERACION DE GRAFICAS
figure('Name', 'Sistema de Comunicacion Optica - EL2113', 'Color', [1 1 1], 'Position', [100 100 1000 700]);

% Grafica 1: Voltaje del DAC
subplot(2, 2, 1);
stairs(datos_decimales, voltajes_dac, 'b', 'LineWidth', 1.8);
hold on;
plot(datos_decimales, voltajes_dac, 'ro', 'MarkerFaceColor', 'r');
grid on;
title('1. Voltaje de salida del DAC (R-2R)');
xlabel('Valor Decimal (0 al 15)');
ylabel('Voltaje V_{DAC} (V)');
ylim([0 5.2]);
xlim([0 15]);

% Grafica 2: Corriente en el LED
subplot(2, 2, 2);
plot(datos_decimales, corriente_led_mA, 'r-s', 'LineWidth', 1.5, 'MarkerFaceColor', [1 0.3 0.3]);
grid on;
title('2. Corriente en el LED Emisor');
xlabel('Valor Decimal (0 al 15)');
ylabel('Corriente I_{LED} (mA)');
xlim([0 15]);

% Grafica 3: Resistencia de la LDR
subplot(2, 2, 3);
plot(datos_decimales, R_ldr_estimada / 1e3, 'm-^', 'LineWidth', 1.5, 'MarkerFaceColor', 'm');
grid on;
title('3. Resistencia de la Fotoresistencia (LDR)');
xlabel('Valor Decimal (0 al 15)');
ylabel('Resistencia R_{LDR} (kOhms)');
xlim([0 15]);

% Grafica 4: Frecuencia del tono final
subplot(2, 2, 4);
plot(datos_decimales, frecuencia_555_Hz, 'g-d', 'LineWidth', 1.8, 'MarkerFaceColor', [0.2 0.7 0.2]);
grid on;
title('4. Frecuencia del Buzzer (NE555)');
xlabel('Valor Decimal (0 al 15)');
ylabel('Frecuencia (Hz)');
xlim([0 15]);

fprintf('Simulacion completada con exito. Graficas generadas.\n');
