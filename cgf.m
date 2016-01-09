%isotropic gaussian
function [result] = iso_gauss2(x, y, sigma)

%norm = 1.0/(sqrt(2*pi) * sigma);
ga=(1/(2 * pi * sigma^2));
%exponent = -0.5*((x./ sigma).^2 + (y./ sigma).^2);
exponent = -0.5*( (x.^2 + y.^2)./ sigma^2);
%result = norm * exp(exponent);
result = ga * exp(exponent);
end

%cosine of CircularGaborFilter
function [result] = cgf_cos(x,y,F)

result = cos(2 * pi * F * sqrt(x.^2 + y.^2));

end

%sine of CircularGaborFilter
function [result] = cgf_sin(x,y,F)

result = sin(2 * pi * F * sqrt(x.^2 + y.^2));

end

%real part of CGF
function [result] = cgf_Zr(x,y,sigma, F)

result = iso_gauss2(x,y,sigma) .* cgf_cos(x,y,F);

end

%imaginary part of CGF
function [result] = cgf_Zi(x,y,sigma, F)

result = iso_gauss2(x,y,sigma) .* cgf_sin(x,y,F);

end

%CGF= Zr + Zi

%discrete points in grid
% I(x,y) = N * N = 256 * 256
ticks = [-128:1:127];
[x, y] = meshgrid(ticks, ticks);

%N * N = 256 * 256
%sigma = 4, F = 32/256
sigma =4;
F = 32/256;

%surf(x,y, iso_gauss2(x,y,sigma) .* cgf_sin(x,y,F));

%CGF kernel image
figure;
surf(x,y,cgf_Zr(x,y,sigma,F) );

figure;
surf(x,y,cgf_Zi(x,y,sigma,F) );

%ICFG
sigma = 3.7;
F=2.1;
figure;
surf(x,y,cgf_Zr(x,y,sigma,F) - cgf_Zi(x,y,sigma,F) );


%gauss
gz=(1/(2 * pi * sigma^2)) * exp(-.5*(x.^2 + y.^2)/sigma^2);

%discrete points in grid
%256 * 256
ticks = [-128:1:127];
[x, y] = meshgrid(ticks, ticks);
z = sin (sqrt (x.^2 + y.^2)) ./ (sqrt (x.^2 + y.^2));
surf(x,y,z);
%gticks = [-2.0:0.1:2.0];

%>3sigma covers 99.9%
gticks = [-4.0*sigma:sigma/10:4.0*sigma];
[x, y] = meshgrid(gticks, gticks);

gz=(1/(2 * pi * sigma^2)) * exp(-.5*(x^2 + y^2)/sigma^2);
surf(x,y,gz);

% isotropic gauss : g(x,y) = (1/(2 * PI * sigma^2)) * exp( -(x^2 + y^2)/(2 * sigma^2))
%(1/(2 * pi * sigma^2)) * exp(-.5*(xp^2 + yp^2)/sigma^2)
(1/(2 * pi * sigma^2)) * exp(-.5*(x^2 + y^2)/sigma^2)

% CGF(x,y) = g(x,y) * exp( j * 2 * PI * F * sqrt(x*x + y*y) )
% CGF(x,y) = real part + imaginary part : Zr(x,y) + jZi(x,y)
% Zr(x,y) = g(x,y) * cos(2 * PI * F * sqrt(x*x + y*y))
(1/(2 * pi * sigma^2)) * exp(-.5*(xp^2 + yp^2)/sigma^2) *
cos(2 * pi * F * sqrt(xp^2 + yp^2))

(1/(2 * pi * sigma^2)) * exp(-0.5*( (x^2 + y^2)/ sigma^2)) * cos(2 * pi * F * sqrt(x^2 + y^2))

Zr=(1/(2 * pi * sigma^2)) * exp(-0.5*( (x.^2 + y.^2)./ sigma^2)) * cos(2 * pi * F * sqrt(x.^2 + y.^2));


% Zi(x,y) = g(x,y) * sin(2 * PI * F * sqrt(x*x + y*y))
(1/(2 * pi * sigma^2)) * exp(-.5*(xp^2 + yp^2)/sigma^2) *
sin(2 * pi * F * sqrt(xp^2 + yp^2))

(1/(2 * pi * sigma^2)) * exp(-0.5*( (x^2 + y^2)/ sigma^2)) * sin(2 * pi * F * sqrt(x^2 + y^2))

Zi=(1/(2 * pi * sigma^2)) * exp(-0.5*( (x.^2 + y.^2)./ sigma^2)) * sin(2 * pi * F * sqrt(x.^2 + y.^2));

%Improved Circular Gabor Filter : ICGF
%Zr - jZi
