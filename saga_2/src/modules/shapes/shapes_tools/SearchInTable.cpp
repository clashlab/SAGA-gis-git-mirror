/*******************************************************************************
    SearchInTable.cpp
    Copyright (C) Victor Olaya

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*******************************************************************************/
#include "SearchInTable.h"
#include "ShapeSearchSelector.h"

#define METHOD_NEW_SEL 0
#define METHOD_ADD_TO_SEL 1
#define METHOD_SELECT_FROM_SEL 2

CSearchInTable::CSearchInTable(void){

	Parameters.Set_Name(_TL("Search in attributes table"));

	Parameters.Set_Description(_TL("(c) 2004 Victor Olaya. Searches for an expression in the attributes table and selects records where the expression is found"));

	Parameters.Add_Shapes(NULL,
						"SHAPES",
						_TL("Shapes"),
						"",
						PARAMETER_INPUT);

	Parameters.Add_String(NULL, "EXPRESSION", _TL("Expression"), "", "");

	Parameters.Add_Choice(NULL, 
						"METHOD", 
						_TL("Method"), 
						"",
						_TL(
						"New selection|"
						"Add to current selection|"
						"Select from current selection|"),
						0);

}//constructor

CSearchInTable::~CSearchInTable(void){}

bool CSearchInTable::On_Execute(void){

	CAPI_String sExpression;
	CShapes *pShapes;
	CTable *pTable;
	CShapeSearchSelector *pSelector;	
	bool *pRecordWasSelected;
	int *pSelectedRecords;
	int iNumSelectedRecords = 0;
	int iMethod;
	int iRecord;
	int i;
	
	pShapes = Parameters("SHAPES")->asShapes();
	pTable	= &pShapes->Get_Table();	
	sExpression = Parameters("EXPRESSION")->asString();
	iMethod = Parameters("METHOD")->asInt();

	pRecordWasSelected = new bool[pTable->Get_Record_Count()];

	if (iMethod == METHOD_SELECT_FROM_SEL){
		for (i = 0; i < pTable->Get_Record_Count(); i++){
			if (pTable->Get_Record(i)->is_Selected()){
				pRecordWasSelected[i] = true;
			}//if
			else{
				pRecordWasSelected[i] = false;
			}//else
		}//for
	}//if

	if (iMethod != METHOD_ADD_TO_SEL){
		for (i = 0; i < pTable->Get_Record_Count(); i++){
			if (pTable->Get_Record(i)->is_Selected()){
				pTable->Select(i, true);
			}//if
		}//for
	}//if

	pSelector = new CShapeSearchSelector(pShapes, sExpression);
	pSelectedRecords = &pSelector->GetSelectedRecords();
	iNumSelectedRecords = pSelector->GetSelectedRecordsCount();

	for (i = 0; i < iNumSelectedRecords; i++){
		iRecord = pSelectedRecords[i];
		if (!pTable->Get_Record(iRecord)->is_Selected()){
			if (iMethod == METHOD_SELECT_FROM_SEL){
				if (pRecordWasSelected[iRecord]){
					pTable->Select(iRecord, true);
				}//if
			}//if
			else{
				pTable->Select(iRecord, true);
			}//else
		}//if
	}//for

	DataObject_Update(pShapes, false);

	return true;
	
}//method
