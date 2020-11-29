#ifndef _GOL_H__
#define _GOL_H__

// setting board size
// width and height should always be int

#define BOARD_SIZE 100
#define BOARD_WIDTH 10
#define BOARD_HEIGHT (BOARD_SIZE / BOARD_WIDTH)

//definitions to help positioning
#define TOP_N(i) (i + BOARD_WIDTH)
#define BOT_N(i) (i - BOARD_WIDTH)
#define LEFT_N(i) (i - 1)
#define RIGHT_N(i) (i + 1)
#define TOPLEFT_N(i) (TOP_N(i) - 1)
#define TOPRIGHT_N(i) (TOP_N(i) + 1)
#define BOTLEFT_N(i) (BOT_N(i) - 1)
#define BOTRIGHT_N(i) (BOT_N(i) + 1)

#define POS(x, y) (y * BOARD_WIDTH + x)

unsigned short gameboard[BOARD_SIZE] = {0};

//some patterns
#define PATTERN_ACORNS 1
#define PATTERN_GLIDER 2
#define PATTERN_DIE_HARD 3

void set(int x, int y)
{
  gameboard[(POS(x, y) % BOARD_SIZE)] = 1;
}

void add_pattern(
    int pattern,
    int x,
    int y)
{
  switch (pattern)
  {
  case PATTERN_ACORNS:
    set(x, y);
    set(x + 2, y + 1);
    set(x, y + 1);
    set(x + 1, y + 3);
    set(x, y + 4);
    set(x, y + 5);
    set(x, y + 6);
    break;
  case PATTERN_GLIDER:
    set(x, y); //todo not implimented
    break;
  case PATTERN_DIE_HARD:
    set(x, y + 1);
    set(x + 1, y + 1);
    set(x + 1, y);

    set(x + 5, y);
    set(x + 6, y);
    set(x + 7, y);
    set(x + 6, y + 2);
    break;
  }
}

void randomize_board()
{
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    if (random(200) % 4 == 0)
    {
      gameboard[i] = 1;
    }
  }
}

unsigned int count_cells()
{
  unsigned int count = 0;
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    count += gameboard[i] & 1;
  }
  return count;
}

void next_generation()
{
  //just bit-shift down,
  for (int i = 0; i < BOARD_SIZE; i++)
  {
    gameboard[i] >>= 1;
  }
}
#define TEST(x) (gameboard[x] & 1);
#define GET_X(i) (i % BOARD_WIDTH)
#define GET_Y(i) (i / BOARD_WIDTH)

unsigned short countNeighbours(int index)
{
  unsigned short ret = 0;
  unsigned short x = GET_X(index);
  unsigned short y = GET_Y(index);

  if (x > 0 && x < BOARD_HEIGHT - 1 && y > 0 && y < BOARD_WIDTH - 1)
  { //inner area, count as normal
    ret += TEST(TOP_N(index));
    ret += TEST(BOT_N(index));
    ret += TEST(LEFT_N(index));
    ret += TEST(RIGHT_N(index));
    ret += TEST(BOTRIGHT_N(index));
    ret += TEST(TOPRIGHT_N(index));
    ret += TEST(TOPLEFT_N(index));
    ret += TEST(BOTLEFT_N(index));
    return ret;
  }

  if (x == 0)
  { //bottom layer, we can safely test the top,
    ret += TEST(TOP_N(index));

    if (y != 0) //safe to test side
    {
      ret += TEST(LEFT_N(index));
      ret += TEST(TOPLEFT_N(index));
    }
    if (y != BOARD_WIDTH - 1)
    {
      ret += TEST(RIGHT_N(index));
      ret += TEST(TOPRIGHT_N(index));
    }
  }
  else if (x == BOARD_HEIGHT - 1)
  {
    ret += TEST(BOT_N(index));
    if (y != 0) //safe to test side
    {
      ret += TEST(LEFT_N(index));
      ret += TEST(BOTLEFT_N(index));
    }
    if (y != BOARD_WIDTH - 1)
    {
      ret += TEST(RIGHT_N(index));
      ret += TEST(BOTRIGHT_N(index));
    }
  }
  return ret;
}
int process_board()
{
  //returns the change expected next gen
  int count = 0;

  //preprocess board
  for (int i = 0; i < BOARD_SIZE; i++)
  {

    unsigned short n = countNeighbours(i);
    if (gameboard[i] & 1)
    { //alive cells
      if (n == 2 || n == 3)
      {                    //with moderate neighbours
        gameboard[i] |= 2; //stays alive
      }
      else
      {
        gameboard[i] = 1; //otherwise will die next gen
        count -= 1;
      }
    }
    else
    { // dead cell,
      if (n == 3)
      {                    //with neighbours
        gameboard[i] |= 2; //will come alive
        count += 1;
      }
      else
      {
        gameboard[i] = 0; //otherwise will stay ded
      }
    }
  }
  return count; //delta cells
}
#endif
