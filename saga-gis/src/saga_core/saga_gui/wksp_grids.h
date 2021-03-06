/**********************************************************
 * Version $Id$
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    User Interface                     //
//                                                       //
//                    Program: SAGA                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//                    WKSP_Grids.h                       //
//                                                       //
//          Copyright (C) 2017 by Olaf Conrad            //
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
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Hamburg                  //
//                Germany                                //
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Grids_H
#define _HEADER_INCLUDED__SAGA_GUI__WKSP_Grids_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "wksp_layer.h"
#include "wksp_layer_classify.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
class CWKSP_Grids : public CWKSP_Layer
{
public:
	CWKSP_Grids(CSG_Grids *pGrids);

	virtual TWKSP_Item			Get_Type				(void)	{	return( WKSP_ITEM_Grids );	}

	CSG_Grids *					Get_Grids				(void)	{	return( (CSG_Grids *)m_pObject );	}

	CSG_Grid *					Get_Grid				(void);
	CSG_Grid *					Get_Grid				(int i);

	virtual wxString			Get_Description			(void);

	virtual wxMenu *			Get_Menu				(void);

	virtual bool				On_Command				(int Cmd_ID);
	virtual bool				On_Command_UI			(wxUpdateUIEvent &event);

	virtual wxString			Get_Value				(CSG_Point ptWorld, double Epsilon)	{	return( "" );	}
	virtual double				Get_Value_Minimum		(void);
	virtual double				Get_Value_Maximum		(void);
	virtual double				Get_Value_Range			(void);
	virtual double				Get_Value_Mean			(void);
	virtual double				Get_Value_StdDev		(void);

	virtual TSG_Rect			Edit_Get_Extent			(void)	{	return( Get_Extent() );	}
	virtual bool				Edit_Set_Attributes		(void);

	bool						Fit_Colors				(const CSG_Rect &rWorld);

	virtual bool				asImage					(CSG_Grid *pImage);

	virtual bool				Update					(CWKSP_Layer *pChanged);


protected:

	bool						Get_Image_Grid			(wxBitmap &BMP, bool bFitSize = true);
	bool						Get_Image_Legend		(wxBitmap &BMP, double Zoom);

	virtual void				On_Create_Parameters	(void);
	virtual void				On_DataObject_Changed	(void);
	virtual void				On_Parameters_Changed	(void);
	virtual int					On_Parameter_Changed	(CSG_Parameters *pParameters, CSG_Parameter *pParameter, int Flags);

	virtual bool				Fit_Colors				(void);

	virtual void				On_Draw					(CWKSP_Map_DC &dc_Map, int Flags);


private:

	CWKSP_Layer_Classify		m_Classify[3];


	CSG_String					_Get_List_Attributes	(void);
	CSG_String					_Get_List_Bands			(int Attribute = -1);

	bool						_Fit_Colors				(bool bRefresh = false);
	bool						_Fit_Colors				(CSG_Parameters &Parameters, double &Minimum, double &Maximum);
	bool						_Fit_Colors				(CSG_Simple_Statistics &s, CWKSP_Layer_Classify *pClassify, bool bRefresh = false);

	void						_LUT_Create				(void);

	void						_Save_Image				(void);

	void						_Draw_Grid_Points		(CWKSP_Map_DC &dc_Map, CSG_Grid *pBands[3], TSG_Grid_Resampling Resampling);
	void						_Draw_Grid_Line			(CWKSP_Map_DC &dc_Map, CSG_Grid *pBands[3], TSG_Grid_Resampling Resampling, int Mode, int yDC, int axDC, int bxDC);
	void						_Draw_Grid_Cells		(CWKSP_Map_DC &dc_Map, CSG_Grid *pBands[3]);

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef _HEADER_INCLUDED__SAGA_GUI__WKSP_Grids_H
