/* Fortran 2008 port of TA16; S.K. Morley, Nov. 2021
 * A data-based RBF model driven by interplanetary and ground-based data
 * Ported from version dated 10/19/2016, including the correction of Nov. 2021
 * which was found during the porting process.
 * This model is based on fitting to a data set with 732,746 records
 * References: (1) Tsyganenko, N. A., and V. A. Andreeva (2016), "An empirical RBF model of the magnetosphere
 *                 parameterized by interplanetary and ground-based drivers", v.121, doi:10.1002/2016JA023217,
 *                 accepted by JGRA, 10/17/2016.
 *             (2) Andreeva, V. A., and N. A. Tsyganenko (2016), "Reconstructing the magnetosphere from data 
 *                 using radial basis functions, JGRA Space Physics, v.121, 2249-2263, doi:10.1002/2015JA022242.
 * ORIGINAL CODE BY: N. A. Tsyganenko AND V. A. Andreeva
 *------------------------------------------------------------------------------------------------------
 * NOTE: THE MODEL IS VALID ONLY UP TO Xgsw=-15 Re and should NOT be used tailward of that distance
 *------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Lgm/Lgm_MagModelInfo.h"
#include "Lgm/Lgm_TA2016.h"

static double Lgm_TA16_As[22] = {
    12.544, -0.194, 0.305, 0.0573, 2.178,
    0.0571, -0.999, 16.473, 0.00152, 0.382, 0.0, 0.0, 
    -0.210, 0.0, 0.0, -0.636, -2.600, 0.832, -5.328, 
    1.103, -0.907, 1.450};


void Lgm_Init_TA16(LgmTA16_Info *ta){
    ta->SetupDone = FALSE;
    ta->lenA = 23329;  //23328 parameter values, 1-based indexing
    ta->lenL = 1297;  // max val of L in fortran code is 1296
    LGM_ARRAY_1D( ta->A,   ta->lenA, double );
    LGM_ARRAY_1D( ta->XX,   ta->lenL, double );
    LGM_ARRAY_1D( ta->YY,   ta->lenL, double );
    LGM_ARRAY_1D( ta->ZZ,   ta->lenL, double );
    LGM_ARRAY_1D( ta->ST,   ta->lenL, double );
    LGM_ARRAY_1D( ta->RHO,   ta->lenL, double );
    LGM_ARRAY_1D( ta->ZSP,   ta->lenL, double );
    LGM_ARRAY_1D( ta->ZCP,   ta->lenL, double );
    LGM_ARRAY_1D( ta->RHBR,   ta->lenL, double );
    ta->AAlloced = TRUE;
    return;
}


int Lgm_SetCoeffs_TA16(long int Date, double UTC, LgmTA16_Info *Info) {
    FILE *fp; 
    char line[64];
    int idx=1, maxidx=23329;

    if ((fp = fopen("TA16_RBF.par", "r")) != NULL) {
        while (fgets(line, sizeof(line), fp) != NULL) {
            if ( line[0] != '/') {
                sscanf(line, "%lf", Info->A[idx]);
                ++idx;
            }
        }
    }
    fclose(fp);
    return(1);
}

int TA2016_SetGrid(LgmTA16_Info *Info) {
    // initialize the TA2016 coefficient set, grid, etc.
    int Klat, Nlat, Nlon, I, J, K, L=0;
    double D=4.0;
    double Alpha, XXXX, YYYY, ZZZZ, RHighGrid, RLowGrid, X4sq, Y4sq, Z4sq;
    double Xlon, XlonD, Xcolat, XcolatD, sinXco, sinXlo, cosXlo;
    double R0, R1, RH, RM, Rho1, R, RLAST, STS, STT, STN;
    double CN, CS, CTN, CTS, CTT, DN, DS, EN, ES, CP, SP;
    double T, PSIN, PSIS, dLonDeg;
    double B0, F1, PD_TR, PM=0.0;
  
    Klat  = 8;          // Klat IS THE NUMBER OF LATITUDE CIRCLES IN THE NORTHERN HEMISPHERE (EXCLUDING THE POLE)
    RLowGrid  = 3.3;  //  THE INNERMOST SPHERE RADIUS
    RHighGrid =16.0;   //  UPPER LIMIT ON THE RADIUS OF OUTERMOST RBF SPHERE
    // Upper limit on the data tailward limit = 20.0
  
    RH=8.0;        // HINGING PARAMETERS (SEE TAG-2015)
    Alpha=3.0;     // Alpha PARAMETER (SEE TAG-2015)
    Nlat = Klat+1;   // NUMBER OF LATITUDE CIRCLES IN THE N. HEMISPHERE INCLUDING THE POLE
  
    R = RLowGrid;  //  THE INNERMOST SPHERE RADIUS
  
    PD_TR = 0.5;    //  this is just to filter out RBF centers outside the Lin et al model magnetopause
    CTN = cos(Lgm_TA16_As[19]);
    CTS = CTN;
    STN = sin(Lgm_TA16_As[19]);
    STS = STN;
    CN = 0.0;
    CS = 0.0;
  
    for (J=1; J<=100; J++) {   // J COUNTS THE NUMBER OF SPHERES WITH RBF CENTERS
                 // (WHICH IS ACTUALLY MUCH LESS THAN 100)
      for (I=1; I<=Nlat; I++) {     // I COUNTS THE LATITUDE CIRCLES (FROM NORTH POLE DOWN TO EQUATOR)
        XcolatD = (90.0/(Nlat-0.5))*((double)(I)-1.0);  //  COLATITUDE OF THE Ith CIRCLE (in degs)
        Nlon=4*(I-1);                       //  NUMBER OF RBF CENTERS ON THE ItH CIRCLE
        if (I != 1) {
          dLonDeg = 360.0/Nlon;            //  LONGITUDE INTERVAL BETWEEN CENTERS (degs)
        } else {
          Nlon = 1;                         //  NUMBER OF RBF CENTERS ON THE NORTH POLE
          dLonDeg = 0.0;
        }
        for (K=1; K<=Nlon; K++) {
          XlonD = (K-1)*dLonDeg;
          // number below is radian conversion... do with named variable
          Xcolat = XcolatD*0.01745329252;  // COLATITUDE and
          Xlon = XlonD*0.01745329252;      // LONGITUDE OF THE Lth RBF CENTER

          sinXco = sin(Xcolat);
          sinXlo = sin(Xlon);
          cosXlo = cos(Xlon);
          XXXX = R*sinXco*cosXlo;
          YYYY = R*sinXco*sinXlo;
          ZZZZ = R*cos(Xcolat);
          // HERE WE CALCULATE THE DISTANCE FROM THE RBF NODE TO THE LIN ET AL. [2010] MODEL MAGNETOPAUSE.
          // THE RBF NODES LYING OUTSIDE THE BOUNDARY ARE NOT INCLUDED IN THE GRID
          EN = Lgm_TA16_As[21];
          ES = Lgm_TA16_As[21];
  
          X4sq = XXXX*XXXX;
          Y4sq = YYYY*YYYY;
          Z4sq = ZZZZ*ZZZZ;
          R1 = sqrt(X4sq+Y4sq+Z4sq);
          Rho1 = sqrt(Y4sq+Z4sq);

          CTT=XXXX/R1;
          STT = sqrt(Y4sq+Z4sq)/R1;
          T = atan2(STT, CTT);
          if (Rho1 > 1.0e-5) {  // WE ARE NOT ON THE X-AXIS - NO SINGULARITIES TO WORRY ABOUT
            SP = ZZZZ/Rho1;
            CP = YYYY/Rho1;
          } else {                   // WE ARE ON THE X-AXIS
            if (XXXX > 0.0) {     //   IF ON THE DAYSIDE, NO PROBLEM EITHER
              SP = 0.0;
              CP = 1.0;
            } else {                 // WE ARE ON THE TAIL AXIS; TO AVOID SINGULARITY:
              RM = 1000.0;             //  ASSIGN RM=1000 (A CONVENTIONAL SUBSTITUTE VALUE)
              // Raise an error here? Or set bad values and a warning?
              return;               //  AND EXIT
            }
          }
  
          PSIN = acos(CTT*CTN+STT*STN*SP);
          PSIS = acos(CTT*CTS-STT*STS*SP);
  
          DN = Lgm_TA16_As[16];
          DS = Lgm_TA16_As[16];
  
          B0 = Lgm_TA16_As[6];
          F1 = pow(sqrt(0.5*(1.0+CTT))+Lgm_TA16_As[5]*2.0*STT*CTT*(1.0-exp(-T)), B0);
          R0 = pow(Lgm_TA16_As[0]*(PD_TR+PM), Lgm_TA16_As[1]);
          RM = R0*F1+CN*exp(DN*pow(PSIN, EN))+CS*exp(DS*pow(PSIS, ES));    // POSITION OF THE MODEL MAGNETOPAUSE
  
          if (R <= RM) {
            L = L+1;              //  COUNTER OF THE RBF CENTERS
            Info->XX[L] = XXXX;
            Info->YY[L] = YYYY;
            Info->ZZ[L] = ZZZZ;
            Info->ST[L] = sinXco;
            Info->RHO[L] = R*sinXco;
            Info->ZSP[L] = ZZZZ*sinXlo;
            Info->ZCP[L] = ZZZZ*cosXlo;
            Info->RHBR[L] = pow(RH/R*(1.0-(1.0+pow(R/RH, Alpha))), 1.0/Alpha);
          }
        }  // End loop over K
      }  // End loop over I
  
      RLAST = R;
      R = R*(Nlat-0.5+M_PI/4.0)/(Nlat-0.5-M_PI/4.0);  // INCREMENT R BY A FIXED FACTOR
      if (R > RHighGrid) exit(-1);                         // CENTERS CREATED ONLY INSIDE R=RHIGH
    }  // End loop over J
  Info->SetupDone = TRUE ;
}


int TA2016(Lgm_Vector *posGSM, double *PARMOD, double diptilt, Lgm_Vector *BvecGSM, LgmTA16_Info *Info) {
    double posGSM;
    double PARMOD;
    double PS;
    int I;
    Lgm_Vector posSM, BvecSM;
    Lgm_Vector DP, DM, Parr, Marr, PCP, PCM, DCM, DCP, DCMsq, DCPsq;
    Lgm_Vector TCM, TCP;
    Lgm_Vector CTarr, CParr, STarr, SParr;
    double cPS, sPS, tPS, CM, CP;
    double Pdyn, SymV, Xind, ByIMF, FPD;
    double ACP, ACT, AP, ASP,AST, AT, sDT, cDTM1;
    double DCMXY, DCMXZ, DCMYZ, DCPXY, DCPXZ, DCPYZ, DELTA_ZR, DTHETA;
    double D=4.0;
    int x=1, y=2, z=3;
  
    cPS = cos(PS);  // cos(dipole tilt)
    sPS = sin(PS);  // sin(dipole tilt)
    tPS = sPS/cPS;  // tan(dipole tilt)
  
    posSM.x = posGSM->x*cPS - posGSM->z*sPS;  //  RBF EXPANSIONS ARE IN SM COORDINATES
    posSM.y = posGSM->y;                      //  ->  CONVERT X,Y,Z FROM GSW TO SM 
    posSM.z = posGSM->z*cPS + posGSM->x*sPS;
    
    Pdyn  = PARMOD[1];
    SymV  = PARMOD[2]/50.0;  // Sym-H divided by 50
    Xind  = PARMOD[3];
    ByIMF = PARMOD[4];
    FPD = sqrt(Pdyn/2.0)-1.0;
  
    BvecSM.x = 0.0; BvecSM.y = 0.0; BvecSM.z = 0.0;
    for (I=1; I<=1296; I++) {
      Parr.x = Info->XX[I];
      Parr.y = Info->YY[I];
      Parr.z = Info->ZZ[I];
      Marr.x = Parr.x;
      Marr.y = Parr.y;
      Marr.z = Parr.z;
    
      DELTA_ZR = RHBR(I)*tPS;
      DTHETA = -asin(DELTA_ZR)*ST(I);
      sDT = sin(DTHETA);
      cDTM1 = cos(DTHETA)-1.0;
      DP.x = Parr.x*cDTM1+sDT*Info->ZCP[I];
      DP.y = Parr.y*cDTM1+sDT*Info->ZSP[I];
      DP.z = Parr.z*cDTM1-Info->RHO[I]*sDT;
      DM.x = Marr.x*cDTM1-sDT*Info->ZCP[I];
      DM.y = Marr.y*cDTM1-sDT*Info->ZSP[I];
      DM.z = Marr.z*cDTM1-Info->RHO[I]*sDT;
    
      CP = sqrt(pow(posSM.x-Parr.x-DP.x, 2) + pow(posSM.y-Parr.y-DP.y, 2) + pow(posSM.z-Parr.z-DP.z, 2) + D*D);    // RBF Ch_i+
      CM = sqrt(pow(posSM.x-Marr.x-DM.x, 2) + pow(posSM.y-Marr.y-DM.y, 2) + pow(posSM.z-Marr.z-DM.z, 2) + D*D);    // RBF Ch_i-

      Lgm_VecSub(&DCP, &posSM, &Parr);
      Lgm_VecSub(&DCP, &DCP, &DP);
      Lgm_VecDivideScalar(&DCP, CP);
      Lgm_VecSub(&DCM, &posSM, &Marr);
      Lgm_VecSub(&DCM, &DCP, &DM);
      Lgm_VecDivideScalar(&DCM, CM);

      Lgm_VecMult(&DCPsq, &DCP, &DCP);
      Lgm_VecDivideScalar(&DCPsq, CP);
      DCPsq.x = 1.0/CP - DCPsq.x;
      DCPsq.y = 1.0/CP - DCPsq.y;
      DCPsq.z = 1.0/CP - DCPsq.z;
      Lgm_VecMult(&DCMsq, &DCM, &DCM);
      Lgm_VecDivideScalar(&DCMsq, CM);
      DCMsq.x = 1.0/CM - DCMsq.x;
      DCMsq.y = 1.0/CM - DCMsq.y;
      DCMsq.z = 1.0/CM - DCMsq.z;
      DCPXY = -DCP.x*DCP.y/CP;
      DCPXZ = -DCP.x*DCP.z/CP;
      DCPYZ = -DCP.y*DCP.z/CP;
      DCMXY = -DCM.x*DCM.y/CM;
      DCMXZ = -DCM.x*DCM.z/CM;
      DCMYZ = -DCM.y*DCM.z/CM;
   
      TCP.x = posSM.z*DCP.y - posSM.y*DCP.z;
      TCP.y = posSM.x*DCP.z - posSM.z*DCP.x;
      TCP.z = posSM.y*DCP.x - posSM.x*DCP.y;
      TCM.x = posSM.z*DCM.y - posSM.y*DCM.z;
      TCM.y = posSM.x*DCM.z - posSM.z*DCM.x;
      TCM.z = posSM.y*DCM.x - posSM.x*DCM.y;
    
      PCP.x = 2.0*DCP.x-posSM.x*(DCPsq.y+DCPsq.z)+posSM.y*DCPXY+posSM.z*DCPXZ;
      PCP.y = 2.0*DCP.y-posSM.y*(DCPsq.x+DCPsq.z)+posSM.z*DCPYZ+posSM.x*DCPXY;
      PCP.z = 2.0*DCP.z-posSM.z*(DCPsq.x+DCPsq.y)+posSM.x*DCPXZ+posSM.y*DCPYZ;
      PCM.x = 2.0*DCM.x-posSM.x*(DCMsq.y+DCMsq.z)+posSM.y*DCMXY+posSM.z*DCMXZ;
      PCM.y = 2.0*DCM.y-posSM.y*(DCMsq.x+DCMsq.z)+posSM.z*DCMYZ+posSM.x*DCMXY;
      PCM.z = 2.0*DCM.z-posSM.z*(DCMsq.x+DCMsq.y)+posSM.x*DCMXZ+posSM.y*DCMYZ;

      Lgm_VecAdd(&CTarr, &TCP, &TCM); 
      Lgm_VecMultiplyScalar(&CTarr, cPS);
      Lgm_VecSub(&STarr, &TCP, &TCM); 
      Lgm_VecMultiplyScalar(&CTarr, sPS);
      Lgm_VecSub(&CParr, &PCP, &PCM); 
      Lgm_VecMultiplyScalar(&CParr, cPS);
      Lgm_VecAdd(&SParr, &PCP, &PCM); 
      Lgm_VecMultiplyScalar(&SParr, sPS);
    
    // -----------------   TOTAL FIELD:    -----------------------------------
      ACT = Info->A[I]+Info->A[I+5184]*FPD+Info->A[I+10368]*SymV+Info->A[I+15552]*Xind;
      AST = Info->A[I+1296]+Info->A[I+6480]*FPD+Info->A[I+11664]*SymV+Info->A[I+16848]*Xind;
      AT  = Info->A[I+20736]*ByIMF;
      ACP = Info->A[I+2592]+Info->A[I+7776]*FPD+Info->A[I+12960]*SymV+Info->A[I+18144]*Xind;
      ASP = Info->A[I+3888]+Info->A[I+9072]*FPD+Info->A[I+14256]*SymV+Info->A[I+19440]*Xind;
      AP  = Info->A[I+22032]*ByIMF;
      BvecSM.x = BvecSM.x + CTarr.x*ACT + STarr.x*AST + (TCP.x-TCM.x)*AT + CParr.x*ACP + SParr.x*ASP + (PCP.x+PCM.x)*AP;
      BvecSM.y = BvecSM.y + CTarr.y*ACT + STarr.y*AST + (TCP.y-TCM.y)*AT + CParr.y*ACP + SParr.y*ASP + (PCP.y+PCM.y)*AP;
      BvecSM.z = BvecSM.z + CTarr.z*ACT + STarr.z*AST + (TCP.z-TCM.z)*AT + CParr.z*ACP + SParr.z*ASP + (PCP.z+PCM.z)*AP;
    }
    BvecGSM = SM_to_GSM(BvecSM, cPS, sPS);
    return(1);
}

int Lgm_B_TA16( Lgm_Vector *rGSM, Lgm_Vector *B, Lgm_MagModelInfo *Info ) {
    // Evaluate Tsyganenko & Andreeva 2016 model
    Lgm_Vector      Bext, Bint;
    double          PARMOD[11];
    int             retval;

    retval = TA2016(rGSM, PARMOD, Info->c->psi, &B, &Info->TA16_Info);
    switch ( Info->InternalModel ){
        case LGM_CDIP:
                        Lgm_B_cdip(rGSM, &Bint, Info);
                        break;
        case LGM_EDIP:
                        Lgm_B_edip(rGSM, &Bint, Info);
                        break;
        case LGM_IGRF:
                        Lgm_B_igrf(rGSM, &Bint, Info);
                        break;
        default:
                        fprintf(stderr, "Lgm_B_TA16: Unknown internal model (%d)\n", Info->InternalModel );
                        break;
    }

    // Add requested internal field model to obtain total field
    B->x = Bint.x + Bext.x;
    B->y = Bint.y + Bext.y;
    B->z = Bint.z + Bext.z;

    ++Info->nFunc;  // increment counter of function calls

    return(1);

}
