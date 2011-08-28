#include <QtGui>
#include "NcMainWindow.h"
#include "NcTestGui.h"
#include "NcPerson.h"
#include "NcFormPersonDataEntry.h"
#include "NcEvent.h"
#include "NcPersonFile.h"

class NcPimplMainWindow
{
public:
  QMenu* menuFile;
  QMenu* menuTest;
  QMenu* menuHelp;

  //File
  QAction* actNew;
  QAction* actExit;

  //Test
  QAction* actTest1;
  QAction* actGenAtlasDb;

  //Help
  QAction* actHelp;
  QAction* actAbout;

};


NcMainWindow::NcMainWindow(QWidget* parent)
  : QMainWindow(parent),d(new NcPimplMainWindow)
{
  resize(640,480);
  init();
  onTest1();
}

NcMainWindow::~NcMainWindow()
{
  delete d;
}


void NcMainWindow::init()
{
  createActions();
  createMenus();
}

void NcMainWindow::createActions()
{
  //File
  d->actNew = new QAction(tr("New"),this);
  d->actExit = new QAction(tr("Exit"),this);

  connect(d->actNew,SIGNAL(triggered(bool)),this,SLOT(onNew()));
  connect(d->actExit,SIGNAL(triggered(bool)),this,SLOT(onExit()));

  //Test
  d->actTest1 = new QAction(tr("Test1"),this);
  d->actGenAtlasDb = new QAction(tr("Generate atlas DB"),this);

  connect(d->actTest1, SIGNAL(triggered(bool)), this, SLOT(onTest1()));
  connect(d->actGenAtlasDb, SIGNAL(triggered(bool)), this, SLOT(onGenAtlasDb()));

  //Help
  d->actHelp = new QAction(tr("Help"),this);
  d->actAbout = new QAction(tr("About"),this);

  connect(d->actHelp,SIGNAL(triggered(bool)),this,SLOT(onHelp()));
  connect(d->actAbout,SIGNAL(triggered(bool)),this,SLOT(onAbout()));
}

void NcMainWindow::createMenus()
{
  QMenuBar* bar = menuBar();

  d->menuFile = bar->addMenu(tr("File"));
  d->menuFile->addAction(d->actNew);
  d->menuFile->addAction(d->actExit);

  d->menuTest = bar->addMenu(tr("Test"));
  d->menuTest->addAction(d->actTest1);
  d->menuTest->addAction(d->actGenAtlasDb);

  d->menuHelp = bar->addMenu(tr("Help"));
  d->menuHelp->addAction(d->actHelp);
  d->menuHelp->addAction(d->actAbout);
}

void NcMainWindow::onNew()
{
  NcPerson* person = new NcPerson;
  NcFormPersonDataEntry form;
  if ( QDialog::Accepted == form.exec()  )
  {
    form.getData( person );
    person->birthEvent()->update();

    NcPersonFile file;

    file.savePerson( person, "/home/nish/Desktop/charts/nishant.ajc" );

    TestDialog* dlg = new TestDialog(person);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    this->setCentralWidget(dlg);

  }

}

void NcMainWindow::onExit()
{
  QApplication::closeAllWindows();
}

void NcMainWindow::onTest1()
{
  //NcMaitreyaTextclient tc;
  //tc.run();
   //TestDialog* dlg = new TestDialog;
  //dlg->setAttribute(Qt::WA_DeleteOnClose);
  //this->setCentralWidget(dlg);
}

void NcMainWindow::onGenAtlasDb()
{
  QString srcPath = QCoreApplication::applicationDirPath();
  QString destPath = srcPath;

  srcPath += "/data/atlas/raw";
  destPath += "/data/atlas/db";

//  AtlasGenerator ag;
//  if(ag.generateDatabase(srcPath, destPath))
//    QMessageBox::information(this,tr("Success"),tr("Done"));
//  else
//    QMessageBox::information(this,tr("Error"),ag.lastError());
}

void NcMainWindow::onHelp()
{
  QMessageBox::about(this,tr("Help"),"No help is written till now");
}

void NcMainWindow::onAbout()
{
  QMessageBox::about(this,tr("About"),tr("Version 0"));
}
