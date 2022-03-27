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

#include "Arduino.h"
#include <DHT.h>
#include "vindriktning.h"


#include "thermometer.h"

namespace Supla {
namespace Sensor {

Vindriktning::Vindriktning(int rxPin, int txPin) : sensorSerial(rxPin, txPin)
{

	_rxPin = rxPin;
	_txPin = txPin;
	
  channel.setType(SUPLA_CHANNELTYPE_HUMIDITYSENSOR);
  channel.setDefault(SUPLA_CHANNELFNC_HUMIDITY);
	
}

double Vindriktning::getHumi() {
  double value = TEMPERATURE_NOT_AVAILABLE;
     
   value = (double)state.avgPM25;

  return value;
}


void Vindriktning::parseState() {
	/**
	 *         MSB  DF 3     DF 4  LSB
	 * uint16_t = xxxxxxxx xxxxxxxx
	 */
	const uint16_t pm25 = (serialRxBuf[5] << 8) | serialRxBuf[6];

	Serial.printf("Received PM 2.5 reading: %d\n", pm25);

	state.measurements[state.measurementIdx] = pm25;
	Serial.print("state.measurementIdx : ");
	Serial.println(state.measurementIdx);
	state.measurementIdx = state.measurementIdx + 1;

	if (state.measurementIdx == 5) {
		double avgPM25 = 0.0;

		for (uint8_t i = 0; i < 5; ++i) avgPM25 += state.measurements[i] ;
		
		state.avgPM25 = avgPM25 / 5;
		state.valid = true;
		state.measurementIdx = 0;
		Serial.printf("New Avg PM25: %d\n", state.avgPM25);
	}

	clearRxBuf();
}


bool Vindriktning::isValidHeader() {
	bool headerValid = serialRxBuf[0] == 0x16 && serialRxBuf[1] == 0x11 && serialRxBuf[2] == 0x0B;

	if (!headerValid) {
		Serial.println("Received message with invalid header.");
	}

	return headerValid;
}

bool Vindriktning::isValidChecksum() {
	uint8_t checksum = 0;

	for (uint8_t i = 0; i < 20; i++) {
		checksum += serialRxBuf[i];
	}

	if (checksum != 0) {
		Serial.printf("Received message with invalid checksum. Expected: 0. Actual: %d\n", checksum);
	}

	return checksum == 0;
}



void Vindriktning::clearRxBuf() {
	// Clear everything for the next message
	memset(serialRxBuf, 0, sizeof(serialRxBuf));
	rxBufIdx = 0;
}

void Vindriktning::iterateAlways() {
  if (lastReadTime + 10000 < millis()) {
	lastReadTime = millis();
	channel.setNewValue(HUMIDITY_NOT_AVAILABLE, getHumi());
	
	
	 if (!sensorSerial.available()) {
            return;
        }

        Serial.print("Receiving:");
        while (sensorSerial.available()) {
			uint8_t cos =sensorSerial.read();
            serialRxBuf[rxBufIdx++] = cos;
			Serial.print(cos);
            Serial.print(".");

            // Without this delay, receiving data breaks for reasons that are beyond me
            delay(15);

            if (rxBufIdx >= 64) {
                clearRxBuf();
            }
        }
        Serial.println("Done.");

        if (isValidHeader() && isValidChecksum()) {
            parseState();

            Serial.printf(
                "Current measurements: %d, %d, %d, %d, %d\n",

                state.measurements[0],
                state.measurements[1],
                state.measurements[2],
                state.measurements[3],
                state.measurements[4]
            );
        } else {
            clearRxBuf();
        }
	
	
	
	
  }
}

void Vindriktning::onInit() {
  sensorSerial.begin(9600);	
  channel.setNewValue(getHumi());
  
}


};  // namespace Sensor
};  // namespace Supla
