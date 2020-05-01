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

void TemplateManager::AddTemplate(const unsigned inID, const std::string& inTitle, const std::string& inContent)
{
	auto outString = inContent;
	while (outString.find("\r") != std::string::npos) {
		outString.erase(outString.find("\r"), 1);
	}
	m_Templates.push_back(Templates(inID, inTitle, outString));
}

bool TemplateManager::RemoveTemplate(const std::string& inTitle)
{

	//m_Templates.erase(m_Templates.begin() + i); uses implicit copy assigmment operators, which const members delete. 
	bool foundTemplate = false;
	std::vector<Templates> TempList;
	for (unsigned i = 0; i < m_Templates.size(); i++) {
		if (m_Templates[i].m_Title != inTitle) {
			TempList.push_back(m_Templates[i]);
		}
		else {
			m_LastRemoved = i;
			foundTemplate = true;
		}
	}
	m_Templates.clear();
	for (unsigned i = 0; i < TempList.size(); i++) {
		m_Templates.push_back(TempList[i]);
	}

	return foundTemplate;

}

void TemplateManager::SaveTemplates(void) const
{
	std::ofstream out(m_TemplateFile, std::ofstream::trunc);
	std::string outString = ";;[Template]ID\n;;Title\n;;Content";
	for (unsigned i = 0; i < m_Templates.size(); i++) {
		outString.append("\n[Template]");
		outString.append(std::to_string(m_Templates[i].m_ID));
		outString.append("\n");
		outString.append(m_Templates[i].m_Title);
		outString.append("\n");
		outString.append(m_Templates[i].m_Content);
	}

	while (outString.find("\r") != std::string::npos) {
		outString.erase(outString.find("\r"), 1);
	}

	out << outString;

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
