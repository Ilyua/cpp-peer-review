void CheckRange(int value, int min_value, int max_value, const string& error_msg) {
    if (value < min_value || value > max_value) {
        throw domain_error(error_msg);
    }
}

bool IsLeapYear(int year) {
    return (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckRange(dt.year, 1, 9999, "year is out of range"s);
    CheckRange(dt.month, 1, 12, "month is out of range"s);

    const bool is_leap_year = IsLeapYear(dt.year);
    const array<int, 12> month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    CheckRange(dt.day, 1, month_lengths[dt.month - 1], "day is out of range"s);

    CheckRange(dt.hour, 0, 23, "hour is out of range"s);
    CheckRange(dt.minute, 0, 59, "minute is out of range"s);
    CheckRange(dt.second, 0, 59, "second is out of range"s);
}
