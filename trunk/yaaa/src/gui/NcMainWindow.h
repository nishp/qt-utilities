#ifndef _NCMAINWINDOW_H_
#define _NCMAINWINDOW_H_

#include <QMainWindow>

class NcPimplMainWindow;

class NcMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  NcMainWindow(QWidget* parent = 0);
  ~NcMainWindow();

private slots:
  //File
  void onNew();
  void onOpen();
  void onSave();
  void onSaveAll();
  void onClose();
  void onCloseAll();
  void onQuit();

  //Edit
  void onEditBirthData();

  //Tools
  void onOptions();

  //Help
  void onHelp();
  void onAbout();

  //Test
  void onTest1();
  void onGenAtlasDb();

private:
  void createMenuBar();
  void createToolBar();
  void createMdiArea();
  void createStatusBar();

  void createFileMenu();
  void createEditMenu();
  void createToolsMenu();
  void createHelpMenu();
  void createTestMenu();

private:
  NcPimplMainWindow* d;

};

#endif // _NCMAINWINDOW_H_
