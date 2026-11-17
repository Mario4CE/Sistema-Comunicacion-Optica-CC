* Mediciones automaticas del receptor (NE555 astable con LDR). Resultados en View > SPICE Error Log
* Se mide entre el 3er y 4to flanco de subida para ignorar el primer ciclo (arranque con C1 descargado)
.meas TRAN PERIODO TRIG V(OUT_555) VAL=2.5 RISE=3 TARG V(OUT_555) VAL=2.5 RISE=4
.meas TRAN T_ALTO  TRIG V(OUT_555) VAL=2.5 RISE=3 TARG V(OUT_555) VAL=2.5 FALL=3
.meas TRAN FREQ    param 1/PERIODO
.meas TRAN F_TEO   param 1.443/((1k+2*RLDR)*100n)
.meas TRAN ERR_F   param abs(FREQ-F_TEO)/F_TEO*100
.meas TRAN DUTY    param T_ALTO/PERIODO*100
.meas TRAN VTHR_MAX MAX V(THR) FROM 20m TO 40m
.meas TRAN VTHR_MIN MIN V(THR) FROM 20m TO 40m
