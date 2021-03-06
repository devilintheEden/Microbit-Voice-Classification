/*
The MIT License (MIT)

Copyright (c) 2016 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/
#include "MicroBit.h"
#include "AudioStreamer.h"
#include "Tests.h"

/**
 * Creates a simple component that logs a stream of signed 16 bit data as signed 8-bit data over serial.
 * @param source a DataSource to measure the level of.
 */
AudioStreamer::AudioStreamer(DataSource &source) : upstream(source)
{
    // Register with our upstream component
    source.connect(*this);
}

/**
 * Callback provided when data is ready.
 */
int AudioStreamer::pullRequest()
{
    static volatile int pr = 0;
    if(!pr)
    {
        pr++;
        while(pr)
        {
            lastBuffer = upstream.pull();
            streamBuffer(lastBuffer);
            pr--;
        }
    }
    else
    {
        pr++;
    }
    
    return DEVICE_OK;
}

/**
    * returns the last buffer processed by this component
    */
ManagedBuffer AudioStreamer::getLastBuffer()
{
    return lastBuffer;
}

/**
 * Callback provided when data is ready.
 */
void AudioStreamer::streamBuffer(ManagedBuffer buffer)
{
    uint8_t *p = &buffer[0];
    uint8_t *end = p + buffer.length();

    while (p < end) {
        int8_t data = *p++;
        uBit.serial.putc(data);
    }
}


