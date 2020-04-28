#pragma once
#include <string>

class SettingsHandler {
public: 
	SettingsHandler(void);
	void ResetSettings(void);
	void SaveSettingsToFile();
	std::string GetName(void) const { return m_Name; }
	std::string GetEmail(void) const { return m_Email; }
	std::string GetMisc1(void) const { return m_Misc1; }
	std::string GetMisc2(void) const { return m_Misc2; }
	std::string GetMisc3(void) const { return m_Misc3; }

	void SetName(const std::string& inStr) { m_Name = inStr; }
	void SetEmail(const std::string& inStr) { m_Email = inStr; }
	void SetMisc1(const std::string& inStr) { m_Misc1 = inStr; }
	void SetMisc2(const std::string& inStr) { m_Misc2 = inStr; }
	void SetMisc3(const std::string& inStr) { m_Misc3 = inStr; }


private:
	const std::string m_SettingsFile = "Settings\\Settings.txt";
	std::string m_Name = "";
	std::string m_Email = "";
	std::string m_Misc1 = "";
	std::string m_Misc2 = "";
	std::string m_Misc3 = "";

	bool StoB(const std::string& inStr) const;
};