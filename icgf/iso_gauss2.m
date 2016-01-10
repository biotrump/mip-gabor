%isotropic gaussian
function [result] = iso_gauss2(x, y, sigma)

%norm = 1.0/(sqrt(2*pi) * sigma);
ga=(1/(2 * pi * sigma^2));
%exponent = -0.5*((x./ sigma).^2 + (y./ sigma).^2);
exponent = -0.5*( (x.^2 + y.^2)./ sigma^2);
%result = norm * exp(exponent);
result = ga * exp(exponent);
end
