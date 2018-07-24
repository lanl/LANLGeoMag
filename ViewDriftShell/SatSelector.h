#ifndef SATSELECTOR_H
#define SATSELECTOR_H

#include "support.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include <GL/gl.h>





#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <Lgm/Lgm_Vec.h>
#include <Lgm/Lgm_Sgp.h>
//#include "Objects.h"
#include <Lgm_Objects.h>
typedef struct _SpaceObjectItem {

    _SgpTLE TLE;

    int     ObjectType;                 // TBD, e.g. R/B, DEB, Active Sat, NonActive Sat, etc...

    double  x, y, z;                    // save current x, y, z so we dont have to recompute it more than needed..

    int     Draw;                       // whether to draw anything related to this object (means for filtering out
                                        //  DEB and R/B, etc..) Better to set the flag once than have to re-determine everywhere.



    int     DrawLabel;                          // whether to draw the label

    int     DrawSatToGroundLine;                // whether to draw a line from the sat to the ground 
    float   ssglRed, ssglGrn, ssglBlu, ssglAlf; // Color of ground path.

    int     DrawSatFieldLines;                  // controls drawing Field lines from sat position.
    float   ssflRed, ssflGrn, ssflBlu, ssflAlf; // Color of field lines.

    int     DrawSatFLFootpoints;                // controls drawing Footpoints from Sat position.
    float   ssfpRed, ssfpGrn, ssfpBlu, ssfpAlf; // Color of footpoints.






    int     DrawOrbit;                      // whether to draw full orbit
    float   oRed, oGrn, oBlu, oAlf;         // Color of Orbit
    double  oPeriodFrac;                    // Fraction of an orbital period to show
                                            // (centered on current position)
    int     DrawGroundPathOfOrbit;          // whether to draw ground path of orbit
    float   ogpRed, ogpGrn, ogpBlu, ogpAlf; // Color of ground path.

    int     DrawOrbitToGroundLines;         // whether to draw lines from the sat to the ground 
    float   oglRed, oglGrn, oglBlu, oglAlf; // Color of ground path.

    int     DrawOrbitFieldLines;            // controls drawing Field lines from orbit positions.
    float   oflRed, oflGrn, oflBlu, oflAlf; // Color of field lines.

    int     DrawOrbitFLFootpoints;          // controls drawing Footpoints from orbit positions.
    float   ofpRed, ofpGrn, ofpBlu, ofpAlf; // Color of footpoints.







    int     DrawStreak;                     // whether to draw streaks
    float   sRed, sGrn, sBlu, sAlf;         // Color of Streak
    double  sPeriodFrac;                    // Fraction of an orbital period to show as a streak

    int     DrawGroundPathOfStreak;         // whether to draw ground path of orbit
    float   sgpRed, sgpGrn, sgpBlu, sgpAlf; // Color of ground path.

    int     DrawStreakToGroundLines;        // whether to draw lines from the sat to the ground 
    float   sglRed, sglGrn, sglBlu, sglAlf; // Color of ground path.


    int     DrawStreakFieldLines;           // controls drawing Field lines from Streak positions.
    float   sflRed, sflGrn, sflBlu, sflAlf; // Color of field lines.

    int     DrawStreakFLFootpoints;         // controls drawing Footpoints from Streak positions.
    float   sfpRed, sfpGrn, sfpBlu, sfpAlf; // Color of footpoints.









    // not implemented yet...
    int     DrawSatToEqPlaneLines;          // whether to draw a line from the sat to Eq Plane
    float   selRed, selGrn, selBlu, selAlf; // Color of ground path.




    int     UsePointSprites;        // if FALSE, use dots





} _SpaceObjectItem;



typedef struct _SpaceObjects {

    _SpaceObjectItem    *Sat;
    int                 DrawGroup;              // whether to draw this group
    int                 DrawSatellites;         // whether to draw satellites
    int                 DrawRocketBodies;       // whether to draw rocket bodies
    int                 DrawDebris;             // whether to draw debris
    int                 DrawAsPointSprites;     // whether to draw as a point sprite or as a dot
    int                 DrawPosition;           // whether to draw the current position

    int                 OverRideSatColors;      // whether to override the nominal colors for sats
    int                 OverRideOrbitColors;    // whether to override the nominal colors for sats
    int                 OverRideStreakColors;   // whether to override the nominal colors for sats

    float               SatRed, SatGrn, SatBlu, SatAlf;
    float               RbRed,  RbGrn,  RbBlu, RbAlf;
    float               DebRed, DebGrn, DebBlu, DebAlf;
    int                 nSat;


    // OverRide Colors (per-group)
    float   SatRed_OverRide,  SatGrn_OverRide,  SatBlu_OverRide,  SatAlf_OverRide;  // Sat
    float   ssglRed_OverRide, ssglGrn_OverRide, ssglBlu_OverRide, ssglAlf_OverRide; // Single Sat to ground line
    float   ssflRed_OverRide, ssflGrn_OverRide, ssflBlu_OverRide, ssflAlf_OverRide; // Single Field Line
    float   ssfpRed_OverRide, ssfpGrn_OverRide, ssfpBlu_OverRide, ssfpAlf_OverRide; // Single Footpoint

    float   oRed_OverRide,   oGrn_OverRide,   oBlu_OverRide,   oAlf_OverRide;       // Orbit
    float   ogpRed_OverRide, ogpGrn_OverRide, ogpBlu_OverRide, ogpAlf_OverRide;     // Orbit ground path
    float   oglRed_OverRide, oglGrn_OverRide, oglBlu_OverRide, oglAlf_OverRide;     // Orbit to ground lines
    float   oflRed_OverRide, oflGrn_OverRide, oflBlu_OverRide, oflAlf_OverRide;     // Orbit Field Lines
    float   ofpRed_OverRide, ofpGrn_OverRide, ofpBlu_OverRide, ofpAlf_OverRide;     // Orbit Footpoint Path
     
    float   sRed_OverRide,   sGrn_OverRide,   sBlu_OverRide,   sAlf_OverRide;       // Streak
    float   sgpRed_OverRide, sgpGrn_OverRide, sgpBlu_OverRide, sgpAlf_OverRide;     // Streak ground path
    float   sglRed_OverRide, sglGrn_OverRide, sglBlu_OverRide, sglAlf_OverRide;     // Streak to ground lines
    float   sflRed_OverRide, sflGrn_OverRide, sflBlu_OverRide, sflAlf_OverRide;     // Streak Field Lines
    float   sfpRed_OverRide, sfpGrn_OverRide, sfpBlu_OverRide, sfpAlf_OverRide;     // Streak Footpoint Path
     



} _SpaceObjects;

_SpaceObjects *SpaceObjects;






/*
 * We want to organize the TLEs in groups -- each of which can have different
 * numbers of entries. We also want to be able to add and remove groups easily.
 * So, we will define a doubley linked list where each node of the list points
 * to a different group.
 */

// Define a "Node" in the linked list
typedef struct _GroupNode {

    _SpaceObjects   *Group;
    char            *GroupName;

    struct _GroupNode      *Prev;
    struct _GroupNode      *Next;
    
} _GroupNode;



typedef struct _SatSelectorInfo {

    _GroupNode  *SatGroupList;
    int          nSatGroups;

    // OverRide colors for Field Lines
    int         OverRideFLColors;
    float       OverRideFLColors_Red;
    float       OverRideFLColors_Grn;
    float       OverRideFLColors_Blu;
    float       OverRideFLColors_Alf;

    // OverRide drawing of FL types
    int         DrawOpenNorthFLs;
    int         DrawOpenSouthFLs;
    int         DrawClosedFLs;
    int         DrawImfFLs;

    // OverRide colors by FL types
    int         OverRideFLTypeColors;
    float       OverRideFLClosedColors_Red;
    float       OverRideFLClosedColors_Grn;
    float       OverRideFLClosedColors_Blu;
    float       OverRideFLClosedColors_Alf;
    float       OverRideFLOpenNorthColors_Red;
    float       OverRideFLOpenNorthColors_Grn;
    float       OverRideFLOpenNorthColors_Blu;
    float       OverRideFLOpenNorthColors_Alf;
    float       OverRideFLOpenSouthColors_Red;
    float       OverRideFLOpenSouthColors_Grn;
    float       OverRideFLOpenSouthColors_Blu;
    float       OverRideFLOpenSouthColors_Alf;
    float       OverRideFLImfColors_Red;
    float       OverRideFLImfColors_Grn;
    float       OverRideFLImfColors_Blu;
    float       OverRideFLImfColors_Alf;

} _SatSelectorInfo;



/*
 * Function Prototypes
 */
void        Add_SatGroupNode( _GroupNode **List, char *GroupName, _SpaceObjects *Group );
int         Delete_SatGroupNode( _GroupNode **List, char *GroupName );
void        InitSatSelectorInfo( );
void        FreeSatSelectorInfo( );
GtkWidget   *CreateSatSelector( int nGroups, char *GroupNames[] );
void        ToggleOrbitOptions( GtkWidget  *w, unsigned int *data );
void        ChangeOrbitOptions( GtkWidget  *w, unsigned int *data );
void        ChangeSatColor( GtkWidget  *button, gpointer data );
void        ChangeIllumFLParams( GtkWidget  *w, unsigned int *data );
int         SatGroupFilter( const struct dirent *d );
int         LoadSpaceObjects( );
void        ReLoadSpaceObjects( );
_SpaceObjects    *New_SpaceObjects();
_SatSelectorInfo *New_SatSelectorInfo();




/*
 * Global defs
 */
#ifdef MAIN
/*
 *  Make aInfo Global. We could get around this,
 *  but it adds complexity.
 */
_SatSelectorInfo *SatSelectorInfo;
#else
extern _SatSelectorInfo *SatSelectorInfo;
extern GtkWidget        *drawing_area;
extern int              ShowSatellites[];
gboolean expose_event( GtkWidget *widget, GdkEventExpose *event, gpointer data);
void ReCreateSats();
void ReCreateSatOrbits();
#endif



#endif
