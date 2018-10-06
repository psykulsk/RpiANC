function [MSE_e, MSE_f, Mean_f] = lms1_loop(K, M, h, sigma_d, alpha)

    L = length(h);

    e_sum = zeros(1,M);
    f_error_sum = zeros(1,M);
    ff_sum = zeros(L,M);
    ff_error_sum = zeros(L,M);
    
    
    for i = 1:K
        x = randn(1, M);
        d = filter(h, 1, x) + sigma_d*randn(1, M);
        [e,y,ff] = lms1(x,d,L,alpha);
        e_sum = e_sum + e.^2;
        ff_sum = ff_sum + ff;
        for j = 1:M
            ff_error_sum(:,j) = ff_error_sum(:,j) + (h - ff(:,j)).^2;
        end   
    end
    
    
    MSE_e = e_sum./K;
    MSE_f = ff_error_sum./K;
    Mean_f = ff_sum./K;


end

