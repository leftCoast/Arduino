/**
 * Copyright (c) 2019, Enrique Condes
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. The names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALLTHE CREATOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

 /*
 Simple example that generates a custom timestamp from the information provided.
 The customization of the timestamp consists in being able to stablish the year
 that will be used as EPOCH. The custom timestamp is INCOMPATIBLE with the
 standard implementation of Linux timestamp. The advantage of a custom timestamp
 is that by moving the epoch closer to the current year, it allows to store dates
 beyond January 19, 2038.
 */

 #include "timestamp32bits.h"

 timestamp32bits stamp = timestamp32bits(2000);

 void setup() {
   Serial.begin(115200);
   //timestamp paramaters order is (year, month, day, hour, minute, second)
   Serial.println(stamp.timestamp(19,11,07,11,22,33)); //It should print 626440953
   while(1);
 }

 void loop() {
   // put your main code here, to run repeatedly:

 }
