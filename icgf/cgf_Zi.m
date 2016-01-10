%imaginary part of CGF
function [result] = cgf_Zi(x,y,sigma, F)

result = iso_gauss2(x,y,sigma) .* cgf_sin(x,y,F);

end