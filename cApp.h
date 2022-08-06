#pragma once
#include "wx/wx.h"
#include "Main.h"
class cApp : public wxApp
{
private:
	NineMensMorris* m_frame1 = nullptr;
public:
	cApp();
	~cApp();
	virtual bool OnInit();
};

