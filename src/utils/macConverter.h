#ifndef _MAC_CONVERTER_H_
#define _MAC_CONVERTER_H_

#include <StreamString.h>

namespace Utils
{
    String macToStr(const uint8_t* mac)
    {
      String result;
      for (int i = 0; i < 6; ++i) {
        result += String(mac[i], 16);
        if (i < 5)
          result += ':';
      }
      return result;
    }
}

#endif //_MAC_CONVERTER_H_