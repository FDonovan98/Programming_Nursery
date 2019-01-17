#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

int main()
{
    clock_t tStart = clock();

    double TotalLift = 0;
    double WingLift = 0;
    double TailLift = 0;
    double Weight = 0;
    double TailAlpha = 2;
    double WingAlpha = 0;
    double TailArea = 0;
    double WingArea = 0;
    double TailSpan = 0;
    double TailChord = 0;
    double WingSpan = 0;
    double WingChord = 0;
    double WingCl = 0;
    double WingTakeOffCl = 0;
    double TailTakeOffCl = 0;
    double TailCl = 0;
    double WingWeight = 0;
    double TailWeight = 0;
    double JetWeight = 0.5;
    double PassengerWeight = 0.3;
    double Vel = 0;
    double VelMax = 0;
    double Time = 0;
    double TimeMax = 1.5;
    double Acc = 0;
    double Force = 10;
    double Mass = 0;
    double FuseLength = 0;
    double Rho = 1.225;
    double TakeOffDist = 0;
    double TakeOffAngle = 0;
    double Runway = 1.9;
    double FuseAlpha = 0;
    double WingEffectiveAlpha = 0;
    double TailEffectiveAlpha = 0;
    double FrontGear = 0;
    double RearGear = 0;
    bool LiftOff = false;

    /* Variables for calculating moments */
    double WingX = 0;
    double TailX = 0;
    double JetX = 0;
    double JetY = 0;
    double PassengerX = 0;
    double MaxTailLift = 0;
    double MaxWingLift = 0;
    double WeightMoment = 0;
    double LiftMaxMoment = 0;
    double LiftMoment = 0;
    double JetMoment = 0;
    double TakeOffMoment = 0;
    double CosTheta = 0;
    double CruiseAngle = 0;
    double MaxAngle = 90;
    double GlideAngle = 0;

    double MaxWingDrag = 0;
    double MaxTailDrag = 0;
    double TotalMaxDrag = 0;

    double WingTakeOffDrag = 0;
    double TailTakeOffDrag = 0;
    double TotalTakeOffDrag = 0;

    double WingCd = 0;
    double TailCd = 0;

    double Cd0 = 0.01;
    double K = 0.045; 

    double PoweredX = 0;
    double PoweredY = 0;
    double PoweredDist = 0;
    double GlideX = 0;
    double TotalX = 0;

    double BestLift = 0;
    double BestWingAlpha = 0;
    double BestFuseLength = 0;
    double BestWingSpan = 0;
    double BestWingChord = 0;
    double BestTailSpan = 0;
    double BestTailChord = 0;
    double BestMass = 0;
    double BestTakeOffDist = 0;
    double BestTakeOffTime = 0;
    double BestCruiseAngle = 0;
    double BestWingX = 0;
    double BestTailX = 0;
    double BestPassengerX = 0;
    double BestJetX = 0;
    double BestJetY = 0;
    double BestGlideAngle = 0;
    double BestFrontGear = 0;
    double BestRearGear = 0;
    double BestTotalX = 0;
    double BestTakeOffMoment = 0;
    double BestTakeOffAngle = 0;

    double StepSize = 0.2;

    double RunTime = 0;

    ofstream Results;

    /* int i = 1; */

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
                                            
                                            for (TailX = (WingX + 0.5 * WingChord + 0.5 * TailChord); TailX <= (FuseLength - 0.5 * TailChord); TailX = (TailX + 0.5 * StepSize)) {
                                                for (JetX = 0; JetX <= FuseLength; JetX = (JetX + StepSize)) {
                                                    for (PassengerX = 0; PassengerX <= FuseLength; PassengerX = (PassengerX + StepSize)) {
                                                        for (JetY = -0.5; JetY <= 0.5; JetY = (JetY + StepSize)) {

                                                            WeightMoment = WingX * WingWeight + (FuseLength / 2) * FuseLength + TailX * TailWeight + JetX * JetWeight;
                                                            
                                                            /*
                                                            LiftMaxMoment = WingX * MaxWingLift + TailX * MaxTailLift; see velmax comment
                                                            */

                                                            LiftOff = false;

                                                            for (Time = 0; Time <= TimeMax and not(LiftOff); Time = Time + StepSize) {
                                                                
                                                                Vel = (Time / TimeMax) * VelMax;
                                                                TailLift = TailTakeOffCl*0.5*Rho*Vel*Vel*TailArea;
                                                                WingLift = WingTakeOffCl*0.5*Rho*Vel*Vel*WingArea;
                                                                TotalLift = TailLift + WingLift;

                                                                if (TotalLift > Weight) {
                                                                    TakeOffDist = 0.5 * Acc * Time * Time;

                                                                    
                                                                    
                                                                    if (TakeOffDist < Runway) {
                                                                        LiftOff = true;
                                                                                                                                                
                                                                        LiftMoment = WingX * WingLift + TailX * TailLift;
                                                                        JetMoment = JetY * Force;
                                                                        
                                                                        TakeOffMoment = ((LiftMoment + JetMoment) / WeightMoment); 
                                                                        
                                                                        if (TakeOffMoment > 0) {

                                                                            
                                                                            WingTakeOffDrag = WingCd*0.5*Rho*Vel*Vel*WingArea;
                                                                            TailTakeOffDrag = TailCd*0.5*Rho*Vel*Vel*TailArea;
                                                                            TotalTakeOffDrag = WingTakeOffDrag + TailTakeOffDrag;

                                                                            TakeOffAngle = (asin((Force - TotalTakeOffDrag) / Weight) * (180 / M_PI));


                                                                            if ((CruiseAngle >= 0) and (CruiseAngle <= MaxAngle)) {

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

                                                                                    BestTakeOffTime = Time;
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
    }

    RunTime = (double)(clock() - tStart)/CLOCKS_PER_SEC;

    Results.open ("ResultsV1.0.txt");
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
        Results << "Time Taken For Program To Run " << RunTime << " seconds \n";
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
        cout << "Time Taken For Program To Run " << RunTime << " seconds \n";
    }
    Results.close();

    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;

}