x=[];
y=[];

for i=-2:0.1:2;
    y=[y, exp(i)];
    x=[x,i];
end

plot(x,y,':dr')
title('An exponential graph')
xlabel('Value of i')
ylabel('exp(i)')