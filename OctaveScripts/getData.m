load('../cmake-build-debug/out1.dat');
load('../cmake-build-debug/out2.dat');

out1L = out1(2:2:end);
out1R = out1(1:2:end);
out2L = out2(2:2:end);
out2R = out2(1:2:end);

figure(1)
subplot(2,2,1)
plot(out1L)
title("Time series")
subplot(2,2,2)
plot(out1R)
subplot(2,2,3)
plot(out2L)
subplot(2,2,4)
plot(out2R)

figure(2)
subplot(2,2,1)
plot(real(fftshift(fft(out1L))))
title("FFT")
subplot(2,2,2)
plot(real(fftshift(fft(out1R))))
subplot(2,2,3)
plot(real(fftshift(fft(out2L))))
subplot(2,2,4)
plot(real(fftshift(fft(out2R))))
