#include "SettingsHandler.h"
#include <fstream>

SettingsHandler::SettingsHandler(void)
{
	ResetSettings();
}

void SettingsHandler::ResetSettings(void)
{
	std::ifstream in(m_SettingsFile);
	if (in) {
		std::string token = "";
		do {
			do {
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
			} while (token.empty() && !in.eof());
			if (token == "[Name]") {
				token.clear();
				std::getline(in, token);
				SetName(token);
			}
			else if (token == "[Email]") {
				token.clear();
				std::getline(in, token);
				SetEmail(token);
			}
			else if (token == "[Misc1]") {
				token.clear();
				std::getline(in, token);
				SetMisc1(token);
			}
			else if (token == "[Misc2]") {
				token.clear();
				std::getline(in, token);
				SetMisc2(token);
			}
			else if (token == "[Misc3]") {
				token.clear();
				std::getline(in, token);
				SetMisc3(token);
			}

		} while (!in.eof());
	}
	else {
		std::ofstream out(m_SettingsFile);
	}
}

void SettingsHandler::SaveSettingsToFile()
{
	std::ofstream out(m_SettingsFile, std::ofstream::trunc);

	if (!m_Name.empty()) {
		out << "[Name]\n";
		out << m_Name << "\n";
	}
	if (!m_Email.empty()) {
		out << "[Email]\n";
		out << m_Email << "\n";
	}
	if (!m_Misc1.empty()) {
		out << "[Misc1]\n";
		out << m_Misc1 << "\n";
	}
	if (!m_Misc2.empty()) {
		out << "[Misc2]\n";
		out << m_Misc2 << "\n";
	}
	if (!m_Misc3.empty()) {
		out << "[Misc3]\n";
		out << m_Misc3 << "\n";
	}
}

bool SettingsHandler::StoB(const std::string & inStr) const
{
	auto str = inStr;
	//Lower all Letters
	for (unsigned i = 0; i < str.size(); i++) {
		if (str[i] >= 65 && str[i] <= 90) {
			str[i] = str[i] + 32; //Make into lowercase
		}
	}
	//Bool
	if (str == "true") {
		return true;
	}
	else if (str == "false") {
		return false;
	}
	else {
		return false;
	}
}
