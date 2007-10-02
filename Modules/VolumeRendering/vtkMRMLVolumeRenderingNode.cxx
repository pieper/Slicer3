#include "vtkMRMLVolumeRenderingNode.h"
#include "vtkMRMLNode.h"
#include "vtkVolumeTextureMapper3D.h"
#include "vtkPiecewiseFunction.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>


vtkMRMLVolumeRenderingNode* vtkMRMLVolumeRenderingNode::New()
{
        // First try to create the object from the vtkObjectFactory
        vtkObject* ret = vtkObjectFactory::CreateInstance("vtkMRMLVolumeRenderingNode");
        if(ret)
        {
                return (vtkMRMLVolumeRenderingNode*)ret;
        }
        // If the factory was unable to create the object, then create it here.
        return new vtkMRMLVolumeRenderingNode;
}

vtkMRMLNode* vtkMRMLVolumeRenderingNode::CreateNodeInstance(void)
{
        // First try to create the object from the vtkObjectFactory
        vtkObject* ret = vtkObjectFactory::CreateInstance("vtkMRMLVolumeRenderingNode");
        if(ret)
        {
                return (vtkMRMLVolumeRenderingNode*)ret;
        }
        // If the factory was unable to create the object, then create it here.
        return new vtkMRMLVolumeRenderingNode;
}

vtkMRMLVolumeRenderingNode::vtkMRMLVolumeRenderingNode(void)
{   
    Buffer=NULL;
    this->DebugOff();
    this->VolumeProperty=vtkVolumeProperty::New();
    //Standard is no Labelmap
    this->IsLabelMapOff();
    //Standard is 3D-Volume Texture Mapper
    this->Mapper=0;
}

vtkMRMLVolumeRenderingNode::~vtkMRMLVolumeRenderingNode(void)
{
}
void vtkMRMLVolumeRenderingNode::WriteXML(ostream& of, int nIndent)
{
  // Write all attributes not equal to their defaults
  
  Superclass::WriteXML(of, nIndent);
  
  //vtkIndent indent(nIndent);
  of << " isLabelmap=\""<<this->GetIsLabelMap() <<"\"";
  of << " references=\""<<this->References.size()<<" ";
  for(unsigned int i=0;i<this->References.size();i++)
  {
      of<<this->References.at(i);
      if(i!=(this->References.size()-1))
      {
        of<<" ";
      }
  }
  of<<"\"";

  of << " scalarOpacity=\"" << this->getPiecewiseFunctionString(this->VolumeProperty->GetScalarOpacity())  << "\"";
  of << " gradientOpacity=\"" <<this->getPiecewiseFunctionString(this->VolumeProperty->GetGradientOpacity())<< "\"";

  of << " colorTransfer=\"" <<this->getColorTransferFunctionString(this->VolumeProperty->GetRGBTransferFunction())<< "\"";
  of << " interpolation=\"" <<this->VolumeProperty->GetInterpolationType()<< "\"";
  of << " shade=\"" <<this->VolumeProperty->GetShade()<< "\"";
  of << " diffuse=\"" <<this->VolumeProperty->GetDiffuse()<< "\"";
  of << " ambient=\"" <<this->VolumeProperty->GetAmbient()<< "\"";
  of << " specular=\"" <<this->VolumeProperty->GetSpecular()<< "\"";
  of << " specularPower=\"" <<this->VolumeProperty->GetSpecularPower()<<"\"";
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeRenderingNode::ReadXMLAttributes(const char** atts)
{
    Superclass::ReadXMLAttributes(atts);

    const char* attName;
    const char* attValue;
    while (*atts!=NULL){
        attName= *(atts++);
        attValue= *(atts++);
        if(!strcmp(attName,"isLabelmap"))
        {
            int lm;
            std::stringstream ss;
            ss<<attValue;
            ss>>lm;
            this->SetIsLabelMap(lm);
        }
        if(!strcmp(attName,"references"))
        {
            int size=0;
            std::stringstream ss;
            ss << attValue;
            ss>>size;
            for(int i=0;i<size;i++)
            {
                std::string str;
                ss>>str;
                this->AddReference(str.c_str());
            }
        }
        if(!strcmp(attName,"scalarOpacity"))
        {
            vtkPiecewiseFunction *scalarOpacity=vtkPiecewiseFunction::New();
            this->GetPiecewiseFunctionFromString(attValue,scalarOpacity);
            this->VolumeProperty->SetScalarOpacity(scalarOpacity);
        }
        else if(!strcmp(attName,"gradientOpacity"))
        {
            vtkPiecewiseFunction *gradientOpacity=vtkPiecewiseFunction::New();
            this->GetPiecewiseFunctionFromString(attValue,gradientOpacity);
            this->VolumeProperty->SetGradientOpacity(gradientOpacity);
        }
        else if(!strcmp(attName,"colorTransfer"))
        {
            vtkColorTransferFunction *colorTransfer=vtkColorTransferFunction::New();
            this->GetColorTransferFunction(attValue,colorTransfer);
            this->VolumeProperty->SetColor(colorTransfer);
        }
        else if(!strcmp(attName,"interpolation"))
        {
            int interpolation;
            std::stringstream ss;
            ss <<attValue;
            ss>>interpolation;
            this->VolumeProperty->SetInterpolationType(interpolation);
        }
        else if(!strcmp(attName,"shade"))
        {
            int shade;
            std::stringstream ss;
            ss <<attValue;
            ss>>shade;
            this->VolumeProperty->SetShade(shade);
        }
        else if(!strcmp(attName,"diffuse"))
        {
            int diffuse;
            std::stringstream ss;
            ss<<attValue;
            ss>>diffuse;
            this->VolumeProperty->SetDiffuse(diffuse);
        }
        else if(!strcmp(attName,"ambient"))
        {
            int ambient;
            std::stringstream ss;
            ss<<attValue;
            ss>>ambient;
            this->VolumeProperty->SetAmbient(ambient);
        }
        else if(!strcmp(attName,"specular"))
        {
            int specular;
            std::stringstream ss;
            ss<<attValue;
            ss>>specular;
            this->VolumeProperty->SetSpecular(specular);
        }
        else if(!strcmp(attName,"specularPower"))
        {
            int specularPower;
            std::stringstream ss;
            ss<<attValue;
            ss>>specularPower;
            this->VolumeProperty->SetSpecularPower(specularPower);
        }//else if

        //TODO save Mapper
  }//while
    vtkDebugMacro("Finished reading in xml attributes, list id = " << this->GetID() << " and name = " << this->GetName() << endl);
}

//----------------------------------------------------------------------------
int vtkMRMLVolumeRenderingNode::ReadFile ()
{
  vtkErrorMacro("Subclass has not implemented ReadFile.");
  return 0;
}

//----------------------------------------------------------------------------
// Copy the node's attributes to this object.
// Does NOT copy: ID, FilePrefix, Name, ID
void vtkMRMLVolumeRenderingNode::Copy(vtkMRMLNode *anode)
{
    Superclass::Copy(anode);
    //cast
    vtkMRMLVolumeRenderingNode *node = (vtkMRMLVolumeRenderingNode *) anode;

    this->VolumeProperty->SetIndependentComponents(node->VolumeProperty->GetIndependentComponents());
    this->VolumeProperty->SetInterpolationType(node->VolumeProperty->GetInterpolationType());

    //VolumeProperty
    for (int i=0;i<VTK_MAX_VRCOMP;i++)
    {
        this->VolumeProperty->SetComponentWeight(i,node->GetVolumeProperty()->GetComponentWeight(i));
        //TODO problem no set method
        // vtkPiecewiseFunction *gray=node->GetVolumeProperty()->GetGrayTransferFunction=());L
        //   this->VolumeProperty->SetGry
        //TODO problem no set ColorChannels Method
        //this->VolumeProperty->SetCGetColorChannels(
        //mapping functions
        vtkColorTransferFunction *rgbTransfer=vtkColorTransferFunction::New();
        rgbTransfer->DeepCopy(node->GetVolumeProperty()->GetRGBTransferFunction(i));
        this->VolumeProperty->SetColor(i,rgbTransfer);

        vtkPiecewiseFunction *scalar=vtkPiecewiseFunction::New();
        scalar->DeepCopy(node->GetVolumeProperty()->GetScalarOpacity(i));
        this->VolumeProperty->SetScalarOpacity(i,scalar);

        this->VolumeProperty->SetScalarOpacityUnitDistance(i,this->VolumeProperty->GetScalarOpacityUnitDistance(i));

        vtkPiecewiseFunction *gradient=vtkPiecewiseFunction::New();
        gradient->DeepCopy(node->GetVolumeProperty()->GetGradientOpacity(i));
        this->VolumeProperty->SetGradientOpacity(i,gradient);

        //TODO Copy default gradient?
        this->VolumeProperty->SetDisableGradientOpacity(i,node->GetVolumeProperty()->GetDisableGradientOpacity(i));
        this->VolumeProperty->SetShade(i,node->GetVolumeProperty()->GetShade(i));
        this->VolumeProperty->SetAmbient(node->VolumeProperty->GetAmbient(i));
        this->VolumeProperty->SetDiffuse(node->VolumeProperty->GetDiffuse(i));
        this->VolumeProperty->SetSpecular(node->VolumeProperty->GetSpecular(i));
        this->VolumeProperty->SetSpecularPower(node->VolumeProperty->GetSpecularPower(i));
    }
    this->SetMapper(node->Mapper);
}

//----------------------------------------------------------------------------
void vtkMRMLVolumeRenderingNode::PrintSelf(ostream& os, vtkIndent indent)
{
  
    Superclass::PrintSelf(os,indent);
    os<<indent<<"VolumeProperty: ";
    this->VolumeProperty->PrintSelf(os,indent.GetNextIndent());
    os<<indent<<"Mapper: ";
    if(this->Mapper==Texture)
    {
        os<<"vtkVolumeTextureMapper3D";
    }
    else
    {
        os<<"FixedRayCastMapping";
    }
    os<<indent<<"References: ";
    for(unsigned int i=0;i<this->References.size();i++)
    {
        os<<this->References.at(i);
        if(i!=(this->References.size()-1))
        {
            os<<" ";
        }
    }
}

//-----------------------------------------------------------

void vtkMRMLVolumeRenderingNode::UpdateScene(vtkMRMLScene *scene)
{
   vtkErrorMacro("Subclass has not over ridden this method");
}


//---------------------------------------------------------------------------
void vtkMRMLVolumeRenderingNode::ProcessMRMLEvents ( vtkObject *caller,
                                           unsigned long event, 
                                           void *callData )
{
  vtkErrorMacro("Subclass has not over ridden this method");
  return;
}

//---------------------------------------------------------------------------
int vtkMRMLVolumeRenderingNode::GetFirstType()
{
  vtkErrorMacro("Subclass has not over ridden this method");
  return -1;
}

//---------------------------------------------------------------------------
int vtkMRMLVolumeRenderingNode::GetLastType()
{
  vtkErrorMacro("Subclass has not over ridden this method");
  return -1;
}

//---------------------------------------------------------------------------
const char * vtkMRMLVolumeRenderingNode::GetTypeAsString()
{
  vtkErrorMacro("Subclass has not over ridden this method");
  return "(unknown)";
}

std::string vtkMRMLVolumeRenderingNode::getPiecewiseFunctionString(vtkPiecewiseFunction* function)
{
    std::stringstream resultStream;
    int arraysize=function->GetSize()*2;
    double *data=function->GetDataPointer();
    double *it=data;
    //write header
    resultStream<<"vtkPiecewiseFunction#";
    resultStream<<arraysize;
    for (int i=0;i<arraysize;i++)
    {
        resultStream<<"#";
        resultStream<<*it;
        it++;

    }
    //Delete existing buffer
    if(Buffer)
    {
        delete []Buffer;
    }
    //std::string stri=resultStream.str();
    //Buffer=stri.c_str();
    return resultStream.str();
}
std::string  vtkMRMLVolumeRenderingNode::getColorTransferFunctionString(vtkColorTransferFunction* function)
{
    
    //maybe size*4
    std::stringstream resultStream;
    //resultStream.str
    int arraysize=function->GetSize()*4;
    double *data=function->GetDataPointer();
    double *it=data;
    //write header
    resultStream<<"vtkColorTransferFunction#"<<arraysize;
    for (int i=0;i<arraysize;i++)
    {
        resultStream<<"#";
        resultStream<<*it;
        it++;

    }
    //Delete existing buffer
    if(Buffer)
    {
        delete []Buffer;
    }
     return resultStream.str();
    //std::string stri=resultStream.str();
    //Buffer=stri.c_str();
    //return Buffer;
    //return Buffer;

}
void vtkMRMLVolumeRenderingNode::GetPiecewiseFunctionFromString(const char* string,vtkPiecewiseFunction* result)
{
    
    char *newOne=new char[strlen(string)];
    strcpy(newOne,string);
    int size=0;
    std::vector<double> dataVector;
  char * resultChar;
  resultChar = strtok (newOne,"#");
  if(strcmp(resultChar,"vtkPiecewiseFunction")!=0)
  {
    return;
  }
  else
  {
      resultChar = strtok (NULL,"#");
  }
  //get size
  size=(int)(strtod(resultChar,NULL));
  //nothing to fill
  if (size==0)
  {
      return;
  }
  //getPoints

        resultChar = strtok (NULL, "#");
  while (resultChar != NULL)
  {
    dataVector.push_back(strtod(resultChar,NULL));   
      resultChar = strtok (NULL, "#");

  }
   result->FillFromDataPointer(size/2,&dataVector[0]);
}
void vtkMRMLVolumeRenderingNode::GetColorTransferFunction(const char* string, vtkColorTransferFunction* result)
{

    char *newOne=new char[strlen(string)];
    strcpy(newOne,string);
     int size=0;
    std::vector<double> dataVector;
  char * resultChar;
  resultChar = strtok (newOne,"#");
  if(strcmp(resultChar,"vtkColorTransferFunction")!=0)
  {
    return;
  }
  else
  {
      resultChar = strtok (NULL,"#");
  }
  //get size
  size=(int)(strtod(resultChar,NULL));
  //nothing to fill
  if (size==0)
  {
      return;
  }
  //getPoints
  resultChar = strtok (NULL, "#");
  while (resultChar != NULL)
  {
      
      dataVector.push_back(strtod(resultChar,NULL));
      resultChar = strtok (NULL, "#");
  }
    result->FillFromDataPointer(size/4,&dataVector[0]);
}

void vtkMRMLVolumeRenderingNode::SetOpacityOfLabel(int index, double opacity)
{
        vtkErrorMacro("Not yet implemented");
}
double vtkMRMLVolumeRenderingNode::GetOpacityOfLabel(int index)
{
        vtkErrorMacro("Not yet implemented");
    return .0;
}

void vtkMRMLVolumeRenderingNode::AddReference(std::string id)
{
    //test if we already have a reference
    if(this->HasReference(id))
    {
        return;
    }
    else 
    {
        this->References.push_back(id);
    }

}
bool vtkMRMLVolumeRenderingNode::HasReference(std::string id)
{
    //loop over vector and comparing
    for(unsigned int i=0;i<this->References.size();i++)
    {
        if(strcmp(this->References.at(i).c_str(),id.c_str())==0)
        {
            return true;
        }
    }
    return false;
}
void vtkMRMLVolumeRenderingNode::RemoveReference(std::string id)
{
    vtkErrorMacro("Not yet implemented");

}

