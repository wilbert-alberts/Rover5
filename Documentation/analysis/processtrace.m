v = dlmread("RV_trace.txt");

leftpos = rot90(v(:, 8));
rightpos = rot90(v(:, 9));
leftdc = rot90(v(:, 5));
rightdc = rot90(v(:, 7));
leftdir = rot90(v(:, 4));
rightdir = rot90(v(:, 6));

north = rot90(v(:, 22));
east = rot90(v(:, 23));
south = rot90(v(:, 24));
west = rot90(v(:, 25));

col_ne = rot90(v(:, 18));
col_se = rot90(v(:, 19));
col_sw = rot90(v(:, 20));
col_nw = rot90(v(:, 21));

tq_left = rot90(v(:, 5));
tq_right = rot90(v(:, 7));



T = [0:columns(leftpos)-1];
T = [ T .* 0.01 ];

figure(1);
plot (T, north, T, south, T, east, T, west);
title("Line sensors");
legend ("North", "South", "East", "West");

figure(2);
plot (T, col_ne, T, col_nw, T, col_se, T, col_sw);
title("Collision sensors");
legend ("NE", "NW", "SE", "SW");

figure(3);
plot (T, tq_left, T, tq_right);
title("Torque ");
legend ("Left", "Right");


c_s_raw_n = rot90(v(:, 27));
c_s_raw_e = rot90(v(:, 28));
c_s_raw_s = rot90(v(:, 29));
c_s_raw_w = rot90(v(:, 30));

c_s_go_n = rot90(v(:, 31)); 
c_s_go_e = rot90(v(:, 32)); 
c_s_go_s = rot90(v(:, 33)); 
c_s_go_w = rot90(v(:, 34)); 

c_s_flt_n = rot90(v(:, 35)); 
c_s_flt_e = rot90(v(:, 36)); 
c_s_flt_s = rot90(v(:, 37)); 
c_s_flt_w = rot90(v(:, 38)); 

c_a_in_l =  rot90(v(:, 39)); 
c_a_in_r =  rot90(v(:, 40)); 

c_a_out_l =  rot90(v(:, 41)); 
c_a_out_r =  rot90(v(:, 42)); 

c_c_err = rot90(v(:, 48)); 
c_c_out_raw = rot90(v(:, 49)); 
c_c_out_norm = rot90(v(:, 50)); 

c_c_pid_in = rot90(v(:, 43)); 
c_c_pid_p = rot90(v(:, 44)); 
c_c_pid_i = rot90(v(:, 45)); 
c_c_pid_d = rot90(v(:, 46)); 
c_c_pid_out = rot90(v(:, 47)); 

figure(4);
plot (
      T, c_s_go_n, 
      T, c_s_go_e, 
      T, c_s_go_s, 
      T, c_s_go_w);
title("Sensors after Gain Offset");
legend (
        "GO N", 
        "GO E",
        "GO S",
        "GO W");

figure(5);
plot (T, c_s_flt_n, 
      T, c_s_flt_e, 
      T, c_s_flt_s, 
      T, c_s_flt_w);
title("Sensors after filter");
legend ("Flt N", 
        "Flt E",
        "Flt S",
        "Flt W");

figure(6);
title("Controller");
plot (T, c_c_err, 
      T, c_c_out_raw, 
      T, c_c_out_norm);
legend("error",
       "out(raw)",
       "out(norm");

figure(7);
title("Actuator");
plot(T, c_a_in_l, 
     T, c_a_in_r, 
     T, c_a_out_l, 
     T, c_a_out_r);
legend("in L", "in R", "out L", "out R");
