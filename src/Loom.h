/*! \mainpage An Internet of Things Rapid Prototyping System for applications in environmental sensing

This is the Doxygen generated documentation of the Loom library code.

\section resources Resources

- [Github](https://github.com/OPEnSLab-OSU/Loom)
- [Loom Wiki](https://github.com/OPEnSLab-OSU/Loom/wiki)
- [Project Website](http://www.open-sensing.org/project-loom)

*/

#pragma once

#define LOOM_DEBUG 1

// Find a better home for this thing at some point...
// Fix for undefined std::__throw_bad_alloc()
namespace std
{
  void __throw_bad_alloc()
  {
    return; 
  }
}
///////////////////////////////////////////////////////////////////////////////
// Core Classes
#include "Loom_Manager.h"
