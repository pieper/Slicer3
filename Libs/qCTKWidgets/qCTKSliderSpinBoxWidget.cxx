/*=========================================================================

  Library:   qCTK

  Copyright (c) Kitware Inc. 
  All rights reserved.
  Distributed under a BSD License. See LICENSE.txt file.

  This software is distributed "AS IS" WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the above copyright notice for more information.

=========================================================================*/


#include "qCTKSliderSpinBoxWidget.h"
#include "ui_qCTKSliderSpinBoxWidget.h"

#include <QDebug>

bool equal(double v1, double v2)
{
  return qAbs(v1 - v2) < 0.0001;
}

//-----------------------------------------------------------------------------
class qCTKSliderSpinBoxWidgetPrivate: public qCTKPrivate<qCTKSliderSpinBoxWidget>,
                                      public Ui_qCTKSliderSpinBoxWidget
{
public:
  qCTKSliderSpinBoxWidgetPrivate();
  void updateSpinBoxWidth();
  int synchronizedSpinBoxWidth()const;
  void synchronizeSiblingSpinBox(int newWidth);

  bool AutoSpinBoxWidth;
};

// --------------------------------------------------------------------------
qCTKSliderSpinBoxWidgetPrivate::qCTKSliderSpinBoxWidgetPrivate()
{
  this->AutoSpinBoxWidth = true;
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidgetPrivate::updateSpinBoxWidth()
{
  int spinBoxWidth = this->synchronizedSpinBoxWidth();
  if (this->AutoSpinBoxWidth)
    {
    this->SpinBox->setMinimumWidth(spinBoxWidth);
    }
  else
    {
    this->SpinBox->setMinimumWidth(0);
    }
  this->synchronizeSiblingSpinBox(spinBoxWidth);
}

// --------------------------------------------------------------------------
int qCTKSliderSpinBoxWidgetPrivate::synchronizedSpinBoxWidth()const
{
  QCTK_P(const qCTKSliderSpinBoxWidget);
  int maxWidth = this->SpinBox->sizeHint().width();
  if (!p->parent())
    {
    return maxWidth;
    }
  QList<qCTKSliderSpinBoxWidget*> siblings = 
    p->parent()->findChildren<qCTKSliderSpinBoxWidget*>();
  foreach(qCTKSliderSpinBoxWidget* sibling, siblings)
    {
    maxWidth = qMax(maxWidth, sibling->qctk_d()->SpinBox->sizeHint().width());
    }
  return maxWidth;
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidgetPrivate::synchronizeSiblingSpinBox(int width)
{
  QCTK_P(const qCTKSliderSpinBoxWidget);
  QList<qCTKSliderSpinBoxWidget*> siblings = 
    p->parent()->findChildren<qCTKSliderSpinBoxWidget*>();
  foreach(qCTKSliderSpinBoxWidget* sibling, siblings)
    {
    if (sibling != p && sibling->isAutoSpinBoxWidth())
      {
      sibling->qctk_d()->SpinBox->setMinimumWidth(width);
      }
    }
}

// --------------------------------------------------------------------------
qCTKSliderSpinBoxWidget::qCTKSliderSpinBoxWidget(QWidget* _parent) : Superclass(_parent)
{
  QCTK_INIT_PRIVATE(qCTKSliderSpinBoxWidget);
  QCTK_D(qCTKSliderSpinBoxWidget);
  
  d->setupUi(this);

  //d->Slider->setTracking(false);
  //d->SpinBox->setKeyboardTracking(false);
  d->Slider->setMaximum(d->SpinBox->maximum());
  d->Slider->setMinimum(d->SpinBox->minimum());

  this->connect(d->Slider, SIGNAL(sliderMoved(double)), SIGNAL(sliderMoved(double)));
  this->connect(d->Slider, SIGNAL(valueChanged(double)), d->SpinBox, SLOT(setValue(double)));
  this->connect(d->SpinBox, SIGNAL(valueChanged(double)), d->Slider, SLOT(setValue(double)));
  this->connect(d->Slider, SIGNAL(valueChanged(double)), SIGNAL(valueChanged(double)));
}

// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::minimum()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  Q_ASSERT(equal(d->SpinBox->minimum(),d->Slider->minimum()));
  return d->Slider->minimum();
}

// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::maximum()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  Q_ASSERT(equal(d->SpinBox->maximum(),d->Slider->maximum()));
  return d->Slider->maximum();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setMinimum(double min)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->SpinBox->setMinimum(min);
  // SpinBox can truncate min (depending on decimals).
  // use Spinbox's min to set Slider's min
  d->Slider->setMinimum(d->SpinBox->minimum());
  Q_ASSERT(equal(d->SpinBox->minimum(),d->Slider->minimum()));
  d->updateSpinBoxWidth();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setMaximum(double max)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->SpinBox->setMaximum(max);
  // SpinBox can truncate max (depending on decimals).
  // use Spinbox's max to set Slider's max
  d->Slider->setMaximum(d->SpinBox->maximum());
  Q_ASSERT(equal(d->SpinBox->maximum(), d->Slider->maximum()));
  d->updateSpinBoxWidth();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setRange(double min, double max)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  
  d->SpinBox->setRange(min, max);
  // SpinBox can truncate the range (depending on decimals).
  // use Spinbox's range to set Slider's range
  d->Slider->setRange(d->SpinBox->minimum(), d->SpinBox->maximum());
  Q_ASSERT(equal(d->SpinBox->minimum(), d->Slider->minimum()));
  Q_ASSERT(equal(d->SpinBox->maximum(), d->Slider->maximum()));
  d->updateSpinBoxWidth();
}

// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::sliderPosition()const
{
  return qctk_d()->Slider->sliderPosition();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setSliderPosition(double position)
{
  qctk_d()->Slider->setSliderPosition(position);
}

/*
// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::previousSliderPosition()
{
  return qctk_d()->Slider->previousSliderPosition();
}
*/

// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::value()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  Q_ASSERT(equal(d->Slider->value(), d->SpinBox->value()));
  return d->Slider->value();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setValue(double _value)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  //qDebug() << __FUNCTION__ << "set: " << _value;
  //qDebug() << __FUNCTION__ << "old values: " << d->Slider->value() << " " << d->SpinBox->value();
  d->SpinBox->setValue(_value);
  //qDebug() << __FUNCTION__ << "inter: " << d->Slider->value() << d->SpinBox->value();
//  d->SpinBox->blockSignals(true);
  d->Slider->setValue(d->SpinBox->value());
//  d->SpinBox->blockSignals(false);
  //qDebug() << __FUNCTION__ << "new values: " << d->Slider->value() << " " << d->SpinBox->value();
  Q_ASSERT(equal(d->Slider->value(), d->SpinBox->value()));
}

// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::singleStep()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  Q_ASSERT(equal(d->Slider->singleStep(), d->SpinBox->singleStep()));
  return d->Slider->singleStep();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setSingleStep(double step)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->SpinBox->setSingleStep(step);
  d->Slider->setSingleStep(d->SpinBox->singleStep());
  Q_ASSERT(equal(d->Slider->singleStep(), d->SpinBox->singleStep()));
}

// --------------------------------------------------------------------------
int qCTKSliderSpinBoxWidget::decimals()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  return d->SpinBox->decimals();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setDecimals(int newDecimals)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->SpinBox->setDecimals(newDecimals);
}

// --------------------------------------------------------------------------
QString qCTKSliderSpinBoxWidget::prefix()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  return d->SpinBox->prefix();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setPrefix(const QString& newPrefix)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->SpinBox->setPrefix(newPrefix);
}

// --------------------------------------------------------------------------
QString qCTKSliderSpinBoxWidget::suffix()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  return d->SpinBox->suffix();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setSuffix(const QString& newSuffix)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->SpinBox->setSuffix(newSuffix);
}

// --------------------------------------------------------------------------
double qCTKSliderSpinBoxWidget::tickInterval()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  return d->Slider->tickInterval();
}

// --------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setTickInterval(double ti)
{ 
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->Slider->setTickInterval(ti);
}

// -------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::reset()
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  
  d->Slider->setValue(0);
/*
  d->SpinBox->blockSignals(true);
  d->SpinBox->setValue(0);
  d->SpinBox->blockSignals(false);
*/
}

// -------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setSpinBoxAlignment(Qt::Alignment alignment)
{
  return qctk_d()->SpinBox->setAlignment(alignment);
}

// -------------------------------------------------------------------------
Qt::Alignment qCTKSliderSpinBoxWidget::spinBoxAlignment()const
{
  return qctk_d()->SpinBox->alignment();
}

// -------------------------------------------------------------------------
bool qCTKSliderSpinBoxWidget::isAutoSpinBoxWidth()const
{
  QCTK_D(const qCTKSliderSpinBoxWidget);
  return d->AutoSpinBoxWidth;
}

// -------------------------------------------------------------------------
void qCTKSliderSpinBoxWidget::setAutoSpinBoxWidth(bool autoWidth)
{
  QCTK_D(qCTKSliderSpinBoxWidget);
  d->AutoSpinBoxWidth = autoWidth;
  d->updateSpinBoxWidth();
}
