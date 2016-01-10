% cvGaborFilter2Demo - Demo of cvGaborFilter2
function cvGaborFilter2Demo
%Img = imread('texture.20.png');
%I = imread('retina_512x512x8.png');
I = imread('21_green.png');
Img = imread('21_training.tif');
%red = I(:,:,1); % Green channel
%green = I(:,:,2); % Green channel
%blue = I(:,:,3); % Green channel
%imwrite(green, "21_green.png","png");
figure('name', 'origin'); imshow(Img);
figure('name', 'grey'); imshow(I);
%figure('name','red'); imshow(red);
%figure('name','green'); imshow(green);
%figure('name','blue'); imshow(blue);

%get Dim of I M*N and then get the first dim.
%if the image is square, it returns the same value for width and height.
N = size(I, 1);
gamma = 1; %Lambda = [1 2 4 8 16 32 64];
b = .5;  theta = 0; phi = 0;
for i=1:log2(N/8)
    F = [ (.25 - 2^(i+.5)/N) (.25 + 2^(i-.5)/N) ]; Lambda = 1./F;
    for j=1:2
        lambda = Lambda(j);
        [GO, GF] = cvCircularGaborFilter2(I, gamma, lambda, b, theta, phi);
        GO = cvuNormalize(GO, [0, 255]); % normalize to plot
        figure; imshow(uint8(GO));
    end
end
end
