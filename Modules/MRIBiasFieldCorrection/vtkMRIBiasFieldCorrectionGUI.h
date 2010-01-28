/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkMRIBiasFieldCorrectionGUI.h,v $
  Date:      $Date: 2006/03/19 17:12:29 $
  Version:   $Revision: 1.3 $

=========================================================================auto=*/
#ifndef __vtkMRIBiasFieldCorrectionGUI_h
#define __vtkMRIBiasFieldCorrectionGUI_h

#include "vtkSlicerBaseGUIWin32Header.h"
#include "vtkSlicerModuleGUI.h"

#include "vtkMRMLScene.h"
#include "vtkMRIBiasFieldCorrectionLogic.h"


class vtkSlicerSliceWidget;
class vtkKWFrame;
class vtkKWScaleWithEntry;
class vtkKWPushButton;
class vtkSlicerNodeSelectorWidget;
class vtkKWFrameWithLabel;
class vtkKWSpinBoxWithLabel;
class vtkSlicerApplicationLogic;

class VTK_MRIBiasFieldCorrection_EXPORT vtkMRIBiasFieldCorrectionGUI : public vtkSlicerModuleGUI
{
  public:
  static vtkMRIBiasFieldCorrectionGUI *New();
  vtkTypeMacro(vtkMRIBiasFieldCorrectionGUI,vtkSlicerModuleGUI);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description: Get/Set MRML node
  vtkGetObjectMacro (Logic, vtkMRIBiasFieldCorrectionLogic);
  vtkSetObjectMacro (Logic, vtkMRIBiasFieldCorrectionLogic);

  // Description:
  // Set the logic pointer from parent class pointer.
  // Overloads implementation in vtkSlicerModulesGUI
  // to allow loadable modules.
  virtual void SetModuleLogic ( vtkSlicerLogic *logic )
  {
    this->SetLogic(reinterpret_cast<vtkMRIBiasFieldCorrectionLogic*> (logic)); 
  }

  // Description: Get/Set MRML node
  vtkGetObjectMacro (MRIBiasFieldCorrectionNode, vtkMRMLMRIBiasFieldCorrectionNode);

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
  vtkMRIBiasFieldCorrectionGUI();
  virtual ~vtkMRIBiasFieldCorrectionGUI();
  vtkMRIBiasFieldCorrectionGUI(const vtkMRIBiasFieldCorrectionGUI&);
  void operator=(const vtkMRIBiasFieldCorrectionGUI&);

  // Description:
  // Updates GUI widgets based on parameters values in MRML node
  void UpdateGUI();

  // Description:
  // Updates parameters values in MRML node based on GUI widgets 
  void UpdateMRML();

  vtkKWScaleWithEntry* TimeStepScale;
  vtkKWScaleWithEntry* NumberOfIterationsScale;

  vtkKWScaleWithEntry* Sagittal2Scale;
  
  vtkKWScaleWithEntry* Coronal1Scale;
  vtkKWScaleWithEntry* Coronal2Scale;
  
  vtkSlicerNodeSelectorWidget* VolumeSelector;
  vtkSlicerNodeSelectorWidget* OutVolumeSelector;
  vtkSlicerNodeSelectorWidget* GADNodeSelector;
  vtkSlicerNodeSelectorWidget* StorageVolumeSelector;
  vtkSlicerNodeSelectorWidget* MaskVolumeSelector;
  
  vtkKWPushButton* ApplyButton;
  vtkKWPushButton* CatchButton;
  
  vtkMRIBiasFieldCorrectionLogic *Logic;
  vtkMRMLMRIBiasFieldCorrectionNode* MRIBiasFieldCorrectionNode;
  vtkKWFrameWithLabel* VolumeSelectionFrame;
  
  vtkKWFrameWithLabel* ParametersFrame;
  vtkKWFrameWithLabel* AdvancedParametersFrame;
  
  vtkKWScaleWithEntry* ShrinkFactor;
  vtkKWScaleWithEntry* MaxNumOfIt;
  vtkKWScaleWithEntry* NumOfFitLev;
  vtkKWScaleWithEntry* WienFilNoi;
  vtkKWScaleWithEntry* BiasField;
  vtkKWScaleWithEntry* Conv;
  
  vtkKWScaleWithEntry* SlidePositionScale;
  vtkKWScaleWithEntry* BiasIntensityScale;
  
};

#endif

