%cosine of CircularGaborFilter
function [result] = cgf_cos(x,y,F)

result = cos(2 * pi * F * sqrt(x.^2 + y.^2));

end