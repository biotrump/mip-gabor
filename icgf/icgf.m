function [result] = icgf(x,y,sigma, F)
result = cgf_Zr(x,y,sigma,F) - cgf_Zi(x,y,sigma,F);
end