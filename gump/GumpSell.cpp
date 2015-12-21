/****************************************************************************
**
** SellGump.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
TGumpSell::TGumpSell(DWORD serial, short x, short y, int count)
: TGump(GT_SHOP, serial, x, y), m_Count(count), m_LastChangedLineTime(0),
m_LastChangeCountTime(0), m_StartChangeCountTime(0), m_SelectedLine1(0),
m_SelectedLine2(0), m_SelectedListCount(0), m_ContinueCounter(0)
{
}
//---------------------------------------------------------------------------
TGumpSell::~TGumpSell()
{
}
//----------------------------------------------------------------------------
void TGumpSell::PrepareTextures()
{
	UO->ExecuteGumpPart(0x0872, 2);
	UO->ExecuteGumpPart(0x0039, 3);
	UO->ExecuteGump(0x0828);
	UO->ExecuteGumpPart(0x0037, 2);

	TGumpSellObject *go = (TGumpSellObject*)m_Items;

	while (go != NULL)
	{
		TTextureObject *to = UO->ExecuteStaticArt(go->Graphic);

		if (go->Color && to)
			to->GetColoredTexture(go->Color);

		go = (TGumpSellObject*)go->m_Next;
	}
}
//----------------------------------------------------------------------------
void TGumpSell::CalculateSelectedListCount()
{
	m_SelectedListCount = 0;

	TGumpSellObject *go = (TGumpSellObject*)m_Items;

	while (go != NULL)
	{
		if (go->SelectedCount)
			m_SelectedListCount++;

		go = (TGumpSellObject*)go->m_Next;
	}
}
//---------------------------------------------------------------------------
void TGumpSell::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	TGameCharacter *vendor = World->FindWorldCharacter(Serial);
	if (vendor == NULL)
		return; //������, � �������� �������� ���� - �����

	if (m_Items == NULL)
		return;

	CalculateSelectedListCount(); //���������� �������� ����� ��������� ��������

	if (!NoClose)
		m_ContinueCounter = 0;

	DWORD index = (DWORD)this;

	//������ �� ������ � ����?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//������ �� ������ � ����?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//����� �� ���� ����� �������?
	int CanPressedButton = 0;
	if (IsScrollerPressed) CanPressedButton = g_LastObjectLeftMouseDown;
	
	int scrollerY1 = 0;
	int scrollerY2 = 0;

	int visibleLines = m_Count - 1;
	int maxScrollerY = 91;

	if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GS_SCROLL_1) //Scroller 1 pressed
	{
		int currentY = (g_MouseY - 10) - posY; //Scroller 1 position

		scrollerY1 = CalculateScrollerAndTextPosition(m_SelectedLine1, visibleLines, maxScrollerY, currentY);
	}
	else if (m_SelectedLine1)
		scrollerY1 = CalculateScrollerY(m_SelectedLine1, visibleLines, maxScrollerY);
	
	visibleLines = m_SelectedListCount - 1;
	maxScrollerY = 57;

	if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GS_SCROLL_2) //Scroller 2 pressed
	{
		int currentY = (g_MouseY - 10) - posY; //Scroller 2 position

		scrollerY2 = CalculateScrollerAndTextPosition(m_SelectedLine2, visibleLines, maxScrollerY, currentY);
	}
	else if (m_SelectedLine2)
		scrollerY2 = CalculateScrollerY(m_SelectedLine2, visibleLines, maxScrollerY);

	glNewList((GLuint)index, GL_COMPILE);

		UO->DrawGump(0x0872, 0, posX, posY);

		//My Inventory

		g_GL.ViewPort(posX + 30, posY + 60, 200, 176);

		int drawY = posY + 60;
			
		TTextureObject *thDelim1 = UO->ExecuteGump(0x0039);
		TTextureObject *thDelim2 = UO->ExecuteGump(0x003A);
		TTextureObject *thDelim3 = UO->ExecuteGump(0x003B);
		
		int delim1Height = 20;
		int delim1Width = 30;
		int delim1posX = posX + 32;
		int delim2posX = posX + 62;
		int delim3posX = 30;
		int delimLineWidth = 140;

		TGumpSellObject *go = (TGumpSellObject*)m_Items;

		if (thDelim1 == NULL || thDelim2 == NULL || thDelim3 == NULL)
		{
			//TPRINT("Error, Delimiter line gump not found!\n");
		}
		else
		{
			delim1Height = thDelim1->Height;
			delim1Width = thDelim1->Width;
			delim2posX = posX + 32 + delim1Width;
			delim3posX = posX + 226 - thDelim3->Width;
			delimLineWidth = 200 - (thDelim1->Width + thDelim3->Width);
		}

		int currentIndex = 0;
		int startIndex = m_SelectedLine1;

		int drawYBounds = posY + 236;

		while (go != NULL)
		{
			if (currentIndex < startIndex)
			{
				currentIndex++;
				go = (TGumpSellObject*)go->m_Next;

				continue;
			}

			if (drawY >= drawYBounds)
				break;

			TTextureObject *th = UO->ExecuteStaticArt(go->Graphic);

			if (th != NULL)
			{
				WORD textColor = 0x021F;
				if (go->Selected)
					textColor = 0x0021;

				TTextTexture tth;
				string str = go->GetName() + " at " + std::to_string(go->Price) + "gp";
				FontManager->GenerateA(9, tth, str.c_str(), textColor, 90);

				if (!tth.Empty())
				{
					int imageOffsetY = 0;
					int textOffsetY = 0;

					int maxOffset = th->Height;

					if (maxOffset < tth.Height)
					{
						imageOffsetY = ((tth.Height - maxOffset) / 2);
						maxOffset = tth.Height;
					}
					else
						textOffsetY = ((maxOffset - tth.Height) / 2);

					tth.Draw(posX + 82, drawY + textOffsetY);
					tth.Clear();
					
					str = std::to_string(go->Count - go->SelectedCount);
					int amountWidth = FontManager->GetWidthA(9, str.c_str(), str.length());
					FontManager->GenerateA(9, tth, str.c_str(), textColor, amountWidth);

					if (!tth.Empty())
					{
						tth.Draw(posX + 226 - amountWidth, drawY + (maxOffset / 2) - (tth.Height / 2));
						tth.Clear();
					}

					UO->DrawStaticArtInContainer(go->Graphic, go->Color, delim1posX, drawY + imageOffsetY);
					
					drawY += maxOffset;
					
					UO->DrawGump(0x0039, 0, delim1posX, drawY);
					UO->DrawGump(0x003A, 0, delim2posX, drawY, delimLineWidth, 0);
					UO->DrawGump(0x003B, 0, delim3posX, drawY);

					drawY += delim1Height;
				}
			}

			currentIndex++;

			go = (TGumpSellObject*)go->m_Next;
		}

		g_GL.RestorePort();
		
		//Up
		//UO->DrawGump(0x0828, 0, posX + 237, posY + 66); //Scroller 1 min
		//Down
		//UO->DrawGump(0x0828, 0, posX + 237, posY + 157); //Scroller 1 max
			
		UO->DrawGump(0x0828, 0, posX + 237, posY + 66 + scrollerY1); //Scroller 1

		//Offer

		UO->DrawGump(0x0873, 0, posX + 170, posY + 214);
			
		g_GL.ViewPort(posX + 200, posY + 280, 200, 92);

		drawY = posY + 282;

		currentIndex = 0;
		startIndex = m_SelectedLine2;

		drawYBounds = posY + 372;

		int totalPrice = 0;

		go = (TGumpSellObject*)m_Items;

		while (go != NULL)
		{
			if (!go->SelectedCount)
			{
				go = (TGumpSellObject*)go->m_Next;

				continue;
			}

			totalPrice += (go->SelectedCount * (int)go->Price);
				
			if (currentIndex < startIndex)
			{
				currentIndex++;
				go = (TGumpSellObject*)go->m_Next;

				continue;
			}

			if (drawY >= drawYBounds)
			{
				go = (TGumpSellObject*)go->m_Next;
				continue;
			}

			WORD textColor = 0x021F;

			TTextTexture tth;
			string str = go->GetName();
			FontManager->GenerateA(9, tth, str.c_str(), textColor, 100);

			if (!tth.Empty())
			{
				int maxOffset = tth.Height;

				tth.Draw(posX + 234, drawY);
				tth.Clear();

				str = "at " + std::to_string(go->Price) + " g.p.";
				FontManager->GenerateA(9, tth, str.c_str(), textColor, 100);

				if (!tth.Empty())
				{
					tth.Draw(posX + 234, drawY + maxOffset - 2);

					maxOffset += (tth.Height - 2);

					tth.Clear();
				}
						
				str = std::to_string(go->SelectedCount);
				int amountWidth = FontManager->GetWidthA(9, str.c_str(), str.length());
				FontManager->GenerateA(9, tth, str.c_str(), textColor, amountWidth);

				if (!tth.Empty())
				{
					tth.Draw(posX + 204, drawY);
					tth.Clear();
				}

				int drawOffsetY = drawY + (maxOffset / 2) - 6;
					
				UO->DrawGump(0x0037, 0, posX + 356, drawOffsetY); //+
				UO->DrawGump(0x0038, 0, posX + 376, drawOffsetY); //-

				drawY += maxOffset;
			}

			currentIndex++;

			go = (TGumpSellObject*)go->m_Next;
		}

		g_GL.RestorePort();
		
		//Up
		//UO->DrawGump(0x0828, 0, posX + 407, posY + 280); //Scroller 2 min
		//Down
		//UO->DrawGump(0x0828, 0, posX + 407, posY + 337); //Scroller 2 max

		UO->DrawGump(0x0828, 0, posX + 407, posY + 280 + scrollerY2); //Scroller 2

		FontManager->DrawA(9, std::to_string(totalPrice).c_str(), 0x0386, posX + 358, posY + 386);
		
		if (NoClose)
		{
			if (g_Player != NULL)
			{
				string name = g_Player->GetName();

				if (m_ContinueCounter > (int)name.length())
					m_ContinueCounter = name.length();

				name.resize(m_ContinueCounter);

				static int sCounterCount = 0;

				if (m_ContinueCounter == g_Player->GetName().length())
				{
					NoClose = false;
					SendSellList();
				}
				else if (!sCounterCount)
				{
					if (m_ContinueCounter < (int)g_Player->GetName().length())
						m_ContinueCounter++;
				}

				sCounterCount++;

				if (sCounterCount >= 3)
					sCounterCount = 0;

				FontManager->DrawA(5, name.c_str(), 0x0386, posX + 242, posY + 408);
			}
		}

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSell::Draw(bool &mode)
{
	TGameCharacter *vendor = World->FindWorldCharacter(Serial);

	if (vendor == NULL)
		return 0; //������, � �������� �������� ���� - �����

	if (m_Items == NULL)
		return 0;
	
	DWORD index = (DWORD)this;

	//��� �������� �������
	int posX = X;
	int posY = Y;

	CalculateSelectedListCount(); //���������� �������� ����� ��������� ��������

	//������ �� ������ � ����?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//������ �� ������ � ����?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//����� �� ���� ����� �������?
	int CanPressedButton = 0;
	if (IsScrollerPressed)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	DWORD ticks = GetTickCount();

	bool needRedraw = false;

	if (!NoClose)
		m_ContinueCounter = 0;
	else
		needRedraw = true;
	
	if (mode && CanPressedButton >= ID_GS_BUTTON_INC && CanPressedButton < ID_GS_BUTTON_END && m_LastChangeCountTime < ticks)
	{
		int countToAdd = 1;
			
		if (m_StartChangeCountTime < 7)
			{}
		else if (m_StartChangeCountTime < 14)
			countToAdd = 3;
		else if (m_StartChangeCountTime < 20)
			countToAdd = 5;
		else if (m_StartChangeCountTime < 35)
			countToAdd = 10;
		else
			countToAdd = 20;

		m_StartChangeCountTime++;

		if (CanPressedButton < ID_GS_BUTTON_DEC) //+
		{
			int idx = CanPressedButton - ID_GS_BUTTON_INC;

			ChangeItemCount(true, idx, countToAdd);
		}
		else //-
		{
			int idx = CanPressedButton - ID_GS_BUTTON_DEC;
			
			if (ChangeItemCount(false, idx, countToAdd))
			{
				g_LeftMouseDown = false;
				g_LastGumpLeftMouseDown = 0;
				g_LastObjectLeftMouseDown = 0;
			}
		}

		m_LastChangeCountTime = ticks + CHANGE_SHOP_COUNT_DELAY;

		needRedraw = true;
	}
	
	if (IsScrollerPressed && m_LastChangedLineTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_1) //1 gump ^
		{
			ListingList(true, false);
			needRedraw = true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_1) //1 gump v
		{
			ListingList(false, false);
			needRedraw = true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_2) //2 gump ^
		{
			ListingList(true, true);
			needRedraw = true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_2) //2 gump v
		{
			ListingList(false, true);
			needRedraw = true;
		}
	}
	
	//���� ������ ��������� ��� ����������� - �������� �������
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
	}

	int scrollerY1 = 0;
	int scrollerY2 = 0;
	bool noRedraw = false;
	int visibleLines = m_Count - 1;
	int maxScrollerY = 91;

	if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GS_SCROLL_1) //Scroller 1 pressed
	{
		int currentY = (g_MouseY - 10) - posY; //Scroller 1 position

		scrollerY1 = CalculateScrollerAndTextPosition(m_SelectedLine1, visibleLines, maxScrollerY, currentY);

		if (mode)
			GenerateFrame(posX, posY);

		noRedraw = true;
	}
	else if (m_SelectedLine1)
	{
		scrollerY1 = CalculateScrollerY(m_SelectedLine1, visibleLines, maxScrollerY);

		if (mode)
			GenerateFrame(posX, posY);

		noRedraw = true;
	}
	
	visibleLines = m_SelectedListCount - 1;
	maxScrollerY = 57;

	if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GS_SCROLL_2) //Scroller 2 pressed
	{
		int currentY = (g_MouseY - 10) - posY; //Scroller 2 position

		scrollerY2 = CalculateScrollerAndTextPosition(m_SelectedLine2, visibleLines, maxScrollerY, currentY);

		if (mode)
			GenerateFrame(posX, posY);

		noRedraw = true;
	}
	else if (m_SelectedLine2)
	{
		scrollerY2 = CalculateScrollerY(m_SelectedLine2, visibleLines, maxScrollerY);

		if (mode)
			GenerateFrame(posX, posY);

		noRedraw = true;
	}

	//���� ������ ��������� ��� ����������� - ���������� ����
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		if (!noRedraw)
			GenerateFrame(posX, posY);
	}
	else if (mode && !noRedraw)
	{
		if (needRedraw)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode) //���������
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else //����� ��������
	{
		int LSG = 0;

		bool secondGumpSelected = false;

		if (UO->GumpPixelsInXY(0x0873, posX + 170, posY + 214))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			secondGumpSelected = true;
		}
		else if (UO->GumpPixelsInXY(0x0872, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_LastSelectedGump == index)
		{
			//My Inventory

			int drawY = posY + 60;
			
			TTextureObject *thDelim1 = UO->ExecuteGump(0x0039);
			
			int delimHeight = 20;

			if (thDelim1 != NULL)
				delimHeight = thDelim1->Height;

			int currentIndex = 0;
			int startIndex = m_SelectedLine1;

			int drawYBounds = posY + 236;
			
			bool completedSearch = false;
			
			TGumpSellObject *go = (TGumpSellObject*)m_Items;

			if (!secondGumpSelected)
			{
				while (go != NULL)
				{
					if (currentIndex < startIndex)
					{
						currentIndex++;
						go = (TGumpSellObject*)go->m_Next;
						continue;
					}

					if (drawY >= drawYBounds)
						break;

					POINT p_art = {0, 0};
					UO->GetArtDimension(go->Graphic + 0x4000, p_art);

					if (p_art.y)
					{
						string str = go->GetName() + " at " + std::to_string(go->Price) + "gp";
						int textHeight = FontManager->GetHeightA(9, str.c_str(), 90);
						
						if (textHeight)
						{
							int maxOffset = p_art.y;

							if (maxOffset < textHeight)
								maxOffset = textHeight;

							int checkHeight = maxOffset;
							if (drawY + maxOffset > drawYBounds)
								checkHeight = drawYBounds - drawY;
						
							RECT rc = {0, 0, 200, checkHeight};
							POINT p = {g_MouseX - (posX + 30), g_MouseY - drawY};
			
							if (PtInRect(&rc, p)) //Item in list on gump 1
							{
								LSG = ID_GS_ITEM_LIST + currentIndex;
								completedSearch = true;

								break;
							}

							drawY += (maxOffset + delimHeight);
						}
					}

					currentIndex++;

					if (completedSearch)
						break;

					go = (TGumpSellObject*)go->m_Next;
				}
			}

			if (!completedSearch) //Offer
			{
				drawY = posY + 282;

				currentIndex = 0;
				startIndex = m_SelectedLine2;

				drawYBounds = posY + 372;

				go = (TGumpSellObject*)m_Items;

				while (go != NULL)
				{
					if (!go->SelectedCount)
					{
						go = (TGumpSellObject*)go->m_Next;
						continue;
					}

					if (currentIndex < startIndex)
					{
						currentIndex++;
						go = (TGumpSellObject*)go->m_Next;

						continue;
					}

					if (drawY >= drawYBounds)
						break;

					string str = go->GetName();
					int textHeight = FontManager->GetHeightA(9, str.c_str(), 100);

					if (textHeight)
					{
						int maxOffset = textHeight;

						str = "at " + std::to_string(go->Price) + " g.p.";
						textHeight = FontManager->GetHeightA(9, str.c_str(), 100);

						if (textHeight)
							maxOffset += (textHeight - 2);
							
						int drawOffsetY = drawY + (maxOffset / 2) - 4;
					
						RECT rc = {0, 0, 14, 14};
						POINT p = {g_MouseX - (posX + 356), g_MouseY - drawOffsetY};
			
						if (PtInRect(&rc, p)) //+
						{
							LSG = ID_GS_BUTTON_INC + currentIndex;
							completedSearch = true;

							break;
						}

						p.x = g_MouseX - (posX + 376);
							
						if (PtInRect(&rc, p)) //-
						{
							LSG = ID_GS_BUTTON_DEC + currentIndex;
							completedSearch = true;

							break;
						}

						drawY += maxOffset;
					}

					currentIndex++;

					if (completedSearch)
						break;

					go = (TGumpSellObject*)go->m_Next;
				}
			}
		}
		
		if (UO->GumpPixelsInXY(0x0828, posX + 237, posY + 66 + scrollerY1)) //Scroller 1
		{
			g_LastSelectedGump = index;
			LSG = ID_GS_SCROLL_1;
		}
		else if (UO->GumpPixelsInXY(0x0828, posX + 407, posY + 280 + scrollerY2)) //Scroller 2
		{
			g_LastSelectedGump = index;
			LSG = ID_GS_SCROLL_2;
		}
		else if (g_LastSelectedGump == index)
		{
			RECT rc = {0, 0, 20, 16};
			POINT p = {g_MouseX - (posX + 233), g_MouseY - (posY + 49)};
			
			if (PtInRect(&rc, p)) //1 gump ^
				LSG = ID_GS_BUTTON_UP_1;

			p.y = g_MouseY - (posY + 191);
			if (PtInRect(&rc, p)) //1 gump v
				LSG = ID_GS_BUTTON_DOWN_1;

			p.x = g_MouseX - (posX + 403);
			p.y = g_MouseY - (posY + 264);
			if (PtInRect(&rc, p)) //2 gump ^
				LSG = ID_GS_BUTTON_UP_2;
			
			p.y = g_MouseY - (posY + 371);
			if (PtInRect(&rc, p)) //2 gump v
				LSG = ID_GS_BUTTON_DOWN_2;
			
			rc.right = 34;
			rc.bottom = 30;
			p.x = g_MouseX - (posX + 200);
			p.y = g_MouseY - (posY + 406);
			if (PtInRect(&rc, p)) //Accept
				LSG = ID_GS_BUTTON_ACCEPT;
			
			rc.right = 24;
			rc.bottom = 24;
			p.x = g_MouseX - (posX + 372);
			p.y = g_MouseY - (posY + 410);
			if (PtInRect(&rc, p)) //Clear
				LSG = ID_GS_BUTTON_CLEAR;
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG; //���� ���-�� ������� - ��������

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSell::OnLeftMouseDown()
{
	if (NoClose)
		return;

	if (g_LastSelectedObject >= ID_GS_BUTTON_INC && g_LastSelectedObject < ID_GS_BUTTON_END) // +/-
	{
		m_StartChangeCountTime = 0;
		m_LastChangeCountTime = GetTickCount() + (CHANGE_SHOP_COUNT_DELAY * 5);
	}
}
//----------------------------------------------------------------------------
void TGumpSell::OnLeftMouseUp()
{
	if (NoClose)
		return;

	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_1) //1 gump ^
	{
		//ListingList(gump, true, false);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_1) //1 gump v
	{
		//ListingList(gump, false, false);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_2) //2 gump ^
	{
		//ListingList(gump, true, true);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_2) //2 gump v
	{
		//ListingList(gump, false, true);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_ACCEPT) //Accept
	{
		NoClose = true;
		m_ContinueCounter = 0;
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_CLEAR) //Clear
		m_ContinueCounter = 0;
	else if (g_LastObjectLeftMouseDown >= ID_GS_ITEM_LIST)
	{
		if (g_LastObjectLeftMouseDown < ID_GS_BUTTON_INC) //Item in list on gump 1
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_ITEM_LIST;

			TGumpSellObject *go = (TGumpSellObject*)m_Items;

			int currentIndex = 0;

			while (go != NULL)
			{
				go->Selected = (idx == currentIndex);

				currentIndex++;

				go = (TGumpSellObject*)go->m_Next;
			}

			GenerateFrame(X, Y);
		}
		else if (g_LastObjectLeftMouseDown < ID_GS_BUTTON_DEC) //+
		{
			if (m_LastChangeCountTime > GetTickCount())
				ChangeItemCount(true, g_LastObjectLeftMouseDown - ID_GS_BUTTON_INC);

			GenerateFrame(X, Y);
		}
		else //-
		{
			if (m_LastChangeCountTime > GetTickCount())
				ChangeItemCount(false, g_LastObjectLeftMouseDown - ID_GS_BUTTON_DEC);

			GenerateFrame(X, Y);
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpSell::OnLeftMouseDoubleClick()
{
	if (NoClose)
		return false;

	if (g_LastObjectLeftMouseDown >= ID_GS_ITEM_LIST && g_LastObjectLeftMouseDown < ID_GS_BUTTON_INC) //Item in list on gump 1
	{
		if (m_Items != NULL) //Sell
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_ITEM_LIST;

			TGumpSellObject *go = (TGumpSellObject*)m_Items;

			int currentIndex = 0;

			while (go != NULL)
			{
				if (idx == currentIndex)
				{
					go->Selected = true;

					if (go->SelectedCount < go->Count)
						go->IncSelectedCount();
				}
				else
					go->Selected = false;

				currentIndex++;

				go = (TGumpSellObject*)go->m_Next;
			}

			GenerateFrame(X, Y);

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool TGumpSell::ChangeItemCount(bool increase, int idx, int count)
{
	if (NoClose || m_Items == NULL)
		return false;

	TGumpSellObject *go = (TGumpSellObject*)m_Items;

	int currentIndex = 0;

	while (go != NULL)
	{
		if (go->SelectedCount)
		{
			if (idx == currentIndex)
			{
				if (increase) //+
				{
					if (go->SelectedCount < go->Count)
					{
						go->AddSelectedCount(count);

						if (go->SelectedCount > go->Count)
							go->SelectedCount = go->Count;
					}
				}
				else //-
				{
					go->AddSelectedCount(-count);

					if (go->SelectedCount <= 0)
					{
						go->SelectedCount = 0;

						CalculateSelectedListCount();

						if (m_SelectedLine2 >= m_SelectedListCount)
						{
							if (m_SelectedListCount)
								m_SelectedLine2 = m_SelectedListCount - 1;
							else
								m_SelectedLine2 = 0;
						}

						return true;
					}
				}

				break;
			}
					
			currentIndex++;
		}

		go = (TGumpSellObject*)go->m_Next;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpSell::ListingList(bool direction, bool gumpNumber, int divizor)
{
	if (NoClose) return;

	DWORD ticks = GetTickCount();

	if (!gumpNumber) //First gump
	{
		if (direction) //Up
		{
			if (m_SelectedLine1 > 0)
				m_SelectedLine1--;
			else
				m_SelectedLine1 = 0;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
		else //Down
		{
			int visibleLines = m_Count - 1;

			if (m_SelectedLine1 < visibleLines)
				m_SelectedLine1++;
			else
				m_SelectedLine1 = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
	}
	else //Second gump
	{
		CalculateSelectedListCount();

		if (direction) //Up
		{
			if (m_SelectedLine2 > 0)
				m_SelectedLine2--;
			else
				m_SelectedLine2 = 0;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
		else if (m_SelectedListCount) //Down
		{
			int visibleLines = m_SelectedListCount - 1;

			if (m_SelectedLine2 < visibleLines)
				m_SelectedLine2++;
			else
				m_SelectedLine2 = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSell::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (NoClose)
		return;

	if (!g_LeftMouseDown && !g_RightMouseDown && state != MWS_PRESS)
	{
		if (m_LastChangedLineTime > GetTickCount())
			return;

		register int x = X;
		register int y = Y;

		if (UO->GumpPixelsInXY(0x0873, x + 170, y + 214))
		{
			ListingList((bool)(state == MWS_UP), true, 5);

			GenerateFrame(x, y);
		}
		else
		{
			if (UO->GumpPixelsInXY(0x0872, x, y))
			{
				ListingList((bool)(state == MWS_UP), false, 5);

				GenerateFrame(x, y);
			}
		}
	}
}
//----------------------------------------------------------------------------