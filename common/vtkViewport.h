/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkViewport.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


Copyright (c) 1993-1998 Ken Martin, Will Schroeder, Bill Lorensen.

This software is copyrighted by Ken Martin, Will Schroeder and Bill Lorensen.
The following terms apply to all files associated with the software unless
explicitly disclaimed in individual files. This copyright specifically does
not apply to the related textbook "The Visualization Toolkit" ISBN
013199837-4 published by Prentice Hall which is covered by its own copyright.

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.


=========================================================================*/
// .NAME vtkViewport - abstract specification for Viewports
// .SECTION Description
// vtkViewport provides an abstract specification for Viewports. A Viewport
// is an object that controls the rendering process for objects. Rendering
// is the process of converting geometry, a specification for lights, and 
// a camera view into an image. vtkViewport also performs coordinate 
// transformation between world coordinates, view coordinates (the computer
// graphics rendering coordinate system), and display coordinates (the 
// actual screen coordinates on the display device). Certain advanced 
// rendering features such as two-sided lighting can also be controlled.

// .SECTION See Also
// vtkWindow vtkImager vtkRenderer

#ifndef __vtkViewport_h
#define __vtkViewport_h

#include "vtkObject.h"
#include "vtkPropCollection.h"

class vtkWindow;
class vtkActor2DCollection;

class VTK_EXPORT vtkViewport : public vtkObject
{
public:

  // Description:
  // Create a vtkViewport with a black background, a white ambient light, 
  // two-sided lighting turned on, a viewport of (0,0,1,1), and backface 
  // culling turned off.
  vtkViewport();

  ~vtkViewport();
  const char *GetClassName() {return "vtkViewport";};
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Add a prop to the list of props. Prop is the superclass of all 
  // actors, volumes, 2D actors, composite props etc.
  void AddProp(vtkProp *);

  // Description:
  // Return any props in this viewport.
  vtkPropCollection *GetProps() {return this->Props;};

  // Description:
  // Remove an actor from the list of actors.
  void RemoveProp(vtkProp *);

  // Description:
  // Add/Remove different types of props to the renderer.
  // These methods are all synonyms to AddProp and RemoveProp.
  // They are here for convinience and backwards compatability.
  void AddActor2D(vtkProp* p) {this->AddProp(p);};
  void RemoveActor2D(vtkProp* p) {this->RemoveProp(p);};
  vtkActor2DCollection *GetActors2D();

  // Description:
  // Set/Get the background color of the rendering screen using an rgb color
  // specification.
  vtkSetVector3Macro(Background,float);
  vtkGetVectorMacro(Background,float,3);

  // Description:
  // Set the aspect ratio of the rendered image. This is computed 
  // automatically and should not be set by the user.
  vtkSetVector2Macro(Aspect,float);
  vtkGetVectorMacro(Aspect,float,2);

  // Description:
  // Specify the viewport for the Viewport to draw in the rendering window. 
  // Coordinates are expressed as (xmin,ymin,xmax,ymax), where each
  // coordinate is 0 <= coordinate <= 1.0.
  vtkSetVector4Macro(Viewport,float);
  vtkGetVectorMacro(Viewport,float,4);

  // Description:
  // Set/get a point location in display (or screen) coordinates.
  // The lower left corner of the window is the origin and y increases
  // as you go up the screen.
  vtkSetVector3Macro(DisplayPoint,float);
  vtkGetVectorMacro(DisplayPoint,float,3);
  void GetDisplayPoint(double *a) 
    {
      a[0] = this->DisplayPoint[0];
      a[1] = this->DisplayPoint[1];
      a[2] = this->DisplayPoint[2];
      a[3] = 1.0;
    };

  // Description:
  // Specify a point location in view coordinates. The origin is in the 
  // middle of the viewport and it extends from -1 to 1 in all three
  // dimensions.
  vtkSetVector3Macro(ViewPoint,float);
  vtkGetVectorMacro(ViewPoint,float,3);

  // Description:
  // Specify a point location in world coordinates. This method takes 
  // homogeneous coordinates. 
  vtkSetVector4Macro(WorldPoint,float);
  vtkGetVectorMacro(WorldPoint,float,4);
  void GetWorldPoint(double *a) 
    {
      a[0] = this->WorldPoint[0];
      a[1] = this->WorldPoint[1];
      a[2] = this->WorldPoint[2];
      a[3] = this->WorldPoint[3];
    };
  
  
  // Description:
  // Return the center of this viewport in display coordinates.
  virtual float *GetCenter();

  // Description:
  // Is a given display point in this Viewport's viewport.
  virtual int IsInViewport(int x,int y); 

  // Description:
  // Return the vtkWindow that owns this vtkViewport.
  virtual vtkWindow *GetVTKWindow() = 0;

  // Description:
  // Specify a function to be called before rendering process begins.
  // Function will be called with argument provided.
  void SetStartRenderMethod(void (*f)(void *), void *arg);

  // Description:
  // Specify a function to be called when rendering process completes.
  // Function will be called with argument provided.
  void SetEndRenderMethod(void (*f)(void *), void *arg);

  // Description:
  // Set the arg delete method. This is used to free user memory.
  void SetStartRenderMethodArgDelete(void (*f)(void *));

  // Description:
  // Set the arg delete method. This is used to free user memory.
  void SetEndRenderMethodArgDelete(void (*f)(void *));

  // Description:
  // Convert display coordinates to view coordinates.
  virtual void DisplayToView(); // these get modified in subclasses

  // Description:
  // Convert view coordinates to display coordinates.
  virtual void ViewToDisplay(); // to handle stereo rendering

  // Description:
  // Convert world point coordinates to view coordinates.
  virtual void WorldToView();

  // Description:
  // Convert view point coordinates to world coordinates.
  virtual void ViewToWorld();

  // Description:
  // Convert display (or screen) coordinates to world coordinates.
  void DisplayToWorld() {this->DisplayToView(); this->ViewToWorld();};

  // Description:
  // Convert world point coordinates to display (or screen) coordinates.
  void WorldToDisplay() {this->WorldToView(); this->ViewToDisplay();};

  // Description:
  // These methods map from one coordinate system to another.
  // They are primarily used by the vtkCoordinate object and
  // are often strung together.
  virtual void LocalDisplayToDisplay(float &x, float &y);
  virtual void DisplayToNormalizedDisplay(float &u, float &v);
  virtual void NormalizedDisplayToViewport(float &x, float &y);
  virtual void ViewportToNormalizedViewport(float &u, float &v);
  virtual void NormalizedViewportToView(float &x, float &y, float &z);
  virtual void ViewToWorld(float &, float &, float &) {};
  virtual void DisplayToLocalDisplay(float &x, float &y);
  virtual void NormalizedDisplayToDisplay(float &u, float &v);
  virtual void ViewportToNormalizedDisplay(float &x, float &y);
  virtual void NormalizedViewportToViewport(float &u, float &v);
  virtual void ViewToNormalizedViewport(float &x, float &y, float &z);
  virtual void WorldToView(float &, float &, float &) {};

  // Description:
  // Get the size and origin of the viewport in display coordinates
  int *GetSize();
  int *GetOrigin();

protected:
  vtkPropCollection *Props;
  vtkActor2DCollection *Actors2D;
  vtkWindow *VTKWindow;
  float Background[3];  
  float Viewport[4];
  float Aspect[2];
  float Center[2];
  void (*StartRenderMethod)(void *);
  void (*StartRenderMethodArgDelete)(void *);
  void *StartRenderMethodArg;
  void (*EndRenderMethod)(void *);
  void (*EndRenderMethodArgDelete)(void *);
  void *EndRenderMethodArg;

  int Size[2];
  int Origin[2];
  float DisplayPoint[3];
  float ViewPoint[3];
  float WorldPoint[4];

};



#endif
