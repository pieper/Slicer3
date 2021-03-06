#ifndef __qMRMLNodeTreeSelectorPlugin_h
#define __qMRMLNodeTreeSelectorPlugin_h

#include "qMRMLWidgetsAbstractPlugin.h"

class QMRML_WIDGETS_PLUGIN_EXPORT qMRMLNodeTreeSelectorPlugin : public QObject,
                                         public qMRMLWidgetsAbstractPlugin
{
  Q_OBJECT

public:
  qMRMLNodeTreeSelectorPlugin(QObject *_parent = 0);
  
  QWidget *createWidget(QWidget *_parent);
  QString  domXml() const;
  QIcon    icon() const;
  QString  includeFile() const;
  bool     isContainer() const;
  QString  name() const;
  
};

#endif
