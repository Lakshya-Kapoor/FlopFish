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

   | version | v1             | v2 (total nodes) | v2 (iteration nodes) | v3 (iteration nodes) |
   | ------- | -------------- | ---------------- | -------------------- | -------------------- |
   | 5       | 49949 nodes    | 35831 nodes      | 32773 nodes          | 32660 nodes          |
   | 6       | 493225 nodes   | 517958 nodes     | 333331 nodes         | 330645 nodes         |
   | 7       | 3171611 nodes  | 2315754 nodes    | 1593975 nodes        | 1616203 nodes        |
   | 8       | 19106141 nodes | 16016148 nodes   | 14616725 nodes       | 14169530 nodes       |

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
