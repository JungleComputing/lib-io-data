/*
 * Copyright 2019 Vrije Universiteit Amsterdam, The Netherlands
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "io_data_constants.h"


float allowed_error = ERROR;
float allowed_float_error = FLOAT_ERROR;
bool doPrintOutput = false;


#include "io_data.h"


/* setting the error */
void setError(float e) {
    allowed_error = e;
}
void setFloatError(float e) {
    allowed_float_error = e;
}
void turnOnPrintingOutput() {
    doPrintOutput = true;
}


