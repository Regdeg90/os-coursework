/*
 * CMOS Real-time Clock
 * SKELETON IMPLEMENTATION -- TO BE FILLED IN FOR TASK (3)
 */

/*
 * STUDENT NUMBER: s
 */
#include <infos/drivers/timer/rtc.h>
#include <arch/x86/pio.h>

using namespace infos::drivers;
using namespace infos::drivers::timer;

class CMOSRTC : public RTC {
public:
	static const DeviceClass CMOSRTCDeviceClass;

	const DeviceClass& device_class() const override
	{
		return CMOSRTCDeviceClass;
	}

	/**
	 * Interrogates the RTC to read the current date & time.
	 * @param tp Populates the tp structure with the current data & time, as
	 * given by the CMOS RTC device.
	 */

	//Gets the data from x offset
	int get_data(int x) {
		infos::arch::x86::__outb(0x70, x);
		return infos::arch::x86::__inb(0x71);
	}

	//Checks if there is an update happening
	int get_update_in_progress_flag() {
      infos::arch::x86::__outb(0x70, 0x0A);
      return (infos::arch::x86::__inb(0x71) & 0x80);
	}


	void read_timepoint(RTCTimePoint& tp) override
	{

		unsigned char last_second;
      	unsigned char last_minute;
      	unsigned char last_hour;
      	unsigned char last_day;
      	unsigned char last_month;
      	unsigned char last_year;
      	unsigned char registerB;

		unsigned char second;
		unsigned char minute;
		unsigned char hour;
		unsigned char day;
		unsigned char month;
		unsigned int year;

	    while (get_update_in_progress_flag());
      	second = get_data(0x00);
      	minute = get_data(0x02);
      	hour = get_data(0x04);
      	day = get_data(0x07);
     	month = get_data(0x08);
      	year = get_data(0x09);

      do {
            last_second = second;
            last_minute = minute;
            last_hour = hour;
            last_day = day;
            last_month = month;
            last_year = year;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            second = get_data(0x00);
            minute = get_data(0x02);
            hour = get_data(0x04);
            day = get_data(0x07);
            month = get_data(0x08);
            year = get_data(0x09);
      } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) ||
               (last_day != day) || (last_month != month) || (last_year != year));
 
      registerB = get_data(0x0B);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
      }
		tp.seconds = second;
		tp.minutes = minute;
		tp.hours = hour;
		tp.day_of_month = day;
		tp.month = month;
		tp.year = year;
	}


};

const DeviceClass CMOSRTC::CMOSRTCDeviceClass(RTC::RTCDeviceClass, "cmos-rtc");

RegisterDevice(CMOSRTC);
