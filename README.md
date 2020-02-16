# checkers
A C implementation of checkers with a negamax-powered AI.

It runs completely on the terminal. Here's how it looks waiting for your first move:
```
   | a | b | c | d | e | f | g | h
---+---+---+---+---+---+---+---+---
 1 |   | x |   | x |   | x |   | x
---+---+---+---+---+---+---+---+---
 2 | x |   | x |   | x |   | x |
---+---+---+---+---+---+---+---+---
 3 |   | x |   | x |   | x |   | x
---+---+---+---+---+---+---+---+---
 4 |   |   |   |   |   |   |   |
---+---+---+---+---+---+---+---+---
 5 |   |   |   |   |   |   |   |
---+---+---+---+---+---+---+---+---
 6 | o |   | o |   | o |   | o |
---+---+---+---+---+---+---+---+---
 7 |   | o |   | o |   | o |   | o
---+---+---+---+---+---+---+---+---
 8 | o |   | o |   | o |   | o |
x>
```
Lowercase characters represent men, while uppercase characters represent kings.
In order to capture you have to move your piece to the square immediately beyond your opponent.
No zig-zagging is allowed.

To actually make a move, type `[column][row] [column][row]`. For example, this move:
```
> b3 a4
```
will move the piece at `b3` to `a4`.

## arguments
If no arguments are provided, checkers runs in two-player mode. You can provide the argument `--ai` followed by an `X` or an `O`, if you want the AI to play Xs or Os, respectively.
