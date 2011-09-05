#ifndef _NCMDISUBWINDOW_H_
#define _NCMDISUBWINDOW_H_

#include <QMdiSubWindow>
class NcPerson;

class NcMdiSubWindow : public QMdiSubWindow
{
  Q_OBJECT
public:
  explicit NcMdiSubWindow( NcPerson *native );
  virtual ~NcMdiSubWindow();

signals:

public slots:

 private:
  NcPerson *native;

};

#endif // _NCMDISUBWINDOW_H_
