%pkg install image-2.4.1.tar.gz
%CGF= Zr + Zi
function icgf_demo
pkg load image;
Img = imread('21_training.tif');
figure('name', 'origin'); imshow(Img);
red = Img(:,:,1);
figure('name', 'red'); imshow(red);
green = Img(:,:,2);
figure('name', 'green'); imshow(green);
[level, sep] = graythresh(green);%otsu
GBW = im2bw(green, level);
figure('name', 'GBW'); imshow(GBW);
blue = Img(:,:,3);
figure('name', 'blue'); imshow(blue);
%J = imread('retina_512x512x8.png');
%figure('name', 'grey512x512'); imshow(J);
%I = histeq(J, 256);
%I=J;
I=green;
figure('name', 'histeq'); imshow(I);
printf("I:min=%f, Max=%f\n", min(min(I)), max(max(I)) );

[level, sep] = graythresh(I);
printf("I:level=%f, sep=%f\n", level, sep);
BW = im2bw(I, level);
figure('name', 'hst BW'); imshow(BW);

N = size(I, 1);
N=128;
%sigma = 4;
%F = 64/N;%F={4,8,16,32,64,128,...N/4}/N
F = 4/N;%F={4,8,16,32,64,128,...N/4}/N
sigma=3.7;
%F=2.1;
ticks = [-N/2:1:(N-1)/2];
%ticks = [0:1:N];
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
P=GO;
% GO is normalized to [0-255]
nP = uint8(cvuNormalize(GO, [0, 255])); % normalize to plot
%nP = uint8(cvuNormalize(GO, [255,0])); % normalize to plot
figure('name', 'nP=I*ICGF'); imshow(nP);
printf("nP:min=%f, Max=%f\n", min(min(nP)), max(max(nP)) );

%ICGF = cvuNormalize(ICGF, [0, 255]); % normalize to plot
%figure('name', 'ICGF'); imshow(uint8(ICGF));

%I-ICGF
%NI = uint8(cvuNormalize(I, [0, 255])); % normalize to plot
%figure('name', 'NI'); imshow(NI);

% Rp is temporal variable [-255-+255]
%Rp = single(I) - single(P);
Rp = single(I) - P;
%figure('name', 'Rp'); imshow(Rp);
printf("Rp:min=%f, max=%f\n", min(min(Rp)), max(max(Rp)) ) ;

% normalize R' to R[0-255]
% R = (R'-min(R')) * 255 / (max(R') - min(R') )
nRp = uint8(cvuNormalize(Rp, [0, 255])); % normalize to plot
figure('name', 'nRp'); imshow(nRp);
printf("R=nRp:min=%f, max=%f\n", min(min(nRp)), max(max(nRp)) ) ;
R=nRp;
figure('name', 'histeq(nRp)'); imshow(histeq(nRp));

%"Perona & Malik" : anisotropic diffuse
%smooth but edge preserves
%The function g in the update equation determines the type of the result. 
%By default g(d) = exp(-(d./K).^2) where K = 25. iteration
%This choice gives privileges to high-contrast edges over low-contrast ones.
% K is the sensitivity to stop smooth, if K is small, thin edge is kept.
K=2;
%An alternative is to set g(d) = 1./(1 + (d./K).^2), 
%which gives privileges to wide regions over smaller ones

%I += lambda * (g(dN).*dN + g(dS).*dS + g(dE).*dE + g(dW).*dW) where lambda=[0-0.25]
%0.25 in 2D, 0.125 in 3D
lambda=0.25;
g = @(d) exp(-(d./K).^2);
gl = @(d) 1./(1 + (d./K).^2);
repeats=50;
aR = imsmooth(R, "p&m", repeats, lambda, g);
printf("aR:min=%f, max=%f\n", min(min(aR)), max(max(aR)) ) ;
figure('name', 'aR'); imshow(aR);
figure('name', 'histeq(aR)'); imshow(histeq(aR));

%otsu threshold
[level, sep] = graythresh(aR, "Otsu");
printf("ar:level=%f, sep=%f\n", level, sep);
aRBW = im2bw(aR, level);
figure('name', 'aRBW'); imshow(aRBW);

naR = uint8(cvuNormalize(aR, [0, 255])); % normalize to plot
figure('name', 'naR'); imshow(naR);
printf("naR:min=%f, max=%f\n", min(min(naR)), max(max(naR)) ) ;
%figure('name', 'NRp'); imshow(Rp);
figure('name', 'histeq(naR)'); imshow(histeq(naR));

%otsu threshold
[level, sep] = graythresh(naR, "Otsu");
%[level, sep] = graythresh(Rp, "Otsu");
printf("level=%f, sep=%f\n", level, sep);

naRBW = im2bw(naR, level);
figure('name', 'naRBW'); imshow(naRBW);
end