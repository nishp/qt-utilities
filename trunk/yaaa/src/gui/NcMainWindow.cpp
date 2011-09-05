#include <QtGui>
#include "NcMainWindow.h"
#include "NcTestGui.h"
#include "NcPerson.h"
#include "NcFormPersonDataEntry.h"
#include "NcEvent.h"
#include "NcPersonFile.h"
#include "NcMdiSubWindow.h"

class NcPimplMainWindow
{
public:

  QMenu *menuMain;

  //File
  QMenu* menuFile;
  QAction* actNew;
  QAction* actOpen;
  QAction* actSave;
  QAction* actSaveAll;
  QAction* actClose;
  QAction* actCloseAll;
  QAction* actQuit;

  //Edit
  QMenu* menuEdit;
  QAction *actEditBirthData;

  //Tools
  QMenu* menuTools;
  QAction *actOptions;

  //Help
  QMenu* menuHelp;
  QAction* actHelp;
  QAction* actAbout;

  //Test
  QMenu* menuTest;
  QAction* actTest1;
  QAction* actGenAtlasDb;

  QMdiArea *mdiArea;

};


NcMainWindow::NcMainWindow(QWidget* parent)
  : QMainWindow(parent),d(new NcPimplMainWindow)
{
  //qApp->setStyle( new QPlastiqueStyle );
  //qApp->setStyle( new QWindowsStyle );
  createMenuBar();
  createToolBar();
  createMdiArea();
  createStatusBar();

  //resize(640,480);

}

NcMainWindow::~NcMainWindow()
{
  delete d;
}

void NcMainWindow::createMenuBar()
{
  createFileMenu();
  createEditMenu();
  createToolsMenu();
  createHelpMenu();
  createTestMenu();

  QMenuBar *bar = menuBar();
  //QMenu *bar = new QMenu(tr("Menu"),this);
  //menuBar()->addMenu(bar);
  //d->menuMain = bar;


  bar->addMenu( d->menuFile );
  bar->addMenu( d->menuEdit );
  bar->addMenu( d->menuTools );
  bar->addMenu( d->menuHelp );
  bar->addSeparator();
  bar->addMenu( d->menuTest );
}

void NcMainWindow::createToolBar()
{
  QToolBar *toolbar = addToolBar(tr("Main"));
  //toolbar->setMovable(false);
  //QToolButton *btn = new QToolButton;
  //btn->setPopupMode(QToolButton::InstantPopup);
  //btn->setMenu(d->menuMain);
  //toolbar->addWidget(btn);
  toolbar->addAction(d->actNew);
}

void NcMainWindow::createMdiArea()
{
  d->mdiArea = new QMdiArea;
  d->mdiArea->setViewMode( QMdiArea::TabbedView );
  //d->mdiArea->setDocumentMode(true);
  this->setCentralWidget(d->mdiArea);

}

void NcMainWindow::createStatusBar()
{
  statusBar()->addPermanentWidget( new QLabel(tr("Ready")) );
}

void NcMainWindow::createFileMenu()
{

  QStyle *style = qApp->style();

  d->actNew = new QAction(tr("New"),this);
  d->actNew->setShortcut(QKeySequence::New);
  d->actNew->setIcon(style->standardIcon(QStyle::SP_FileIcon));
  connect(d->actNew,SIGNAL(triggered(bool)),this,SLOT(onNew()));

  d->actOpen = new QAction(tr("Open"),this);
  d->actOpen->setShortcut(QKeySequence::Open);
  d->actOpen->setIcon(style->standardIcon(QStyle::SP_DirOpenIcon));
  connect(d->actOpen,SIGNAL(triggered(bool)),this,SLOT(onOpen()));

  d->actSave = new QAction(tr("Save"),this);
  d->actSave->setShortcut(QKeySequence::Save);
  d->actSave->setIcon(style->standardIcon(QStyle::SP_DriveFDIcon));
  connect(d->actSave,SIGNAL(triggered(bool)),this,SLOT(onSave()));

  d->actSaveAll = new QAction(tr("Save All"),this);
  //d->actSaveAll->setShortcut(QKeySequence::SaveAll);
  d->actSaveAll->setIcon(style->standardIcon(QStyle::SP_DialogOpenButton));
  connect(d->actSaveAll,SIGNAL(triggered(bool)),this,SLOT(onSaveAll()));

  d->actClose = new QAction(tr("Close"),this);
  d->actClose->setShortcut(QKeySequence::Close);
  //d->->setIcon(style->standardIcon(QStyle::SP_));
  connect(d->actClose,SIGNAL(triggered(bool)),this,SLOT(onClose()));

  d->actCloseAll = new QAction(tr("Close All"),this);
  //d->actCloseAll->setShortcut(QKeySequence::);
  connect(d->actCloseAll,SIGNAL(triggered(bool)),this,SLOT(onCloseAll()));

  d->actQuit = new QAction(tr("Exit"),this);
  d->actQuit->setShortcut(QKeySequence::Quit);
  connect(d->actQuit,SIGNAL(triggered(bool)),this,SLOT(onQuit()));

  d->menuFile = new QMenu(tr("&File"), this);
  d->menuFile->addAction(d->actNew);
  d->menuFile->addAction(d->actOpen);
  d->menuFile->addSeparator();
  d->menuFile->addAction(d->actSave);
  d->menuFile->addAction(d->actSaveAll);
  d->menuFile->addSeparator();
  d->menuFile->addAction(d->actClose);
  d->menuFile->addAction(d->actCloseAll);
  d->menuFile->addSeparator();
  d->menuFile->addAction(d->actQuit);
}

void NcMainWindow::createEditMenu()
{
  d->actEditBirthData = new QAction(tr("Birth Data"),this);
  //d->actQuit->setShortcut(QKeySequence::Quit);
  connect(d->actEditBirthData,SIGNAL(triggered(bool)),this,SLOT(onEditBirthData()));

  d->menuEdit = new QMenu(tr("Edit"), this);
  d->menuEdit->addAction(d->actEditBirthData);
}

void NcMainWindow::createToolsMenu()
{
  d->actOptions = new QAction(tr("Options"),this);
  //d->actQuit->setShortcut(QKeySequence::Quit);
  connect(d->actOptions,SIGNAL(triggered(bool)),this,SLOT(onOptions()));

  d->menuTools = new QMenu(tr("Tools"), this);
  d->menuTools->addAction(d->actOptions);
}

void NcMainWindow::createHelpMenu()
{
  d->actHelp = new QAction(tr("Help"),this);
  //d->actQuit->setShortcut(QKeySequence::Quit);
  connect(d->actHelp,SIGNAL(triggered(bool)),this,SLOT(onHelp()));

  d->actAbout = new QAction(tr("About"),this);
  //d->actQuit->setShortcut(QKeySequence::Quit);
  connect(d->actAbout,SIGNAL(triggered(bool)),this,SLOT(onAbout()));

  d->menuHelp = new QMenu(tr("Help"), this);
  d->menuHelp->addAction(d->actHelp);
  d->menuHelp->addAction(d->actAbout);

}

void NcMainWindow::createTestMenu()
{
  d->actTest1 = new QAction(tr("Test1"),this);
  connect(d->actTest1, SIGNAL(triggered(bool)), this, SLOT(onTest1()));

  d->actGenAtlasDb = new QAction(tr("Generate atlas DB"),this);
  connect(d->actGenAtlasDb, SIGNAL(triggered(bool)), this, SLOT(onGenAtlasDb()));

  d->menuTest = new QMenu(tr("Test"), this);
  d->menuTest->addAction(d->actTest1);
  d->menuTest->addAction(d->actGenAtlasDb);

}

void NcMainWindow::onNew()
{
  NcPerson* person = new NcPerson;
  NcFormPersonDataEntry form;
  form.getData( person );
  person->birthEvent()->update();

  QMdiSubWindow *sw = new NcMdiSubWindow(person);
  d->mdiArea->addSubWindow( sw )->setWindowState(Qt::WindowMaximized);
  //sw->showMaximized();


  //if ( QDialog::Accepted == form.exec()  )
  {

    //NcPersonFile file;

    //file.savePerson( person, "/home/nish/Desktop/charts/nishant.ajc" );

    //TestDialog* dlg = new TestDialog(person);
    //dlg->setAttribute(Qt::WA_DeleteOnClose);
   // QMdiSubWindow* subWindow = d->mdiArea->addSubWindow(dlg);
    //subWindow->setAttribute(Qt::WA_DeleteOnClose);
    //subWindow->setWindowState(Qt::WindowMaximized);
    //subWindow->setWindowTitle(person->fullName());


  }

}

void NcMainWindow::onOpen()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

void NcMainWindow::onSave()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

void NcMainWindow::onSaveAll()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

void NcMainWindow::onClose()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

void NcMainWindow::onCloseAll()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

void NcMainWindow::onQuit()
{
  QApplication::closeAllWindows();
}

//Edit
void NcMainWindow::onEditBirthData()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

//Tools
void NcMainWindow::onOptions()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
}

void NcMainWindow::onHelp()
{
  QMessageBox::about(this,tr("Help"),"No help is written till now");
}

void NcMainWindow::onAbout()
{
  QMessageBox::about(this,tr("About"),tr("Version 0"));
}


void NcMainWindow::onTest1()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
  //NcMaitreyaTextclient tc;
  //tc.run();
   //TestDialog* dlg = new TestDialog;
  //dlg->setAttribute(Qt::WA_DeleteOnClose);
  //this->setCentralWidget(dlg);
}

void NcMainWindow::onGenAtlasDb()
{
  QMessageBox::information(this,Q_FUNC_INFO,Q_FUNC_INFO);
  return;

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

