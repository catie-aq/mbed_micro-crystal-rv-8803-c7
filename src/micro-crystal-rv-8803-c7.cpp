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

namespace sixtron {

RV_8803_C7::RV_8803_C7(I2C *i2c)
{
	_i2c = i2c;
}

void RV_8803_C7::set_seconds(uint8_t seconds)
{
    // Data must be converted to BCD format
    uint8_t seconds_bcd = ((seconds / 10)) << 4 + (seconds % 10);
}

int RV_8803_C7::i2c_set_register(RegisterAddress registerAddress, char value)
{
    static char data[2];
    data[0] = static_cast<char>(registerAddress);
    data[1] = value;
    if (_i2c->write(static_cast<int>(_i2c_address) << 1, data, 2, false) != 0) {
        return -1;
    }
    return 0;
}

int RV_8803_C7::i2c_read_register(RegisterAddress registerAddress, char *value)
{
    char data = static_cast<char>(registerAddress);
    if (_i2c->write(static_cast<int>(_i2c_address) << 1, &data, 1, true) != 0) {
        return -1;
    }
    if (_i2c->read(static_cast<int>(_i2c_address) << 1, value, 1, false) != 0) {
        return -2;
    }
    return 0;
}

} // namespace sixtron

