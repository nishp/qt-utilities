#include "NcMdiSubWindow.h"
#include "NcPerson.h"

NcMdiSubWindow::NcMdiSubWindow(NcPerson *native)
{
  this->native = native;
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowState(Qt::WindowMaximized);
  setWindowTitle(native->fullName());
}

NcMdiSubWindow::~NcMdiSubWindow()
{

}
