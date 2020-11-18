#include "NewsQueue.h"

#include <cassert>
#include <cstdlib>

#include "DebugFunc.h"

int NewsQueue::m_capacity = 0;

NewsQueue::NewsQueue (int capacity)
{
	printf ("NewsQ constr start!\n");

	if (!Initialize (capacity))
	{
		RETURN_THROW;
	}

	printf ("NewsQ constr finish!\n");
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
