#ifndef _NCFORMPERSONDATAENTRY_H_
#define _NCFORMPERSONDATAENTRY_H_

#include <QDialog>

class NcPerson;
class NcPimplFormPersonDataEntry;
class NcFormPersonDataEntry : public QDialog
{
  Q_OBJECT
public:
  explicit NcFormPersonDataEntry( QWidget *parent = 0 );
  ~NcFormPersonDataEntry();

  void setData( NcPerson *person );
  void getData( NcPerson *person );

private:
  NcPimplFormPersonDataEntry* d;

};

#endif // _NCFORMPERSONDATAENTRY_H_
