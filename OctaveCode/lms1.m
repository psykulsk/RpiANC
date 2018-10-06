function [e, y, ff] = lms1(x, d, L, alpha)

N = length(x);

e = zeros(1,N); 
y = zeros(1,N);                   
ff = zeros(L,N);                

f_n = zeros(L,1);                 
x_n = zeros(L,1);

for n = 1:N
        
    x_n = [x(n); x_n(1:end-1,1)];
      
    y(n) = f_n.'*x_n;
    e(n) = d(n) - y(n);
    f_n = f_n + alpha*e(n)*x_n;
    
    ff(:,n) = f_n;
    
end

end