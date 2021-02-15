#pragma once

#include <Windows.h>

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
        DEFINE_MEMBER_N(vec3_f, pos, 0xA0);
        DEFINE_MEMBER_N(uintptr_t, team_number, 0xF4);
    };

};

class EntityInfo
{
public:
    Entity* ent;
    char pad[12];
};


