/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/DataSet.cpp
 *  Release    6.0
 *  Author     M. Pettau
 *  Copyright  2003-2010 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
****************************************************************************/

#ifdef __GNUG__
	#pragma implementation "DataSet.h"
#endif

#include "DataSet.h"

#include <wx/log.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <math.h>

#include "constants.h"
#include "Conf.h"
#include "Calculator.h"
#include "Lang.h"
#include "mathbase.h"

extern Config *config;

/*****************************************************
**
**   JDate   ---   Constructor 
**
******************************************************/
JDate::JDate()
{
	setActualDate();
}

/*****************************************************
**
**   JDate   ---   Constructor 
**
******************************************************/
JDate::JDate( const double &jd )
{
	this->jd = jd;
}

/*****************************************************
**
**   JDate   ---   Constructor 
**
******************************************************/
JDate::JDate( const JDate &org )
{
	setDate( ((JDate*)&org)->getJD());
}

/*****************************************************
**
**   JDate   ---   setActualDate 
**
******************************************************/
void JDate::setActualDate()
{
	time_t tt;
	time( &tt );
	double mytime;

	mytime = 2440587.5 + (double)tt / 86400.0; // 1.1.1970 + seconds elapsed / number of seconds per day
	setDate( mytime );
}

/*****************************************************
**
**   JDate   ---   updateJD 
**
******************************************************/
void JDate::updateJD( const int &day, const int &month, const int &year, const double &time )
{
	jd = CalculatorFactory().getCalculator()->calc_jd( year, month, day, time );
}

/*****************************************************
**
**   JDate   ---   setDate 
**
******************************************************/
void JDate::setDate( const double &jd )
{
	this->jd = jd;
}

/*****************************************************
**
**   JDate   ---   setDate 
**
******************************************************/
void JDate::setDate( const int &day, const int &month, const int &year, const double &time )
{
	updateJD( day, month, year, time );
}

/**************************************************************
***
** Date   ---   operator =
***
***************************************************************/
void JDate::operator=( const JDate& _date )
{
	setDate( ((JDate*)&_date)->getJD() );
}

/*****************************************************
**
**   JDate   ---   dump 
**
******************************************************/
void JDate::dump(wxString &o)
{
	Formatter *formatter = Formatter::get();
	o << _( "Date" ) << wxT( " : " ) << formatter->getDateStringFromJD( jd ) << Endl
		<< _( "Universal Time" ) << wxT( " : " ) << formatter->getTimeFormatted( getTimeFromJD(jd)) << Endl;
}

/*****************************************************
**
**   Location   ---   Constructor 
**
******************************************************/
Location::Location()
{
	locName = wxT( "" );
	longitude = 0;
	latitude = 0;
	tz = 0;
	dst = 0;
}

/*****************************************************
**
**   Location   ---   Constructor 
**
******************************************************/
Location::Location( const Location &l )
{
  setLocation( l.locName, l.longitude, l.latitude, l.tz, l.dst );	
}

/**************************************************************
***
** Location   ---   operator=
***
***************************************************************/
void Location::operator=( const Location& l )
{
  setLocation( l.locName, l.longitude, l.latitude, l.tz, l.dst );	
}

/*****************************************************
**
**   Location   ---   setLocation 
**
******************************************************/
void Location::setLocation( const wxString &locname, const double &longitude, const double &latitude,
	const double &tz, const double &dst )
{
	this->locName = locname;
	this->longitude = longitude;
	this->latitude = latitude;
	this->tz = tz;
	this->dst = dst;
}

/*****************************************************
**
**   Location   ---   dump 
**
******************************************************/
void Location::dump(wxString &o)
{
	Formatter *formatter = Formatter::get();

	o << _( "Location" ) << wxT( ": " ) << locName << Endl
		<< _( "Longitude" ) << wxT( " : " ) << formatter->getLongitudeFormatted( longitude ) << Endl
		<< _( "Latitude" ) << wxT( " : " ) << formatter->getLatitudeFormatted( latitude ) << Endl;
}

/*****************************************************
**
**   DataSet   ---   Constructor 
**
******************************************************/
DataSet::DataSet()
{
	setName( _( "nn" ));
	date = new JDate();
	location = new Location();
}

/*****************************************************
**
**   DataSet   ---   Constructor 
**
******************************************************/
DataSet::DataSet( const Location &loc )
{
	setName( _( "nn" ));
	date = new JDate();
	location = new Location( loc );
}

/*****************************************************
**
**   DataSet   ---   Constructor 
**
******************************************************/
DataSet::DataSet( const DataSet &org )
{
	this->name = org.name;
	this->date = new JDate( org.getJD() );
	location = new Location( *(((DataSet*)&org)->getLocation()));
	remark = ((DataSet*)&org)->getRemark();
}

/*****************************************************
**
**   DataSet   ---   Destructor 
**
******************************************************/
DataSet::~DataSet()
{
	delete date;
	delete location;
}

/**************************************************************
***
** DataSet   ---   operator =
***
***************************************************************/
void DataSet::operator=( const DataSet &d )
{
	*location = *d.location;
	*date     = *d.date;
	name      = d.name;
	remark    = d.remark;
}

/*****************************************************
**
**   DataSet   ---   setLocation 
**
******************************************************/
void DataSet::setLocation( const wxString &locName, const double &longitude, const double &latitute,
	const double &timeZone, const double &dst )
{
	location->setLocation( locName, longitude, latitute, timeZone, dst );
}

/*****************************************************
**
**   DataSet   ---   setLocation 
**
******************************************************/
void DataSet::setLocation( const Location &loc )
{
	*location = loc;
}

/*****************************************************
**
**   DataSet   ---   setName 
**
******************************************************/
void DataSet::setName( const wxString &name )
{
	this->name = name;
}

/*****************************************************
**
**   DataSet   ---   setRemark 
**
******************************************************/
void DataSet::setRemark( const wxString &remark )
{
	this->remark = remark;
}

/*****************************************************
**
**   DataSet   ---   setDate 
**
******************************************************/
void DataSet::setDate( const double &jd )
{
	date->setDate( jd );
}

/*****************************************************
**
**   DataSet   ---   setDate 
**
******************************************************/
void DataSet::setDate( const int &day, const int &month, const int &year, const double &time )
{
	date->setDate( day, month, year, time );
}

/*****************************************************
**
**   DataSet   ---   setActualDate 
**
******************************************************/
void DataSet::setActualDate()
{
	date->setActualDate();
}

/*****************************************************
**
**   DataSet   ---   getWeekDay
**
******************************************************/
int DataSet::getWeekDay()
{
  return (int)( getJD() + ( location->getTimeZone() + location->getDST() )/24.0 + 1.5 ) % 7;
}

/*****************************************************
**
**   DataSet   ---   getLocalMeanTime
**
******************************************************/
double DataSet::getLocalMeanTime()
{
	double t = getTimeFromJD( getJD());
	t += getLocation()->getLongitude() / 15.0;
	return t;
}

/*****************************************************
**
**   DataSet   ---   dump 
**
******************************************************/
void DataSet::dump(wxString &o)
{
	double time;
	Formatter *f = Formatter::get();

	o << _( "Name" ) << wxT( " : " ) << name << Endl;
	date->dump(o);

	time = a_red( getTimeFromJD(date->getJD()) + location->getTimeZone() + location->getDST(), 24 );
	o << _( "Local Time" ) << wxT( " : " ) << f->getTimeFormatted( time ) << Endl
		<< _( "Time Zone" ) << wxT( " : " ) << location->getTimeZone() << Endl
		<< _( "DST" ) << wxT( " : " ) << location->getDST() << Endl;

	location->dump(o);
}

/*****************************************************
**
**   FileDataSet   ---   Constructor 
**
******************************************************/
FileDataSet::FileDataSet() : DataSet()
{
}

/*****************************************************
**
**   FileDataSet   ---   Constructor 
**
******************************************************/
FileDataSet::FileDataSet( const Location &loc )
	: DataSet( loc )
{
}

/*****************************************************
**
**   FileDataSet   ---   Destructor 
**
******************************************************/
FileDataSet::~FileDataSet()
{
}

/*****************************************************
**
**   FileDataSet   ---   openFile 
**
******************************************************/
bool FileDataSet::openFile( const wxString &fn, bool isAtxFile )
{
	wxString s;
	filename = fn;
	bool ret = true;
	if ( ! theFile.Open( filename ))
	{
		wxLogError( wxT( "Cannot open file %s" ), (const wxChar*)filename );
		return false;
	}
  wxString c = this->filename.Right( 3 );
	if ( c == wxT( "jhd" )) ret = readJhdFile();
	else if ( c == wxT( "atx" )) ret = readAtxFile();
	else ret = readMtxFile();
	theFile.Close();
	return ret;
}

/*****************************************************
**
**   FileDataSet   ---   readAtxFile 
**
******************************************************/
bool FileDataSet::readAtxFile()
{
	long day, month, year, hour, minute, second, ldeg, bdeg;
	double timeZone, dmin, lmin, bmin, dst;
	
	wxString buf;
	wxString s;

	setName( theFile.GetFirstLine());

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &day ))
	{
		wxLogError( wxT( "Invalid day value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( day < 0 ) || ( day > 31 ))
	{
		wxLogError( wxT( "Invalid day value %ld in file %s." ), day, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &month ))
	{
		wxLogError( wxT( "Invalid month value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( month < 0 ) || ( month > 12 ))
	{
		wxLogError( wxT( "Invalid month value %ld in file %s." ), month, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &year ))
	{
		wxLogError( wxT( "Invalid year value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( year < -2000 ) || ( year > 4000 ))
	{
		wxLogError( wxT( "Invalid year value %ld in file %s." ), year, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &hour ))
	{
		wxLogError( wxT( "Invalid hour value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( hour < 0 ) || ( hour > 24 ))
	{
		wxLogError( wxT( "Invalid hour value %ld in file %s." ), hour, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	dmin = myatof( buf );
	if (( dmin < 0 ) || ( dmin > 60 ))
	{
		wxLogError( wxT( "Invalid time value %f for minute in file %s." ), dmin, (const wxChar*)filename );
		return false;
	}

	dmin += .00000125;
	minute = (int)dmin;
	second = (int)(( dmin - minute )*60);

	double time = 3600 * hour + 60 * minute + second;
	time /= 3600;

	buf = theFile.GetNextLine();
	timeZone = myatof( buf );
	if (( timeZone < -12 ) || ( timeZone > 12 ))
	{
		wxLogError( wxT( "Invalid time zone value %f in file %s." ), timeZone, (const wxChar*)filename );
		return false;
	}
	time -= timeZone;
	dst = 0;

	setDate( day, month, year, time );

	buf = theFile.GetNextLine();
	s = buf;

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &ldeg ))
	{
		wxLogError( wxT( "Invalid longitude value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( ldeg < -180 ) || ( ldeg > 180 ))
	{
		wxLogError( wxT( "Invalid longitude value %f in file %s." ), timeZone, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	lmin = myatof( buf );
	if (( lmin < -60 ) || ( lmin > 60 ))
	{
		wxLogError( wxT( "Invalid longitude minute value %f in file %s." ), lmin, (const wxChar*)filename );
		return false;
	}

	double longitude = 60 * ::fabs( (double)ldeg ) + ::fabs( (double)lmin );
	if ( ldeg < 0 ) longitude *= -1;
	longitude /= 60;

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &bdeg ))
	{
		wxLogError( wxT( "Invalid latitude value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( bdeg < -180 ) || ( bdeg > 180 ))
	{
		wxLogError( wxT( "Invalid latitude value %ld in file %s." ), bdeg, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	bmin = myatof( buf );
	if (( bmin < -60 ) || ( bmin > 60 ))
	{
		wxLogError( wxT( "Invalid latitude minute value %f in file %s." ), bmin, (const wxChar*)filename );
		return false;
	}

	double latitude = 60 * ::fabs( (double)bdeg ) + ::fabs( (double)bmin );
	if ( bdeg < 0 ) latitude *= -1;
	latitude /= 60;

	setLocation( s, longitude, latitude, timeZone, dst );

	setRemark( theFile.GetNextLine());
	return true;
}

/*****************************************************
**
**   FileDataSet   ---   readMtxFile 
**
******************************************************/
bool FileDataSet::readMtxFile()
{
	long day, month, year;
	double time, timeZone, longitude, latitude, dst;
	
	wxString buf;
	wxString s;

	// Commented line
	buf = theFile.GetFirstLine();
	if ( ! buf.StartsWith( wxT( "#" )))
	{
		wxLogError( wxT( "Invalid input format in file %s." ), (const wxChar*)filename );
		return false;
	}
	setName( theFile.GetNextLine());

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &day ))
	{
		wxLogError( wxT( "Invalid day value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( day < 0 ) || ( day > 31 ))
	{
		wxLogError( wxT( "Invalid day value %ld in file %s." ), day, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &month ))
	{
		wxLogError( wxT( "Invalid month value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( month < 0 ) || ( month > 12 ))
	{
		wxLogError( wxT( "Invalid month value %ld in file %s." ), month, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	if ( ! buf.ToLong( &year ))
	{
		wxLogError( wxT( "Invalid year value \"%s\" in file %s." ), (const wxChar*)buf, (const wxChar*)filename );
		return false;
	}
	if (( year < -2000 ) || ( year > 4000 ))
	{
		wxLogError( wxT( "Invalid year value %ld in file %s." ), year, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	time = myatof( buf );
	if (( time < 0 ) || ( time > 24 ))
	{
		wxLogError( wxT( "Invalid time value %f in file %s." ), time, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	timeZone = myatof( buf );
	if (( timeZone < -12 ) || ( timeZone > 12 ))
	{
		wxLogError( wxT( "Invalid time zone value %f in file %s." ), timeZone, (const wxChar*)filename );
		return false;
	}
	time -= timeZone;

	buf = theFile.GetNextLine();
	dst = myatof( buf );
	if (( dst < -12 ) || ( dst > 12 ))
	{
		wxLogError( wxT( "Invalid DST value %f in file %s." ), dst, (const wxChar*)filename );
		return false;
	}
	time -= dst;

	s = theFile.GetNextLine();

	buf = theFile.GetNextLine();
	longitude = myatof( buf );
	if (( longitude < -180 ) || ( longitude > 180 ))
	{
		wxLogError( wxT( "Invalid longitude value %f in file %s." ), longitude, (const wxChar*)filename );
		return false;
	}

	buf = theFile.GetNextLine();
	latitude = myatof( buf );
	if (( latitude < -180 ) || ( latitude > 180 ))
	{
		wxLogError( wxT( "Invalid latitude value %f in file %s." ), latitude, (const wxChar*)filename );
		return false;
	}

	setDate( day, month, year, time );
	setLocation( s, longitude, latitude, timeZone, dst );

	remark.Clear();
	while( ! theFile.Eof())
	{
		buf = theFile.GetNextLine();
		if ( remark.Length() > 0 ) remark += Endl;
		remark << buf;
	}
	return true;
}

/*****************************************************
**
**   FileDataSet   ---   readJhdFile 
**
******************************************************/
bool FileDataSet::readJhdFile()
{
	long day, month, year, hour, minute, second, ldeg, bdeg, lmin, bmin;
	double timeZone, tz, ll, bb, daytime;
	wxString s;
	wxString buf;

	int slashpos = filename.rfind( '/', filename.size() );
	s = filename.substr( slashpos + 1 );

	if ( s.substr( s.size() - 4, 4 ) == wxT( ".jhd" )) s = s.substr( 0, s.size() - 4 );
	setName( (const wxChar*)s );

	buf = theFile.GetFirstLine();
	buf.ToLong( &month );

	buf = theFile.GetNextLine();
	buf.ToLong( &day );

	buf = theFile.GetNextLine();
	buf.ToLong( &year );
	buf = theFile.GetNextLine();
	daytime = myatof( buf );
	hour = (int)daytime;
	double min = ( daytime - (double)hour ) * 100;
	minute = (int)min;
	double sec = ( min - (double)minute) * 60;
	second = (int)sec;

	double time = 3600 * hour + 60 * minute + second;
	time /= 3600;
	buf = theFile.GetNextLine();
	tz = myatof( buf );
	timeZone = (int)tz + ( tz - (double)(int)tz ) * 5/3;
	timeZone *= -1;
	
	time -= timeZone;
	setDate( day, month, year, time );

	buf = theFile.GetNextLine();
	ll = myatof( buf );
	ldeg = (int)ll;
	lmin = (int)(( ll - (double)ldeg ) * 100);
	double longitude = 60 * abs( ldeg ) + abs( lmin );
	if ( ldeg > 0 ) longitude *= -1;
	longitude /= 60;

	buf = theFile.GetNextLine();
	bb = myatof( buf );
	bdeg = (int)bb;
	bmin = (int)(( bb - (double)bdeg ) * 100);
	double latitude = 60 * abs( bdeg ) + abs( bmin );
	if ( bdeg < 0 ) latitude *= -1;
	latitude /= 60;
	if ( ! theFile.Eof() ) buf = theFile.GetNextLine();
	buf = wxEmptyString;
	if ( ! theFile.Eof() ) buf = theFile.GetNextLine();
	if ( ! theFile.Eof() ) buf = theFile.GetNextLine();
	if ( ! theFile.Eof() ) buf = theFile.GetNextLine();
	if ( ! theFile.Eof() ) buf = theFile.GetNextLine();
	if ( ! theFile.Eof() ) buf = theFile.GetNextLine();
	if ( buf.IsEmpty()) buf = wxT( "NN" );
	setLocation( buf, longitude, latitude, timeZone, 0 );
	return true;
}

/*****************************************************
**
**   FileDataSet   ---   saveAs 
**
******************************************************/
bool FileDataSet::saveAs( const wxString &fn )
{
	if ( ! fn )
	{
		wxLogError( wxT( "Error: filename is NULL (FileDataSet::saveAs)" ));
		return false;
	}
	filename =  fn;
	return save();
}

/*****************************************************
**
**   FileDataSet   ---   save 
**
******************************************************/
bool FileDataSet::save()
{
	double jjd;
	wxString c;

	if ( filename.Length() < 1  )
	{
		wxLogError( wxT( "Filename is not set!" ));
		return false;
	}

  c = filename.Right( 3 );
	if ( c.CmpNoCase( wxT( "jhd" )) == 0 )
	{
		wxLogError( wxT( "Can't write jhd format!" ));
		return false;
	}
	else if ( c.CmpNoCase( wxT( "atx" )) == 0 )
	{
		wxLogError( wxT( "Can't write atx format!" ));
		return false;
	}
	else if ( c.CmpNoCase( wxT( "mtx" )) != 0 ) filename += wxT( ".mtx" );

	wxTextFile wfile( filename );
	if ( wfile.Exists() )
	{
	 wxRemove( filename );
	}
	if ( ! wfile.Create( filename ) ) 
	{
		wxLogError( wxT( "Cannot create file!" )); 
		return false;
	}

	wfile.AddLine( wxT( "#Maitreya 3.0" ));
	wfile.AddLine( getName() );

	jjd = getJD() + ( getLocation()->getTimeZone() + getLocation()->getDST() ) / 24;

	int day, month, year;
	Formatter::get()->getDateIntsFromJD( jjd, day, month, year );

	c.Printf( wxT( "%d" ), day );
	wfile.AddLine( c );
	c.Printf( wxT( "%d" ), month );
	wfile.AddLine( c );
	c.Printf( wxT( "%d" ), year );
	wfile.AddLine( c );

	double jjd2 = jjd - .5;
	double dd = ( jjd2 - (double)(int)jjd2) * 24;
	c.Printf( wxT( "%2.8f" ), dd );
	wfile.AddLine( c );
	c.Printf( wxT( "%2.8f" ), getLocation()->getTimeZone() );
	wfile.AddLine( c );
	c.Printf( wxT( "%2.8f" ), getLocation()->getDST() );
	wfile.AddLine( c );
	wfile.AddLine( getLocation()->getLocName() );
	
	double lon = getLocation()->getLongitude() + .000005;
	double lat = getLocation()->getLatitude() + .000005;

	c.Printf( wxT( "%2.8f" ), lon );
	wfile.AddLine( c );
	c.Printf( wxT( "%2.8f" ), lat );
	wfile.AddLine( c );
	wxStringTokenizer t( getRemark(), Endl, wxTOKEN_RET_EMPTY );
	while( t.HasMoreTokens() )
	{
		c = t.GetNextToken();
		wfile.AddLine( c );
	}

	wfile.Write();
	wfile.Close();
	return true;
}

/*****************************************************
**
**   LocationEntry   ---   Constructor 
**
******************************************************/
LocationEntry::LocationEntry()
{
	name = wxT( "" );
	country = wxT( "" );
	longitude = 0;
	latitude = 0;
	timeZone = 0;
}

/*****************************************************
**
**   LocationEntry   ---   Constructor 
**
******************************************************/
LocationEntry::LocationEntry( wxString n, wxString c, wxString s, const double &lon, const double &lat, const double &tz )
{
	static int i = 0;
	i++;
	name = n;
	country = c;
	state = s;
	longitude = lon;
	latitude = lat;
	timeZone = tz;
}

/*****************************************************
**
**   LocationEntry   ---   Constructor 
**
******************************************************/
LocationEntry::LocationEntry( const wxString &s )
{
	wxString buf;
	wxStringTokenizer t( s, wxT( ";" ));
	
	name = t.GetNextToken();
	country = t.GetNextToken();
	state = t.GetNextToken();

	buf = t.GetNextToken();
	longitude = myatof( buf );

	buf = t.GetNextToken();
	latitude = myatof( buf );

	buf = t.GetNextToken();
	timeZone = myatof( buf );
}  

/**************************************************************
***
** LocationEntry   ---   Destructor
***
***************************************************************/
LocationEntry::~LocationEntry()
{
	//printf( "Destroy location entry\n" );
}

/**************************************************************
***
** LocationEntry   ---   operator =
***
***************************************************************/
void LocationEntry::operator=( const LocationEntry& e )
{
	name = e.name;
	country = e.country;
	state = e.state;
	longitude = e.longitude;
	latitude = e.latitude;
	timeZone = e.timeZone;
}
  
/**************************************************************
***
** LocationEntry   ---   operator ==
***
***************************************************************/
bool LocationEntry::operator==( const LocationEntry& e )
{
	if ( name.Cmp( e.name )) return false;
	if ( country.Cmp( e.country )) return false;
	if ( state.Cmp( e.state )) return false;
	if ( longitude != e.longitude ) return false;
	if ( latitude != e.latitude ) return false;
	if ( timeZone != e.timeZone ) return false;
	return true;
}

  


