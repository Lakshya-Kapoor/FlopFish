# Performance metrics

## Effect of move ordering on performance

Move ordering significantly improves pruning. The following table shows the number of nodes searched for different depths with no move ordering, partial move ordering (captures are moved to the front of the move list), and full move ordering (captures are moved to the front of the move list and captures are sorted using MVV-LVA).

1. Initial: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

   | depth | no reordering | partial reordering | full reordering |
   | ----- | ------------- | ------------------ | --------------- |
   | 5     | 62905 nodes   | 49969 nodes        | 49949 nodes     |
   | 6     | 934391 nodes  | 493584 nodes       | 493225 nodes    |
   | 7     | 5420847 nodes | 3194627 nodes      | 3171611 nodes   |

2. Mid game: "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"

   | depth | no reordering  | partial reordering | full reordering |
   | ----- | -------------- | ------------------ | --------------- |
   | 5     | 2768686 nodes  | 393981 nodes       | 282098 nodes    |
   | 6     | 35311500 nodes | 2075904 nodes      | 1365780 nodes   |
   | 7     | Too high       | 22462485 nodes     | 13992796 nodes  |

3. End game: "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"

   | depth | no reordering  | partial reordering | full reordering |
   | ----- | -------------- | ------------------ | --------------- |
   | 5     | 83626 nodes    | 18336 nodes        | 18361 nodes     |
   | 6     | 371680 nodes   | 25950 nodes        | 25500 nodes     |
   | 7     | 2617353 nodes  | 206341 nodes       | 198815 nodes    |
   | 8     | 16069474 nodes | 566111 nodes       | 553905 nodes    |

The benefits of partial reordering is clear in all the tables, whereas full reordering is significantly improving performance in mid game scenarios.

## Move reordering using iterative deepening

v1: normal negamax with alpha-beta pruning and move reordering to

v2: iterative deepening with first move reordering along with capture reordering (captures first, MVV-LVA).

v3: iterative deepening with complete PV move reordering along with capture reordering (captures first, MVV-LVA).

1. Initial: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

   | depth | v1             | v2 (total nodes) | v2 (iteration nodes) | v3 (iteration nodes) |
   | ----- | -------------- | ---------------- | -------------------- | -------------------- |
   | 5     | 49949 nodes    | 35831 nodes      | 32773 nodes          | 32660 nodes          |
   | 6     | 493225 nodes   | 517958 nodes     | 333331 nodes         | 330645 nodes         |
   | 7     | 3171611 nodes  | 2315754 nodes    | 1593975 nodes        | 1616203 nodes        |
   | 8     | 19106141 nodes | 16016148 nodes   | 14616725 nodes       | 14169530 nodes       |

2. Mid game: "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"

   | depth | v1             | v2 (total nodes) | v2 (iteration nodes) | v3 (iteration nodes) |
   | ----- | -------------- | ---------------- | -------------------- | -------------------- |
   | 5     | 282098 nodes   | 281778 nodes     | 275024 nodes         | 275544 nodes         |
   | 6     | 1365780 nodes  | 1508619 nodes    | 1226841 nodes        | 1227766 nodes        |
   | 7     | 13992796 nodes | 14271927 nodes   | 12763308 nodes       | 12673278 nodes       |

3. End game: "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"

   | depth | v1           | v2 (total nodes) | v2 (iteration nodes) | v3 (iteration nodes) |
   | ----- | ------------ | ---------------- | -------------------- | -------------------- |
   | 5     | 18361 nodes  | 20320 nodes      | 18361 nodes          | 18353 nodes          |
   | 6     | 25500 nodes  | 45820 nodes      | 25500 nodes          | 24753 nodes          |
   | 7     | 198815 nodes | 244635 nodes     | 198815 nodes         | 199091 nodes         |
   | 8     | 553905 nodes | 798540 nodes     | 553905 nodes         | 530741 nodes         |

## Transposition table reordering

1. Initial: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

   | depth | v2             | v4 (no tt reordering) | v4 (tt reordering) |
   | ----- | -------------- | --------------------- | ------------------ |
   | 5     | 35831 nodes    | 46867 nodes           | 44768 nodes        |
   | 6     | 517958 nodes   | 384084 nodes          | 343763 nodes       |
   | 7     | 2315754 nodes  | 2362624 nodes         | 2039545 nodes      |
   | 8     | 16016148 nodes | 11164701 nodes        | 9268030 nodes      |

2. Mid game: "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"

   | depth | v2 (total nodes) | v4 (no tt reordering) | v4 (tt reordering) |
   | ----- | ---------------- | --------------------- | ------------------ |
   | 5     | 281778 nodes     | 265695 nodes          | 227618 nodes       |
   | 6     | 1508619 nodes    | 1358010 nodes         | 1177074 nodes      |
   | 7     | 14271927 nodes   | 10209706 nodes        | 8400171 nodes      |

3. End game: "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"

   | depth | v2 (total nodes) | v4 (no tt reordering) | v4 (tt reordering) |
   | ----- | ---------------- | --------------------- | ------------------ |
   | 5     | 20320 nodes      | 15906 nodes           | 12176 nodes        |
   | 6     | 45820 nodes      | 33106 nodes           | 21310 nodes        |
   | 7     | 244635 nodes     | 138463 nodes          | 72372 nodes        |
   | 8     | 798540 nodes     | 411991 nodes          | 288602 nodes       |

## Transposition table size

1. Initial: "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

   | depth | 2^18 entries   | 2^20 entries  | 2^22 entries  |
   | ----- | -------------- | ------------- | ------------- |
   | 5     | 44715 nodes    | 44768 nodes   | 44724 nodes   |
   | 6     | 345876 nodes   | 343763 nodes  | 341927 nodes  |
   | 7     | 2129870 nodes  | 2039545 nodes | 1991990 nodes |
   | 8     | 12127271 nodes | 9268030 nodes | 7444991 nodes |

2. Mid game: "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10"

   | depth | 2^18 entries   | 2^20 entries  | 2^22 entries  |
   | ----- | -------------- | ------------- | ------------- |
   | 5     | 227820 nodes   | 227618 nodes  | 227584 nodes  |
   | 6     | 1214638 nodes  | 1177074 nodes | 1163268 nodes |
   | 7     | 10239336 nodes | 8400171 nodes | 7321954 nodes |

## Move generation efficiency

1. Naive legal move generation: Seeing whether a move is legal by making the move and checking if the king is in check.

   | Position | Depth | Time (s) | Nodes/s |
   | -------- | ----- | -------- | ------- |
   | Initial  | 8     | 21.0876  | 350519  |
   | Mid game | 7     | 22.75    | 315051  |
   | End game | 10    | 3.44851  | 445775  |

2. Legal move generation while keeping track of pinned pieces and evasion squares.

   | Position | Depth | Time (s) | Nodes/s |
   | -------- | ----- | -------- | ------- |
   | Initial  | 8     | 9.22925  | 819204  |
   | Mid game | 7     | 8.62106  | 835264  |
   | End game | 10    | 0.910092 | 824558  |
