#ifndef __vtkMRMLModelMirrorNode_h
#define __vtkMRMLModelMirrorNode_h

#include "vtkStringArray.h"

#include "vtkMRML.h"
#include "vtkMRMLNode.h"
#include "vtkMRMLModelNode.h"

#include "vtkModelMirrorWin32Header.h"
#include <string>
#include <vector>


class VTK_MODELMIRROR_EXPORT vtkMRMLModelMirrorNode : public vtkMRMLNode
{
  public:
  static vtkMRMLModelMirrorNode *New();
  vtkTypeRevisionMacro(vtkMRMLModelMirrorNode, vtkMRMLNode);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Create instance 
  virtual vtkMRMLNode* CreateNodeInstance();

  // Description:
  // Set node attributes from name/value pairs
  virtual void ReadXMLAttributes( const char** atts);

  // Description:
  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);
  
  // Description:
  // Get unique node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {return "ModelMirrorNode"; };
  
  vtkSetObjectMacro ( InputModel, vtkMRMLModelNode );
  vtkGetObjectMacro ( InputModel, vtkMRMLModelNode );

  vtkSetObjectMacro ( OutputModel, vtkMRMLModelNode );
  vtkGetObjectMacro ( OutputModel, vtkMRMLModelNode );
  
  vtkGetMacro ( MirrorPlane, int );
  vtkSetMacro ( MirrorPlane, int );

  vtkGetStringMacro ( NewModelName );
  vtkSetStringMacro ( NewModelName );
  
  //BTX
  enum
    {
      ModelAddedEvent = 11000,
    };
  enum
    {
      AxialMirror = 0,
      SaggitalMirror,
      CoronalMirror,
    };
  //ETX
  vtkMRMLModelNode *InputModel;
  vtkMRMLModelNode *OutputModel;
  char *NewModelName;
  int MirrorPlane;
  

 protected:
  vtkMRMLModelMirrorNode();
  ~vtkMRMLModelMirrorNode();
  vtkMRMLModelMirrorNode(const vtkMRMLModelMirrorNode&);
  void operator=(const vtkMRMLModelMirrorNode&);

};

#endif

