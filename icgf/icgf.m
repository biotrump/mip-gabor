% icgf = Zr(x,y) - Zi(x,y)*i
% since Zi(x,y) is imaginary part, so it has an "i";
% Zi(x,y) = Zi'(x,y) * i, where Zi'(x,y) is a real function.
% when "Zi(x,y)" times "i", it gets i^2 = -1, 
% so -Zi(x,y)*i = -Zi'(x,y) * i *i = -Zi'(x,y) *(-1) = Zi'(x,y)*(-1)*(-1)=Zi'(x,y)
% icgf = Zr(x,y) + Zi'(x,y)
function [result] = icgf(x,y,sigma, F)
%result = -(cgf_Zr(x,y,sigma,F) + cgf_Zi(x,y,sigma,F));
result = cgf_Zr(x,y,sigma,F) + cgf_Zi(x,y,sigma,F);
end
