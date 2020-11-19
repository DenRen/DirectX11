#include "NewsQueue.h"

#include <cassert>
#include <cstdlib>

#include "DebugFunc.h"
#include "InputCoorMouse.h"

int NewsQueue::m_capacity = 0;

NewsQueue::NewsQueue (int capacity)
{
	if (!Initialize (capacity))
	{
		RETURN_THROW;
	}
}

NewsQueue::~NewsQueue ()
{
	Release ();
}

void NewsQueue::SetCapacity (int capacity)
{
	m_capacity = capacity;
}

NewsQueue *NewsQueue::GetNewsQueue ()
{
	static NewsQueue newsQueue (m_capacity);
	
	return &newsQueue;
}

int NewsQueue::GetSize ()
{
	return m_tail - m_begin;
}

void NewsQueue::GetWinAPINews (UINT msg, WPARAM wParam, LPARAM lParam)
{ 
	#define ADDNEWS(msg)			\
	case WM_##msg:					\
	{								\
		news.m_news = NEWS::msg;	\
		AddNews (news);				\
	} break

	if (m_capacity == 0)
	{
		return;
	}

	News news = {};
	news.m_idSender = (uint16_t) SENDER_NEWS::WINAPIWNDPROC;

	if (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)
	{
		WndCnf::ConvertMouseCoor (lParam, news.m_mousePos.x, news.m_mousePos.y);

		switch (msg) {
			case WM_MOUSEMOVE:
			{
				InputCoorMouse::SetPosition (news.m_mousePos);
			} break;

			ADDNEWS (LBUTTONDOWN);
			ADDNEWS (LBUTTONUP);
			ADDNEWS (LBUTTONDBLCLK);
			ADDNEWS (RBUTTONDOWN);
			ADDNEWS (RBUTTONUP);
			ADDNEWS (RBUTTONDBLCLK);

			ADDNEWS (MBUTTONDOWN);
			ADDNEWS (MBUTTONUP);
			ADDNEWS (MBUTTONDBLCLK);

			ADDNEWS (XBUTTONDOWN);
			ADDNEWS (XBUTTONUP);
			ADDNEWS (XBUTTONDBLCLK);

			ADDNEWS (MOUSEHWHEEL);
		}
	}

	#undef ADDNEWS
}


void NewsQueue::AddNews (const News &news)
{
	SetReadyForAdd ();

	m_buffer[++m_tail] = news;
}

News NewsQueue::GetNews ()
{
	MYASSERT (m_begin + 1 <= m_tail);

	if (m_begin == m_tail)
	{
		// Empty buffer !!!
		RETURN_THROW;
	}

	return m_buffer[++m_begin];
}

NewsQueue &NewsQueue::operator += (const News &news)
{
	AddNews (news);

	return *this;
}

bool NewsQueue::Initialize (int capacity)
{
	MYASSERT (capacity > 0);

	if (m_buffer != nullptr)
	{
		Release ();
	}

	m_buffer = (News *) calloc (capacity, sizeof (News));
	if (m_buffer == nullptr)
	{
		RETURN_FALSE;
	}

	m_begin = -1;
	m_tail = -1;

	m_capacity = capacity;

	return true;
}

void NewsQueue::Release ()
{
	free (m_buffer);
}

void NewsQueue::SetReadyForAdd ()
{
	MYASSERT (m_tail <= m_capacity);
	MYASSERT (m_begin >= -1);

	if (m_begin == m_tail && m_begin != 0)
	{
		m_begin = -1;
		m_tail = -1;

		return;
	}

	if (m_tail == m_capacity - 1)
	{
		if (m_begin != -1)
		{
			memcpy (m_buffer, &m_buffer[m_begin + 1], m_tail - m_begin);
			m_tail = m_begin + 1;
			m_begin = -1;
		}
		else
		{
			// Bufferr overfulled!
			RETURN_THROW;
		}
	}
}

News::News () :
	News (0, (NEWS) 0, nullptr)
{}

News::News (uint16_t idSender) :
	News (idSender, (NEWS) 0, nullptr)
{}

News::News (uint16_t idSender, NEWS news) :
	News (idSender, news, nullptr)
{}

News::News (uint16_t idSender, NEWS news, void *args) :
	m_idSender (idSender),
	m_news (news),
	m_args (args)
{}
