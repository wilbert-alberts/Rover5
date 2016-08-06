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

