#ifndef _NCPERSONFILE_H_
#define _NCPERSONFILE_H_

#include <QString>

class NcPerson;
class NcPimplPersonFile;
class NcPersonFile
{

public:
  enum
  {
    ERROR_OK = 0,
    ERROR_UNKNOWN,
    ERROR_FILE_ACCESS_DENIED,
    ERROR_FILE_EXISTS
  };

  NcPersonFile();
  ~NcPersonFile();

  QString lastErrorString();
  int lastError();

  bool savePerson( const NcPerson* person, QString filePath );
  bool loadPerson( QString filePath, NcPerson* person );

private:
  NcPimplPersonFile *d;

};

#endif // _NCPERSONFILE_H_
