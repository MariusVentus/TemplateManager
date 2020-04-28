#pragma once
#include <string>

class TimeClock {
public:
	TimeClock(void);
	void RefreshTime(void);
	std::string GetFullTime(void) const;
	std::string GetDate(void) const;
	std::string GetDateShiftX(int x)const;
	std::string GetDateNDay(void) const;
	std::string GetDay(void) const;
	std::string GetDayNum(void) const;
	std::string GetMonth(void) const;
	std::string GetYear(void) const;
	std::string GetTime(void) const;
	void SetTime(const std::string& inTime) { m_time = inTime; }
	void SetTimeShiftedX(int xDaysShifted);
	void TestAll(void);

	unsigned DayofWeektoInt(void) const;
	std::string WeekDayShiftX(int x) const;
private:
	std::string m_time;
	unsigned DaysinMonth(unsigned month, unsigned year) const; //Months from 0!
	unsigned DaytoInt(void) const;
	unsigned MonthtoInt(void) const; //Counts from 0!
	unsigned YeartoInt(void) const;
	std::string NumDatetoString(unsigned day, unsigned month, unsigned year) const;

};
