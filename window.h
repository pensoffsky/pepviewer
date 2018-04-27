#pragma once
#include "container.h"

#include "listbox.h"
#include "label.h"
#include "combobox.h"
#include "guielement.h"
#include "stackpanel.h"
#include "listboxcombo.h"
#include "listboxfilebrowser.h"
#include "listboxcombobookmarks.h"
#include "labeldesigned.h"


class CWindow : public CContainer
{
	protected:
	
	public:
		CWindow();	
		~CWindow();

		virtual void Init(){};
};


