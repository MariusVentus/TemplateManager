#include "TimeClock.h"
#include <ctime>

TimeClock::TimeClock(void)
{
	RefreshTime();
//	TestAll();
}

void TimeClock::RefreshTime(void)
{
	time_t result = time(NULL);
	char cstr[26] = "";
	ctime_s(cstr, sizeof cstr, &result);
	m_time = cstr;
	//Remove Double Whitespace
	while (m_time.find("  ") != std::string::npos) {
		m_time.erase(m_time.find("  "), 1);
	}
	//Remove newline
	if (m_time.find("\n") != std::string::npos) {
		m_time.erase(m_time.find("\n"), 1);
	}
}

std::string TimeClock::GetFullTime(void) const
{
	return m_time;
}

std::string TimeClock::GetDate(void) const
{
	auto str = m_time;

	//Remove Day
	str.erase(0, str.find(" ") + 1);

	//EraseTime
	str.erase(str.find(":") - 2, 2);
	str.erase(str.find(":"), 6);

	//Remove Double Whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}

	//Whitespace to -
	while (str.find(" ") != std::string::npos) {
		str.replace(str.find(" "), 1, "-");
	}
	return str;
}

std::string TimeClock::GetDateShiftX(int x) const
{
	if (x != 0) {
		int day = (int)DaytoInt();
		int month = (int)MonthtoInt();
		int year = (int)YeartoInt();
		//Positive
		if (x > 0) {
			if (x > (int)DaysinMonth(month, year) - day) {
				x = x - (DaysinMonth(month, year) - day);
				if (month != 11) { month++; }
				else { month = 0; year++; }
				while (x > (int)DaysinMonth(month, year)) {
					x = x - DaysinMonth(month, year);
					if (month != 11) { month++; }
					else { month = 0; year++; }
				}
				day = x;
				return NumDatetoString(day, month, year);
			}
			else {
				day += x;
				return NumDatetoString(day, month, year);
			}
		}
		//Negative
		else {
			x = -x;
			if (x >= day) {
				x = x - day;
				if (month != 0) { month--; }
				else { month = 11; year--; }
				while (x >= (int)DaysinMonth(month, year)) {
					x = x - DaysinMonth(month, year);
					if (month != 0) { month--; }
					else { month = 11; year--; }
				}
				day = DaysinMonth(month, year) - x;
				return NumDatetoString(day, month, year);
			}
			else {
				day = day - x;
				return NumDatetoString(day, month, year);
			}
		}

	}
	else {
		return GetDate();
	}
}

std::string TimeClock::GetDateNDay(void) const
{
	auto str = m_time;

	//EraseTime
	str.erase(str.find(":") - 2, 2);
	str.erase(str.find(":"), 6);

	//Remove Double Whitespace
	while (str.find("  ") != std::string::npos) {
		str.erase(str.find("  "), 1);
	}

	return str;
}

std::string TimeClock::GetDay(void) const
{
	auto str = m_time;
	//Remove all but Day
	str.erase(str.find(" "));

	return str;
}

std::string TimeClock::GetDayNum(void) const
{
	auto str = m_time;
	//Remove Day
	str.erase(0, str.find(" ") + 1);
	//Remove Month
	str.erase(0, str.find(" ") + 1);
	//Remove Everything after Day Number
	str.erase(str.find(" "));

	return str;
}

std::string TimeClock::GetMonth(void) const
{
	auto str = m_time;
	//Remove Day
	str.erase(0, str.find(" ") + 1);
	//Then everything after Month
	str.erase(str.find(" "));
	return str;
}

std::string TimeClock::GetYear(void) const
{
	//Erase all but Year
	auto str = m_time;
	str.erase(0, str.find_last_of(" ")+1);
	return str;
}

std::string TimeClock::GetTime(void) const
{
	std::string temp = m_time;
	temp.erase(0, temp.find(":") - 2);
	temp.erase(temp.find_last_of(":") + 3);
	return temp;
}

void TimeClock::SetTimeShiftedX(int xDaysShifted)
{
	std::string temp = WeekDayShiftX(xDaysShifted);
	std::string tempTime = GetTime();
	temp.append(" ");
	temp.append(GetDateShiftX(xDaysShifted));
	while (temp.find("-") != std::string::npos) {
		temp.replace(temp.find("-"), 1, " ");
	}
	m_time = temp;
	temp.clear();
	temp.append(" ");
	temp.append(tempTime);
	temp.append(" ");
	m_time.replace(m_time.find_last_of(" "), 1, temp);
}

void TimeClock::TestAll(void)
{
	//Getters
	auto str = GetFullTime();
	str = GetDate();
	str = GetDateNDay();
	str = GetDay();
	str = GetDayNum();
	str = GetMonth();
	str = GetYear();
	str = GetTime();
	//Shift Checks
	str = GetDateShiftX(0);
	str = GetDateShiftX(1);
	str = GetDateShiftX(69);
	str = GetDateShiftX(369); 
	str = GetDateShiftX(-1);
	str = GetDateShiftX(-6);
	str = GetDateShiftX(-69);
	str = GetDateShiftX(-369);
	str = WeekDayShiftX(20);
	str = WeekDayShiftX(10);
	str = WeekDayShiftX(1);
	str = WeekDayShiftX(0);
	str = WeekDayShiftX(-1);
	str = WeekDayShiftX(-10);
	str = WeekDayShiftX(-20);
	//Check Date Change
	SetTimeShiftedX(1);
	str = GetFullTime();
	str = GetDate();
	str = GetDateNDay();
	str = GetDay();
	str = GetDayNum();
	str = GetMonth();
	str = GetYear();
	//Converters
	unsigned num = DayofWeektoInt();
	num = DaytoInt();
	num = MonthtoInt();
	num = YeartoInt();
	num = DaysinMonth(0, 2020);
	num = DaysinMonth(3, 2020);
	num = DaysinMonth(1, 2000);
	num = DaysinMonth(1, 1900);
	num = DaysinMonth(1, 2020);

}

unsigned TimeClock::DaysinMonth(unsigned month, unsigned year) const
{
	if (month == 3 || month == 5 || month == 8 || month == 10) { 
		return 30; 
	}
	else if (month == 1)
	{
		bool leapyear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
		if (leapyear){
			return 29;
		}
		else {
			return 28;
		}
	}
	else { 
		return 31; 
	}

}

unsigned TimeClock::DayofWeektoInt(void) const
{
	if (GetDay() == "Sun") { return 0; }
	else if (GetDay() == "Mon") { return 1; }
	else if (GetDay() == "Tue") { return 2; }
	else if (GetDay() == "Wed") { return 3; }
	else if (GetDay() == "Thu") { return 4; }
	else if (GetDay() == "Fri") { return 5; }
	else if (GetDay() == "Sat") { return 6; }
	else {
		return -1;
	}

}

std::string TimeClock::WeekDayShiftX(int x) const
{
	int dayN = (int)DayofWeektoInt();
	//Positive
	if (x >= 0) {
		if (x > 6 - dayN) {
			x = x - (7 - dayN);
			while (x > 6) {
				x = x - 7;
			}
			dayN = x;
		}
		else {
			dayN += x;
		}
	}
	//Negative
	else {
		x = -x;
		if (x > dayN) {
			x = x - (dayN + 1);
			while (x > 6) {
				x = x - 7;
			}
			dayN = 6 - x;
		}
		else {
			dayN = dayN - x;
		}
	}

	if (dayN == 0) { return "Sun"; }
	else if (dayN == 1) { return "Mon"; }
	else if (dayN == 2) { return "Tue"; }
	else if (dayN == 3) { return "Wed"; }
	else if (dayN == 4) { return "Thu"; }
	else if (dayN == 5) { return "Fri"; }
	else if (dayN == 6) { return "Sat"; }
	else {
		return "";
	}
}

unsigned TimeClock::DaytoInt(void) const
{
	return std::stoul(GetDayNum());
}

unsigned TimeClock::MonthtoInt(void) const
{
	auto month = GetMonth();
	if (month == "Jan") { return 0; }
	else if (month == "Feb") { return 1; }
	else if (month == "Mar") { return 2; }
	else if (month == "Apr") { return 3; }
	else if (month == "May") { return 4; }
	else if (month == "Jun") { return 5; }
	else if (month == "Jul") { return 6; }
	else if (month == "Aug") { return 7; }
	else if (month == "Sep") { return 8; }
	else if (month == "Oct") { return 9; }
	else if (month == "Nov") { return 10; }
	else if (month == "Dec") { return 11; }
	else { return -1; }

}

unsigned TimeClock::YeartoInt(void) const
{
	return std::stoul(GetYear());
}

std::string TimeClock::NumDatetoString(unsigned day, unsigned month, unsigned year) const
{
	std::string date = "";
	if (month == 0) { date = "Jan"; }
	else if (month == 1) { date = "Feb"; }
	else if (month == 2) { date = "Mar"; }
	else if (month == 3) { date = "Apr"; }
	else if (month == 4) { date = "May"; }
	else if (month == 5) { date = "Jun"; }
	else if (month == 6) { date = "Jul"; }
	else if (month == 7) { date = "Aug"; }
	else if (month == 8) { date = "Sep"; }
	else if (month == 9) { date = "Oct"; }
	else if (month == 10) { date = "Nov"; }
	else if (month == 11) { date = "Dec"; }

	date.append("-");
	date.append(std::to_string(day));
	date.append("-");
	date.append(std::to_string(year));
	return date;
}
