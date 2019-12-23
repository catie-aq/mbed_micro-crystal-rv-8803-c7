/*
 * Copyright (c) 2019, CATIE
 * SPDX-License-Identifier: Apache-2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef CATIE_SIXTRON_ZESTRTCRV8803C7_H_
#define CATIE_SIXTRON_ZESTRTCRV8803C7_H_

namespace sixtron {

class Zest_RTC_RV-8803-C7
{
public:

    enum class RegisterAddress : char {
        /* Basic time and calendar register */
        Seconds                 = (0x00),
        Minutes                 = (0x01),
        Hours                   = (0x02),
        Weekday                 = (0x03),
        Date                    = (0x04),
        Month                   = (0x05),
        Year                    = (0x06),
        RAM                     = (0x07),
        Minutes_Alarm           = (0x08),
        Hours_Alarm             = (0x09),
        Weekday_Alarm           = (0x0A),
        Date_Alarm              = (0x0A),
        Time_Counter_0          = (0x0B),
        Time_Counter_1          = (0x0C),
        Extension               = (0x0D),
        Flag                    = (0x0E),
        Control                 = (0x0F),

        /* Extension register */
        Seconds_100th           = (0x10),
        Seconds_ext             = (0x11),
        Minutes_ext             = (0x12),
        Hours_ext               = (0x13),
        Weekday_ext             = (0x14),
        Date_ext                = (0x15),
        Month_ext               = (0x16),
        Year_ext                = (0x17),
        Minutes_Alarm_ext       = (0x18),
        Hours_Alarm_ext         = (0x19),
        Weekday_Alarm_ext       = (0x1A),
        Date_Alarm_ext          = (0x1A),
        Time_Counter_0_ext      = (0x1B),
        Time_Counter_1_ext      = (0x1C),
        Extension_ext           = (0x1D),
        Flag_ext                = (0x1E),
        Control_ext             = (0x1F),
        Seconds_100th_CP        = (0x20),
        Seconds_CP              = (0x21),
        Offset                  = (0x2C),
        Event_Control           = (0x2D)
    };

    Zest_RTC_RV-8803-C7(I2C *i2c, I2CAddress i2c_address);

    // TODO : On reading procedure, if seconds == 59, a 2nd read must be done to confirm that data has been correctly retrieved

    /** Store the count of seconds
    *
    @param seconds number of seconds (from 0 to 59)
    */
    set_seconds(uint8_t seconds);

private:
    I2C *_i2c;
    char _i2c_address = 0x64;
};

} // namespace sixtron

#endif // CATIE_SIXTRON_ZESTRTCRV8803C7_H_

