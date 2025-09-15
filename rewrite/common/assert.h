#pragma once
#ifdef NDEBUG
 #undef NDEBUG
 #include <cassert>
 #define NDEBUG
 #define DEBUG_ASSERT ((void)0)
#else
 #include <cassert>
 #define DEBUG_ASSERT assert
#endif

#ifndef IM_ASSERT
 #define IM_ASSERT assert
#endif