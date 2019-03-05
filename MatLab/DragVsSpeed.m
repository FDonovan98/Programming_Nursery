rho=1.225;
WSpan=50;
CD0=0.03;
K=0.04;
AllD=[];
Speeds=[];

for W=50000:25000:100000
    for Vel=20:1:100
        D=0.5*rho*Vel*WSpan*CD0+K*W/(0.5*rho*Vel*WSpan);
        AllD=[AllD, D];
        Speeds=[Speeds,Vel];
    end
    
    if W==50000
        S1 = Speeds;
        D1=AllD;
    elseif W==75000
        S2=Speeds;
        D2=AllD;
    else 
        S3=Speeds;
        D3=AllD;
    end
        
end

plot(S1, D1, 'or', S2, D2, 'dr', S3, D3, 'xg')
title('Speed Vs Drag for Varying Weights')
xlabel('Speed')
ylabel('Drag')

legend('50kN', '75kN', '100kN')