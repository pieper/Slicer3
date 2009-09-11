#include "vtkObject.h"
#include "vtkObjectFactory.h"

#include "vtkPatientToImageRegistrationGUI.h"
#include "vtkSlicerApplication.h"
#include "vtkSlicerModuleCollapsibleFrame.h"
#include "vtkSlicerSliceControllerWidget.h"
#include "vtkSlicerColor.h"
#include "vtkSlicerTheme.h"

#include "vtkKWRenderWidget.h"
#include "vtkKWWidget.h"
#include "vtkKWMenuButton.h"
#include "vtkKWCheckButton.h"
#include "vtkKWPushButton.h"
#include "vtkKWFrameWithLabel.h"
#include "vtkKWFrame.h"
#include "vtkKWLoadSaveButton.h"
#include "vtkKWLoadSaveButtonWithLabel.h"
#include "vtkKWLoadSaveDialog.h"
#include "vtkKWEntry.h"
#include "vtkKWEntryWithLabel.h"
#include "vtkKWMenuButtonWithLabel.h"
#include "vtkKWScaleWithEntry.h"
#include "vtkKWMenu.h"
#include "vtkKWLabel.h"
#include "vtkKWMultiColumnList.h"
#include "vtkKWMessageDialog.h"
#include "vtkKWMultiColumnListWithScrollbars.h"
#include "vtkKWEvent.h"


#include "vtkKWTkUtilities.h"
#include "vtkMRMLModelDisplayNode.h"
#include "vtkCylinderSource.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCornerAnnotation.h"

#include "vtkIGTDataStream.h"

#include "vtkCylinderSource.h"
#include "vtkMRMLLinearTransformNode.h"


//---------------------------------------------------------------------------
vtkCxxRevisionMacro ( vtkPatientToImageRegistrationGUI, "$Revision: 1.0 $");


vtkPatientToImageRegistrationGUI* vtkPatientToImageRegistrationGUI::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = 
    vtkObjectFactory::CreateInstance("vtkPatientToImageRegistrationGUI");
  if (ret)
    {
    return (vtkPatientToImageRegistrationGUI*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkPatientToImageRegistrationGUI;
}



//---------------------------------------------------------------------------
vtkPatientToImageRegistrationGUI::vtkPatientToImageRegistrationGUI ( )
{
  this->Logic = NULL;

  this->TransformNodeNameEntry = NULL;
  this->FiducialListNodeNameEntry = NULL;

  this->PatCoordinatesEntry = NULL;
  this->SlicerCoordinatesEntry = NULL;
  this->GetPatCoordinatesPushButton = NULL;
  this->AddPointPairPushButton = NULL;

  this->PointPairMultiColumnList = NULL;

  // this->LoadPointPairPushButton = NULL;
  // this->SavePointPairPushButton = NULL;
  this->DeletePointPairPushButton = NULL;
  this->DeleteAllPointPairPushButton = NULL;
  this->RegisterPushButton = NULL;
  this->ResetPushButton = NULL;

  this->CloseScene = false;
  this->TimerFlag = 0;

  this->TranslationScale = NULL;
}


//---------------------------------------------------------------------------
vtkPatientToImageRegistrationGUI::~vtkPatientToImageRegistrationGUI ( )
{
  this->RemoveGUIObservers();

 if (this->TransformNodeNameEntry)
    {
    this->TransformNodeNameEntry->SetParent(NULL);
    this->TransformNodeNameEntry->Delete();
    }
  if (this->FiducialListNodeNameEntry)
    {
    this->FiducialListNodeNameEntry->SetParent(NULL);
    this->FiducialListNodeNameEntry->Delete();
    }
  if (this->PatCoordinatesEntry)
    {
    this->PatCoordinatesEntry->SetParent(NULL );
    this->PatCoordinatesEntry->Delete ( );
    }
  if (this->SlicerCoordinatesEntry)
    {
    this->SlicerCoordinatesEntry->SetParent(NULL );
    this->SlicerCoordinatesEntry->Delete ( );
    }
  if (this->GetPatCoordinatesPushButton)
    {
    this->GetPatCoordinatesPushButton->SetParent(NULL );
    this->GetPatCoordinatesPushButton->Delete ( );
    }
  if (this->AddPointPairPushButton)
    {
    this->AddPointPairPushButton->SetParent(NULL );
    this->AddPointPairPushButton->Delete ( );
    }

  if (this->PointPairMultiColumnList)
    {
    this->PointPairMultiColumnList->SetParent(NULL );
    this->PointPairMultiColumnList->Delete ( );
    }

  /*
     if (this->LoadPointPairPushButton)
     {
     this->LoadPointPairPushButton->SetParent(NULL );
     this->LoadPointPairPushButton->Delete ( );
     }
     if (this->SavePointPairPushButton)
     {
     this->SavePointPairPushButton->SetParent(NULL );
     this->SavePointPairPushButton->Delete ( );
     }
     */
  if (this->DeletePointPairPushButton)
    {
    this->DeletePointPairPushButton->SetParent(NULL );
    this->DeletePointPairPushButton->Delete ( );
    }
  if (this->DeleteAllPointPairPushButton)
    {
    this->DeleteAllPointPairPushButton->SetParent(NULL );
    this->DeleteAllPointPairPushButton->Delete ( );
    }
  if (this->RegisterPushButton)
    {
    this->RegisterPushButton->SetParent(NULL );
    this->RegisterPushButton->Delete ( );
    }
  if (this->ResetPushButton)
    {
    this->ResetPushButton->SetParent(NULL );
    this->ResetPushButton->Delete ( );
    }

  if (this->TranslationScale)
    {
    this->TranslationScale->SetParent(NULL);
    this->TranslationScale->Delete();
    this->TranslationScale = NULL;
    }

  this->SetModuleLogic ( NULL );
}



//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::PrintSelf ( ostream& os, vtkIndent indent )
{
  this->vtkObject::PrintSelf ( os, indent );

  os << indent << "PatientToImageRegistrationGUI: " << this->GetClassName ( ) << "\n";
  os << indent << "Logic: " << this->GetLogic ( ) << "\n";
  // print widgets?
}


//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::RemoveGUIObservers ( )
{
  vtkSlicerApplicationGUI *appGUI = this->GetApplicationGUI();
  
  if ( appGUI )
    {
    appGUI->GetMainSliceGUI("Red")->GetSliceViewer()->GetRenderWidget()
      ->GetRenderWindowInteractor()->GetInteractorStyle()->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    appGUI->GetMainSliceGUI("Yellow")->GetSliceViewer()->GetRenderWidget()
      ->GetRenderWindowInteractor()->GetInteractorStyle()->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    appGUI->GetMainSliceGUI("Green")->GetSliceViewer()->GetRenderWidget()
      ->GetRenderWindowInteractor()->GetInteractorStyle()->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }

//  vtkSlicerApplicationGUI *appGUI = this->GetApplicationGUI();
//  appGUI->GetMainSliceGUI("Red")->GetSliceViewer()->GetRenderWidget()->GetRenderWindowInteractor()->GetInteractorStyle()->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
//  appGUI->GetMainSliceGUI("Yellow")->GetSliceViewer()->GetRenderWidget()->GetRenderWindowInteractor()->GetInteractorStyle()->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
//  appGUI->GetMainSliceGUI("Blue")->GetSliceViewer()->GetRenderWidget()->GetRenderWindowInteractor()->GetInteractorStyle()->RemoveObserver((vtkCommand *)this->GUICallbackCommand);


  if (this->GetPatCoordinatesPushButton)
    {
    this->GetPatCoordinatesPushButton->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }
  if (this->AddPointPairPushButton)
    {
    this->AddPointPairPushButton->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }
  if (this->DeletePointPairPushButton)
    {
    this->DeletePointPairPushButton->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }
  if (this->DeleteAllPointPairPushButton)
    {
    this->DeleteAllPointPairPushButton->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }
  if (this->RegisterPushButton)
    {
    this->RegisterPushButton->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }
  if (this->ResetPushButton)
    {
    this->ResetPushButton->RemoveObserver((vtkCommand *)this->GUICallbackCommand);
    }
}


//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::AddGUIObservers ( )
{
  this->RemoveGUIObservers();

  // make a user interactor style to process our events
  // look at the InteractorStyle to get our events

  vtkSlicerApplicationGUI *appGUI = this->GetApplicationGUI();

  //----------------------------------------------------------------
  // MRML

  vtkIntArray* events = vtkIntArray::New();
  //events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  //events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::SceneCloseEvent);
  
  if (this->GetMRMLScene() != NULL)
    {
    this->SetAndObserveMRMLSceneEvents(this->GetMRMLScene(), events);
    }
  events->Delete();

  
  //----------------------------------------------------------------
  // Main Slice GUI

  appGUI->GetMainSliceGUI("Red")->GetSliceViewer()->GetRenderWidget()
    ->GetRenderWindowInteractor()->GetInteractorStyle()
    ->AddObserver(vtkCommand::LeftButtonPressEvent, (vtkCommand *)this->GUICallbackCommand);
  appGUI->GetMainSliceGUI("Yellow")->GetSliceViewer()->GetRenderWidget()
    ->GetRenderWindowInteractor()->GetInteractorStyle()
    ->AddObserver(vtkCommand::LeftButtonPressEvent, (vtkCommand *)this->GUICallbackCommand);
  appGUI->GetMainSliceGUI("Green")->GetSliceViewer()->GetRenderWidget()
    ->GetRenderWindowInteractor()->GetInteractorStyle()
    ->AddObserver(vtkCommand::LeftButtonPressEvent, (vtkCommand *)this->GUICallbackCommand);
  

  // Fill in
  // observer load volume button
  this->GetPatCoordinatesPushButton->AddObserver ( vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->AddPointPairPushButton->AddObserver ( vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->DeletePointPairPushButton->AddObserver ( vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->DeleteAllPointPairPushButton->AddObserver ( vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->RegisterPushButton->AddObserver ( vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ResetPushButton->AddObserver ( vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
}



void vtkPatientToImageRegistrationGUI::HandleMouseEvent(vtkSlicerInteractorStyle *style)
{
  vtkSlicerApplicationGUI *appGUI = this->GetApplicationGUI();

  vtkSlicerInteractorStyle *istyle0 = 
    vtkSlicerInteractorStyle::SafeDownCast(appGUI->GetMainSliceGUI("Red")->GetSliceViewer()->GetRenderWidget()
                                           ->GetRenderWindowInteractor()->GetInteractorStyle());

  vtkSlicerInteractorStyle *istyle1 = 
    vtkSlicerInteractorStyle::SafeDownCast(appGUI->GetMainSliceGUI("Yellow")->GetSliceViewer()->GetRenderWidget()
                                           ->GetRenderWindowInteractor()->GetInteractorStyle());

  vtkSlicerInteractorStyle *istyle2 = 
    vtkSlicerInteractorStyle::SafeDownCast(appGUI->GetMainSliceGUI("Green")->GetSliceViewer()->GetRenderWidget()
                                           ->GetRenderWindowInteractor()->GetInteractorStyle());


/*
    vtkSlicerInteractorStyle::SafeDownCast(appGUI->GetMainSliceGUI("Red")->GetSliceViewer()
                                           ->GetRenderWidget()->GetRenderWindowInteractor()->GetInteractorStyle());
  vtkSlicerInteractorStyle *istyle1 = 
    vtkSlicerInteractorStyle::SafeDownCast(appGUI->GetMainSliceGUI("Yellow")->GetSliceViewer()
                                           ->GetRenderWidget()->GetRenderWindowInteractor()->GetInteractorStyle());
  vtkSlicerInteractorStyle *istyle2 = 
    vtkSlicerInteractorStyle::SafeDownCast(appGUI->GetMainSliceGUI("Blue")->GetSliceViewer()
                                           ->GetRenderWidget()->GetRenderWindowInteractor()->GetInteractorStyle());
*/

  vtkCornerAnnotation *anno = NULL;
  if (style == istyle0)
    {
    anno = appGUI->GetMainSliceGUI("Red")->GetSliceViewer()->GetRenderWidget()->GetCornerAnnotation();
    }
  else if (style == istyle1)
    {
    anno = appGUI->GetMainSliceGUI("Yellow")->GetSliceViewer()->GetRenderWidget()->GetCornerAnnotation();
    }
  else if (style == istyle2)
    {
    // ??!?!?! this would /never/ work!
    //anno = appGUI->GetMainSliceGUI("Blue")->GetSliceViewer()->GetRenderWidget()->GetCornerAnnotation();
    }

  if (anno)
    {
    const char *rasText = anno->GetText(1);
    if ( rasText != NULL )
      {
      std::string ras = std::string(rasText);

      // remove "R:," "A:," and "S:" from the string
      size_t loc = ras.find("R:", 0);
      if ( loc != std::string::npos ) 
        {
        ras = ras.replace(loc, 2, "");
        }
      loc = ras.find("A:", 0);
      if ( loc != std::string::npos ) 
        {
        ras = ras.replace(loc, 2, "");
        }
      loc = ras.find("S:", 0);
      if ( loc != std::string::npos ) 
        {
        ras = ras.replace(loc, 2, "");
        }

      // remove "\n" from the string
      size_t found = ras.find("\n", 0);
      while ( found != std::string::npos )
        {
        ras = ras.replace(found, 1, " ");
        found = ras.find("\n", 0);
        }

      this->SlicerCoordinatesEntry->GetWidget()->SetValue(ras.c_str());
      }
    }
}


//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::ProcessGUIEvents ( vtkObject *caller,
                                        unsigned long event, void *callData )
{
  const char *eventName = vtkCommand::GetStringFromEventId(event);

  if (strcmp(eventName, "LeftButtonPressEvent") == 0)
    {
    vtkSlicerInteractorStyle *style = vtkSlicerInteractorStyle::SafeDownCast(caller);
    HandleMouseEvent(style);
    }
  else
    {
    if (this->GetPatCoordinatesPushButton == vtkKWPushButton::SafeDownCast(caller) 
        && event == vtkKWPushButton::InvokedEvent)
      {
      double px, py, pz;
      char value[50];
      this->GetLogic()->GetCurrentPosition(&px, &py, &pz);
      sprintf(value, "%6.2f  %6.2f  %6.2f", px, py, pz);
      this->PatCoordinatesEntry->GetWidget()->SetValue(value);
      }
      else if (this->AddPointPairPushButton == vtkKWPushButton::SafeDownCast(caller) 
               && event == vtkKWPushButton::InvokedEvent)
        {
        int scSize = 0;
        int pcSize = 0;
        const char *pc = this->PatCoordinatesEntry->GetWidget()->GetValue();
        const char *sc = this->SlicerCoordinatesEntry->GetWidget()->GetValue();

        if (pc) 
          {
          const vtksys_stl::string pcCor(pc);
          pcSize = pcCor.size();
        }
      if (sc) 
        {
        const vtksys_stl::string scCor(sc);
        scSize = scCor.size();
        }

      if (pcSize < 5 || scSize < 5)
        {
        vtkSlicerApplication::GetInstance()->ErrorMessage("Patient or Slicer coordinates are invalid."); 
        }
      else 
        {
        int row = this->PointPairMultiColumnList->GetWidget()->GetNumberOfRows();
        this->PointPairMultiColumnList->GetWidget()->AddRow();
        this->PointPairMultiColumnList->GetWidget()->SetCellText(row, 0, pc);
        this->PointPairMultiColumnList->GetWidget()->SetCellText(row, 1, sc);
        }
      }
    else if (this->DeletePointPairPushButton == vtkKWPushButton::SafeDownCast(caller) 
             && event == vtkKWPushButton::InvokedEvent)
      {
      int numOfRows = this->PointPairMultiColumnList->GetWidget()->GetNumberOfSelectedRows();
      if (numOfRows == 1)
        {
        int index[2];
        this->PointPairMultiColumnList->GetWidget()->GetSelectedRows(index);
        this->PointPairMultiColumnList->GetWidget()->DeleteRow(index[0]);
        }
      }
    else if (this->DeleteAllPointPairPushButton == vtkKWPushButton::SafeDownCast(caller) 
             && event == vtkKWPushButton::InvokedEvent)
      {
      this->PointPairMultiColumnList->GetWidget()->DeleteAllRows();
      }
    else if (this->RegisterPushButton == vtkKWPushButton::SafeDownCast(caller) 
             && event == vtkKWPushButton::InvokedEvent)
      {
      int row = this->PointPairMultiColumnList->GetWidget()->GetNumberOfRows();
      if (row < 2)
        {
        vtkSlicerApplication::GetInstance()->ErrorMessage("At least 2 pairs of landmarks are needed for patient to image registration.");
        }
      else
        {
        this->GetLogic()->GetPat2ImgReg()->SetNumberOfPoints(row);
        float pc1 = 0.0, pc2 = 0.0, pc3 = 0.0, sc1 = 0.0, sc2 = 0.0, sc3 = 0.0;
        for (int r = 0; r < row; r++)
          {
          for (int c = 0; c < 2; c++)
            {
            const char *val = this->PointPairMultiColumnList->GetWidget()->GetCellText(r, c);
            if (c == 0)
              {
              sscanf(val, "%f %f %f", &pc1, &pc2, &pc3);
              }
            else
              {
              sscanf(val, "%f %f %f", &sc1, &sc2, &sc3);
              }
            }
          this->GetLogic()->GetPat2ImgReg()->AddPoint(r, sc1, sc2, sc3, pc1, pc2, pc3);
          }

        int error = this->GetLogic()->PerformPatientToImageRegistration();
        if (error)
          {
          vtkSlicerApplication::GetInstance()->ErrorMessage("Error registration between patient and image landmarks.");
          return;
          }
        }
      }
    else if (this->ResetPushButton == vtkKWPushButton::SafeDownCast(caller) 
             && event == vtkKWPushButton::InvokedEvent)
      {
      this->GetLogic()->SetUseRegistration(0);
      }
    }
} 



//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::ProcessLogicEvents ( vtkObject *caller,
                                          unsigned long event, void *callData )
{
  // Fill in
}


//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::ProcessMRMLEvents ( vtkObject *caller,
                                         unsigned long event, void *callData )
{
  if (event == vtkMRMLScene::SceneCloseEvent)
    {
    }
}



//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::Enter ( )
{
  // Fill in

  if (this->TimerFlag == 0)
    {
    this->TimerFlag = 1;
    this->TimerInterval = 100;  // 100 ms
    ProcessTimerEvents();
    }

}



//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::ProcessTimerEvents()
{
  if (this->TimerFlag)
    {

    vtkKWTkUtilities::CreateTimerHandler(vtkKWApplication::GetMainInterp(), 
                                         this->TimerInterval,
                                         this, "ProcessTimerEvents");        
    }
}



//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::Exit ( )
{
  // Fill in
}


//---------------------------------------------------------------------------
void vtkPatientToImageRegistrationGUI::BuildGUI ( )
{
  // ---
  // MODULE GUI FRAME 
  // create a page
  this->UIPanel->AddPage ( "PatientToImageRegistration", "PatientToImageRegistration", NULL );

  BuildGUIForHelpFrame();
  BuildGUIForLandmarksFrame();
  BuildGUIForICPFrame();
}



void vtkPatientToImageRegistrationGUI::BuildGUIForHelpFrame()
{

  // ----------------------------------------------------------------
  // HELP FRAME         
  // ----------------------------------------------------------------

  // Define your help text here.

  const char *help = "PatientToImageRegistration is a patient to image registration module for surgical navigation. \n<a>http://wiki.slicer.org/slicerWiki/index.php/Modules:PatientToImageRegistration-Documentation-3.4</a>";
  const char *about = "This work is supported by NA-MIC, NAC, BIRN, NCIGT, and the Slicer Community. See <a>http://www.slicer.org</a> for details. The PatientToImageRegistration module was contributed by Haiying Liu and Noby Hata at SPL, BWH (Ron Kikinis).";

  vtkKWWidget *page = this->UIPanel->GetPageWidget ( "PatientToImageRegistration" );
  this->BuildHelpAndAboutFrame (page, help, about);

}



void vtkPatientToImageRegistrationGUI::BuildGUIForLandmarksFrame ()
{
  vtkSlicerApplication *app = (vtkSlicerApplication *)this->GetApplication();
  vtkKWWidget *page = this->UIPanel->GetPageWidget ( "PatientToImageRegistration" );

  // ----------------------------------------------------------------
  // REGISTRATION FRAME            
  // ----------------------------------------------------------------
  vtkSlicerModuleCollapsibleFrame *regFrame = vtkSlicerModuleCollapsibleFrame::New ( );
  regFrame->SetParent ( page );
  regFrame->Create();
  regFrame->SetLabelText ("Landmark Registration");
  regFrame->CollapseFrame ( );
  app->Script ( "pack %s -side top -anchor nw -fill x -padx 2 -pady 2 -in %s",
                regFrame->GetWidgetName(), page->GetWidgetName());


  // add a point pair 
  vtkKWFrameWithLabel *addFrame = vtkKWFrameWithLabel::New();
  addFrame->SetParent ( regFrame->GetFrame() );
  addFrame->Create ( );
  addFrame->SetLabelText ("Acquire landmarks");
  this->Script( "pack %s -side top -anchor nw -fill x -padx 2 -pady 2",
                addFrame->GetWidgetName());

  vtkKWFrame *patFrame = vtkKWFrame::New();
  patFrame->SetParent ( addFrame->GetFrame() );
  patFrame->Create ( );
  this->Script( "pack %s -side top -anchor nw -expand n -padx 2 -pady 2",
                patFrame->GetWidgetName());

  vtkKWFrame *okFrame = vtkKWFrame::New();
  okFrame->SetParent ( addFrame->GetFrame() );
  okFrame->Create ( );
  this->Script( "pack %s -side top -anchor nw -expand n -padx 2 -pady 2",
                okFrame->GetWidgetName());

  this->PatCoordinatesEntry = vtkKWEntryWithLabel::New();
  this->PatCoordinatesEntry->SetParent(patFrame);
  this->PatCoordinatesEntry->Create();
  this->PatCoordinatesEntry->SetWidth(35);
  this->PatCoordinatesEntry->SetLabelWidth(16);
  this->PatCoordinatesEntry->SetLabelText("Patient landmark:");
  this->PatCoordinatesEntry->GetWidget()->SetValue ( "" );

  this->GetPatCoordinatesPushButton = vtkKWPushButton::New();
  this->GetPatCoordinatesPushButton->SetParent(patFrame);
  this->GetPatCoordinatesPushButton->Create();
  this->GetPatCoordinatesPushButton->SetText("Get");
  this->GetPatCoordinatesPushButton->SetWidth (10);

  this->Script(
               "pack %s %s -side left -anchor nw -expand n -padx 2 -pady 2", 
               this->PatCoordinatesEntry->GetWidgetName(),
               this->GetPatCoordinatesPushButton->GetWidgetName());

  this->SlicerCoordinatesEntry = vtkKWEntryWithLabel::New();
  this->SlicerCoordinatesEntry->SetParent(okFrame);
  this->SlicerCoordinatesEntry->Create();
  this->SlicerCoordinatesEntry->SetWidth(35);
  this->SlicerCoordinatesEntry->SetLabelWidth(16);
  this->SlicerCoordinatesEntry->SetLabelText("Image landmark:");
  this->SlicerCoordinatesEntry->GetWidget()->SetValue ( "" );
  this->Script( "pack %s -side top -anchor nw -expand n -padx 2 -pady 2",
                this->SlicerCoordinatesEntry->GetWidgetName());

  this->AddPointPairPushButton = vtkKWPushButton::New();
  this->AddPointPairPushButton->SetParent(okFrame);
  this->AddPointPairPushButton->Create();
  this->AddPointPairPushButton->SetText( "Add" );
  this->AddPointPairPushButton->SetWidth (10);
  this->Script( "pack %s -side top -anchor nw -expand n -padx 2 -pady 2",
                this->AddPointPairPushButton->GetWidgetName());

  // list of defined point pairs 
  vtkKWFrameWithLabel *listFrame = vtkKWFrameWithLabel::New();
  listFrame->SetParent ( regFrame->GetFrame() );
  listFrame->Create ( );
  listFrame->SetLabelText ("Acquired landmarks");
  this->Script( "pack %s -side top -anchor nw -fill x -padx 2 -pady 2",
                listFrame->GetWidgetName());


  // add the multicolumn list to show the points
  this->PointPairMultiColumnList = vtkKWMultiColumnListWithScrollbars::New ( );
  this->PointPairMultiColumnList->SetParent ( listFrame->GetFrame() );
  this->PointPairMultiColumnList->Create ( );
  this->PointPairMultiColumnList->SetHeight(1);
  this->PointPairMultiColumnList->GetWidget()->SetSelectionTypeToRow();
  this->PointPairMultiColumnList->GetWidget()->MovableRowsOff();
  this->PointPairMultiColumnList->GetWidget()->MovableColumnsOff();
  // set up the columns of data for each point
  // refer to the header file for order
  this->PointPairMultiColumnList->GetWidget()->AddColumn("On patient");
  this->PointPairMultiColumnList->GetWidget()->AddColumn("On image");

  // make the selected column editable by checkbox
  //    this->PointPairMultiColumnList->GetWidget()->SetColumnEditWindowToCheckButton(this->SelectedColumn);

  // now set the attributes that are equal across the columns
  for (int col = 0; col < 2; col++)
    {
    this->PointPairMultiColumnList->GetWidget()->SetColumnWidth(col, 22);
    this->PointPairMultiColumnList->GetWidget()->SetColumnAlignmentToLeft(col);
    this->PointPairMultiColumnList->GetWidget()->ColumnEditableOff(col);
    }
  /*
     if (col >= this->XColumn && col <= this->OrZColumn)
     {
     this->PointPairMultiColumnList->GetWidget()->SetColumnEditWindowToSpinBox(col);
     }
     */
  // set the name column width to be higher
  // this->PointPairMultiColumnList->GetWidget()->SetColumnWidth(this->NameColumn, 15);
  // set the selected column width a bit higher
  // this->PointPairMultiColumnList->GetWidget()->SetColumnWidth(this->SelectedColumn, 9);

  app->Script ( "pack %s -fill both -expand true",
                this->PointPairMultiColumnList->GetWidgetName());
  //                  listFrame->GetWidgetName());
  //    this->PointPairMultiColumnList->GetWidget()->SetCellUpdatedCommand(this, "UpdateElement");

  // button frame
  vtkKWFrame *buttonFrame = vtkKWFrame::New();
  buttonFrame->SetParent ( listFrame->GetFrame() );
  buttonFrame->Create ( );
  app->Script ("pack %s -side top -anchor nw -fill x -pady 0 -in %s",
               buttonFrame->GetWidgetName(),
               listFrame->GetFrame()->GetWidgetName());
  /*    
  // add an load button
  this->LoadPointPairPushButton = vtkKWPushButton::New ( );
  this->LoadPointPairPushButton->SetParent ( buttonFrame );
  this->LoadPointPairPushButton->Create ( );
  this->LoadPointPairPushButton->SetText ("Load Points");
  this->LoadPointPairPushButton->SetWidth (12);
  this->LoadPointPairPushButton->SetBalloonHelpString("Load point pairs from a file.");

  // add a save button
  this->SavePointPairPushButton = vtkKWPushButton::New ( );
  this->SavePointPairPushButton->SetParent ( buttonFrame );
  this->SavePointPairPushButton->Create ( );
  this->SavePointPairPushButton->SetText ("Save Points");
  this->SavePointPairPushButton->SetWidth (12);
  this->SavePointPairPushButton->SetBalloonHelpString("Save all defined point pairs to a file.");
  */

  // add a delete button 
  this->DeletePointPairPushButton = vtkKWPushButton::New ( );
  this->DeletePointPairPushButton->SetParent ( buttonFrame );
  this->DeletePointPairPushButton->Create ( );
  this->DeletePointPairPushButton->SetText ("Delete");
  this->DeletePointPairPushButton->SetWidth (12);
  this->DeletePointPairPushButton->SetBalloonHelpString("Delete the selected landmarks.");

  // add a delete button 
  this->DeleteAllPointPairPushButton = vtkKWPushButton::New ( );
  this->DeleteAllPointPairPushButton->SetParent ( buttonFrame );
  this->DeleteAllPointPairPushButton->Create ( );
  this->DeleteAllPointPairPushButton->SetText ("Delete all");
  this->DeleteAllPointPairPushButton->SetWidth (12);
  this->DeleteAllPointPairPushButton->SetBalloonHelpString("Delete all landmarks.");

  app->Script("pack %s %s -side left -anchor w -padx 2 -pady 2", 
              this->DeletePointPairPushButton->GetWidgetName(),
              this->DeleteAllPointPairPushButton->GetWidgetName());



  // do registration
  vtkKWFrame *actionFrame = vtkKWFrame::New();
  actionFrame->SetParent ( regFrame->GetFrame() );
  actionFrame->Create ( );
  this->Script( "pack %s -side top -anchor nw -fill x -padx 2 -pady 2",
                actionFrame->GetWidgetName());

  // add a register button 
  this->RegisterPushButton = vtkKWPushButton::New ( );
  this->RegisterPushButton->SetParent ( actionFrame );
  this->RegisterPushButton->Create ( );
  this->RegisterPushButton->SetText ("Register");
  this->RegisterPushButton->SetWidth (12);
  this->RegisterPushButton->SetBalloonHelpString("Perform patient to image registration.");

  // add a reset button 
  this->ResetPushButton = vtkKWPushButton::New ( );
  this->ResetPushButton->SetParent ( actionFrame );
  this->ResetPushButton->Create ( );
  this->ResetPushButton->SetText ("Reset");
  this->ResetPushButton->SetWidth (12);
  this->ResetPushButton->SetBalloonHelpString("Ignore the current registration.");


  app->Script("pack %s %s -side left -anchor w -padx 2 -pady 2", 
              this->RegisterPushButton->GetWidgetName(),
              this->ResetPushButton->GetWidgetName());


  regFrame->Delete ();
  addFrame->Delete ();
  patFrame->Delete ();
  okFrame->Delete ();
  listFrame->Delete ();
  buttonFrame->Delete ();
  actionFrame->Delete ();

}


void vtkPatientToImageRegistrationGUI::BuildGUIForICPFrame ()
{
  vtkSlicerApplication *app = (vtkSlicerApplication *)this->GetApplication();
  vtkKWWidget *page = this->UIPanel->GetPageWidget ( "PatientToImageRegistration" );


  // ----------------------------------------------------------------
  // Navigation FRAME            
  // ----------------------------------------------------------------
  vtkSlicerModuleCollapsibleFrame *icpFrame = vtkSlicerModuleCollapsibleFrame::New ( );    
  icpFrame->SetParent ( page );
  icpFrame->Create ( );
  icpFrame->SetLabelText ("Iterative Closest Point (ICP) Registration");
  icpFrame->CollapseFrame ( );
  app->Script ( "pack %s -side top -anchor nw -fill x -padx 2 -pady 2 -in %s",
                icpFrame->GetWidgetName(), page->GetWidgetName());

  icpFrame->Delete();
}


void vtkPatientToImageRegistrationGUI::TransformChangedCallback(double)
{
//    cout << "Transform changed.\n";
}


void vtkPatientToImageRegistrationGUI::TransformChangingCallback(double)
{
//    cout << "Transform changing.\n";
}

