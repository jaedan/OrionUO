/****************************************************************************
**
** GumpGrayMenu.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef GumpGrayMenuH
#define GumpGrayMenuH
//---------------------------------------------------------------------------
class TGumpGrayMenu : public TGump
{
private:
	static const int ID_GGM_CANCEL = 100;
	static const int ID_GGM_CONTINUE = 101;

	int m_Count;
	string m_Text;
public:
	TGumpGrayMenu(DWORD serial, short x, short y, string text);
	virtual ~TGumpGrayMenu();

	SETGET(int, Count)
	SETGET(string, Text)

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame(int posX, int posY);

	void OnLeftMouseUp();
};
//---------------------------------------------------------------------------
#endif