pkg load signal
 
v = load("C:\\Localdata\\Work\\Innovationteam\\OOTIProject2016\\Rover5\\Documentation\\LineSensor.txt");
north = rot90(v(:, 3));
east = rot90(v(:, 4));
south = rot90(v(:, 5));
west = rot90(v(:, 6));

t = [0:0.0125:6];
t = t(1: columns(north));

[b,a] = butter(3, 5/40);
fnorth = filter(b, a, north);
fsouth = filter(b, a, south);
feast = filter(b,a, east);
fwest = filter(b,a,west);

figure(1);
plot(t, fnorth, t, feast, t, fsouth, t, fwest);
figure(2);
plot(t, north, t, east, t, south, t, west);
