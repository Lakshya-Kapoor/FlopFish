#pragma once

#include "move.h"
#include "position.h"

class Player {
   public:
    virtual Move getMove(Position pos) = 0;
};