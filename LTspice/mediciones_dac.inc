* Mediciones automaticas del DAC R-2R (T = 1 ms por codigo). Resultados en View > SPICE Error Log
* Vkk = V(V_DAC) a mitad del codigo kk ; Ekk = error relativo en % respecto a kk*5/16
.meas TRAN V00 FIND V(V_DAC) AT 0.5m
.meas TRAN V01 FIND V(V_DAC) AT 1.5m
.meas TRAN V02 FIND V(V_DAC) AT 2.5m
.meas TRAN V03 FIND V(V_DAC) AT 3.5m
.meas TRAN V04 FIND V(V_DAC) AT 4.5m
.meas TRAN V05 FIND V(V_DAC) AT 5.5m
.meas TRAN V06 FIND V(V_DAC) AT 6.5m
.meas TRAN V07 FIND V(V_DAC) AT 7.5m
.meas TRAN V08 FIND V(V_DAC) AT 8.5m
.meas TRAN V09 FIND V(V_DAC) AT 9.5m
.meas TRAN V10 FIND V(V_DAC) AT 10.5m
.meas TRAN V11 FIND V(V_DAC) AT 11.5m
.meas TRAN V12 FIND V(V_DAC) AT 12.5m
.meas TRAN V13 FIND V(V_DAC) AT 13.5m
.meas TRAN V14 FIND V(V_DAC) AT 14.5m
.meas TRAN V15 FIND V(V_DAC) AT 15.5m
.meas TRAN E00_mV param abs(V00)*1000
.meas TRAN E01 param abs(V01-0.3125)/0.3125*100
.meas TRAN E02 param abs(V02-0.625)/0.625*100
.meas TRAN E03 param abs(V03-0.9375)/0.9375*100
.meas TRAN E04 param abs(V04-1.25)/1.25*100
.meas TRAN E05 param abs(V05-1.5625)/1.5625*100
.meas TRAN E06 param abs(V06-1.875)/1.875*100
.meas TRAN E07 param abs(V07-2.1875)/2.1875*100
.meas TRAN E08 param abs(V08-2.5)/2.5*100
.meas TRAN E09 param abs(V09-2.8125)/2.8125*100
.meas TRAN E10 param abs(V10-3.125)/3.125*100
.meas TRAN E11 param abs(V11-3.4375)/3.4375*100
.meas TRAN E12 param abs(V12-3.75)/3.75*100
.meas TRAN E13 param abs(V13-4.0625)/4.0625*100
.meas TRAN E14 param abs(V14-4.375)/4.375*100
.meas TRAN E15 param abs(V15-4.6875)/4.6875*100
* Etapa 3 (interfaz) y Etapa 4 (corriente del LED)
.meas TRAN VINT15 FIND V(V_INTERFAZ) AT 15.5m
.meas TRAN ILED05 FIND I(D_LED) AT 5.5m
.meas TRAN ILED06 FIND I(D_LED) AT 6.5m
.meas TRAN ILED10 FIND I(D_LED) AT 10.5m
.meas TRAN ILED15 FIND I(D_LED) AT 15.5m
