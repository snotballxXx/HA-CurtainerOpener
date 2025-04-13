#ifndef _STATE_H
#define _STATE_H

namespace Control
{
    enum State
    {
        Open,
        Opening,
        Closing,
        Closed,
        Stopped,
        PendingChange,
        Calibrate
    };
}
#endif