//-----------------------------------------------------------------------------
//                                                                            |
//                               Softing AG                                   |
//                        Richard-Reitzner-Allee 6                            |
//                           85540 Haar, Germany                              |
//                                                                            |
//                 This is a part of the Softing OPC Toolbox                  |
//                   Copyright (C) Softing AG 1998 - 2008                     |
//                           All Rights Reserved                              |
//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                             OPC Toolbox C++                                |
//                                                                            |
//  Filename    : stdafx.h                                                    |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : precompiled header definitions					          |
//                                                                            |
//-----------------------------------------------------------------------------

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vole/vole.hpp>

// reference to the toolbox Operating system speciffics
#include <OSCompat.h>