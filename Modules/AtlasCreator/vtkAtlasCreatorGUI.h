/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkAtlasCreatorGUI.h,v $
  Date:      $Date: 2006/03/19 17:12:29 $
  Version:   $Revision: 1.3 $

=========================================================================auto=*/
#ifndef __vtkAtlasCreatorGUI_h
#define __vtkAtlasCreatorGUI_h

#include "vtkSlicerBaseGUIWin32Header.h"
#include "vtkSlicerModuleGUI.h"

#include "vtkMRMLScene.h"
#include "vtkAtlasCreatorLogic.h"


class vtkSlicerSliceWidget;
class vtkKWFrame;
class vtkKWScaleWithEntry;
class vtkKWPushButton;
class vtkSlicerNodeSelectorWidget;

class VTK_ATLASCREATOR_EXPORT vtkAtlasCreatorGUI : public vtkSlicerModuleGUI
{
  public:
  static vtkAtlasCreatorGUI *New();
  vtkTypeMacro(vtkAtlasCreatorGUI,vtkSlicerModuleGUI);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description: Get/Set MRML node
  vtkGetObjectMacro (Logic, vtkAtlasCreatorLogic);
  vtkSetObjectMacro (Logic, vtkAtlasCreatorLogic);

  // Description:
  // Set the logic pointer from parent class pointer.
  // Overloads implementation in vtkSlicerModulesGUI
  // to allow loadable modules.
  virtual void SetModuleLogic ( vtkSlicerLogic *logic )
  {
    this->SetLogic(reinterpret_cast<vtkAtlasCreatorLogic*> (logic));
  }

  // Description: Get/Set MRML node
  vtkGetObjectMacro (AtlasCreatorNode, vtkMRMLAtlasCreatorNode);

  // Description:
  // Create widgets
  virtual void BuildGUI ( );
  //BTX
  using vtkSlicerComponentGUI::BuildGUI; 
  //ETX

  // Description:
  // Add obsereves to GUI widgets
  virtual void AddGUIObservers ( );

  // Description:
  // Remove obsereves to GUI widgets
  virtual void RemoveGUIObservers ( );

  // Description:
  // Pprocess events generated by Logic
  virtual void ProcessLogicEvents(vtkObject *vtkNotUsed(caller),
                                  unsigned long vtkNotUsed(event),
                                  void *vtkNotUsed(callData)){};

  // Description:
  // Pprocess events generated by GUI widgets
  virtual void ProcessGUIEvents ( vtkObject *caller, unsigned long event,
                                  void *callData );

  // Description:
  // Pprocess events generated by MRML
  virtual void ProcessMRMLEvents ( vtkObject *caller, unsigned long event,
                                  void *callData);
  // Description:
  // Describe behavior at module startup and exit.
  virtual void Enter ( ){};
  //BTX
  using vtkSlicerComponentGUI::Enter; 
  //ETX
  virtual void Exit ( ){};

  // Description:
  // Get the categorization of the module.  The category is used for
  // grouping modules together into menus.
  const char *GetCategory() const {return "Filtering";}

protected:
  vtkAtlasCreatorGUI();
  virtual ~vtkAtlasCreatorGUI();
  vtkAtlasCreatorGUI(const vtkAtlasCreatorGUI&);
  void operator=(const vtkAtlasCreatorGUI&);

  // Description:
  // Updates GUI widgets based on parameters values in MRML node
  void UpdateGUI();

  // Description:
  // Updates parameters values in MRML node based on GUI widgets
  void UpdateMRML();

  vtkKWScaleWithEntry* ConductanceScale;
  vtkKWScaleWithEntry* TimeStepScale;
  vtkKWScaleWithEntry* NumberOfIterationsScale;
  vtkSlicerNodeSelectorWidget* VolumeSelector;
  vtkSlicerNodeSelectorWidget* OutVolumeSelector;
  vtkSlicerNodeSelectorWidget* GADNodeSelector;
  vtkKWPushButton* ApplyButton;

  vtkAtlasCreatorLogic *Logic;
  vtkMRMLAtlasCreatorNode* AtlasCreatorNode;

};

#endif

