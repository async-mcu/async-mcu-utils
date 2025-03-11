#pragma once
#include <Arduino.h>

/**
 * @class Duration
 * @brief Represents a time duration with conversions between units (microseconds to hours).
 * 
 * The class is designed to work with time intervals, allowing you to create, 
 * store and manipulate durations in various units of measurement (microseconds, milliseconds, seconds, etc.). 
 * It is used in asynchronous tasks to manage execution time.
 */
namespace async { 
    class Duration {
        private:
            long value_ms; ///< Internal storage in milliseconds
            explicit Duration(long ms) : value_ms(ms) {}

        public:
            ///@name Time Unit Constants
            ///@{
            static const int MICRO   = 0;   ///< Microseconds (μs)
            static const int MILLIS  = 1;   ///< Milliseconds (ms)
            static const int SECONDS = 2;   ///< Seconds (s)
            static const int MINUTES = 3;   ///< Minutes (min)
            static const int HOURS   = 4;   ///< Hours (hr)
            ///@}

            /**
             * @brief Calculate the difference between two Duration objects.
             * @param other The Duration to subtract from this one.
             * @return New Duration object representing the difference (this - other).
             */
            Duration diff(const Duration& other) const {
                return Duration(value_ms - other.value_ms);
            }
        
            /**
             * @brief Add another Duration to this one.
             * @param other The Duration to add.
             * @return New Duration object representing the sum (this + other).
             */
            Duration add(const Duration& other) const {
                return Duration(value_ms + other.value_ms);
            }
        
            /**
             * @brief Subtract another Duration from this one.
             * @param other The Duration to subtract.
             * @return New Duration object representing the result (this - other).
             */
            Duration subtract(const Duration& other) const {
                return Duration(value_ms - other.value_ms);
            }

            /**
             * @brief Check if this Duration is after another Duration.
             * @param other The Duration to compare with.
             * @return True if this Duration is after `other`, false otherwise.
             */
            bool after(const Duration& other) const {
                return value_ms > other.value_ms;
            }

            /**
             * @brief Check if this Duration is before another Duration.
             * @param other The Duration to compare with.
             * @return True if this Duration is before `other`, false otherwise.
             */
            bool before(const Duration& other) const {
                return value_ms < other.value_ms;
            }

            /**
             * @brief Get the duration in the specified time unit.
             * @param type Time unit (default: MILLIS). One of: MICRO, MILLIS, SECONDS, etc.
             * @return Duration value converted to the requested unit.
             * 
             * @note For MICRO, returns value_ms * 1000 (no fractional microseconds).
             * @note For SECONDS/MINUTES/HOURS, returns integer division (e.g., 1500ms → 1 second).
             */
            long get(int type = MILLIS) const {
                switch(type) {
                    case MICRO:   return value_ms * 1000L;
                    case MILLIS:  return value_ms;
                    case SECONDS: return value_ms / 1000L;
                    case MINUTES: return value_ms / (1000L * 60);
                    case HOURS:   return value_ms / (1000L * 3600);
                    default:      return value_ms;
                }
            };

            ///@name Factory Methods
            ///@{
            
            /**
             * @brief Create a Duration from milliseconds.
             * @param value Duration in milliseconds.
             * @return Duration object.
             */
            static Duration ms(long value) {
                return Duration(value);
            };

            /**
             * @brief Get the current time since Arduino startup.
             * @return Duration object representing current time.
             * 
             * @note Wraps around every ~49.7 days (Arduino millis() behavior).
             */
            static Duration now() {
                return Duration(millis());
            };
    };
}
