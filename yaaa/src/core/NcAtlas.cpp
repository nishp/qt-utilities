#include <QtCore>
#include <QtSql>
#include "NcAtlas.h"


#define ST_NULL     "NULL"
#define ST_INTEGER  "INTEGER"
#define ST_REAL     "REAL"
#define ST_TEXT     "TEXT"
#define ST_BLOB     "BLOB"
#define ST_INTEGER_PRIMARY_KEY  "INTEGER PRIMARY KEY"

#define NC_TBL_COUNTRY_CODES "tbl_countryCodes"
#define NC_COL_CODE      "col_code"
#define NC_COL_NAME      "col_name"

#define NC_TBL_ADMIN1_CODES "tbl_admin1Codes"
#define NC_COL_COUNTRY_CODE  "col_countryCode"

#define NC_TBL_COUNTRY_XX "tbl_country_"
#define NC_COL_CITY "col_city"
#define NC_COL_ADMIN1_CODE "col_admin1code"
#define NC_COL_LONGITUDE "col_longitude"
#define NC_COL_LATITUDE "col_latitude"
#define NC_COL_TZ "col_tz"

class NcPimplAtlas
{
public:
  bool createTableCountryCodes( QString inputFilePath );
  bool createTableAdmin1Codes( QString inputFilePath );

private:
  QStringList readFileCountryInfo( const QString &fileName );
  QStringList readFileAdmin1CodesAscii( const QString &fileName );

  bool executeQuery( const QString &sql );
};

bool NcPimplAtlas::createTableCountryCodes( QString inputFilePath )
{
  QStringList lines = readFileCountryInfo(inputFilePath);

  if ( lines.isEmpty() )
  {
    return false;
  }


  QString sql = "CREATE TABLE "NC_TBL_COUNTRY_CODES" ("
                NC_COL_CODE" "ST_TEXT", "
                NC_COL_NAME" "ST_TEXT")";


  if ( !executeQuery(sql) )
  {
    return false;
  }

  unsigned i,size = lines.size();
  size = 10;

  qDebug() << "size = " << size;
  for ( i = 0; i < size; ++i )
  {
    QSqlQuery q;
    q.prepare("INSERT INTO "NC_TBL_COUNTRY_CODES" "
              "("NC_COL_CODE","NC_COL_NAME") "
              "VALUES (:"NC_COL_CODE",:"NC_COL_NAME")");

    //qDebug() << line.count('\t') << line.section("\t",0,1);
    //qDebug() << line.count('\t') << line.section("\t",1,1);
    //<< line.section('\t',4.4);
    //continue;
    q.bindValue( ":"NC_COL_CODE, lines[i].section('\t',1,1) );
    q.bindValue( ":"NC_COL_NAME, lines[i].section('\t',0,0) );

    if ( !q.exec() )
    {
      qDebug() << "Error:- " << q.lastError().text();
      return false;
    }

    qDebug()<< NC_TBL_COUNTRY_CODES << i+1 << "of" << size;

  }

  return true;
}

QStringList NcPimplAtlas::readFileCountryInfo( const QString &fileName )
{
  QStringList lines;

  QFile inputFile(fileName);

  if ( !inputFile.open(QFile::ReadOnly|QFile::Text) )
  {
    qDebug() << "Cannot open input file-> " << inputFile.fileName();
    return lines;
  }


  QTextStream stream(&inputFile);
  QString singleLine;

  while ( !stream.atEnd() )
  {
    singleLine = stream.readLine();

    if ( !singleLine.startsWith('#') )
    {
      lines += singleLine.section('\t',4,4) +
               '\t' + singleLine.section('\t',0,0);
    }

  }

  lines.sort();

  inputFile.close();

  return lines;
}

bool NcPimplAtlas::createTableAdmin1Codes(QString inputFilePath)
{
  QStringList lines = readFileAdmin1CodesAscii(inputFilePath);

  if ( lines.isEmpty() )
  {
    return false;
  }

  QString sql = "CREATE TABLE "NC_TBL_ADMIN1_CODES" ("
        NC_COL_COUNTRY_CODE" "ST_TEXT", "
        NC_COL_CODE" "ST_TEXT", "
        NC_COL_NAME" "ST_TEXT")";

  if ( !executeQuery(sql) )
  {
    return false;
  }

  unsigned i,size = lines.size();
  size = 10;
  qDebug() << "size = " << size;

  for ( i = 0; i < size; ++i )
  {

    QSqlQuery q;
    q.prepare("INSERT INTO "NC_TBL_ADMIN1_CODES" "
              "("NC_COL_COUNTRY_CODE","NC_COL_CODE","NC_COL_NAME") "
              "VALUES (:"NC_COL_COUNTRY_CODE",:"NC_COL_CODE",:"NC_COL_NAME")");

    //qDebug() << line.count('\t') << line.section("\t",0,1);
    //qDebug() << line.count('\t') << line.section("\t",1,1);
    //<< line.section('\t',4.4);
    //continue;
    q.bindValue( ":"NC_COL_COUNTRY_CODE, lines[i].section('\t',0,0) );
    q.bindValue( ":"NC_COL_CODE, lines[i].section('\t',1,1) );
    q.bindValue( ":"NC_COL_NAME, lines[i].section('\t',2,2) );

    if ( !q.exec() )
    {
      qDebug() << "Error:- " << q.lastError().text();
      return false;
    }

    qDebug()<< NC_TBL_ADMIN1_CODES << i+1 << "of" << size;

  }

  return true;
}

QStringList NcPimplAtlas::readFileAdmin1CodesAscii(const QString &fileName)
{
  QStringList lines;

  QFile inputFile(fileName);

  if ( !inputFile.open(QFile::ReadOnly|QFile::Text) )
  {
    qDebug() << "Cannot open input file-> " << inputFile.fileName();
    return lines;
  }


  QTextStream stream(&inputFile);
  QString singleLine;

  while ( !stream.atEnd() )
  {
    singleLine = stream.readLine();

    lines += singleLine.section('\t',0,0) +
             '\t' + singleLine.section('\t',1,1);
    //qDebug() << singleLine.count('\t') << singleLine.section('\t',1,1);;

    lines[lines.size()-1][2] = '\t';
  }


  inputFile.close();

  return lines;
}

bool NcPimplAtlas::executeQuery( const QString &sql )
{
  QSqlQuery q;

  if ( !q.exec(sql) )
  {
    qDebug() << "Error:- " << q.lastError().text();
    return false;
  }

  return true;
}

NcAtlas::NcAtlas( QString atlasFilePath )
  :d( new NcPimplAtlas )
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

  db.setDatabaseName(atlasFilePath);

  if ( !db.open() )
  {
    qDebug() << "Cannot open database";
  }

}

NcAtlas::~NcAtlas()
{
  QString name;

  {
    name = QSqlDatabase::database().connectionName();
  }

  QSqlDatabase::removeDatabase(name);
  delete d;
}

QString createCountryTableQuery( QString countryCode )
{
  QString sql = "CREATE TABLE "NC_TBL_COUNTRY_XX+countryCode;

  sql += " ("
         NC_COL_CITY" "ST_TEXT", "
         NC_COL_ADMIN1_CODE" "ST_TEXT", "
         NC_COL_LONGITUDE" "ST_REAL", "
         NC_COL_LATITUDE" "ST_REAL", "
         NC_COL_TZ" "ST_REAL")";

  return sql;

}



QStringList readLinesFromFile( const QString &fileName, QString commentStartString = QString() )
{
  QStringList lines;

  QFile inputFile(fileName);

  if ( !inputFile.open(QFile::ReadOnly|QFile::Text) )
  {
    qDebug() << "Cannot open input file-> " << inputFile.fileName();
    return lines;
  }

  bool ignoreComments = commentStartString.length() > 0;

  QTextStream stream(&inputFile);
  QString line;

  while ( !stream.atEnd() )
  {
    line = stream.readLine();
    if ( ignoreComments )
    {
      if ( !line.startsWith(commentStartString) )
      {
        lines += line;
      }
    }
    else
    {
      lines += line;
    }
  }

  inputFile.close();

  return lines;

}

bool NcAtlas::cities1000ToDb( QString inputDirPath )
{
  QDir iDir(inputDirPath);

  if ( false == d->createTableCountryCodes(iDir.absoluteFilePath("countryInfo.txt")) )
  {
    return false;
  }

  if ( false == d->createTableAdmin1Codes(iDir.absoluteFilePath("admin1CodesASCII.txt")) )
  {
    return false;
  }


  //for ( i = 0; i < size; ++i )
  {

   // QSqlQuery q;

    //if ( !q.exec( createCountryTableQuery( lines[i].section('\t',1,1) ) ) )
    {
      //qDebug() << "Error:- " << q.lastError().text();
      //return false;
    }

  }
  return true;
}



