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
	m_Templates.clear();

	std::ifstream in(m_TemplateFile);
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
				if (token.find("[Template]") == std::string::npos) {
					tempTitle = token;
				}
				while (token.find("[Template]") == std::string::npos && !in.eof()) {
					token.clear();
					std::getline(in, token);
					if (token.find("[Template]") == std::string::npos) {
						tempContent.append(token);
						if (!token.empty()) {
							tempContent.append("\r\n");
						}
					}
					else {
						break;
					}
				}
				m_Templates.push_back(Templates(tempID, tempTitle, tempContent));
			}

		} while (!in.eof());
	}

}

std::string TemplateManager::GetTemplateXContent(unsigned inX)
{
	std::string localContent = m_Templates[inX].m_Content;
	std::string flags[8] = { "[Date]","[RandomPercentage]","[Name]","[Email]","[Misc1]","[Misc2]","[Misc3]","[Time]" };
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
	//Dates and Randoms
	while (localContent.find(flags[0]) != std::string::npos) {
		localContent.replace(localContent.find(flags[0]), flags[0].size(), m_Timer.GetDate());
	}
	while (localContent.find(flags[1]) != std::string::npos) {
		localContent.replace(localContent.find(flags[1]), flags[1].size(), std::to_string(m_Rand.Generate(0, 100)));
	}
	while (localContent.find(flags[7]) != std::string::npos) {
		m_LocalClock.RefreshTime();
		localContent.replace(localContent.find(flags[7]), flags[7].size(), m_LocalClock.GetTime());
	}


	return localContent;
}

TemplateManager::Templates::Templates(unsigned id, const std::string& title, const std::string& content)
	:
	m_ID(id),
	m_Title(title),
	m_Content(content)
{
}
