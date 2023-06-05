#include <array>
#include <chrono>
#include <cstdio>
#include <curses.h>
#include <functional>
#include <ncurses.h>
#include <random>
#include <signal.h>
#include <thread>

volatile sig_atomic_t stop;

void inthand(int signum) { stop = 1; }
constexpr int MAX_X = 30;
constexpr int MAX_Y = 30;
WINDOW *win;

unsigned int
countNeighbours(const std::array<std::array<bool, MAX_X>, MAX_Y> &state, int i,
                int j) {
  unsigned int livingNeighbors = 0;
  if (i > 0) {
    if (j > 0) {
      livingNeighbors += state[i - 1][j - 1] ? 1 : 0;
    }
    livingNeighbors += state[i - 1][j];
    if (j < MAX_X - 1) {
      livingNeighbors += state[i - 1][j + 1] ? 1 : 0;
    }
  }
  if (j > 0) {
    livingNeighbors += state[i][j - 1];
  }
  if (j < MAX_X - 1) {
    livingNeighbors += state[i][j + 1];
  }
  if (i < MAX_Y - 1) {
    if (j > 0) {
      livingNeighbors += state[i + 1][j - 1];
    }
    livingNeighbors += state[i + 1][j];
    if (j < MAX_X - 1) {
      livingNeighbors += state[i + 1][j + 1];
    }
  }
  return livingNeighbors;
}

std::array<std::array<bool, MAX_X>, MAX_Y>
updateState(const std::array<std::array<bool, MAX_X>, MAX_Y> &state) {
  auto newState = state;

  for (int i = 0; i < MAX_Y; ++i) {
    for (int j = 0; j < MAX_X; ++j) {
      // count living neighbors
      unsigned int livingNeighbors = countNeighbours(state, i, j);

      if (state[i][j]) {
        if (livingNeighbors < 2 || livingNeighbors > 3) {
          newState[i][j] = false;
        } else if (livingNeighbors == 3) {
          newState[i][j] = true;
        }
      } else if (!state[i][j] && livingNeighbors == 3) {
        newState[i][j] = true;
      }
    }
  }
  return newState;
}

void printState(const std::array<std::array<bool, MAX_X>, MAX_Y> &state) {
  // swap buffer with actual window
  unsigned int x = 0;
  unsigned int y = 0;

  int w, h;
  getmaxyx(win, h, w);

  for (const auto &row : state) {
    x = 0;
    if (y > h - 2) {
      break;
    }
    for (const auto &cell : row) {
      if (x > w) {
        break;
      }
      mvaddch(y, x, cell ? 'X' : ' ');
      ++x;
    }
    ++y;
  }
  refresh();
}

void seedState(std::array<std::array<bool, MAX_X>, MAX_Y> &state) {

  // seed with a mersenne twister engine
  std::random_device rd;  // a seed source for the random number engine
  std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<> distrib(0, 1);

  // unsigned int x = 0;
  // for (auto &row : state) {

  //   for (auto &cell : row) {
  //     cell = distrib(gen);
  //     ++x;
  //   }
  // }
  // create a glider
  state[0][2] = true;
  state[1][0] = true;
  state[1][2] = true;
  state[2][1] = true;
  state[2][2] = true;
}

int main(int, char **) {

  std::array<std::array<bool, MAX_X>, MAX_Y> state{false};

  // register CTRL-C handler
  signal(SIGINT, inthand);
  // Fetch window size
  win = initscr();
  seedState(state);

  while (!stop) {

    // Add your implementation here

    printState(state);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    // getch();
    state = updateState(state);
  }
  // mvaddch(h- 1, w-1, 'A');

  endwin();
  return 0;
}