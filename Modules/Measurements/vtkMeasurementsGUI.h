// .NAME vtkMeasurementsGUI
// .SECTION Description
// Slicer Welcome and Overview


#ifndef __vtkMeasurementsGUI_h
#define __vtkMeasurementsGUI_h

#include "vtkMeasurementsWin32Header.h"
#include "vtkSlicerModuleGUI.h"
#include "vtkMeasurementsLogic.h"
#include "vtkSlicerModuleCollapsibleFrame.h"
#include <string>

class vtkKWFrame;
class vtkKWCheckButton;
class vtkAffineWidget;
class vtkAffineRepresentation2D;
class vtkSlicerNodeSelectorWidget;
class vtkMeasurementsRulerWidget;
class vtkMeasurementsAngleWidget;
class VTK_MEASUREMENTS_EXPORT vtkMeasurementsGUI : public vtkSlicerModuleGUI
{
  public:
  static vtkMeasurementsGUI *New();
  vtkTypeRevisionMacro(vtkMeasurementsGUI,vtkSlicerModuleGUI);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the logic pointer from parent class pointer.
  // Overloads implementation in vtkSlicerModulesGUI
  // to allow loadable modules.
  virtual void SetModuleLogic ( vtkSlicerLogic *logic )
  {
  this->SetLogic(reinterpret_cast<vtkMeasurementsLogic*> (logic)); 
  };

   // Description: Get/Set logic
  vtkGetObjectMacro (Logic, vtkMeasurementsLogic);
  vtkSetObjectMacro (Logic, vtkMeasurementsLogic);

  // Description:
  // Create widgets

  virtual void BuildGUI ( );
  //BTX
  using vtkSlicerComponentGUI::BuildGUI;
  //ETX
  virtual void TearDownGUI ( );
  vtkIntArray *NewObservableEvents();

  // Description:
  // Add obsereves to GUI widgets
  virtual void AddGUIObservers ( );

  // Description:
  // Remove obsereves to GUI widgets
  virtual void RemoveGUIObservers ( );
  
  // Description:
  // Pprocess events generated by GUI widgets
  virtual void ProcessGUIEvents ( vtkObject *caller, unsigned long event,
                                  void *callData );

  // Description:
  // Describe behavior at module startup and exit.
  virtual void Enter ( );
  //BTX
  using vtkSlicerComponentGUI::Enter;
  //ETX
  virtual void Exit ( );
  virtual void Init ( );

  // Description:
  // Access widgets
  vtkGetObjectMacro(RulerWidget, vtkMeasurementsRulerWidget);
  vtkGetObjectMacro(AngleWidget, vtkMeasurementsAngleWidget);

  // Description:
  // assign the mrml node that's selected in the TransformableNodeSelectorWidget to the TransformWidget to be manipulated via it's transform node 
  void UpdateTransformableNode();

protected:
  vtkMeasurementsGUI();
  ~vtkMeasurementsGUI();
  vtkMeasurementsGUI(const vtkMeasurementsGUI&);
  void operator=(const vtkMeasurementsGUI&);

  // Description:
  // updates status text in the slicer window.
  void SetStatusText( const char *txt);
  vtkMeasurementsLogic *Logic;

  // Description
  // transform widget
  vtkAffineRepresentation2D *TransformRepresentation;
  vtkAffineWidget *TransformWidget;


  // Description:
  // Transform GUI elements
  vtkKWCheckButton *TransformCheckButton;
  vtkSlicerNodeSelectorWidget* TransformableNodeSelectorWidget;

  // Description:
  // ruler gui elements
  vtkMeasurementsRulerWidget *RulerWidget;

  // Description:
  // angle widget gui elements
  vtkMeasurementsAngleWidget *AngleWidget;
};

#endif

