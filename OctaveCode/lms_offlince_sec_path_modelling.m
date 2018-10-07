recorded_file = fopen("recorded_whitenoise.raw","r");
recorded_noise = fread(recorded_file, Inf, "int32");
original_file = fopen("whitenoise.raw","r");
original_noise = fread(original_file, Inf, "int32");
original_noise = double(original_noise)/double(intmax("int32"));
recorded_noise = double(recorded_noise)/double(intmax("int32"));


M = 500;

L = 50;

sigma_d = 0.001;
x = original_noise(2:2:60000)';

h = [1, 0.8, 0.6, 0.4, 0.2];
d = filter(h,1,x);
#d = recorded_noise(1:30000)';

alpha= [0.0001, 0.0001, 0.0002, 0.0003, 0.0005, 0.001];

for i = 1:length(alpha)
    figure(1)
    [e,y,ff] = lms1(x,d,L,alpha(i));
    
    subplot(2,1,1);
    semilogy(abs(e))
    
    subplot(2,1,2);
    plot(ff')
    
    waitforbuttonpress;
    
    
end