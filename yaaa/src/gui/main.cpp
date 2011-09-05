#include <QApplication>
#include "NcMainWindow.h"

int main( int argc, char** argv )
{
  QApplication app( argc, argv );
  app.setAttribute(Qt::AA_DontShowIconsInMenus, false );

  NcMainWindow mw;
  mw.show();

  return app.exec();
}
