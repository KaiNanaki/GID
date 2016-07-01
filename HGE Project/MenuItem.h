//MenuItem Class header
#ifndef MENU_ITEM
#define MENU_ITEM

//--------------------------------------------------------------------------------------------
//
//  Name:	MenuItem.h
//
//  Desc:	Defines the menu item class used to build individual menus. Adapted slightly from
//			http://hge.relishgames.com website, Tutorial 06 - Creating menus 
//
//--------------------------------------------------------------------------------------------

#include "hge.h"
#include "hgegui.h"
#include "hgefont.h"
#include "hgecolor.h"


class MenuItem : public hgeGUIObject
{
public:
	MenuItem(int id, hgeFont *fnt, HEFFECT menuEffect, float x, float y, float delay, char *title) ;
	//~MenuItem() ;

	virtual void	Render() ;
	virtual void	Update(float frameTime) ;

	virtual void	Enter() ;
	virtual void	Leave() ;
	virtual bool	IsDone() ;
	virtual void	Focus(bool bFocused) ;
	virtual void	MouseOver(bool bOver) ;

	virtual bool	MouseLButton(bool bDown) ;
	virtual bool	KeyClick(int key, int chr) ;

private:
	hgeFont		*fnt ;
	HEFFECT		menuEffect ;
	float		delay ;
	char		*title ;

	hgeColor	scolor, dcolor, scolor2, dcolor2, sshadow, dshadow ;
	hgeColor	color, shadow ;
	float		soffset, doffset, offset ;
	float		timer, timer2 ;
}; 
#endif 
//EOF MenuItem.h
