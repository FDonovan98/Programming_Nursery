#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

int main()
{
    clock_t tStart = clock();

    float TotalLift = 0;
    float WingLift = 0;
    float TailLift = 0;
    float Weight = 0;
    float TailAlpha = 2;
    float WingAlpha = 0;
    float TailArea = 0;
    float WingArea = 0;
    float TailSpan = 0;
    float TailChord = 0;
    float WingSpan = 0;
    float WingChord = 0;
    float WingCl = 0;
    float WingTakeOffCl = 0;
    float TailTakeOffCl = 0;
    float TailCl = 0;
    float WingWeight = 0;
    float TailWeight = 0;
    float JetWeight = 0.5;
    float PassengerWeight = 0.3;
    float Vel = 0;
    float VelMax = 0;
    float TimeMin = 0;
    float TimeMid = 0;
    float TimeMax = 1.5;
    float Acc = 0;
    float Force = 10;
    float Mass = 0;
    float FuseLength = 0;
    float Rho = 1.225;
    float TakeOffDist = 0;
    float TakeOffAngle = 0;
    float Runway = 1.9;
    float FuseAlpha = 0;
    float WingEffectiveAlpha = 0;
    float TailEffectiveAlpha = 0;
    float FrontGear = 0;
    float RearGear = 0;
    bool LiftOff = false;
    bool MadeRunway = true;

    /* Variables for calculating moments */
    float WingX = 0;
    float TailX = 0;
    float JetX = 0;
    float JetY = 0;
    float PassengerX = 0;
    float MaxTailLift = 0;
    float MaxWingLift = 0;
    float WeightMoment = 0;
    float LiftMaxMoment = 0;
    float LiftMoment = 0;
    float JetMoment = 0;
    float TakeOffMoment = 0;
    float CosTheta = 0;
    float CruiseAngle = 0;
    float MaxAngle = 90;
    float GlideAngle = 0;

    float MaxWingDrag = 0;
    float MaxTailDrag = 0;
    float TotalMaxDrag = 0;

    float WingTakeOffDrag = 0;
    float TailTakeOffDrag = 0;
    float TotalTakeOffDrag = 0;

    float WingCd = 0;
    float TailCd = 0;

    float Cd0 = 0.01;
    float K = 0.045; 

    float PoweredX = 0;
    float PoweredY = 0;
    float PoweredDist = 0;
    float GlideX = 0;
    float TotalX = 0;

    float BestLift = 0;
    float BestWingAlpha = 0;
    float BestFuseLength = 0;
    float BestWingSpan = 0;
    float BestWingChord = 0;
    float BestTailSpan = 0;
    float BestTailChord = 0;
    float BestMass = 0;
    float BestTakeOffDist = 0;
    float BestTakeOffTime = 0;
    float BestCruiseAngle = 0;
    float BestWingX = 0;
    float BestTailX = 0;
    float BestPassengerX = 0;
    float BestJetX = 0;
    float BestJetY = 0;
    float BestGlideAngle = 0;
    float BestFrontGear = 0;
    float BestRearGear = 0;
    float BestTotalX = 0;
    float BestTakeOffMoment = 0;
    float BestTakeOffAngle = 0;

    float StepSize = 0.1;

    float RunTime = 0;

    ofstream Results;

    /* 
    Program assumes wing and tail wing can not overlap
    Program assumes tail is behind the main wing 
    */


    for (FuseLength = StepSize; FuseLength <= 1; FuseLength = FuseLength + StepSize) {
        for (WingSpan = StepSize; WingSpan <= 1; WingSpan = WingSpan + StepSize) {
            for (WingChord = StepSize; WingChord <= FuseLength; WingChord = WingChord + StepSize) {
                for (WingX = (0.5*WingChord); WingX <= (FuseLength-(0.5*WingChord)); WingX = WingX + 0.5 * StepSize) {
                    for (WingAlpha = -2; WingAlpha <= 4; WingAlpha = WingAlpha + 2) { 
                        for (TailSpan = 0; TailSpan <= 1; TailSpan = TailSpan + StepSize) {
                            for (TailChord = 0; TailChord <= (FuseLength-(WingX + 0.5 * WingChord)); TailChord = TailChord + StepSize) {
                                
                                TailArea = TailChord * TailSpan;
                                WingArea = WingChord * WingSpan;

                                WingWeight = 4 * WingArea;
                                TailWeight = 4 * TailArea;
                                Weight = FuseLength + WingWeight + TailWeight + JetWeight + PassengerWeight;
                                
                                Mass = Weight/9.81;
                                Acc = Force/Mass;
                                                               
                                VelMax = sqrt(((Force / Weight) * (Weight / (WingArea + TailArea)) + (Weight / (WingArea + TailArea)) * sqrt((Force / Weight) * (Force / Weight) - (4 * Cd0 * K))) / (Rho * Cd0));

                                /*
                                VelMax equation is from http://www.srmuniv.ac.in/sites/default/files/downloads/class10-2012.pdf
                                */
                                
                                if (VelMax > (TimeMax * Acc)) {
                                    VelMax = TimeMax * Acc;
                                }
                                                               
                                for (FrontGear = 0; FrontGear <= 1; FrontGear = FrontGear + StepSize) {
                                    for (RearGear = 0; RearGear <= 1; RearGear = RearGear + StepSize) {

                                        if (FrontGear > RearGear) {
                                            FuseAlpha = asin((FrontGear - RearGear) / FuseLength) * 180 / M_PI;                                               
                                        } else {
                                            FuseAlpha = -asin((RearGear - FrontGear) / FuseLength) * 180 / M_PI;
                                        }

                                        TailEffectiveAlpha = TailAlpha + FuseAlpha;
                                        WingEffectiveAlpha = WingAlpha + FuseAlpha; 

                                        if ((floor(WingEffectiveAlpha) == WingEffectiveAlpha) and (WingEffectiveAlpha >= -2) and (WingEffectiveAlpha <= 4)) {
                                            
                                            WingTakeOffCl = 0.1 * WingEffectiveAlpha + 0.2;
                                            TailTakeOffCl = 0.1 * TailEffectiveAlpha + 0.2;

                                            WingCl = 0.1 * WingAlpha + 0.2;
                                            TailCl = 0.1 * TailAlpha + 0.2;

                                            if (WingCl == 0) {
                                                WingCd = 0.025;
                                            } else if (WingCl == 0.2) {
                                                WingCd = 0.0268;
                                            } else if (WingCl == 0.4) {
                                                WingCd = 0.0322;
                                            } else {
                                                WingCd = 0.0412; 
                                            }

                                            if (TailCl == 0) {
                                                TailCd = 0.01;
                                            } else if (TailCl == 0.2) {
                                                TailCd = 0.0118;
                                            } else if (TailCl == 0.4) {
                                                TailCd = 0.0172;
                                            } else {
                                                TailCd = 0.0262; 
                                            }
                                            
                                            MaxWingDrag = WingCd*0.5*Rho*VelMax*VelMax*WingArea;
                                            MaxTailDrag = TailCd*0.5*Rho*VelMax*VelMax*TailArea;
                                            TotalMaxDrag = MaxWingDrag + MaxTailDrag;
                                            
                                            CruiseAngle = (asin((Force - TotalMaxDrag) / Weight) * (180 / M_PI));

                                            if ((CruiseAngle >= 0) and (CruiseAngle <= MaxAngle)) {
                                            
                                                for (TailX = (WingX + 0.5 * WingChord + 0.5 * TailChord); TailX <= (FuseLength - 0.5 * TailChord); TailX = (TailX + 0.5 * StepSize)) {
                                                    for (JetX = 0; JetX <= FuseLength; JetX = (JetX + StepSize)) {
                                                        for (PassengerX = 0; PassengerX <= FuseLength; PassengerX = (PassengerX + StepSize)) {
                                                            for (JetY = -0.5; JetY <= 0.5; JetY = (JetY + StepSize)) {

                                                                WeightMoment = WingX * WingWeight + (FuseLength / 2) * FuseLength + TailX * TailWeight + JetX * JetWeight;

                                                                TailLift = TailTakeOffCl*0.5*Rho*VelMax*VelMax*TailArea;
                                                                WingLift = WingTakeOffCl*0.5*Rho*VelMax*VelMax*WingArea;
                                                                TotalLift = TailLift + WingLift;

                                                                if (TotalLift > Weight) {

                                                                    do {

                                                                        MadeRunway = false;
                                                                        LiftOff = false;
                                                                        TimeMid = (TimeMax + TimeMin) / 2;

                                                                        Vel = (TimeMid / 1.5) * VelMax;
                                                                        /* Assumes acceleration to max speed is linear */
                                                                        TailLift = TailTakeOffCl*0.5*Rho*Vel*Vel*TailArea;
                                                                        WingLift = WingTakeOffCl*0.5*Rho*Vel*Vel*WingArea;
                                                                        TotalLift = TailLift + WingLift;

                                                                        if (TotalLift > Weight) {

                                                                            TakeOffDist = 0.5 * Acc * TimeMid * TimeMid;                   
                                                                            /* Doesn't account for drag */

                                                                            if (TakeOffDist < Runway) {
                                                                                LiftOff = true;
                                                                                MadeRunway = true; 
                                                                                TimeMax = TimeMid;
                                                                            
                                                                            } else {

                                                                                TimeMax = TimeMid;
                                                                                
                                                                            }

                                                                        } else {

                                                                            TimeMin = TimeMid;
                                                                            
                                                                        }

                                                                        if (((TimeMax - TimeMin) < StepSize) and (MadeRunway == false)) {
                                                                            LiftOff = true;
                                                                        }
                                                                        
                                                                    }
                                                                    while (((TimeMax - TimeMin) > StepSize) or (LiftOff == false)) ;

                                                                    TimeMax = 1.5;
                                                                    TimeMin = 0;

                                                                    if (MadeRunway == true) {

                                                                        LiftMoment = WingX * WingLift + TailX * TailLift;
                                                                        JetMoment = JetY * Force;
                                                                        
                                                                        TakeOffMoment = ((LiftMoment + JetMoment) / WeightMoment); 
                                                                        
                                                                        if (TakeOffMoment > 0) {

                                                                            
                                                                            WingTakeOffDrag = WingCd*0.5*Rho*Vel*Vel*WingArea;
                                                                            TailTakeOffDrag = TailCd*0.5*Rho*Vel*Vel*TailArea;
                                                                            TotalTakeOffDrag = WingTakeOffDrag + TailTakeOffDrag;

                                                                            TakeOffAngle = (asin((Force - TotalTakeOffDrag) / Weight) * (180 / M_PI));

                                                                            GlideAngle = (atan(WingCd / WingCl) * (180 / M_PI)); 
                                            
                                                                            PoweredDist = 0.5 * VelMax * TimeMax;  
                                                                            PoweredY = PoweredDist * sin(CruiseAngle * (M_PI / 180));
                                                                            PoweredX = PoweredDist * cos(CruiseAngle * (M_PI / 180));
                                                                            GlideX = PoweredY / tan(GlideAngle * (M_PI / 180));
                                                                            TotalX = PoweredX + GlideX;

                                                                            if (TotalX > BestTotalX) {

                                                                                BestTakeOffMoment = TakeOffMoment;

                                                                                BestCruiseAngle = CruiseAngle;
                                                                                BestGlideAngle = GlideAngle;

                                                                                BestTotalX = TotalX;
                                                                                
                                                                                BestFrontGear = FrontGear;
                                                                                BestRearGear = RearGear;

                                                                                BestTakeOffTime = TimeMid;
                                                                                BestLift = TotalLift;
                                                                                BestWingAlpha = WingAlpha;
                                                                                BestFuseLength = FuseLength;
                                                                                BestWingSpan = WingSpan;
                                                                                BestWingChord = WingChord;
                                                                                BestTailSpan = TailSpan;
                                                                                BestTailChord = TailChord;
                                                                                BestMass = Mass;
                                                                                BestTakeOffDist = TakeOffDist;
                                                                                BestWingX = WingX;
                                                                                BestTailX = TailX;
                                                                                BestPassengerX = PassengerX;
                                                                                BestJetX = JetX;
                                                                                BestJetY = JetY;
                                                                                BestTakeOffAngle = TakeOffAngle;
                                                                            }
                                                                        }
                                                                    }
                                                                } 
                                                            }
                                                        }
                                                    }
                                                } 
                                            }
                                        }
                                    }
                                }      
                            }
                        }
                    }
                }
            }
        }
    }

    RunTime = (float)(clock() - tStart)/CLOCKS_PER_SEC;

    Results.open ("ResultsV1.1.txt");
    if (Results.is_open()) {
        Results << "Best Total X Distance " << BestTotalX << "\n";
        Results << "Best Lift " << BestLift << "\n";
        Results << "Best Mass " << BestMass << "\n";
        Results << "Best Wing Alpha " << BestWingAlpha << "\n";
        Results << "Best Fuselage Length " << BestFuseLength << "\n";
        Results << "Best Wing Span " << BestWingSpan << "\n";
        Results << "Best Wing Chord " << BestWingChord << "\n";
        Results << "Best Tail Span " << BestTailSpan << "\n";
        Results << "Best Tail Chord " << BestTailChord << "\n";
        Results << "Best Take Off Distance " << BestTakeOffDist << "\n";
        Results << "Best Take Off Time " << BestTakeOffTime << "\n";
        Results << "Best Cruise Angle " << BestCruiseAngle << "\n";
        Results << "Best Glide Angle " << BestGlideAngle << "\n";
        Results << "Best Wing X " << BestWingX << "\n";
        Results << "Best Tail X " << BestTailX << "\n";
        Results << "Best Passenger X " << BestPassengerX << "\n";
        Results << "Best Jet X " << BestJetX << "\n";
        Results << "Best Jet Y " << BestJetY << "\n";
        Results << "Best Front Gear Length " << BestFrontGear << "\n";
        Results << "Best Rear Gear Length " << BestRearGear << "\n";
        Results << "Best Take Off Moment " << BestTakeOffMoment << "\n";
        Results << "Best Take Off Angle " << BestTakeOffAngle << "\n";
        Results << "Time Taken For Program To Run " << floor(RunTime/60/60) << " hours " << floor(fmod(RunTime/60,60)) << " minutes and " << remainder(RunTime, 60) << " seconds \n";
    } else {
        cout << "Unable to save results to file. Outputting results \n";
        cout << "Best Total X Distance " << BestTotalX << "\n";
        cout << "Best Lift " << BestLift << "\n";
        cout << "Best Mass " << BestMass << "\n";
        cout << "Best Wing Alpha " << BestWingAlpha << "\n";
        cout << "Best Fuselage Length " << BestFuseLength << "\n";
        cout << "Best Wing Span " << BestWingSpan << "\n";
        cout << "Best Wing Chord " << BestWingChord << "\n";
        cout << "Best Tail Span " << BestTailSpan << "\n";
        cout << "Best Tail Chord " << BestTailChord << "\n";
        cout << "Best Take Off Distance " << BestTakeOffDist << "\n";
        cout << "Best Take Off Time " << BestTakeOffTime << "\n";
        cout << "Best Cruise Angle " << BestCruiseAngle << "\n";
        cout << "Best Glide Angle " << BestGlideAngle << "\n";
        cout << "Best Wing X " << BestWingX << "\n";
        cout << "Best Tail X " << BestTailX << "\n";
        cout << "Best Passenger X " << BestPassengerX << "\n";
        cout << "Best Jet X " << BestJetX << "\n";
        cout << "Best Jet Y " << BestJetY << "\n";
        cout << "Best Front Gear Length " << BestFrontGear << "\n";
        cout << "Best Rear Gear Length " << BestRearGear << "\n";
        cout << "Best Take Off Moment " << BestTakeOffMoment << "\n";
        cout << "Best Take Off Angle " << BestTakeOffAngle << "\n";
        cout << "Time Taken For Program To Run " << floor(RunTime/60/60) << " hours " << floor(fmod(RunTime/60,60)) << " minutes and " << remainder(RunTime, 60) << " seconds \n";;
    }
    Results.close();

    cout << "Time Taken For Program To Run " << floor(RunTime/60/60) << " hours " << floor(fmod(RunTime/60,60)) << " minutes and " << remainder(RunTime, 60) << " seconds \n";;

    return 0;

}