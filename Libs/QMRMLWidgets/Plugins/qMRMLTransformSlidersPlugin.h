#ifndef __qMRMLTransformSlidersPlugin_h
#define __qMRMLTransformSlidersPlugin_h

#include "qMRMLWidgetsPlugin.h"
#include "qMRMLWidgetsPluginWin32Header.h"

class QMRML_WIDGETS_PLUGIN_EXPORT qMRMLTransformSlidersPlugin : public QObject,
                                    public qMRMLWidgetsPlugin
{
  Q_OBJECT

public:
  qMRMLTransformSlidersPlugin(QObject *_parent = 0);
  
  QWidget *createWidget(QWidget *_parent);
  QString domXml() const;
  QString includeFile() const;
  bool isContainer() const;
  QString name() const;
  
};

#endif
