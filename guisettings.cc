#include "guisettings.h"

#include "main.h"
extern CMain *Main;
#include "config.h"
extern CConfig *config;

GuiSettings::GuiSettings() : CWindow()
{
	
}


GuiSettings::~GuiSettings()
{
	clearGuiElements();
}

void GuiSettings::Init(){

	std::cout << "GuiSettings::Init()" << std::endl;

	panelInfo = new CStackPanel();
	panelInfo->setX(310);
	panelInfo->setY(10);
	panelInfo->setWidth(160);
	panelInfo->setHeight(230);

	listcombo = new CListboxCombo();
	listcombo->setX(10);
	listcombo->setY(10);
	listcombo->setWidth(290);
	listcombo->setHeight(252);

	CCombobox *combobox;
	CLabel *la1;

	for(int count=0; count<config->GetCount(); count++)
	{
	
		combobox = new CCombobox();
	
	// 	combobox->setX(300);
	// 	combobox->setWidth(50);
		combobox->getListbox()->setX(315);
		combobox->getListbox()->setY(15);
		combobox->getListbox()->setWidth(150);
		combobox->getListbox()->setHeight(150);
		combobox->setText(config->GetSettingsNames(count));
	
	
		for(int choice=0; choice < (config->GetSettingsCount(count)); choice++)
		{
			la1 = new CLabelDesigned();
// 			std::cout << "config: " << config->GetSettingsChoices(count, choice) << std::endl;
			la1->setText(config->GetSettingsChoices(count, choice));
			combobox->addGuiElement( (CGuiElement*)la1);
			
			
		}
		combobox->updateText();
		listcombo->addGuiElement( (CGuiElement*)combobox);
	}

	addGuiElement(listcombo);
	addGuiElement(panelInfo);

	UpdateSettings();
}

void GuiSettings::UpdateSettings(){

	for(int count=0; count<config->GetCount(); count++)
	{
		short choice = config->GetSettings(count);
		CCombobox *tmpCombobox = (CCombobox*) listcombo->GetItem(count + 1);
		tmpCombobox->SetSelectedElement(choice + 1);
		tmpCombobox->updateText();
	}

}

void GuiSettings::KeyClick(){
	if(listcombo->isComboboxOpen())
	{
		short setting = listcombo->getSelectedElement() - 1;
		CCombobox *tmpCombobox = (CCombobox*) listcombo->getSelectedItem();
		tmpCombobox->updateText();
		CListbox *tmpListbox = tmpCombobox->getListbox();
		short choice = tmpListbox->getSelectedElement() - 1;
		std::cout << "setting:" << setting << " / choice:" << choice << std::endl;
		config->SetSettings(setting, choice);
	}
	listcombo->KeyClick();
}

void GuiSettings::KeyCancel(){
//check for selected combobox
	if(listcombo->isComboboxOpen())
		listcombo->KeyCancel();
	else
	{
		#ifdef PSP
 		switch(config->GetCpuspeed()) 
 		{
 			case 0:   scePowerSetClockFrequency(222, 222, 111); break;
 			case 1:   scePowerSetClockFrequency(333, 333, 166); break;
 		}
		#endif
		Main->ChangeMode(EN_MODE_START);
	}
}

void GuiSettings::KeyUp(){
	listcombo->KeyUp();
}

void GuiSettings::KeyDown(){
	listcombo->KeyDown();
}

void GuiSettings::KeyLeft(){
	
}

void GuiSettings::KeyRight(){
	
}

void GuiSettings::KeyL(){
	listcombo->SetSelectedElement(1);
}

void GuiSettings::KeyR(){
	listcombo->SetSelectedElement(listcombo->getElementCount());
}


