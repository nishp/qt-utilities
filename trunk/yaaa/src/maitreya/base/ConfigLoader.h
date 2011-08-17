/***************************************************************************
 *  This is the source code of Maitreya, open source platform
 *  for Vedic and western astrology.

 *  File       src/base/ConfigLoader.h
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

#ifndef _CONFIGLOADER_H_
#define _CONFIGLOADER_H_

#ifdef __GNUG__
	#pragma interface "ConfigLoader.h"
#endif

#include <wx/string.h>

#define DECLARE_CONFIGLOADER( classname, configname ) \
public: \
	~classname(); \
	static classname *get(); \
	void cleanup(); \
	configname *getConfig( const int &i ); \
	vector<configname*> getConfigs(); \
	int getCount(); \
\
protected: \
	classname(); \
	classname( const classname& ) {} \
	void clear(); \
	void init(); \
	vector<configname*> defs; \
	class XmlWorker*worker; \
	static classname *ego;

#define IMPLEMENT_CONFIGLOADER( classname, configname, workername ) \
classname *classname::ego = 0; \
\
classname::classname() { worker = 0; } \
\
classname::~classname() { if ( worker ) delete worker; } \
\
classname *classname::get() \
{ \
	if ( ego == 0 ) ego = new classname; \
	return ego; \
} \
\
void classname::cleanup() { clear(); } \
\
int classname::getCount() { return (int)defs.size(); } \
\
configname *classname::getConfig( const int &i ) { \
	if ( defs.size() == 0 || worker->needsReload() ) init(); \
	assert( i >= 0 && i < (int)defs.size()); \
	return defs[i]; \
} \
\
vector<configname*> classname::getConfigs() { \
	if ( defs.size() == 0 || worker->needsReload() ) init(); \
	return defs; \
} \
\
void classname::clear() { \
	for( unsigned int i = 0; i < defs.size(); i++ ) delete defs[i]; \
	defs.clear(); \
} \
\
void classname::init() { \
	clear(); \
	if ( ! worker ) worker = new workername; \
	((workername*)worker)->readConfig( defs ); \
}


#endif

