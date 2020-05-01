#pragma once
#include <string>
#include <vector>
#include "RandHandler.h"
#include "SettingsHandler.h"
#include "TimeClock.h"

class TemplateManager {
public:
	TemplateManager(const SettingsHandler& inSet, const TimeClock& inTime);
	void RefreshTemplates(void);
	unsigned GetTemplateXID(unsigned inX) const { return m_Templates[inX].m_ID; }
	std::string GetTemplateXTitle(unsigned inX) const { return m_Templates[inX].m_Title; }
	std::string GetTemplateXContent(unsigned inX);
	unsigned GetTemplateCount(void) const { return m_Templates.size(); }
	std::string GetTemplateFileLoc(void) const { return m_TemplateFile; }
	void AddTemplate(const unsigned inID, const std::string& inTitle, const std::string& inContent);
	bool RemoveTemplate(const std::string& inTitle);
	void SaveTemplates(void) const;
	unsigned GetLastRemoved(void) const { return m_LastRemoved; }


private: 
	const std::string m_TemplateFile = "Templates\\Templates.txt";
	const SettingsHandler& m_Settings;
	const TimeClock& m_Timer;
	TimeClock m_LocalClock;
	RandHandler m_Rand;

	struct Templates {
		Templates(unsigned id, const std::string& title, const std::string& content);
		Templates(const Templates& other);


		const unsigned m_ID;
		const std::string m_Title;
		const std::string m_Content;
	};

	std::vector<Templates> m_Templates;
	unsigned m_LastRemoved = -1;

};