#include <cstdio>
#include "AppMgr.h"
#include "DebugFunc.h"
#include "NewsQueue.h"
#include "WindowManager.h"

int main ()
{
	auto App = new AppMgr ();
	if (!App->Initialize ())
	{
		RETURN_FALSE;
	}

	App->Run ();
	
	/*
	NewsQueue::SetCapacity (1024);
	NewsQueue &newsQueue = *NewsQueue::GetNewsQueue ();

	News news[8] = {};

	news[0] = { 1, 2, nullptr };
	news[1] = { 2, 5, nullptr };
	news[2] = { 3, 9, nullptr };

	newsQueue += news[0];
	newsQueue += news[1];

	news[1] = newsQueue.GetNews ();
	news[0] = newsQueue.GetNews ();


	for (int i = 0; i < 128; i++)
		newsQueue += news[2];

	for (int i = 0; i < 128; i++)
		news[0] = newsQueue.GetNews ();

	newsQueue += news[1];
	*/

	DebugEndMain ();

	/*
	fs::path workdir = fs::current_path ();

	fs::directory_iterator begin (workdir);
	fs::directory_iterator end;

	std::vector <fs::path> childdirs;

	for (; begin != end; begin++)
		if (fs::is_directory (begin->path ()))
			cout << begin->path ().filename () << endl;

	*/
}