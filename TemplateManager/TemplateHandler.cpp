#include "TemplateHandler.h"
#include <fstream>

TemplateManager::TemplateManager(const SettingsHandler& inSet, const TimeClock& inTime)
	:
	m_Settings(inSet),
	m_Timer(inTime)
{
	RefreshTemplates();
}

void TemplateManager::RefreshTemplates(void)
{
	RefreshTemplates(m_TemplateFile);
}

void TemplateManager::RefreshTemplates(const std::string& inFileLoc)
{
	m_Templates.clear();

	std::ifstream in(inFileLoc);
	if (in) {
		std::string token = "";


		do {
			unsigned tempID = 0;
			std::string tempTitle = "";
			std::string tempContent = "";
			//Get Template
			do {
				if (token.find("[Template]") == std::string::npos) {
					token.clear();
					std::getline(in, token);
					//Remove whitespace
					while (token.find(" ") != std::string::npos) {
						token.erase(token.find(" "), 1);
					}
					//Remove "comments"
					if (token.find(";;") != std::string::npos) {
						token.erase(token.find(";;"));
					}
				}
				else {
					break;
				}
			} while (token.empty() && !in.eof());
			//Get ID
			if (token.find("[Template]") != std::string::npos) {
				token.erase(0, 10);
				tempID = std::stoul(token);
				//Get Title
				do {
					if (token.find("[Template]") == std::string::npos) {
						token.clear();
						std::getline(in, token);
						//Remove "comments"
						if (token.find(";;") != std::string::npos) {
							token.erase(token.find(";;"));
						}
						//Remove Double whitespace
						while (token.find("  ") != std::string::npos) {
							token.erase(token.find("  "), 1);
						}
						//Remove Leading Whitespace
						if (token.find(" ") == 0) {
							token.erase(token.find(" "), 1);
						}
						//Remove Trailing Whitespace
						if (token.find_last_of(" ") == token.size() - 1 && !token.empty()) {
							token.pop_back();
						}
					}
					else {
						break;
					}
				} while (token.empty() && !in.eof());
				if (token.find("[Template]") == std::string::npos) {
					tempTitle = token;
				}
				while (token.find("[Template]") == std::string::npos && !in.eof()) {
					token.clear();
					std::getline(in, token);
					if (token.find("[Template]") == std::string::npos) {
						tempContent.append(token);
						tempContent.append("\n");
					}
					else {
						if (!tempContent.empty()) {
							if (tempContent.back() == '\n') {
								tempContent.pop_back();
							}
						}
						break;
					}
				}
				if (!tempTitle.empty() && !tempContent.empty()) {
					if (in.eof() && tempContent.back() == '\n') {
						tempContent.pop_back();
					}
					m_Templates.push_back(Templates(tempID, tempTitle, tempContent));
				}
			}

		} while (!in.eof());
	}
}

std::string TemplateManager::GetTemplateXContent(unsigned inX)
{
	if (m_Templates[inX].GetID() == 0) {
		std::string localContent = m_Templates[inX].GetContent();
		std::string flags[15] = { "[Date]","[RandomPercentage]","[Name]","[Email]","[Misc1]","[Misc2]","[Misc3]",
			"[Time]","[FullTimeNDate]", "[Today]", "[Tomorrow]", "[Yesterday]","[TodayWeekday]", "[TomorrowWeekday]", "[YesterdayWeekday]" };
		//Content Flags
		while (localContent.find(flags[2]) != std::string::npos) {
			localContent.replace(localContent.find(flags[2]), flags[2].size(), m_Settings.GetName());
		}
		while (localContent.find(flags[3]) != std::string::npos) {
			localContent.replace(localContent.find(flags[3]), flags[3].size(), m_Settings.GetEmail());
		}
		while (localContent.find(flags[4]) != std::string::npos) {
			localContent.replace(localContent.find(flags[4]), flags[4].size(), m_Settings.GetMisc1());
		}
		while (localContent.find(flags[5]) != std::string::npos) {
			localContent.replace(localContent.find(flags[5]), flags[5].size(), m_Settings.GetMisc2());
		}
		while (localContent.find(flags[6]) != std::string::npos) {
			localContent.replace(localContent.find(flags[6]), flags[6].size(), m_Settings.GetMisc3());
		}
		//Date
		while (localContent.find(flags[0]) != std::string::npos) {
			localContent.replace(localContent.find(flags[0]), flags[0].size(), m_Timer.GetDate());
		}
		//Rand
		while (localContent.find(flags[1]) != std::string::npos) {
			localContent.replace(localContent.find(flags[1]), flags[1].size(), std::to_string(m_Rand.Generate(0, 100)));
		}
		//Time
		while (localContent.find(flags[7]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			localContent.replace(localContent.find(flags[7]), flags[7].size(), m_LocalClock.GetTime());
		}
		//FullTimeNDate
		while (localContent.find(flags[8]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			localContent.replace(localContent.find(flags[8]), flags[8].size(), m_LocalClock.GetFullTime());
		}
		//Today
		while (localContent.find(flags[9]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			localContent.replace(localContent.find(flags[9]), flags[9].size(), m_LocalClock.GetDate());
		}
		//Tomorrow
		while (localContent.find(flags[10]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			m_LocalClock.SetTimeShiftedX(1);
			localContent.replace(localContent.find(flags[10]), flags[10].size(), m_LocalClock.GetDate());
			m_LocalClock.RefreshTime();
		}
		//Yesterday
		while (localContent.find(flags[11]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			m_LocalClock.SetTimeShiftedX(-1);
			localContent.replace(localContent.find(flags[11]), flags[11].size(), m_LocalClock.GetDate());
			m_LocalClock.RefreshTime();
		}
		//Today's Weekday
		while (localContent.find(flags[12]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			localContent.replace(localContent.find(flags[12]), flags[12].size(), m_LocalClock.GetDay());
		}
		//Tomorrow's Weekday
		while (localContent.find(flags[13]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			m_LocalClock.SetTimeShiftedX(1);
			localContent.replace(localContent.find(flags[13]), flags[13].size(), m_LocalClock.GetDay());
			m_LocalClock.RefreshTime();
		}
		//Yesterday's Weekeday
		while (localContent.find(flags[14]) != std::string::npos) {
			m_LocalClock.RefreshTime();
			m_LocalClock.SetTimeShiftedX(-1);
			localContent.replace(localContent.find(flags[14]), flags[14].size(), m_LocalClock.GetDay());
			m_LocalClock.RefreshTime();
		}
		return localContent;
	}
	else {
		return m_Templates[inX].GetContent();
	}
}

std::string TemplateManager::GetTemplateXContentRaw(unsigned inX)
{
	return m_Templates[inX].GetContent();
}

void TemplateManager::OverwriteTemplateID(unsigned tempNum, unsigned inID)
{
	m_Templates[tempNum].OverwriteID(inID);
}

void TemplateManager::OverwriteTemplateContent(unsigned tempNum, const std::string inContent)
{
	auto outString = inContent;
	while (outString.find("\r") != std::string::npos) {
		outString.erase(outString.find("\r"), 1);
	}
	m_Templates[tempNum].OverwriteContent(outString);
}

void TemplateManager::AddTemplate(const unsigned inID, const std::string& inTitle, const std::string& inContent)
{
	auto outString = inContent;
	while (outString.find("\r") != std::string::npos) {
		outString.erase(outString.find("\r"), 1);
	}
	m_Templates.push_back(Templates(inID, inTitle, outString));
}

bool TemplateManager::FindTemplate(const std::string& inTitle) const
{
	bool foundTemplate = false;
	for (unsigned i = 0; i < m_Templates.size(); i++) {
		if (m_Templates[i].GetTitle() == inTitle) {
			foundTemplate = true;
		}
	}
	return foundTemplate;
}

unsigned TemplateManager::FindTemplateIterator(const std::string & inTitle) const
{
	unsigned tempIter = 0;
	while (tempIter < m_Templates.size()){
		if (m_Templates[tempIter].GetTitle() == inTitle) {
			break;
		}
		tempIter++;
	}
	return tempIter;
}

bool TemplateManager::RemoveTemplate(const std::string& inTitle)
{

	//m_Templates.erase(m_Templates.begin() + i); uses implicit copy assigmment operators, which const members delete. 
	//Modified FindTemplate required to generate a list of items to keep.
	bool foundTemplate = false;
	std::vector<Templates> TempList;
	for (unsigned i = 0; i < m_Templates.size(); i++) {
		if (m_Templates[i].GetTitle() != inTitle) {
			TempList.push_back(m_Templates[i]);
		}
		else {
			//m_LastRemoved = i;
			foundTemplate = true;
		}
	}
	if (foundTemplate) {
		m_Templates.clear();
		for (unsigned i = 0; i < TempList.size(); i++) {
			m_Templates.push_back(TempList[i]);
		}
	}

	return foundTemplate;

}

void TemplateManager::SaveTemplates(void) const
{
	std::ofstream out(m_TemplateFile, std::ofstream::trunc);
	std::string outString = ";;[Template]TypeID\n;;Title\n;;Content";
	for (unsigned i = 0; i < m_Templates.size(); i++) {
		outString.append("\n[Template]");
		outString.append(std::to_string(m_Templates[i].GetID()));
		outString.append("\n");
		outString.append(m_Templates[i].GetTitle());
		outString.append("\n");
		outString.append(m_Templates[i].GetContent());
	}

	while (outString.find("\r") != std::string::npos) {
		outString.erase(outString.find("\r"), 1);
	}

	out << outString;

}

void TemplateManager::ClearAllTemplates(void)
{
	m_Templates.clear();
}

void TemplateManager::ResetToDefaultTemplates(void)
{
	RefreshTemplates(m_TemplateDefaultFile);
	SaveTemplates();
}

bool TemplateManager::FileExists(const std::string & inFilename)
{
	std::ifstream in(inFilename);
	if (in) {
		return true;
	}
	else {
		return false;
	}
}

bool TemplateManager::SwapUp(unsigned iterator)
{
	if (iterator > 0) {
		Templates Temp = m_Templates[iterator - 1];
		m_Templates[iterator - 1] = m_Templates[iterator];
		m_Templates[iterator] = Temp;
		return true;
	}
	else {
		return false;
	}
}

bool TemplateManager::SwapDown(unsigned iterator)
{
	if (iterator < m_Templates.size() - 1) {
		Templates Temp = m_Templates[iterator + 1];
		m_Templates[iterator + 1] = m_Templates[iterator];
		m_Templates[iterator] = Temp;
		return true;
	}
	else {
		return false;
	}
}

TemplateManager::Templates::Templates(unsigned id, const std::string& title, const std::string& content)
	:
	m_ID(id),
	m_Title(title),
	m_Content(content)
{
}

TemplateManager::Templates::Templates(const Templates & other)
	:
	m_ID(other.m_ID),
	m_Title(other.m_Title),
	m_Content(other.m_Content)
{
}
