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

trend_noise1 = ones(99,1);
trend_noise2 = ones(99,1);
trend_func1 = ones(99,1);
trend_func2 = ones(99,1);
trend_targetFunc = ones(99,1);

for n = 1:99
    trend_noise1(n) = noise1(n) - noise1(n+1); 
    trend_noise2(n) = noise2(n) - noise2(n+1); 
    
    trend_func1(n) = func1(n) -  func1(n+1);
    trend_func2(n) = func2(n) -  func2(n+1);
    
    trend_targetFunc(n) = targetFunc(n) - targetFunc(n+1);
end

out = [flipud(trend_noise1) flipud(trend_noise2) flipud(trend_func1) flipud(trend_func2) flipud(trend_targetFunc)];

dlmwrite('training_data.txt',out, 'delimiter',' ');
