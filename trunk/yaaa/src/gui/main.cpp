#include <QApplication>
#include "NcMainWindow.h"
#include "ApplicationWindow.h"
#include <wx/app.h>

int main( int argc, char** argv )
{
  QApplication app( argc, argv );
  NcMainWindow mw;
  mw.show();
  return app.exec();
}
