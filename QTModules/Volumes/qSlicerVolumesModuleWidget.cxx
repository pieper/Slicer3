#include "qSlicerVolumesModuleWidget.h"
#include "ui_qSlicerVolumesModule.h"

// QT includes
#include <QDebug>

//-----------------------------------------------------------------------------
class qSlicerVolumesModuleWidgetPrivate: public qCTKPrivate<qSlicerVolumesModuleWidget>,
                                          public Ui_qSlicerVolumesModule
{
public:
};

//-----------------------------------------------------------------------------
QCTK_CONSTRUCTOR_1_ARG_CXX(qSlicerVolumesModuleWidget, QWidget*);

//-----------------------------------------------------------------------------
void qSlicerVolumesModuleWidget::setup()
{
  QCTK_D(qSlicerVolumesModuleWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
QAction* qSlicerVolumesModuleWidget::showModuleAction()
{
  return new QAction(QIcon(":/Icons/Volumes.png"), tr("Show Volumes module"), this);
}
