/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkDisplayableManagerBoxWidget2.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
///  vtkDisplayableManagerBoxWidget2 - 3D widget for manipulating a box
/// 
/// This 3D widget interacts with a vtkDisplayableManagerBoxRepresentation class (i.e., it
/// handles the events that drive its corresponding representation). The
/// representation is assumed to represent a region of interest that is
/// represented by an arbitrarily oriented hexahedron (or box) with interior
/// face angles of 90 degrees (i.e., orthogonal faces). The representation
/// manifests seven handles that can be moused on and manipulated, plus the
/// six faces can also be interacted with. The first six handles are placed on
/// the six faces, the seventh is in the center of the box. In addition, a
/// bounding box outline is shown, the "faces" of which can be selected for
/// object rotation or scaling. A nice feature of vtkDisplayableManagerBoxWidget2, like any 3D
/// widget, will work with the current interactor style. That is, if
/// vtkDisplayableManagerBoxWidget2 does not handle an event, then all other registered
/// observers (including the interactor style) have an opportunity to process
/// the event. Otherwise, the vtkSlicerBoxWidget will terminate the processing of
/// the event that it handles.
//
/// To use this widget, you generally pair it with a vtkDisplayableManagerBoxRepresentation
/// (or a subclass). Variuos options are available in the representation for 
/// controlling how the widget appears, and how the widget functions.
//
/// .SECTION Event Bindings
/// By default, the widget responds to the following VTK events (i.e., it
/// watches the vtkRenderWindowInteractor for these events):
/// <pre>
/// If one of the seven handles are selected:
///   LeftButtonPressEvent - select the appropriate handle 
///   LeftButtonReleaseEvent - release the currently selected handle 
///   MouseMoveEvent - move the handle
/// If one of the faces is selected:
///   LeftButtonPressEvent - select a box face
///   LeftButtonReleaseEvent - release the box face
///   MouseMoveEvent - rotate the box
/// In all the cases, independent of what is picked, the widget responds to the 
/// following VTK events:
///   MiddleButtonPressEvent - translate the widget
///   MiddleButtonReleaseEvent - release the widget
///   RightButtonPressEvent - scale the widget's representation
///   RightButtonReleaseEvent - stop scaling the widget
///   MouseMoveEvent - scale (if right button) or move (if middle button) the widget
/// </pre>
//
/// Note that the event bindings described above can be changed using this
/// class's vtkWidgetEventTranslator. This class translates VTK events 
/// into the vtkDisplayableManagerBoxWidget2's widget events:
/// <pre>
///   vtkWidgetEvent::Select -- some part of the widget has been selected
///   vtkWidgetEvent::EndSelect -- the selection process has completed
///   vtkWidgetEvent::Scale -- some part of the widget has been selected
///   vtkWidgetEvent::EndScale -- the selection process has completed
///   vtkWidgetEvent::Translate -- some part of the widget has been selected
///   vtkWidgetEvent::EndTranslate -- the selection process has completed
///   vtkWidgetEvent::Move -- a request for motion has been invoked
/// </pre>
//
/// In turn, when these widget events are processed, the vtkDisplayableManagerBoxWidget2
/// invokes the following VTK events on itself (which observers can listen for):
/// <pre>
///   vtkCommand::StartInteractionEvent (on vtkWidgetEvent::Select)
///   vtkCommand::EndInteractionEvent (on vtkWidgetEvent::EndSelect)
///   vtkCommand::InteractionEvent (on vtkWidgetEvent::Move)
/// </pre>

/// .SECTION Caveats
/// Note that in some cases the widget can be picked even when it is "behind"
/// other actors.  This is an intended feature and not a bug.
/// 
/// This class, and the affiliated vtkDisplayableManagerBoxRepresentation, are second generation
/// VTK widgets. An earlier version of this functionality was defined in the
/// class vtkSlicerBoxWidget.

/// .SECTION See Also
/// vtkDisplayableManagerBoxRepresentation vtkSlicerBoxWidget

#ifndef __vtkDisplayableManagerBoxWidget2_h
#define __vtkDisplayableManagerBoxWidget2_h

#include "vtkMRMLDisplayableManagerWin32Header.h"

#include "vtkAbstractWidget.h"

class vtkDisplayableManagerBoxRepresentation;
class vtkHandleWidget;


class VTK_MRML_DISPLAYABLEMANAGER_EXPORT vtkDisplayableManagerBoxWidget2 : public vtkAbstractWidget
{
public:
  /// 
  /// Instantiate the object.
  static vtkDisplayableManagerBoxWidget2 *New();

  /// 
  /// Standard class methods for type information and printing.
  vtkTypeRevisionMacro(vtkDisplayableManagerBoxWidget2,vtkAbstractWidget);
  void PrintSelf(ostream& os, vtkIndent indent);

  /// 
  /// Specify an instance of vtkWidgetRepresentation used to represent this
  /// widget in the scene. Note that the representation is a subclass of vtkProp
  /// so it can be added to the renderer independent of the widget.
  void SetRepresentation(vtkDisplayableManagerBoxRepresentation *r)
    {this->Superclass::SetWidgetRepresentation(reinterpret_cast<vtkWidgetRepresentation*>(r));}
  
  /// 
  /// Control the behavior of the widget (i.e., how it processes
  /// events). Translation, rotation, and scaling can all be enabled and
  /// disabled.
  vtkSetMacro(TranslationEnabled,int);
  vtkGetMacro(TranslationEnabled,int);
  vtkBooleanMacro(TranslationEnabled,int);
  vtkSetMacro(ScalingEnabled,int);
  vtkGetMacro(ScalingEnabled,int);
  vtkBooleanMacro(ScalingEnabled,int);
  vtkSetMacro(RotationEnabled,int);
  vtkGetMacro(RotationEnabled,int);
  vtkBooleanMacro(RotationEnabled,int);

  /// 
  /// Create the default widget representation if one is not set. By default,
  /// this is an instance of the vtkDisplayableManagerBoxRepresentation class.
  void CreateDefaultRepresentation();

protected:
  vtkDisplayableManagerBoxWidget2();
  ~vtkDisplayableManagerBoxWidget2();

//BTX - manage the state of the widget
  int WidgetState;
  enum _WidgetState {Start=0,Active};
//ETX
  
  /// These methods handle events
  static void SelectAction(vtkAbstractWidget*);
  static void EndSelectAction(vtkAbstractWidget*);
  static void TranslateAction(vtkAbstractWidget*);
  static void ScaleAction(vtkAbstractWidget*);
  static void MoveAction(vtkAbstractWidget*);

  /// Control whether scaling, rotation, and translation are supported
  int TranslationEnabled;
  int ScalingEnabled;
  int RotationEnabled;

private:
  vtkDisplayableManagerBoxWidget2(const vtkDisplayableManagerBoxWidget2&);  //Not implemented
  void operator=(const vtkDisplayableManagerBoxWidget2&);  //Not implemented
};

#endif