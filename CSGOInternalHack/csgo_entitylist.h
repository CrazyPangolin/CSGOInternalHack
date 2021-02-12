#pragma once
// Created with ReClass.NET 1.2 by KN4CK3R
#include "framework.h"


#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class ClientInfo
{
public:
	class Ent* ent_ptr; //0x0000
	int32_t num; //0x0004
	class ClientInfo* blink; //0x0008
	class ClientInfo* flink; //0x000C
}; //Size: 0x0010

class CBaseEntityList
{
public:
	class ClientInfo ent_list[64]; //0x0010
}; //Size: 0x0C3C

class Ent
{
public:
	union
	{
		DEFINE_MEMBER_N(int32_t, health, 0x0100);
		DEFINE_MEMBER_N(bool, is_spotted, 0x093d);
	};
}; //Size: 0x0144