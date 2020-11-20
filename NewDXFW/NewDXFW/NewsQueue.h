#pragma once

#include <cstdint>
#include <windows.h>

struct MousePosition
{
	float x;
	float y;
};

enum class NEWS;

struct News
{
	News ();
	News (uint16_t idSender);
	News (uint16_t idSender, NEWS news);
	News (uint16_t idSender, NEWS news, void *args);

	uint16_t m_idSender;
	NEWS m_news;
	union {
		void *m_args;
		MousePosition m_mousePos;
	};
};

class NewsQueue
{
public:

	static void SetCapacity (int capacity);
	static NewsQueue *GetNewsQueue ();

	int GetSize ();

	void GetWinAPINews (UINT msg, WPARAM wParam, LPARAM lParam);
	void AddNews (const News &news);
	News GetNews ();

	NewsQueue &operator += (const News &news);

private:
	int m_begin;
	int m_tail;

	static int m_capacity;
	News *m_buffer;

	NewsQueue (int capacity);
	~NewsQueue ();
	bool Initialize (int capacity);

	void Release ();
	void SetReadyForAdd ();
};

enum class SENDER_NEWS
{
	WINAPIWNDPROC,
	SIZE
};

enum class NEWS
{
	// Mouse >--------------------------->begin
	MOUSEFIRST,
	MOUSEMOVE,
	
	LBUTTONDOWN, LBUTTONUP, LBUTTONDBLCLK,
	RBUTTONDOWN, RBUTTONUP, RBUTTONDBLCLK,

	MBUTTONDOWN, MBUTTONUP, MBUTTONDBLCLK,
	XBUTTONDOWN, XBUTTONUP, XBUTTONDBLCLK,

	MOUSEHWHEEL,
	MOUSELAST
	// Mouse >----------------------------->end
};