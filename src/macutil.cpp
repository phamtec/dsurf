/*
  macutil.cpp
  
  Author: Paul Hamilton (phamtec@mac.com)
  Date: 9-Oct-2025
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/phamtec/dsurf
*/

#ifdef MAC_APP

#include "macutil.hpp"

#include <CoreFoundation/CoreFoundation.h>
#include <iostream>

using namespace std;

std::string MacUtil::getFontPath() {

  CFURLRef appUrlRef = CFBundleCopyResourceURL(CFBundleGetMainBundle(), CFSTR("Monaco.ttf"), NULL, NULL);
  CFStringRef filePathRef = CFURLCopyPath(appUrlRef);
  const char* filePath = CFStringGetCStringPtr(filePathRef, kCFStringEncodingUTF8);
  string p(filePath);
  // Release references
  CFRelease(filePathRef);
  CFRelease(appUrlRef);

  return p;
  
}

#endif // MAC_APP
