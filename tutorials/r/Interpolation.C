//More Information for R interpolation in 
//http://stat.ethz.ch/R-manual/R-patched/library/stats/html/approxfun.html
//Author: Omar Zapata
#include<TRInterface.h>
#include<TRandom.h>
void Interpolation()
{
//Creting points
TRandom r;
TVectorD x(10),y(10);
for(int i=0;i<10;i++)
{
  x[i]=i;
  y[i]=r.Gaus();
}

(*gR)["x"]=x;
(*gR)["y"]=y;


gR->Xwin();//Required to active new window for plot
//Plot parameter. Plotting using two rows and one column
gR->Parse("par(mfrow = c(2,1))");

//plotting the points
gR->plot("x, y, main = 'approx(.) and approxfun(.)'");

//The function "approx" returns a list with components x and y, 
//containing n coordinates which interpolate the given data points according to the method (and rule) desired.
gR->points("approx(x, y), col = 2, pch = '*'");
gR->points("approx(x, y, method = 'constant'), col = 4, pch = '*'");


//The function "approxfun" returns a function performing (linear or constant) 
//interpolation of the given data. 
//For a given set of x values, this function will return the corresponding interpolated values.
gR->Parse("f <- approxfun(x, y)");

gR->curve("f(x), 0, 11, col = 'green2'");
gR->points("x, y");

//using approxfun with const method
gR->Parse("fc <- approxfun(x, y, method = 'const')");
gR->curve("fc(x), 0, 10, col = 'darkblue', add = TRUE");
// different interpolation on left and right side :
gR->plot("approxfun(x, y, rule = 2:1), 0, 11,col = 'tomato', add = TRUE, lty = 3, lwd = 2");
}
