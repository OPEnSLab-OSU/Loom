//
//  PluginTypes.hpp
//  Static Registry
//
//  Created by Eli Winkelman on 4/13/20.
//  Copyright © 2020 OPEnS Lab. All rights reserved.
//

#ifndef CompileFlag_h
#define CompileFlag_h

#include <type_traits>

template <typename T> struct compileFlag : std::false_type {};


#endif /* PluginTypes_hpp */
