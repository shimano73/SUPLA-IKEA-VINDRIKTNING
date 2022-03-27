/*
 Copyright (C) AC SOFTWARE SP. Z O.O. & dariuszjszymanski@gmail.com (shimano73)

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _Vindriktning_h
#define _Vindriktning_h

#include "thermometer.h"

#include <SoftwareSerial.h>

#define HUMIDITY_NOT_AVAILABLE -1

struct particleSensorState_t {
		uint16_t avgPM25;
		uint16_t measurements[5];
		byte measurementIdx;
		boolean valid ;
	 };	 
	 
namespace Supla {
namespace Sensor {
class Vindriktning: public Thermometer {
  public:
    Vindriktning(int rxPin, int txPin); 
      

    virtual double getHumi();
	bool isValidHeader() ;
	bool isValidChecksum();	
    void iterateAlways();
    void onInit();	
	void clearRxBuf();
    void parseState();
    
    


    protected:
 	  int _rxPin;
	  int _txPin;
	  int _pm25;
	  uint8_t serialRxBuf[255];
      uint8_t rxBufIdx = 0;
      double lastValidTemp;
 	 SoftwareSerial sensorSerial;   
	 particleSensorState_t state;
	 
      };

};  // namespace Sensor
};  // namespace Supla

#endif
