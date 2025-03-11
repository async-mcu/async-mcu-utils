#pragma once
#include <Arduino.h>

/**
 * @class Timestamp
 * @brief A class for managing Unix timestamps (seconds since January 1, 1970).
 * 
 * This class provides functionality to set, manipulate, and convert timestamps,
 * as well as to check if a timestamp has expired.
 */
class Timestamp {
  private:
    unsigned long timestamp;  // Stores the Unix timestamp

    /**
     * @brief Checks if a year is a leap year.
     * 
     * @param year The year to check.
     * @return true if the year is a leap year, false otherwise.
     */
    bool isLeapYear(int year) {
      return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    /**
     * @brief Returns the number of days in a given month and year.
     * 
     * @param month The month (1–12).
     * @param year The year (to handle leap years).
     * @return The number of days in the month.
     */
    int daysInMonth(int month, int year) {
      if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
      } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
      } else {
        return 31;
      }
    }

    /**
     * @brief Converts a date and time to a Unix timestamp.
     * 
     * @param year The year (e.g., 2023).
     * @param month The month (1–12).
     * @param day The day of the month (1–31).
     * @param hour The hour (0–23).
     * @param minute The minute (0–59).
     * @param second The second (0–59).
     * @return The Unix timestamp (seconds since January 1, 1970).
     */
    unsigned long toUnixTime(int year, int month, int day, int hour, int minute, int second) {
      unsigned long days = 0;

      // Add days from 1970 to the given year
      for (int y = 1970; y < year; y++) {
        days += isLeapYear(y) ? 366 : 365;
      }

      // Add days from January to the given month
      for (int m = 1; m < month; m++) {
        days += daysInMonth(m, year);
      }

      // Add days in the given month
      days += day - 1;

      // Convert days to seconds and add time
      unsigned long seconds = days * 86400UL;
      seconds += hour * 3600UL;
      seconds += minute * 60UL;
      seconds += second;

      return seconds;
    }

    /**
     * @brief Converts a Unix timestamp to a date and time.
     * 
     * @param timestamp The Unix timestamp to convert.
     * @param year Output parameter for the year.
     * @param month Output parameter for the month.
     * @param day Output parameter for the day.
     * @param hour Output parameter for the hour.
     * @param minute Output parameter for the minute.
     * @param second Output parameter for the second.
     */
    void fromUnixTime(unsigned long timestamp, int &year, int &month, int &day, int &hour, int &minute, int &second) {
      // Calculate the year
      year = 1970;
      while (true) {
        int daysInYear = isLeapYear(year) ? 366 : 365;
        unsigned long secondsInYear = daysInYear * 86400UL;
        if (timestamp >= secondsInYear) {
          timestamp -= secondsInYear;
          year++;
        } else {
          break;
        }
      }

      // Calculate the month
      month = 1;
      while (true) {
        int daysInMon = daysInMonth(month, year);
        unsigned long secondsInMon = daysInMon * 86400UL;
        if (timestamp >= secondsInMon) {
          timestamp -= secondsInMon;
          month++;
        } else {
          break;
        }
      }

      // Calculate the day
      day = (timestamp / 86400UL) + 1;
      timestamp %= 86400UL;

      // Calculate the time
      hour = timestamp / 3600UL;
      timestamp %= 3600UL;
      minute = timestamp / 60UL;
      second = timestamp % 60UL;
    }

  public:
    /**
     * @brief Constructor. Initializes the timestamp to 0.
     */
    Timestamp() : timestamp(0) {}

    /**
     * @brief Sets the timestamp using a specific date and time.
     * 
     * @param year The year (e.g., 2023).
     * @param month The month (1–12).
     * @param day The day of the month (1–31).
     * @param hour The hour (0–23).
     * @param minute The minute (0–59).
     * @param second The second (0–59).
     */
    void setTime(int year, int month, int day, int hour, int minute, int second) {
      timestamp = toUnixTime(year, month, day, hour, minute, second);
    }

    /**
     * @brief Returns the current Unix timestamp.
     * 
     * @return The Unix timestamp as an unsigned long.
     */
    unsigned long getTimestamp() {
      return timestamp;
    }

    /**
     * @brief Adds a specified number of seconds to the timestamp.
     * 
     * @param seconds The number of seconds to add.
     */
    void addSeconds(long seconds) {
      timestamp += seconds;
    }

    /**
     * @brief Adds a specified number of minutes to the timestamp.
     * 
     * @param minutes The number of minutes to add.
     */
    void addMinutes(long minutes) {
      timestamp += minutes * 60;
    }

    /**
     * @brief Adds a specified number of hours to the timestamp.
     * 
     * @param hours The number of hours to add.
     */
    void addHours(long hours) {
      timestamp += hours * 3600;
    }

    /**
     * @brief Adds a specified number of days to the timestamp.
     * 
     * @param days The number of days to add.
     */
    void addDays(long days) {
      timestamp += days * 86400;
    }

    /**
     * @brief Converts the timestamp to a human-readable string.
     * 
     * @return A String in the format "YYYY-MM-DD HH:MM:SS".
     */
    String toString() {
      int year, month, day, hour, minute, second;
      fromUnixTime(timestamp, year, month, day, hour, minute, second);

      char buffer[20];
      sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
      return String(buffer);
    }

    /**
     * @brief Checks if the timestamp has expired relative to a given current timestamp.
     * 
     * @param currentTimestamp The current Unix timestamp to compare against.
     * @return true if the timestamp has expired, false otherwise.
     */
    bool isExpired(unsigned long currentTimestamp) {
      return timestamp < currentTimestamp;
    }
};