/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See Doc/copyright/copyright.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkMRMLAtlasCreatorNode.h,v $
  Date:      $Date: 2006/03/19 17:12:29 $
  Version:   $Revision: 1.3 $

=========================================================================auto=*/
#ifndef __vtkMRMLAtlasCreatorNode_h
#define __vtkMRMLAtlasCreatorNode_h

#include "vtkMRML.h"
#include "vtkMRMLNode.h"

#include "vtkAtlasCreatorCxxModuleWin32Header.h"

// Description: 
// The atlas creator node is simply a MRMLNode container for
// a configuration of the Atlas Creator module

class VTK_ATLASCREATORCXXMODULE_EXPORT vtkMRMLAtlasCreatorNode : public vtkMRMLNode
{
  public:
  static vtkMRMLAtlasCreatorNode *New();
  vtkTypeMacro(vtkMRMLAtlasCreatorNode,vtkMRMLNode);
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual vtkMRMLNode* CreateNodeInstance();

  // Description:
  // Set node attributes
  virtual void ReadXMLAttributes( const char** atts);

  // Description:
  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);

  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName()
    {return "AtlasCreator";};

  //BTX
  enum
  {
    LaunchComputationEvent = 31337
  };
  //ETX

  // Description: Launch the Atlas Creator computation with the assigned parameters
  void Launch();

  // Description: The originalImages FilePaths divided by space
  vtkGetStringMacro (OriginalImagesFilePathList);
  vtkSetStringMacro (OriginalImagesFilePathList);

  // Description: The segmentation FilePaths divided by space
  vtkGetStringMacro (SegmentationsFilePathList);
  vtkSetStringMacro (SegmentationsFilePathList);

  // Description: The output Directory
  vtkGetStringMacro (OutputDirectory);
  vtkSetStringMacro (OutputDirectory);


  // Description: The Toolkit: "BRAINSFit" or "CMTK"
  vtkGetStringMacro (Toolkit);
  vtkSetStringMacro (Toolkit);


  // Description: The Template Type: "fixed" or "dynamic"
  vtkGetStringMacro (TemplateType);
  vtkSetStringMacro (TemplateType);

  // Description: The Number of Iterations for DynamicTemplate
  vtkGetMacro(DynamicTemplateIterations,int);
  vtkSetMacro(DynamicTemplateIterations,int);

  // Description: The Default Case FilePath for FixedTemplate
  vtkGetStringMacro (FixedTemplateDefaultCaseFilePath);
  vtkSetStringMacro (FixedTemplateDefaultCaseFilePath);


  // Description: The Labels divided by space
  vtkGetStringMacro (LabelsList);
  vtkSetStringMacro (LabelsList);


  // Description: The Registration Type: "Affine" or "Non-Rigid"
  vtkGetStringMacro (RegistrationType);
  vtkSetStringMacro (RegistrationType);


  // Description: 1: Save Transforms 0: Don't save
  vtkGetMacro(SaveTransforms,int);
  vtkSetMacro(SaveTransforms,int);

  // Description: 1: Delete Aligned Images 0: Don't delete
  vtkGetMacro(DeleteAlignedImages,int);
  vtkSetMacro(DeleteAlignedImages,int);

  // Description: 1: Delete Aligned Segmentations 0: Don't delete
  vtkGetMacro(DeleteAlignedSegmentations,int);
  vtkSetMacro(DeleteAlignedSegmentations,int);

  // Description: 1: Normalize Atlases 0: Don't normalize
  vtkGetMacro(NormalizeAtlases,int);
  vtkSetMacro(NormalizeAtlases,int);


  // Description: The Output Cast
  //   "Char"
  //   "Unsigned Char"
  //   "Double"
  //   "Float"
  //   "Int"
  //   "Unsigned Int"
  //   "Long"
  //   "Unsigned Long"
  //   "Short"
  //   "Unsigned Short"
  vtkGetStringMacro (OutputCast);
  vtkSetStringMacro (OutputCast);


  // Description: 1: Use Cluster 0: Don't use cluster
  vtkGetMacro(UseCluster,int);
  vtkSetMacro(UseCluster,int);

  // Description: The Scheduler Command
  vtkGetStringMacro (SchedulerCommand);
  vtkSetStringMacro (SchedulerCommand);


  // Description: 1: Skip Registration 0: Don't skip registration
  vtkGetMacro(SkipRegistration,int);
  vtkSetMacro(SkipRegistration,int);

  // Description: The Existing Template (Filepath)
  vtkGetStringMacro (ExistingTemplate);
  vtkSetStringMacro (ExistingTemplate);

  // Description: The Transforms Directory
  vtkGetStringMacro (TransformsDirectory);
  vtkSetStringMacro (TransformsDirectory);

protected:

  char *OriginalImagesFilePathList;
  char *SegmentationsFilePathList;
  char *OutputDirectory;

  char *Toolkit;

  char *TemplateType;
  int DynamicTemplateIterations;
  char *FixedTemplateDefaultCaseFilePath;
  
  char *LabelsList;

  char *RegistrationType;

  int SaveTransforms;
  int DeleteAlignedImages;
  int DeleteAlignedSegmentations;
  int NormalizeAtlases;

  char *OutputCast;

  int UseCluster;
  char *SchedulerCommand;

  int SkipRegistration;
  char *ExistingTemplate;
  char *TransformsDirectory;


private:
  vtkMRMLAtlasCreatorNode();
  ~vtkMRMLAtlasCreatorNode();
  vtkMRMLAtlasCreatorNode(const vtkMRMLAtlasCreatorNode&);
  void operator=(const vtkMRMLAtlasCreatorNode&);

};

#endif
