%CGF= Zr + Zi
function cgf_demo
%discrete points in grid
% I(x,y) = N * N = 256 * 256
%ticks = [-128:1:127];
%[x, y] = meshgrid(ticks, ticks);

%rgb retina
Img = imread('21_training.tif');
figure('name', 'origin'); imshow(Img);
%grey 8 bit 512x512 retina
I = imread('retina_512x512x8.png');
figure('name', 'grey512x512'); imshow(I);
%Dimension of Retina image
N = size(I, 1);
sigma =4;
F = 128/N;%F={4,8,16,32,64,128,...N/4}/N
%kernel size : N
ticks = [-N/2:1:(N-1)/2];
[x, y] = meshgrid(ticks, ticks);

figure('name','isotropic gauss2D');
surf(x,y, iso_gauss2(x,y,sigma) );

%CGF kernel image
figure('name','CGF_Zr');
surf(x,y,cgf_Zr(x,y,sigma,F) );

figure('name','CGF_Zi');
surf(x,y,cgf_Zi(x,y,sigma,F) );

%ICFG
%sigma = 3.7;
%F=2.1;
figure('name','ICGF:CGF_Zr-jCGF_Zr');
surf(x,y,cgf_Zr(x,y,sigma,F) - cgf_Zi(x,y,sigma,F) );

end