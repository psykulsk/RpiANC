function [e, y, ff] = rls1(x, d, L, lambda, gamma)

N = length(x);

e = zeros(1,N); 
y = zeros(1,N);                   
ff = zeros(L,N);                

f_n = zeros(L,1);     
x_n = zeros(L,1);

P = gamma*eye(L);
N
for n = 1:N
        
    x_n = [x(n); x_n(1:end-1,1)];
      
    y(n) = f_n.'*x_n;
    e(n) = d(n) - y(n);

    alpha_n = 1/(lambda + (x_n.')*P*x_n);

    f_n = f_n + alpha_n*e(n)*P*x_n;
    P = (1/lambda)*(P-alpha_n*P*x_n*(x_n.')*P);
    
    ff(:,n) = f_n;
    
end   