/**********************************************************
 * Version $Id$
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                     Tool Library                      //
//                    Shapes_Polygon                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                   Shape_Index.cpp                     //
//                                                       //
//                 Copyright (C) 2008 by                 //
//                      Olaf Conrad                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation, either version 2 of the     //
// License, or (at your option) any later version.       //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not, see   //
// <http://www.gnu.org/licenses/>.                       //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "shape_index.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CShape_Index::CShape_Index(void)
{
	//-----------------------------------------------------
	Set_Name		(_TL("Polygon Shape Indices"));

	Set_Author		("O.Conrad (c) 2008");

	Set_Description	(_TW(
		"Various indices describing the shape of polygons, mostly based on "
		"area, perimeter, maximum distance between the vertices of a polygon. "
		"E.g. the 'interior edge ratio' (= perimeter / area) or the "
		"'shape index' (= perimeter / (2 * square root(pi * area))). "
	));

	Add_Reference("Lang, S. & Blaschke, T.",
		"2007", "Landschaftsanalyse mit GIS",
		"Stuttgart."
	);

	Add_Reference("Forman, R.T.T. & Godron, M.",
		"1986", "Landscape Ecology",
		"Cambridge."
	);

	//-----------------------------------------------------
	Parameters.Add_Shapes(
		"", "SHAPES", _TL("Shapes"),
		_TL(""),
		PARAMETER_INPUT, SHAPE_TYPE_Polygon
	);

	Parameters.Add_Shapes(
		"", "INDEX"	, _TL("Shape Index"),
		_TL(""),
		PARAMETER_OUTPUT_OPTIONAL, SHAPE_TYPE_Polygon
	);
}


///////////////////////////////////////////////////////////
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CShape_Index::On_Execute(void)
{
	//-----------------------------------------------------
	CSG_Shapes	*pShapes = Parameters("SHAPES")->asShapes();
	CSG_Shapes	*pIndex  = Parameters("INDEX" )->asShapes();

	//-----------------------------------------------------
	if( pShapes->is_Valid() )
	{
		int		iField	= pShapes->Get_Field_Count();

		if( pIndex == NULL )
		{
			pIndex	= pShapes;
		}

		if( pIndex != pShapes )
		{
			pIndex->Create(SHAPE_TYPE_Polygon, _TL("Shape Index"), pShapes);
		}

		pIndex->Add_Field(_TL("Area"        ), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("Perimeter"   ), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("P/A"         ), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("P/sqrt(A)"   ), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("Max.Distance"), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("D/A"         ), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("D/sqrt(A)"   ), SG_DATATYPE_Double);
		pIndex->Add_Field(_TL("Shape Index" ), SG_DATATYPE_Double);

		for(int iShape=0; iShape<pShapes->Get_Count() && Set_Progress(iShape, pShapes->Get_Count()); iShape++)
		{
			CSG_Shape_Polygon	*pShape	= (CSG_Shape_Polygon *)pShapes->Get_Shape(iShape);

			double	Area		= pShape->Get_Area();
			double	Perimeter	= pShape->Get_Perimeter();
			double	Distance	= Get_Distance(pShape);

			if( Perimeter > 0.0 && Distance > 0.0 )
			{
				if( pIndex != pShapes )
				{
					pShape		= (CSG_Shape_Polygon *)pIndex->Add_Shape(pShape, SHAPE_COPY);
				}

				pShape->Set_Value(iField + 0, Area);
				pShape->Set_Value(iField + 1, Perimeter);

				if (Area > 0.0)
				{
					pShape->Set_Value(iField + 2, Perimeter / Area);
					pShape->Set_Value(iField + 3, Perimeter / sqrt(Area));
				}
				else
				{
					pShape->Set_NoData(iField + 2);
					pShape->Set_NoData(iField + 3);
				}

				pShape->Set_Value(iField + 4, Distance);

				if (Area > 0.0)
				{
					pShape->Set_Value(iField + 5, Distance / Area);
					pShape->Set_Value(iField + 6, Distance / sqrt(Area));
					pShape->Set_Value(iField + 7, Perimeter / (2.0 * sqrt(M_PI * Area)));
				}
				else
				{
					pShape->Set_NoData(iField + 5);
					pShape->Set_NoData(iField + 6);
					pShape->Set_NoData(iField + 7);
				}
			}
		}

		if( pIndex == pShapes )
		{
			DataObject_Update(pShapes);
		}

		return( pIndex->is_Valid() );
	}

	//-----------------------------------------------------
	return( false );
}

//---------------------------------------------------------
double CShape_Index::Get_Distance(CSG_Shape *pShape)
{
	double	dMax	= 0.0;

	for(int iPart=0; iPart<pShape->Get_Part_Count(); iPart++)
	{
		if( pShape->Get_Point_Count(iPart) > 2 )
		{
			for(int iPoint=0; iPoint<pShape->Get_Point_Count(iPart); iPoint++)
			{
				TSG_Point	A	= pShape->Get_Point(iPoint, iPart);

				for(int jPoint=iPoint+1; jPoint<pShape->Get_Point_Count(iPart); jPoint++)
				{
					double	d	= SG_Get_Distance(A, pShape->Get_Point(jPoint, iPart));

					if( dMax < d )
					{
						dMax	= d;
					}
				}
			}
		}
	}

	return( dMax );
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
