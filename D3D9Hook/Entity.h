#pragma once

#include <Windows.h>

#include "csgo.hpp"
#include "Math.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}


class Entity
{
public:

    // be sure to put all the offsets you pad this way in a union
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(uintptr_t, team_number, hazedumper::netvars::m_iTeamNum);
        DEFINE_MEMBER_N(uintptr_t, health, hazedumper::netvars::m_iHealth);
        DEFINE_MEMBER_N(vec3_f, pos, hazedumper::netvars::m_vecOrigin);
    };

};

class EntityInfo
{
public:
    Entity* ent;
    char pad[12];
};
