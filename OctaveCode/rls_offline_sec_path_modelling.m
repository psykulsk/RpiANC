recorded_file = fopen("recorded_whitenoise.raw","r");
recorded_noise = fread(recorded_file, Inf, "int32");
original_file = fopen("whitenoise.raw","r");
original_noise = fread(original_file, Inf, "int32");
original_noise = double(original_noise)/double(intmax("int32"));
recorded_noise = double(recorded_noise)/double(intmax("int32"));

#original_noise = double(original_noise);
#recorded_noise = double(recorded_noise);

M = 500;
sigma_d = 0.001;
h = [1, 0.8, -0.6, 0.4, 0.2, -0.3, -0.5, 0.7, 0.1, 0.5];

x = original_noise(1:2:60000);
#d = filter(h,1,x);
d = recorded_noise(323001:end);

#x = randn(1, M);
#d = filter(h, 1, x);
L = 5;

gamma = 1;
for lambda = [1.01]
    title1 = sprintf('RLS, lamda = %d, gamma = %d\nWsp��czynniki filtru', lambda, gamma);
    
    [e,y,ff] = rls1(x,d,L,lambda,gamma);
    figure(1)
    subplot(2,1,1)
    plot(ff.') 
    title(title1);
   
    subplot(2,1,2)
    semilogy(abs(e))
    ff(:,M);
    title('Sygna� b��du');
   # waitforbuttonpress
end