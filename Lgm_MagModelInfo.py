"""
Lgm_MagModelInfo module, this contains the necessary code Mag Model Info

the ugly top part is
Generated by h2py from ../libLanlGeoMag/Lgm/Lgm_MagModelInfo.h
@todo: go through and remove the bits we don't need

@author: Brian Larsen
@organization: LANL
@contact: balarsen@lanl.gov

@version: V1: 20-Dec-2010 (BAL)
"""


###############################################################################
###############################################################################
###############################################################################

# Included from math.h
LGM_ELECTRON_MASS = (9.10938188e-31)
LGM_AMU = (1.660538e-27)
RE = (6378.135e3)
LGM_CC = (2.99792458e8)
LGM_EE = (1.6022e-19)
TRUE = 1
FALSE = 0
LGM_OPEN_IMF = 0
LGM_CLOSED = 1
LGM_OPEN_N_LOBE = 2
LGM_OPEN_S_LOBE = 3
LGM_INSIDE_EARTH = -1
LGM_TARGET_HEIGHT_UNREACHABLE = -2
LGM_MAGSTEP_KMAX = 9
LGM_MAGSTEP_IMAX = (LGM_MAGSTEP_KMAX+1)
LGM_MAGSTEP_JMAX = (LGM_MAGSTEP_KMAX+2)
LGM_MAGSTEP_REDMAX = 1.0e-5
LGM_MAGSTEP_REDMIN = 0.7
LGM_MAGSTEP_SCLMAX = 0.1
LGM_MAGSTEP_SAFE1 = 0.25
LGM_MAGSTEP_SAFE2 = 0.70
DQAGS = 0
DQAGP = 1
DQK21 = 2
LINEAR = 0
LINEAR_DFI = 1
QUADRATIC = 2
QUADRATIC_DFI = 3
NEWTON_INTERP = 4
LGM_CDIP = 0
LGM_EDIP = 1
LGM_IGRF = 2
LGM_MAX_INTERP_PNTS = 10000
LGM_RELATIVE_JUMP_METHOD = 0
LGM_ABSOLUTE_JUMP_METHOD = 1


###############################################################################
###############################################################################
###############################################################################

import Lgm

import ctypes
from _Lgm import lib
from Lgm_Types import LgmDouble, LgmInt, LgmLong, c_types, c_sizes, LgmUInt, \
        LgmDoubleP, ConstLgmCharP
import Lgm_Vector
from Lgm_CTrans import Lgm_CTrans, Lgm_CTransP
from _Lgm_Octree import Lgm_OctreeCellP


# size_t is either unisigned int or unsigned long (or maybe unsigned short)
# in the Lgm lib is a funstion to tell us
size_t_size = lib.size_t_size()
SIZE_T = c_types[c_sizes.index(size_t_size)]
SIZE_TP = ctypes.POINTER(SIZE_T)

# from gsl gsl_interp.h
class gsl_interp_accel(ctypes.Structure):
    # @classmethod
    # def assign_fields(cls):
    #    cls.
    _fields_ = [ ("cache", SIZE_T), #
        ("miss_count", SIZE_T), #
        ("hit_count", SIZE_T) ]
gsl_interp_accelP = ctypes.POINTER(gsl_interp_accel)




class gsl_interp_type(ctypes.Structure):
    _fields_ = [ ("name", ConstLgmCharP), #
        ("min_size", LgmUInt), #
        ("alloc", ctypes.c_void_p), #
        ("init", ctypes.POINTER(ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                                 LgmDoubleP, LgmDoubleP, SIZE_T))), #
        ("eval", ctypes.POINTER(ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                                 LgmDoubleP, LgmDoubleP, SIZE_T,
                                                 LgmDouble, gsl_interp_accelP,
                                                 LgmDoubleP))), #
        ("eval_deriv", ctypes.POINTER(ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                                       LgmDoubleP, LgmDoubleP,
                                                       SIZE_T, LgmDouble,
                                                       gsl_interp_accelP, LgmDoubleP))), #
        ("eval_deriv2", ctypes.POINTER(ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                                       LgmDoubleP, LgmDoubleP,
                                                       SIZE_T, LgmDouble,
                                                       gsl_interp_accelP, LgmDoubleP))), #
        ("eval_integ", ctypes.POINTER(ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p,
                                                       LgmDoubleP, LgmDoubleP, SIZE_T,
                                                       gsl_interp_accelP, LgmDouble,
                                                       LgmDouble, LgmDoubleP))), #
        ("free", ctypes.c_void_p) ]
gsl_interp_typeP = ctypes.POINTER(gsl_interp_type)



class gsl_interp(ctypes.Structure):
    _fields_ = [ ("type", gsl_interp_typeP), #
        ("xmin", LgmDouble), #
        ("xmax", LgmDouble), #
        ("size", SIZE_T), #
        ("state", ctypes.c_void_p) ]
gsl_interpP = ctypes.POINTER(gsl_interp)

class gsl_spline(ctypes.Structure):
    # @classmethod
    # def assign_fields(cls):
    #    cls.
    _fields_ = [ ("interp", gsl_interpP), #
        ("x", LgmDoubleP), #
        ("y", LgmDoubleP), #
        ("size", SIZE_T) ]
gsl_splineP = ctypes.POINTER(gsl_spline)




class Lgm_MagModelInfo(ctypes.Structure):
    """
    LanlGeoMag class for holing Mag Model infor needed by all models

    @author: Brian Larsen
    @organization: LANL
    @contact: balarsen@lanl.gov

    @version: V1: 22-Dec-2010 (BAL)
    """
    def __init__(self):
        """
        Set the default values from Lgm
        """
        self.InternalModel = LGM_IGRF
        c_tmp = Lgm_CTrans()
        self.c     = ctypes.pointer(c_tmp)
        self.Kp    = 5
        self.P     = 2.1 # SW pressure in nPa
        self.nFunc = 0
        self.B0    = 1.00   # Should nominally be 1.0 See page 30 of Schultz and Lanzerotti, [1974]
        self.B1    = 0.8100 # See page 30 of Schultz and Lanzerotti, [1974]
        self.B2    = 0.4065 # See page 30 of Schultz and Lanzerotti, [1974]
        self.SavePoints = 0
        self.Hmax       = 1.0
        self.UseInterpRoutines = TRUE
        lib.Lgm_Set_Open_Limits( self, -80.0, 30.0, -40.0, 40.0, -40.0, 40.0 )
        self.Lgm_I_integrand_JumpMethod = LGM_ABSOLUTE_JUMP_METHOD
        #
        #  Some inits for MagStep
        #
        self.Lgm_MagStep_FirstTimeThrough = TRUE
        self.Lgm_MagStep_eps_old          = -1.0
        #
        # Set some default tolerances
        #
        self.Lgm_MagFlux_Integrator_epsabs = 0.0
        self.Lgm_MagFlux_Integrator_epsrel = 1e-5

        self.Lgm_LambdaIntegral_Integrator_epsabs = 0.0
        self.Lgm_LambdaIntegral_Integrator_epsrel = 1e-3

        self.Lgm_I_Integrator_epsrel = 0.0
        self.Lgm_I_Integrator_epsabs = 1e-3
        self.Lgm_I_Integrator = DQAGS

        self.Lgm_Sb_Integrator_epsrel = 0.0
        self.Lgm_Sb_Integrator_epsabs = 1e-3
        self.Lgm_Sb_Integrator = DQAGP # not changeable (yet...)

        self.Lgm_FindBmRadius_Tol = 1e-10
        self.Lgm_FindShellLine_I_Tol = 1e-3
        self.Lgm_TraceToMirrorPoint_Tol = 1e-7

        #
        # Bounce Loss Cone Height
        #
        self.Lgm_LossConeHeight = 100.0 # km above the Earth Ellipsoid.

        #
        # Inits for OP77
        #
        self.OP77_TILTL = 99.0

    @classmethod
    def assign_fields(cls):
        cls._fields_ = [ \
        ("c", Lgm_CTransP),
        ("nFunc", LgmLong),
        ("Bfield", ctypes.POINTER(ctypes.CFUNCTYPE(ctypes.c_int))),
        ("SavePoints", LgmInt),
        ("Hmax", LgmDouble),
        ("fp", ctypes.c_void_p),
        ("W", LgmDouble * 6),
        ("G1", LgmDouble),
        ("G2", LgmDouble),
        ("Kp", LgmInt),
        ("Dst", LgmDouble),
        ("P", LgmDouble),
        ("Bx", LgmDouble),
        ("By", LgmDouble),
        ("Bz", LgmDouble),
        ("T96MOD_V", LgmDouble*11),
        ("Trace_s", LgmDouble),
        ("B0", LgmDouble),
        ("B1", LgmDouble),
        ("B2", LgmDouble),
        ("InternalModel", LgmInt),
        ("KineticEnergy", LgmDouble),
        ("Mass", LgmDouble),
        ("PitchAngle", LgmDouble),
        ("Pm_South", Lgm_Vector.Lgm_Vector),
        ("Pm_North", Lgm_Vector.Lgm_Vector),
        ("Bm", LgmDouble),
        ("Sm_South", LgmDouble),
        ("Sm_North", LgmDouble),
        ("Blocal", LgmDouble),
        ("FirstCall", LgmInt),
#        ("epsabs", LgmDouble), # commented out in Lgm c code as of 10Jen2011
#        ("epsrel", LgmDouble), # commented out in Lgm c code as of 10Jen2011
        # Arrays containing FL vals
        ("s", LgmDouble * LGM_MAX_INTERP_PNTS), # distance along FL
        ("Px", LgmDouble * LGM_MAX_INTERP_PNTS), # Px along FL  (in GSM)
        ("Py", LgmDouble * LGM_MAX_INTERP_PNTS), # Py along FL  (in GSM)
        ("Pz", LgmDouble * LGM_MAX_INTERP_PNTS), # Pz along FL  (in GSM)
        ("Bvec", Lgm_Vector.Lgm_Vector * LGM_MAX_INTERP_PNTS), # 3D B-field vector   (in GSM)
        ("Bmag", LgmDouble * LGM_MAX_INTERP_PNTS), # magnitude of B
        ("BminusBcdip", LgmDouble * LGM_MAX_INTERP_PNTS), # magnitude of B minus magnitude of Cent. Dipole
        ("nPnts", LgmInt), # actual number of points defined
        ("ds", LgmDouble), # spacing in s (dist. along FL)
                                    # this will help in seacrhing the
                                    # arrays (e.g. for interpolation).
        ("P_gsm", Lgm_Vector.Lgm_Vector), # S/C position in GSM
        ("S", LgmDouble), # Distance along FL from southern footpoint to S/C location in Re.
        ("B", LgmDouble), # Local (model) B-field magnitude (i.e. at S/C position)
        ("Pmin", Lgm_Vector.Lgm_Vector), # position of minimum |B| in GSM
        ("Bvecmin", Lgm_Vector.Lgm_Vector), # value of Bvecmin
        ("Bmin", LgmDouble), # Value of |Bmin|
        ("Smin", LgmDouble), # Distance from southern footpoint to Pmin along FL.
        ("Spherical_Footprint_Pn", Lgm_Vector.Lgm_Vector), # position of northern footpoint (at 120km)
        ("Spherical_Footprint_Sn", LgmDouble), # Distance along FL from southern foorpoint in Re
        ("Spherical_Footprint_Bn", LgmDouble), # Value of |B| at Footprint_Pn

        ("Spherical_Footprint_Ps", Lgm_Vector.Lgm_Vector), # position of southern footpoint (at 120km)
        ("Spherical_Footprint_Ss", LgmDouble), # Distance along FL from southern foorpoint in Re (i.e. this one is zero by definition)
        ("Spherical_Footprint_Bs", LgmDouble), # Value of |B| at Footprint_Ps
        ("Ellipsoid_Footprint_Pn", Lgm_Vector.Lgm_Vector), # position of northern footpoint (at 120km above surface of WGS84 ellipsoid)
        ("Ellipsoid_Footprint_Sn", LgmDouble), # Distance along FL from southern footpoint in Re
        ("Ellipsoid_Footprint_Bn", LgmDouble), # Value of |B| at Footprint_Pn
        ("Ellipsoid_Footprint_Ps", Lgm_Vector.Lgm_Vector), # position of southern footpoint (at 120km)
        ("Ellipsoid_Footprint_Ss", LgmDouble), # Distance along FL from southern foorpoint in Re (i.e. this one is zero by definition)
        ("Ellipsoid_Footprint_Bs", LgmDouble), # Value of |B| at Footprint_Ps
        ("FieldLineType", LgmInt), # Field line type. (I.e., LGM_OPEN_IMF, LGM_CLOSED, LGM_OPEN_N_LOBE,
                                    # LGM_OPEN_S_LOBE, LGM_INSIDE_EARTH, LGM_TARGET_HEIGHT_UNREACHABLE)
        ("d2B_ds2", LgmDouble), #second derivative of B wrt to s at smin.
        ("Sb0", LgmDouble), #value of Sb integral for eq. mirroring particles.
        ("imin1", LgmInt), #imin1 and imin2 are the indices in the
        ("imin2", LgmInt), #array between which smin is located.
        ("acc", gsl_interp_accelP), #accelerator
        ("accPx", gsl_interp_accelP), #accelerator
        ("accPy", gsl_interp_accelP), #accelerator
        ("accPz", gsl_interp_accelP), #accelerator
        ("spline", gsl_splineP), #spline object
        ("splinePx", gsl_splineP), #spline object
        ("splinePy", gsl_splineP), #spline object
        ("splinePz", gsl_splineP), #spline object

        #Other stuff
        ("VerbosityLevel", LgmInt), #VerbosityLevel
        ("UseInterpRoutines", LgmInt), #whether to use fast I and Sb routines.
        # These variables are needed to make Lgm_MagStep() reentrant/thread-safe.
            #  They basically used to be static declarations within Lgm_MagStep()
        ("Lgm_MagStep_eps_old", LgmDouble), #
        ("Lgm_MagStep_FirstTimeThrough", LgmInt), #
        ("Lgm_MagStep_kmax", LgmInt), #
        ("Lgm_MagStep_kopt", LgmInt), #
        ("Lgm_MagStep_snew", LgmDouble), #
        ("Lgm_MagStep_A", LgmDouble * (LGM_MAGSTEP_JMAX+1)), #
        ("Lgm_MagStep_alpha", LgmDouble * (LGM_MAGSTEP_JMAX+1) * (LGM_MAGSTEP_JMAX+1) ), #
        ("Lgm_MagStep_d", LgmDouble * LGM_MAGSTEP_JMAX * LGM_MAGSTEP_JMAX ), #
        ("Lgm_MagStep_x", LgmDouble * LGM_MAGSTEP_JMAX), #
        # These variables are needed to make I_integrand() reentrant/thread-safe.
            #   They basically used to be static declarations.
        ("Lgm_I_integrand_FirstCall", LgmInt), #
        ("Lgm_I_integrand_JumpMethod", LgmInt), #
        ("Lgm_I_integrand_S", LgmDouble), #
        ("Lgm_I_integrand_P", Lgm_Vector.Lgm_Vector), #
        ("Lgm_I_integrand_u_scale", Lgm_Vector.Lgm_Vector), #
        ("Lgm_n_I_integrand_Calls", LgmInt), #
        ("Lgm_I_Integrator", LgmInt), #
        ("Lgm_I_Integrator_epsrel", LgmDouble), #
        ("Lgm_I_Integrator_epsabs", LgmDouble), #
        # These variables are needed to make Sb_integrand() reentrant/thread-safe.
            #  They basically used to be static declarations.
        ("Lgm_Sb_integrand_FirstCall", LgmInt), #
        ("Lgm_Sb_integrand_S", LgmDouble), #
        ("Lgm_Sb_integrand_P", Lgm_Vector.Lgm_Vector), #
        ("Lgm_Sb_integrand_u_scale", Lgm_Vector.Lgm_Vector), #
        ("Lgm_n_Sb_integrand_Calls", LgmInt), #
        ("Lgm_Sb_Integrator", LgmInt), # This variable is not currently used (for now I force
                                                # Sb to use DQAGP. )
        ("Lgm_Sb_Integrator_epsrel", LgmDouble), #
        ("Lgm_Sb_Integrator_epsabs", LgmDouble), #
        # Variables to control MagFlux integration
        ("Lgm_MagFlux_Integrator", LgmInt), # This variable is not currently used (for now I force
                                                # MagFlux to use DQAGS. )
        ("Lgm_MagFlux_Integrator_epsrel", LgmDouble), #
        ("Lgm_MagFlux_Integrator_epsabs", LgmDouble), #
        # Variables to control LambdaIntegral integration
        ("Lgm_LambdaIntegral_Integrator", LgmInt), # This variable is not currently used (for now I force
                                                #  LambdaIntegral to use DQAGS. )
        ("Lgm_LambdaIntegral_Integrator_epsrel", LgmDouble), #
        ("Lgm_LambdaIntegral_Integrator_epsabs", LgmDouble), #
        # Some other tolerances
        ("Lgm_FindBmRadius_Tol", LgmDouble), #
        ("Lgm_FindShellLine_I_Tol", LgmDouble), #
        ("Lgm_TraceToMirrorPoint_Tol", LgmDouble), #
        # Variables for defining Octree stuff
        ("OctreeRoot", Lgm_OctreeCellP), #
        ("Octree_kNN_k", LgmInt), #
        ("Octree_kNN_InterpMethod", LgmInt), #
        ("Octree_kNN_MaxDist", LgmDouble), #
        ("OctreeScaleMin", LgmDouble), #
        ("OctreeScaleMax", LgmDouble), #
        ("OctreeScaleDiff", LgmDouble), #
        # limits for tracing. If you go outside this box, we consider it open
        ("OpenLimit_xMin", LgmDouble), #
        ("OpenLimit_xMax", LgmDouble), #
        ("OpenLimit_yMin", LgmDouble), #
        ("OpenLimit_yMax", LgmDouble), #
        ("OpenLimit_zMin", LgmDouble), #
        ("OpenLimit_zMax", LgmDouble), #
        ("Lgm_LossConeHeight", LgmDouble), #
        # Globals for OP77 Model
        ("OP77_TILTL", LgmDouble), #
        ("OP77_A", LgmDouble*65), #
        ("OP77_B", LgmDouble*65), #
        ("OP77_C", LgmDouble*45), #
        ("OP77_D", LgmDouble*45), #
        ("OP77_E", LgmDouble*65), #
        ("OP77_F", LgmDouble*65), #
        ("OP77_TT", LgmDouble*5)] #
