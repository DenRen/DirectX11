#pragma once

#include <cstdint>

struct News
{
	uint16_t m_id;
	uint32_t m_event;
	void *m_args;
};

class NewsQueue
{
public:

	static void SetCapacity (int capacity);
	static NewsQueue *GetNewsQueue ();

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
