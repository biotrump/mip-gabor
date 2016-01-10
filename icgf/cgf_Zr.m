
%real part of CGF
function [result] = cgf_Zr(x,y,sigma, F)

result = iso_gauss2(x,y,sigma) .* cgf_cos(x,y,F);

end
