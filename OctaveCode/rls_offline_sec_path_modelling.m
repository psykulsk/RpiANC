recorded_file = fopen("rec/recorded_whitenoise.dat","r");
original_file = fopen("rec/right_channel_noise.dat","r");
recorded_noise = fread(recorded_file, Inf, "int32");
original_noise = fread(original_file, Inf, "int32");
original_noise = double(original_noise)/double(intmax("int32"));
recorded_noise = double(recorded_noise)/double(intmax("int32"));

#original_noise = double(original_noise);
#recorded_noise = double(recorded_noise);

M = 500;
sigma_d = 0.001;
h = [1, 0.8, -0.6, 0.4, 0.2, -0.3, -0.5, 0.7, 0.1, 0.5];

 
#d = filter(h,1,x);
x = original_noise(1:15000);
d = recorded_noise(1160:(length(x)+1160));
#x = randn(1, M);
#d = filter(h, 1, x);
L = 5;

gamma = 1;
for lambda = [1]
    title1 = sprintf('RLS, lamda = %d, gamma = %d\nWsp��czynniki filtru', lambda, gamma);
    
    [e,y,ff] = rls1(x,d,L,lambda,gamma);
    figure(1)
    subplot(4,1,1)
    plot(ff.') 
    title(title1);
   
    subplot(4,1,2)
    semilogy(abs(e))
    subplot(4,1,3)
    plot(x)
    subplot(4,1,4)
    plot(d)
    ff(:,M)
    title('Sygna� b��du');
   # waitforbuttonpress
end