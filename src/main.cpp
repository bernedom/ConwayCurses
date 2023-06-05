#include <array>
#include <functional>
#include <ncurses.h>
#include <random>
#include <signal.h>

volatile sig_atomic_t stop;

void inthand(int signum) { stop = 1; }
constexpr int MAX_X = 100;
constexpr int MAX_Y = 100;
WINDOW *win;

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

  unsigned int x = 0;
  for (auto &row : state) {

    for (auto &cell : row) {
      cell = distrib(gen);
      ++x;
    }
  }

  // create a simple glider
  // state[0][2] = true;
  // state[1][0] = true;
  // state[1][2] = true;
  // state[2][1] = true;
  // state[2][2] = true;
}

int main(int, char **) {

  std::array<std::array<bool, MAX_X>, MAX_Y> state;

  // register CTRL-C handler
  signal(SIGINT, inthand);
  // Fetch window size
  win = initscr();
  seedState(state);

  while (!stop) {

    // Add your implementation here

    printState(state);
  }
  // mvaddch(h- 1, w-1, 'A');

  // reset window
  endwin();
  return 0;
}