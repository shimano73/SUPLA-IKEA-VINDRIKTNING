/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

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

#ifndef _bh1750_h
#define _bh1750_h
#include <Wire.h>
#include <BH1750.h>

#include "supla/sensor/thermometer.h"

namespace Supla {
namespace Sensor {
class Sensor_BH1750 : public Thermometer {
 public:
  Sensor_BH1750(){
  }

  double getValue() {
    float value = TEMPERATURE_NOT_AVAILABLE;
    bool retryDone = false;
	return LightMeter.readLightLevel()/ 1000;
  }

 
  void onInit() {
    
	//sensorStatus = 
	LightMeter.begin();
    channel.setNewValue(getValue());
  }

 

 protected:
  int8_t address;
  bool sensorStatus;
  
 BH1750 LightMeter;  // I2C
};

};  // namespace Sensor
};  // namespace Supla

#endif
