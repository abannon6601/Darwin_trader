clc
close all
clear

space = rot90(linspace(1,100,100));

func1 = sin(space/pi)*13 + 50;

plot(space, func1);

out = [flipud(space) flipud(func1)];

dlmwrite('training_data.txt',out, 'delimiter',' ');