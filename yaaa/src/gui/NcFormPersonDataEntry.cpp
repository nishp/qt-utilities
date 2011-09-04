#include <QtGui>
#include "NcFormPersonDataEntry.h"
#include "NcPerson.h"
#include "NcEvent.h"
#include "NcLocation.h"

class NcPimplFormPersonDataEntry
{
public:
  NcPimplFormPersonDataEntry();

  QVBoxLayout *mainLayout;

  QLabel *lblFirstName;
  QLabel *lblMidName;
  QLabel *lblLastName;

  QLabel *lblDate;
  QLabel *lblTime;

  QLabel *lblCountry;
  QLabel *lblState;
  QLabel *lblCity;

  QLabel *lblLon;
  QLabel *lblLat;
  QLabel *lblTz;
  QLabel *lblDst;

  QLineEdit *leFirstName;
  QLineEdit *leMidName;
  QLineEdit *leLastName;

  QDateEdit *deDate;
  QTimeEdit *teTime;

  QComboBox *cmbCountry;
  QComboBox *cmbState;
  QComboBox *cmbCity;

  QLineEdit *leLon;
  QLineEdit *leLat;
  QLineEdit *leTz;
  QLineEdit *leDst;

  QPushButton *btnOk;
  QPushButton *btnCancel;

private:
  void doLayout();
  void fillDummyData();

};

NcPimplFormPersonDataEntry::NcPimplFormPersonDataEntry()
{
  lblFirstName = new QLabel( QObject::tr("First name") );
  lblMidName = new QLabel( QObject::tr("Middle name") );
  lblLastName = new QLabel( QObject::tr("Last name") );

  lblDate = new QLabel( QObject::tr("Date") );
  lblTime = new QLabel( QObject::tr("Time") );

  lblCountry = new QLabel( QObject::tr("Country") );
  lblState = new QLabel( QObject::tr("State") );
  lblCity = new QLabel( QObject::tr("City") );

  lblLon = new QLabel( QObject::tr("Longitude") );
  lblLat = new QLabel( QObject::tr("Latitude") );
  lblTz = new QLabel( QObject::tr("TimeZone") );
  lblDst = new QLabel( QObject::tr("DST") );

  leFirstName = new QLineEdit;
  leMidName = new QLineEdit;
  leLastName = new QLineEdit;

  deDate = new QDateEdit;
  teTime = new QTimeEdit;

  cmbCountry = new QComboBox;
  cmbState = new QComboBox;
  cmbCity = new QComboBox;

  leLon = new QLineEdit;
  leLat = new QLineEdit;
  leTz = new QLineEdit;
  leDst = new QLineEdit;

  btnOk = new QPushButton(QObject::tr("OK"));
  btnCancel = new QPushButton(QObject::tr("Cancel"));

  cmbCity->setEditable( true );

  doLayout();
  fillDummyData();

}

void NcPimplFormPersonDataEntry::doLayout()
{
  int row, col;
  QGridLayout* gl = new QGridLayout;

  row = 0, col = 0;
  gl->addWidget( lblFirstName, row++, col );
  gl->addWidget( lblMidName,   row++, col );
  gl->addWidget( lblLastName,  row++, col );
  gl->addWidget( lblDate,      row++, col );
  gl->addWidget( lblTime,      row++, col );
  gl->addWidget( lblCountry,   row++, col );
  gl->addWidget( lblState,     row++, col );
  gl->addWidget( lblCity,      row++, col );
  gl->addWidget( lblLon,       row++, col );
  gl->addWidget( lblLat,       row++, col );
  gl->addWidget( lblTz,        row++, col );
  gl->addWidget( lblDst,       row++, col );


  row = 0; col = 1;
  gl->addWidget( leFirstName, row++, col );
  gl->addWidget( leMidName,   row++, col );
  gl->addWidget( leLastName,  row++, col );
  gl->addWidget( deDate,      row++, col );
  gl->addWidget( teTime,      row++, col );
  gl->addWidget( cmbCountry,  row++, col );
  gl->addWidget( cmbState,    row++, col );
  gl->addWidget( cmbCity,     row++, col );
  gl->addWidget( leLon,       row++, col );
  gl->addWidget( leLat,       row++, col );
  gl->addWidget( leTz,        row++, col );
  gl->addWidget( leDst,       row++, col );



  QHBoxLayout* bottomHBox = new QHBoxLayout;
  bottomHBox->addStretch();
  bottomHBox->addWidget( btnCancel );
  bottomHBox->addWidget( btnOk );


  mainLayout = new QVBoxLayout;
  mainLayout->addLayout( gl );
  mainLayout->addStretch();
  mainLayout->addLayout( bottomHBox );

}

void NcPimplFormPersonDataEntry::fillDummyData()
{
  leFirstName->setText( "MyFirstName" );
  leMidName->setText( "MyMidName" );
  leLastName->setText( "MyLastName" );
  deDate->setDate( QDate( 2011, 11, 11 ) );
  teTime->setTime( QTime( 11, 11) );
  cmbCountry->addItem( "India" );
  cmbState->addItem( "Rajasthan" );
  cmbCity->addItem( "Jaipur" );
  leLon->setText("75:49:24 E");
  leLat->setText("26:55:33 N");
  leTz->setText("5.5");
  leDst->setText("0");
}

NcFormPersonDataEntry::NcFormPersonDataEntry( QWidget *parent ) :
  QDialog(parent), d ( new NcPimplFormPersonDataEntry )
{
  this->setLayout( d->mainLayout );

  connect( d->btnOk, SIGNAL(clicked()), this, SLOT(accept()) );
  connect( d->btnCancel, SIGNAL(clicked()), this, SLOT(reject()) );
}

NcFormPersonDataEntry::~NcFormPersonDataEntry()
{
  delete d;
}

void NcFormPersonDataEntry::setData(NcPerson *person)
{
  d->leFirstName->setText( person->firstName() );
  d->leMidName->setText( person->midName() );
  d->leLastName->setText( person->lastName() );

  //NcEvent* event = person->birthEvent();
  //NcLocation* loc = event->location();

//  d->deDate =      person->;
//  d->teTime =      person->;
//  d->cmbCountry =  person->;
//  d->cmbState =    person->;
//  d->cmbCity =     person->;

}

void NcFormPersonDataEntry::getData(NcPerson *person)
{
  person->setFirstName( d->leFirstName->text().trimmed() );
  person->setMidName( d->leMidName->text().trimmed() );
  person->setLastName( d->leLastName->text().trimmed() );

  NcEvent* event = person->birthEvent();
  NcLocation* loc = event->location();

  //QDate date(2011,11,11);
  //QTime time(11,11,11);

  QDate date(1983,10,25);
  QTime time(21,00,0);

  QDateTime dt(date,time);
  //dt.addSecs(5.5*3600);

  event->setDateTime( dt );

  double logitude = 77*3600 + 51*60 + 0;
  logitude /= 3600;

  double latitude = 28*3600 + 14*60 + 0;
  latitude /= 3600;

  loc->setLongitude( logitude );
  loc->setLatitude( latitude );
  loc->setTz( 5.5 );
  loc->setDst( 0 );

  loc->setCountry( d->cmbCountry->currentText() );
  loc->setState( d->cmbState->currentText() );
  loc->setDistrict( d->cmbCity->currentText() );//TODO: FIXME
  loc->setCity( d->cmbCity->currentText() );

}
