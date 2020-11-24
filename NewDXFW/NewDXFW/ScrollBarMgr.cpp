#include <iostream>

#include "ScrollBarMgr.h"

ScrollBarMgr::ScrollBarMgr (float coorX, float coorY, float width, float height,
							float widthScroll, float relHeightSlider, int capacity,
							fs::path initPath) :
	WinMgr (this),
	m_curPath (initPath),
	m_scrollBar (new ScrollBar (coorX, coorY, width, height,
							    widthScroll, relHeightSlider, capacity))
{
	WinMgr::AddChildWidget (m_scrollBar);

	std::vector <std::string> files;

	fs::directory_iterator begin (initPath);
	fs::directory_iterator end;

	std::vector <fs::path> childdirs;

	for (; begin != end; begin++)
		files.push_back (begin->path ().filename ().u8string ());

	m_scrollBar->Update (files);
}

void ScrollBarMgr::Draw ()
{
	WinMgr::Draw ();
}

void ScrollBarMgr::Update ()
{
	WinMgr::Update ();
}

void ScrollBarMgr::HandleNews (News news)
{
	if (news.m_idSender != (uint16_t) SENDER_NEWS::WINAPIWNDPROC)
	{
		if (news.m_idSender == m_scrollBar->GetID ())
		{
			if (news.m_news == NEWS::SELECT_ITEM_BAR)
			{
				std::string nameFile = *((std::string *) news.m_args);
				m_curPath.append (nameFile);

				std::cout << m_curPath << std::endl;

				// m_scrollBar.Update (names);
			}
		}
	}
	else
	{
		WinMgr::HandleNews (news);
	}
}
