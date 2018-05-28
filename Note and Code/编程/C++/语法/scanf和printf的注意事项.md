### %f 与 %lf  
对于输出,使用 printf("%f",...) 和 printf("%lf",...)是没有区别的,变量最终都会当成 double 来处理.  
但对于输入, %f 则对应于 float 变量, %lf 则对应double,不可以搞错  
对于 long double,则要采用 %Lf  
