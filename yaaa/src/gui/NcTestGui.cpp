#include <QtGui>
#include "NcTestGui.h"
#include "NcGfx2D.h"

TestDialog::TestDialog(QWidget* parent) : QDialog(parent)
{
  gfx = new NcGfx2D;
}

TestDialog::~TestDialog()
{
  delete gfx;
}

void TestDialog::paintEvent(QPaintEvent *)
{
  QPainter p(this);
  gfx->drawChart(&p);
}
