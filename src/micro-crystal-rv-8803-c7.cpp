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

#include "micro-crystal-rv-8803-c7/micro-crystal-rv-8803-c7.h"
#include <math.h>

namespace sixtron {

namespace {
#define RESET_BIT    0x01;
char buffer[16];
}

RV_8803_C7::RV_8803_C7(I2C *i2c)
{
    _i2c = i2c;
}

void RV_8803_C7::set_time(time_t time)
{
    char data[7];
    struct tm *tmp = gmtime(&time);
    char control_value;

    printf("Year : %d\n", tmp->tm_year);

    // Set RESET bit to 1 to stop clock
    i2c_read_register(RegisterAddress::Control, &control_value, 1);
    control_value |= RESET_BIT;
    i2c_set_register(RegisterAddress::Control, &control_value, 1);

    // Data needs to be converted to BCD format
    data[0] = bin2bcd(tmp->tm_sec);
    data[1] = bin2bcd(tmp->tm_min);
    data[2] = bin2bcd(tmp->tm_hour);
    data[3] = 1 << tmp->tm_wday;
    data[4] = bin2bcd(tmp->tm_mday);
    data[5] = bin2bcd(tmp->tm_mon + 1);
    data[6] = bin2bcd(tmp->tm_year - 100);

    i2c_set_register(RegisterAddress::Seconds, data, sizeof(data));

    // Restart the clock
    control_value &= ~RESET_BIT;
    i2c_set_register(RegisterAddress::Control, &control_value, 1);
}

time_t RV_8803_C7::get_time()
{
    struct tm tmp;
    char date1[7];
    char date2[7];
    char *date = date1;

    i2c_read_register(RegisterAddress::Seconds, date1, 7);

    // Check if seconds == 59 because we need to read again in this case to confirm correct time
    if ((date1[0] & 0x7f) == bin2bcd(59)) {
        i2c_read_register(RegisterAddress::Seconds, date2, 7);

        if ((date2[0] & 0x7f) != bin2bcd(59)) {
            date = date2;
        }
    }

    tmp.tm_sec = bcd2bin(date[0] & 0x7f);
    tmp.tm_min = bcd2bin(date[1] & 0x7f);
    tmp.tm_hour = bcd2bin(date[2] & 0x3f);
    tmp.tm_wday = log2(date[3] & 0x7f);
    tmp.tm_mday = bcd2bin(date[4] & 0x3f);
    tmp.tm_mon = bcd2bin(date[5] & 0x1f) - 1;
    tmp.tm_year = bcd2bin(date[6]) + 100;

    return mktime(&tmp);
}

int RV_8803_C7::enable_periodic_time_interrupt()
{
    char buffer[2] = {0};
    char data = 0x01;
    data = (data << 5);
    
    buffer[0] = static_cast<char>(RegisterAddress::Control_ext);
    memcpy(buffer + 1, &data, 1);
    if (_i2c->write(static_cast<int>(_i2c_address) << 1, buffer, 2) != 0) {
        return -1;
    }
    return 0;

    //i2c_set_register(RegisterAddress::Control_ext, &data, 1);
}

int RV_8803_C7::i2c_set_register(RegisterAddress register_address, const char *data, int length)
{
    buffer[0] = static_cast<char>(register_address);
    memcpy(buffer + 1, data, length);
    if (_i2c->write(static_cast<int>(_i2c_address) << 1, buffer, length + 1) != 0) {
        return -1;
    }
    return 0;
}

int RV_8803_C7::i2c_read_register(RegisterAddress register_address, char *value, int length)
{
    char data = static_cast<char>(register_address);
    if (_i2c->write(static_cast<int>(_i2c_address) << 1, &data, 1, true) != 0) {
        return -1;
    }
    if (_i2c->read(static_cast<int>(_i2c_address) << 1, value, length, false) != 0) {
        return -2;
    }
    return 0;
}

uint8_t RV_8803_C7::bin2bcd(uint8_t x)
{
    return (((x / 10) << 4) + (x % 10));
}

uint8_t RV_8803_C7::bcd2bin(uint8_t x)
{
    return ((x & 0x0f) + (x >> 4) * 10);
}
} // namespace sixtron

