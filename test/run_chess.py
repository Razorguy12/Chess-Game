import time
import sys
import subprocess

# Terminal command to run your chess engine
# Change this path/command as needed
CMD = ["bash", "-c", "./chess"]   # Example: ./a.out or ./chess

# Moves to automate (first two are player names)
moves = [
    "WhitePlayer",
    "BlackPlayer",
    "g2 g3",
    "e7 e5",
    "f1 g2",
    "d7 d5",
    "g1 f3",
    "b8 c6",
    "b1 c3",
    "f8 b4",
    "d2 d4",
    "f7 f6",
    "c1 e3",
    "e5 e4",
    "f3 h4",
    "g7 g5",
    "h4 f3",
    "d8 e7",
    "0-0",
    "g8 h6",
    "c3 d5",
    "e7 d6",
    "d5 b4",
    "c6 b4",
    "f3 d2",
    "c8 e6",
    "d2 e4",
    "d6 e7",
    "a2 a3",
    "b4 d5",
    "c2 c4",
    "d5 e3",
    "f2 e3",
    "e6 c4",
    "b2 b3",
    "c4 d5",
    "f1 f6",
    "h6 g4",
    "g2 f3",
    "d5 e4",
    "f3 e4",
    "e7 f6",
    "d1 f1",
    "f6 e6",
    "e4 b7",
    "h8 f8",
    "f1 c1",
    "f8 f2",
    "b7 a8",
    "f2 e2",
    "c1 c7",
    "e6 e3",
    "g1 h1",
    "e2 e1",
    "a1 e1",
    "e3 e1",
    "h1 g2",
    "g4 e3",
    "g2 f3",
    "g5 g4",
    "f3 e4",
    "e3 c2",
    "e4 f5",
    "c2 e3",
    "f5 f6",
    "e1 f1",
    "c7 f4",
    "e3 d5",
    "a8 d5",
    "f1 f4",
    "g3 f4",
    "h7 h5",
    "b3 b4",
    "h5 h4",
    "f6 g5",
    "g4 g3",
    "h2 g3",
    "h4 g3",
    "b4 b5",
    "g3 g2",
    "d5 g2",
    "a7 a5",
    "b5 a6",
    "e8 d7",
    "a6 a7",
    "d7 c7",
    "a7 a8",
    "Q",
    "c7 b6",
    "d4 d5",
    "b6 c7",
    "f4 f5",
    "c7 d6",
    "f5 f6",
    "d6 d7",
    "f6 f7",
    "d7 e7",
    "f7 f8",
    "N",
    "e7 d6",
    "a3 a4",
    "d6 c5",
    "a4 a5",
    "c5 b5",
    "a5 a6",
    "b5 b6",
    "a6 a7",
    "b6 c7",
    "a8 b8"
]


def main():
    # Start the chess program
    process = subprocess.Popen(
        CMD,
        stdin=subprocess.PIPE,
        stdout=sys.stdout,
        stderr=sys.stderr,
        text=True,
        bufsize=1
    )

    time.sleep(1)  # give chess program time to print intro

    for line in moves:
        print(line)          # show the input in THIS terminal
        process.stdin.write(line + "\n")   # send input to chess game
        process.stdin.flush()
        time.sleep(0.5)      # delay between moves
    
    process.stdin.close()
    process.wait()

if __name__ == "__main__":
    main()
