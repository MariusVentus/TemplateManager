#pragma once
#include <string>
#include <vector>
#include "RandHandler.h"
#include "SettingsHandler.h"
#include "TimeClock.h"

class TemplateManager {
public:
	TemplateManager(const SettingsHandler& inSet, const TimeClock& inTime);
private:
	void RefreshTemplates(void);
	void RefreshTemplates(const std::string& inFileLoc);
public:
	unsigned GetTemplateXID(unsigned inX) const { return m_Templates[inX].GetID(); }
	std::string GetTemplateXTitle(unsigned inX) const { return m_Templates[inX].GetTitle(); }
	std::string GetTemplateXContent(unsigned inX);
	std::string GetTemplateXContentRaw(unsigned inX);
	void OverwriteTemplateID(unsigned tempNum, unsigned inID);
	void OverwriteTemplateContent(unsigned tempNum, const std::string inContent);
	unsigned GetTemplateCount(void) const { return m_Templates.size(); }
	std::string GetTemplateFileLoc(void) const { return m_TemplateFile; }
	void AddTemplate(const unsigned inID, const std::string& inTitle, const std::string& inContent);
	bool FindTemplate(const std::string& inTitle) const;
	unsigned FindTemplateIterator(const std::string& inTitle) const;
	bool RemoveTemplate(const std::string& inTitle);
	void SaveTemplates(void) const;
	//unsigned GetLastRemoved(void) const { return m_LastRemoved; }

	void ClearAllTemplates(void);
	void ResetToDefaultTemplates(void);

	//File Shennanigans
	bool FileExists(const std::string& inFilename);
	std::string GetDefaultFilesDir(void) const { return m_TemplateDefaultFilesDir; }

	bool SwapUp(unsigned iterator);
	bool SwapDown(unsigned iterator);

private: 
	const std::string m_TemplateFile = "Templates\\Templates.txt";
	const std::string m_TemplateDefaultFile = "Templates\\DefaultFiles\\Default Templates.txt";
	const std::string m_TemplateDefaultFilesDir = "Templates\\DefaultFiles";

	const SettingsHandler& m_Settings;
	const TimeClock& m_Timer;
	TimeClock m_LocalClock;
	RandHandler m_Rand;

	class Templates {
	public:
		Templates(unsigned id, const std::string& title, const std::string& content);
		Templates(const Templates& other);
		unsigned GetID(void) const { return m_ID; }
		std::string GetTitle(void) const { return m_Title; }
		std::string GetContent(void) const { return m_Content; }

		void OverwriteID(unsigned ID) { m_ID = ID; }
		void OverwriteContent(const std::string content) { m_Content = content; }

	private:
		unsigned m_ID; //Repurposing as a TypeID rather than uniue identifier. Not lazy, smrt. 
		std::string m_Title;
		std::string m_Content;
	};

	std::vector<Templates> m_Templates;
	//unsigned m_LastRemoved = -1;

};