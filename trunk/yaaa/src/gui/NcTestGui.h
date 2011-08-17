#ifndef _NCTESTGUI_H_
#define _NCTESTGUI_H_

#include <QDialog>

class NcGfx2D;

class TestDialog : public QDialog
{
public:
  TestDialog(QWidget* parent = 0);
  ~TestDialog();

protected:
  virtual void paintEvent(QPaintEvent *e);

private:
  NcGfx2D* gfx;
};

#endif // _NCTESTGUI_H_
