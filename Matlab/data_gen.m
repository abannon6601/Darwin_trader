clc
close all
clear

space = rot90(linspace(1,100,100));

a = 0;
b = 100;

noise1 = (b-a).*rand(100,1) + a;

a = 25;
b = 82;

noise2 = (b-a).*rand(100,1) + a;

func1 = sin(space/pi)*13 + 50;
func2 = (space/25).^3 + 23;

targetFunc = 0.5*func1 + func2;


hold all
scatter(space, noise1);
scatter(space, noise2);
plot(space, func1, 'r--');
plot(space, func2, 'b--');
plot(space, targetFunc);

xlabel('X');
ylabel('Y');
title('Sample training data');

out = [flipud(space) flipud(noise1) flipud(noise2) flipud(func1) flipud(func2) flipud(targetFunc)];

legend('Noise Channel 1', 'Noise Channel 2', 'Function 1', 'Function 2' , 'Compound Function');

dlmwrite('training_data.txt',out, 'delimiter',' ');
