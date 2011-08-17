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
  void onExit();

  //Test
  void onTest1();
  void onGenAtlasDb();

  //Help
  void onHelp();
  void onAbout();

private:
  void init();
  void createMenus();
  void createActions();

private:
  NcPimplMainWindow* d;

};

#endif // _NCMAINWINDOW_H_
