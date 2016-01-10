%pkg install image-2.4.1.tar.gz
pkg load image;
%CGF= Zr + Zi
function icgf_demo
Img = imread('21_training.tif');
figure('name', 'origin'); imshow(Img);
red = Img(:,:,1);
figure('name', 'red'); imshow(red);
green = Img(:,:,2);
figure('name', 'green'); imshow(green);
blue = Img(:,:,3);
figure('name', 'blue'); imshow(blue);
J = imread('retina_512x512x8.png');
figure('name', 'grey512x512'); imshow(J);
%I = histeq(J);
I=J;
figure('name', 'histeq'); imshow(I);

N = size(I, 1);
%N=32;
sigma = 4;
F = 64/N;%F={4,8,16,32,64,128,...N/4}/N
%sigma=3.7;
%F=2.1;
ticks = [-N/2:1:(N-1)/2];
[x, y] = meshgrid(ticks, ticks);

figure('name','isotropic gauss2D');
surf(x,y, iso_gauss2(x,y,sigma) );

%CGF kernel image
figure('name','CGF_Zr');
surf(x,y,cgf_Zr(x,y,sigma,F) );

figure('name','CGF_Zi');
surf(x,y,cgf_Zi(x,y,sigma,F) );

ICGF = icgf(x,y,sigma, F);
figure('name','ICGF:CGF_Zr-jCGF_Zr');
surf(x,y,ICGF);

shape = 'reflect';
GO = cvConv2(I, double(ICGF), shape);
printf("GO:min=%f, Max=%f\n", min(min(GO)), max(max(GO)) );

P = uint8(cvuNormalize(GO, [0, 255])); % normalize to plot
figure('name', 'P=GO'); imshow(P);

ICGF = cvuNormalize(ICGF, [0, 255]); % normalize to plot
figure('name', 'ICGF'); imshow(uint8(ICGF));

%I-ICGF
%NI = uint8(cvuNormalize(I, [0, 255])); % normalize to plot
%figure('name', 'NI'); imshow(NI);

Rp = I - P.*0.6;
figure('name', 'Rp'); imshow(Rp);

%Rp = cvuNormalize(Rp, [0, 255]); % normalize to plot
%figure('name', 'nRp'); imshow(Rp);

%"Perona & Malik" : anisotropic diffuse
%smooth but edge preserves
%The function g in the update equation determines the type of the result. 
%By default g(d) = exp(-(d./K).^2) where K = 25. iteration
%This choice gives privileges to high-contrast edges over low-contrast ones.
K=20;
%An alternative is to set g(d) = 1./(1 + (d./K).^2), 
%which gives privileges to wide regions over smaller ones

%I += lambda * (g(dN).*dN + g(dS).*dS + g(dE).*dE + g(dW).*dW) where lambda=[0-0.25]
%0.25 in 2D, 0.125 in 3D
lambda=0.25;
g = @(d) exp(-(d./50).^2);
aRp = imsmooth(Rp, "p&m", K, lambda,g);
figure('name', 'aRp'); imshow(aRp);
%Rp = uint8(cvuNormalize(Rp, [0, 255])); % normalize to plot
%figure('name', 'NRp'); imshow(Rp);
figure('name', 'histeq(aRp)'); imshow(histeq(aRp));

end