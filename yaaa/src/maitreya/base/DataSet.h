/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/DataSet.h
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
#ifndef _DATA_SET_
#define _DATA_SET_

#ifdef __GNUG__
	#pragma interface "DataSet.h"
#endif

#include <wx/string.h>
#include <wx/textfile.h>

#include "constants.h"

/*************************************************//**
*
* \brief simple data class
*
******************************************************/
class JDate
{
public:

	JDate();
	JDate( const double& );
	JDate( const JDate & );

	void setActualDate();
	double getJD() const { return jd; }
	void setDate( const double& );
	void setDate( const int &day, const int &month, const int &year, const double &time );
	void operator=( const JDate& date );
	void dump( wxString& );

private:
	double jd;
	void updateJD( const int &day, const int &month, const int &year, const double &time );
};

/*************************************************//**
*
* \brief contains data of the location of a chart
*
******************************************************/
class Location
{
public:

  Location();
  Location( const Location & );

  void operator=( const Location& );
  void setLocation( const wxString &locName, const double &longitude, const double &latitude,
		const double &timeZone, const double &dst );

  double getLongitude() const { return longitude; }
  double getLatitude() const { return latitude; }
  wxString getLocName() const { return locName; }
  double getTimeZone() const { return tz; }
  double getDST() const { return dst; }

  void dump(wxString&);

private:
  double latitude, longitude;
  double tz;
  double dst;
  wxString locName;
};

/*************************************************//**
*
* \brief data for a chart (date, location, name etc.)
*
******************************************************/
class DataSet
{
public:

	DataSet();
	DataSet( const Location& );
	DataSet( const DataSet & );
	~DataSet();

  void operator=( const DataSet& );
	
	void setDate( const double& );
	JDate *getDate() const { return date; }
	void setDate( const int &day, const int &month, const int &year, const double &time );
	void setActualDate();
	
	double getJD() const { return date->getJD(); }
	int getWeekDay();
	double getLocalMeanTime();
	
	void setLocation( const wxString &locName, const double &longitude, const double &latitute,
		const double &timeZone, const double &dst );
	void setLocation( const Location& );
	Location *getLocation() const { return location; }
	
	wxString getName() const { return name; }
	void setName( const wxString& );
	
	wxString getRemark() const { return remark; }
	void setRemark( const wxString& );
	
	void dump(wxString&);

protected:
	wxString remark;

private:
	wxString name;
	JDate *date;
	Location *location;
};

/*************************************************//**
*
* \brief DataSet with file IO methods
*
******************************************************/
class FileDataSet : public DataSet
{
public:

	FileDataSet();
	FileDataSet( const Location& );
	~FileDataSet();

	bool openFile( const wxString&, bool isAtxFile = true );
	bool saveAs( const wxString& );
	bool save();
	const wxString getFilename() const { return filename; }

private:
	bool readJhdFile();
	bool readAtxFile();
	bool readMtxFile();
	wxString filename;
	wxTextFile theFile;
};

/*************************************************//**
*
* \brief location in LocationDialog
*
******************************************************/
class LocationEntry
{
public:
  LocationEntry();
  ~LocationEntry();
  LocationEntry( wxString name, wxString country, wxString state,
		const double &lon, const double &lat, const double &tz );
	LocationEntry( const wxString& );
	void operator=( const LocationEntry& );
	bool operator==( const LocationEntry& );

	wxString name, state, country;
	double longitude, latitude, timeZone;
	int row;
};

#endif


