#include "MenuItem.h"

// This is a GUI control constructor,
// we should initialize all the variables here
MenuItem::MenuItem(int _id, hgeFont *_fnt, HEFFECT _snd, float _x, float _y, float _delay, char *_title)
{
	float w;
	
	id=_id;
	fnt=_fnt;
	menuEffect=_snd;
	delay=_delay;
	title=_title;

	color.SetHWColor(0xFFFFA500);  //orange
	shadow.SetHWColor(0x30000000); // black shadow
	offset=0.0f;
	timer=-1.0f;
	timer2=-1.0f;

	bStatic=false;
	bVisible=true;
	bEnabled=true;

	w=fnt->GetStringWidth(title);
	rect.Set(_x-w/2, _y, _x+w/2, _y+fnt->GetHeight());
}

// This method is called when the control should be rendered
void MenuItem::Render()
{
	fnt->SetColor(shadow.GetHWColor());
	fnt->Render(rect.x1+offset+3, rect.y1+3, HGETEXT_LEFT, title);
	fnt->SetColor(color.GetHWColor());
	fnt->Render(rect.x1-offset, rect.y1-offset, HGETEXT_LEFT, title);
}

// This method is called each frame,
// we should update the animation here
void MenuItem::Update(float frameTime)
{
	if(timer2 != -1.0f)
	{
		timer2 += frameTime;
		if(timer2 >= delay + 0.1f)
		{
			color	= scolor2 + dcolor2;
			shadow	= sshadow + dshadow;
			offset	= 0.0f;
			timer2	= -1.0f;
		}
		else
		{
			if(timer2 < delay)
			{
				color	= scolor2;
				shadow	= sshadow;
			}
			else 
			{
				color	= scolor2 + dcolor2 * (timer2 - delay) * 10;
				shadow	= sshadow + dshadow * (timer2 - delay) * 10;
			}
		}
	}
	else if(timer != -1.0f)
	{
		timer += frameTime;
		if(timer >= 0.2f)
		{
			color	= scolor + dcolor;
			offset	= soffset + doffset;
			timer	= -1.0f;
		}
		else
		{
			color	= scolor + dcolor * timer * 5;
			offset	= soffset + doffset * timer * 5;
		}
	}
}

// This method is called when the GUI
// is about to appear on the screen
void MenuItem::Enter()
{
	hgeColor tcolor2;

	scolor2.SetHWColor(0x00000000);
	tcolor2.SetHWColor(0xFFFFA500);
	dcolor2 = tcolor2 - scolor2;

	sshadow.SetHWColor(0x00000000);
	tcolor2.SetHWColor(0x30000000);
	dshadow = tcolor2 - sshadow;

	timer2 = 0.0f;
}

// This method is called when the GUI
// is about to disappear from the screen
void MenuItem::Leave()
{
	hgeColor tcolor2;

	scolor2.SetHWColor(0xFFEEEEEE);
	tcolor2.SetHWColor(0x00EEEEEE);
	dcolor2 = tcolor2 - scolor2;

	sshadow.SetHWColor(0x30000000);
	tcolor2.SetHWColor(0x00000000);
	dshadow = tcolor2 - sshadow;

	timer2 = 0.0f;

}

// This method is called to test whether the control
// have finished it's Enter/Leave animation
bool MenuItem::IsDone()
{
	if(timer2 == -1.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// This method is called when the control
// receives or loses keyboard input focus
void MenuItem::Focus(bool bFocused)
{
	hgeColor tcolor;
	
	if(bFocused)
	{
		hge->Effect_Play(menuEffect);
		scolor.SetHWColor(0xFF111111);
		tcolor.SetHWColor(0xFF6600CC);
		soffset = 0;
		doffset = 4;
	}
	else
	{
		scolor.SetHWColor(0xFF000000);
		tcolor.SetHWColor(0xFFFFA500);
		soffset = 4;
		doffset = -4;
	}

	dcolor = tcolor - scolor;
	timer = 0.0f;
}

// This method is called to notify the control
// that the mouse cursor has entered or left it's area
void MenuItem::MouseOver(bool bOver)
{
	if(bOver)
	{
		gui->SetFocus(id);
	}
}

// This method is called to notify the control
// that the left mouse button state has changed.
// If it returns true - the caller will receive
// the control's ID
bool MenuItem::MouseLButton(bool bDown)
{
	if(!bDown)
	{
		offset =4;
		return true;
	}
	else 
	{
		hge->Effect_Play(menuEffect);
		offset = 0;
		return false;
	}
}

// This method is called to notify the
// control that a key has been clicked.
// If it returns true - the caller will
// receive the control's ID
bool MenuItem::KeyClick(int key, int chr)
{
	if(key == HGEK_ENTER)
	{
		MouseLButton(true);
		return MouseLButton(false);
	}

	return false;
}

//EOF MenuItem.cpp