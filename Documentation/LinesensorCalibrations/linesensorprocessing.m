pkg load signal
 
function normed = normVector(v)
  gain = 1/(max(v) - min(v));
  offset = min(v);
  normed = [ (v .- offset) .* gain];
endfunction
 
v = load("C:\\Localdata\\Work\\Innovationteam\\OOTIProject2016\\Rover5\\Documentation\\LineSensorCalibrations\\ne.txt");
north = rot90(v(:, 3));
east = rot90(v(:, 4));
south = rot90(v(:, 5));
west = rot90(v(:, 6));

t = [0:0.0125:6];
t = t(1: columns(north));

[b,a] = butter(2, 5/40);
fnorth = filter(b, a, normVector(north));
fsouth = filter(b, a, normVector(south));
feast = filter(b,a, normVector(east));
fwest = filter(b,a, normVector(west));

figure(1);
plot(t, fnorth, t, fsouth, t, feast, t, fwest);
legend("north", "south", "east", "west");
figure(2);
plot(t, north, t, south, t, east, t, west);
legend("north", "south", "east", "west");
