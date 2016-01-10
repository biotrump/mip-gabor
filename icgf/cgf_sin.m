%sine of CircularGaborFilter
function [result] = cgf_sin(x,y,F)

result = sin(2 * pi * F * sqrt(x.^2 + y.^2));

end
