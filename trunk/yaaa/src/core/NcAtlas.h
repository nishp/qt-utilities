#ifndef _NCATLAS_H_
#define _NCATLAS_H_

class NcPimplAtlas;
class NcAtlas
{
public:
  NcAtlas( QString atlasFilePath );
  ~NcAtlas();
  bool cities1000ToDb( QString inputDirPath );

private:
  NcPimplAtlas *d;
};

#endif // _NCATLAS_H_
