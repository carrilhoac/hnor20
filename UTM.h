/* -*- mode: C++ -*-
 *
 *  Conversions between Latitude/Longitude and UTM
 *              (Universal Transverse Mercator) coordinates.
 *
 *  License: Modified BSD Software License Agreement
 *
 *  $Id$
 */

#ifndef _UTM_H
#define _UTM_H

namespace UTM
{
	
    char UTMLetterDesignator(double Lat);

    void LLtoUTM(const double Lat, const double Long,
                               double &UTMNorthing, double &UTMEasting,
                               char* UTMZone);

    void UTMtoLL(const double UTMNorthing, const double UTMEasting,
                               const char* UTMZone, double& Lat,  double& Long );
							   
}

#endif 
