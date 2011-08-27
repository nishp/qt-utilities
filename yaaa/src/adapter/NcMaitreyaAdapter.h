#ifndef _NCMAITREYAADAPTER_H_
#define _NCMAITREYAADAPTER_H_

class NcEvent;
class NcPimplMaitreyaAdapter;
class NcMaitreyaAdapter
{
public:
  NcMaitreyaAdapter();
  ~NcMaitreyaAdapter();

  void setEvent( NcEvent* event );
  int houseForPlanet( int planet );

private:
  NcPimplMaitreyaAdapter* d;
};

#endif // _NCMAITREYAADAPTER_H_
