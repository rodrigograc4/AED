//
// AED, August 2022 (Tomás Oliveira e Silva)
//
// First practical assignement (speed run)
//
// Compile using either
//   cc -Wall -O2 -D_use_zlib_=0 solution_speed_run.c -lm
// or
//   cc -Wall -O2 -D_use_zlib_=1 solution_speed_run.c -lm -lz
//
// Place your student numbers and names here
//   N.Mec. 107637  Name: André Oliveira
//   N.Mec. 107359  Name: Duarte Cruz
//   N.Mec. 107634  Name: Rodrigo Graça
//

//
// static configuration
//

#define _max_road_size_ 800 // the maximum problem size
#define _min_road_speed_ 2  // must not be smaller than 1, shouldnot be smaller than 2
#define _max_road_speed_ 9  // must not be larger than 9 (only because of the PDF figure)

//
// include files --- as this is a small project, we include the PDF generation code directly from make_custom_pdf.c
//

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "./elapsed_time.h"
#include "make_custom_pdf.c"

//
// road stuff
//

static int max_road_speed[1 + _max_road_size_]; // positions 0.._max_road_size_

static void init_road_speeds(void)
{
  double speed;
  int i;

  for (i = 0; i <= _max_road_size_; i++)
  {
    speed = (double)_max_road_speed_ * (0.55 + 0.30 * sin(0.11 * (double)i) + 0.10 * sin(0.17 * (double)i + 1.0) + 0.15 * sin(0.19 * (double)i));
    max_road_speed[i] = (int)floor(0.5 + speed) + (int)((unsigned int)random() % 3u) - 1;
    if (max_road_speed[i] < _min_road_speed_)
      max_road_speed[i] = _min_road_speed_;
    if (max_road_speed[i] > _max_road_speed_)
      max_road_speed[i] = _max_road_speed_;
  }
}

//
// description of a solution
//

typedef struct
{
  int n_moves;                        // the number of moves (the number of positions is one more than the number of moves)
  int positions[1 + _max_road_size_]; // the positions (the first one must be zero)
} solution_t;

static solution_t solution_1, solution_1_best;
static solution_t solution_2, solution_2_best;
static solution_t solution_3, solution_3_best;
static solution_t solution_4, solution_4_best;
static solution_t solution_5, solution_5_best, solution_5_listas_guardadas;
static double solution_1_elapsed_time; // time it took to solve the problem 1
static double solution_2_elapsed_time; // time it took to solve the problem 2
static double solution_3_elapsed_time; // time it took to solve the problem 3
static double solution_4_elapsed_time; // time it took to solve the problem 4
static double solution_5_elapsed_time; // time it took to solve the problem 5
static unsigned long solution_1_count; // effort dispended solving the problem 1
static unsigned long solution_2_count; // effort dispended solving the problem 2
static unsigned long solution_3_count; // effort dispended solving the problem 3
static unsigned long solution_4_count; // effort dispended solving the problem 4
static unsigned long solution_5_count; // effort dispended solving the problem 5

//
// solution 1 (the very inefficient recursive solution given to the students)
//

static void solution_1_recursion(int move_number, int position, int speed, int final_position)
{
  int i, new_speed;

  // record move
  solution_1_count++;
  solution_1.positions[move_number] = position;
  // is it a solution?
  if (position == final_position && speed == 1)
  {
    // is it a better solution?
    if (move_number < solution_1_best.n_moves)
    {
      solution_1_best = solution_1;
      solution_1_best.n_moves = move_number;
    }
    return;
  }
  // no, try all legal speeds
  for (new_speed = speed - 1; new_speed <= speed + 1; new_speed++)
    if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
        ;
      if (i > new_speed)
        solution_1_recursion(move_number + 1, position + new_speed, new_speed, final_position);
    }
}

//
// solution 2 (Solução melhorada do prof)
//

static void solution_2_recursion(int move_number, int position, int speed, int final_position)
{
  int i, new_speed;

  solution_2_count++;
  solution_2.positions[move_number] = position;

  if (position == final_position && speed == 1)
  {
    if (move_number < solution_2_best.n_moves)
    {
      solution_2_best = solution_2;
      solution_2_best.n_moves = move_number;
    }
    return;
  }

  if (solution_2_best.positions[move_number] > solution_2.positions[move_number])
  {
    return;
  }

  for (new_speed = speed + 1; new_speed >= speed - 1; new_speed--)
    if (new_speed >= 1 && new_speed <= _max_road_speed_ && position + new_speed <= final_position)
    {
      for (i = 0; i <= new_speed && new_speed <= max_road_speed[position + i]; i++)
        ;
      if (i > new_speed)
        solution_2_recursion(move_number + 1, position + new_speed, new_speed, final_position);
    }
}

//
// solution 3 (Testa as velocidades seguintes recursivamente, se v=4 vai testar 3+2+1)
//

bool testarVelocidade1(int new_speed, int position, int final_position)
{
  int x, i, casas_a_andar = 0;
  int record_position = 0;

  for (i = 1; i <= new_speed; i++)
  {
    casas_a_andar += i;
  }

  for (x = new_speed; x >= 1; x--)
  {
    for (i = 0; i <= x; i++)
    {
      if (x > max_road_speed[position + record_position + i] || position + casas_a_andar > final_position || new_speed < 1)
      {
        return false;
      }
    }
    record_position += x;
  }
  return true;
}

static void solution_3_recursion(int move_number, int position, int speed, int final_position)
{
  int new_speed;
  bool verificador = false;

  solution_3_count++;
  solution_3.positions[move_number] = position;

  if (position == final_position && speed == 1)
  {
    if (move_number < solution_3_best.n_moves)
    {
      solution_3_best = solution_3;
      solution_3_best.n_moves = move_number;
    }
    return;
  }
  else
  {
    new_speed = speed + 1;

    while (verificador == false)
    {
      verificador = testarVelocidade1(new_speed, position, final_position);
      if (verificador == false)
      {
        new_speed--;
      }
    }

    solution_3_recursion(move_number + 1, position + new_speed, new_speed, final_position);
  }
}

//
// solution 4 (O mesmo que a anterior sem ser recursiva)
//

bool testarVelocidade2(int new_speed, int position, int final_position)
{
  int x, i, casas_a_andar = 0;
  int record_position = 0;

  for (i = 1; i <= new_speed; i++)
  {
    casas_a_andar += i;
  }

  for (x = new_speed; x >= 1; x--)
  {
    // record_position += x;
    for (i = 0; i <= x; i++)
    {
      if (x > max_road_speed[position + record_position + i] || position + casas_a_andar > final_position || new_speed < 1)
      {
        return false;
      }
    }
    record_position += x;
  }
  return true;
}

static void solution_4_non_recursion(int final_position)
{
  int move_number = 0;
  int position = 0;
  int speed = 0;
  int new_speed;
  bool verificador = false;

  solution_3_count++;
  solution_3.positions[move_number] = position;

  while (position != final_position)
  {
    verificador = false;
    new_speed = speed + 1;

    while (verificador == false)
    {
      verificador = testarVelocidade2(new_speed, position, final_position);
      if (verificador == false)
      {
        new_speed--;
      }
    }

    move_number++;
    position += new_speed;
    speed = new_speed;
    solution_4.positions[move_number] = position;
  }

  if (position == final_position && speed == 1)
  {
    if (move_number < solution_4_best.n_moves)
    {
      solution_4_best = solution_4;
      solution_4_best.n_moves = move_number;
    }
    return;
  }
}

//
// solution 5
//

static int last_speed = 0, last_position = 0;
static int guardar_listas = 0, listas_guardadas = 0;

bool testarVelocidade3(int new_speed, int position, int final_position)
{
  int x, i, casas_a_andar = 0;
  int record_position = 0;

  for (i = 1; i <= new_speed; i++)
  {
    casas_a_andar += i;
  }

  int teste_velocidade = position;

  for (x = new_speed; x >= 1; x--)
  {
    for (i = 0; i <= x; i++)
    {
      teste_velocidade++;

      if (teste_velocidade > final_position && final_position > 1)
      {
        guardar_listas = 1;
      }

      if (x > max_road_speed[position + record_position + i] || position + casas_a_andar > final_position || new_speed < 1)
      {
        return false;
      }
    }
    record_position += x;
  }
  return true;
}

static void solution_5_dynamic(int move_number, int position, int speed, int final_position)
{
  int new_speed;
  bool verificador = false;

  solution_5_count++;
  solution_5.positions[move_number] = position;

  if (position == final_position && speed == 1)
  {
    solution_5_best = solution_5;
    solution_5_best.n_moves = move_number;
    return;
  }
  else
  {
    new_speed = speed + 1;

    while (verificador == false)
    {
      verificador = testarVelocidade3(new_speed, position, final_position);
      if (verificador == false)
      {
        new_speed--;
      }
    }

    if (guardar_listas == 1 && listas_guardadas == 0)
    {
      listas_guardadas = 1;
      for (int i = 0; i <= final_position; i++)
      {
        if (i < move_number)
        {
          solution_5_listas_guardadas.positions[i] = solution_5.positions[i];
        }
        else
        {
          solution_5_listas_guardadas.positions[i] = 0;
        }
      }
      solution_5_listas_guardadas.n_moves = move_number - 1;
      last_position = solution_5_listas_guardadas.positions[move_number - 1];
      last_speed = solution_5_listas_guardadas.positions[move_number - 1] - solution_5_listas_guardadas.positions[move_number - 2];
    }

    solution_5_dynamic(move_number + 1, position + new_speed, new_speed, final_position);
  }
}

//
#define solution5
//

static void solve_1(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_1: bad final_position\n");
    exit(1);
  }
  solution_1_elapsed_time = cpu_time();
  solution_1_count = 0ul;
  solution_1_best.n_moves = final_position + 100;
  solution_1_recursion(0, 0, 0, final_position);
  solution_1_elapsed_time = cpu_time() - solution_1_elapsed_time;
}

static void solve_2(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_2: bad final_position\n");
    exit(1);
  }

  solution_2_elapsed_time = cpu_time();
  solution_2_count = 0ul;
  solution_2_best.n_moves = final_position + 100;
  solution_2_recursion(0, 0, 0, final_position);
  solution_2_elapsed_time = cpu_time() - solution_2_elapsed_time;
}

static void solve_3(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_3: bad final_position\n");
    exit(1);
  }

  solution_3_elapsed_time = cpu_time();
  solution_3_count = 0ul;
  solution_3_best.n_moves = final_position + 100;
  solution_3_recursion(0, 0, 0, final_position);
  solution_3_elapsed_time = cpu_time() - solution_3_elapsed_time;
}

static void solve_4(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_4: bad final_position\n");
    exit(1);
  }

  solution_4_elapsed_time = cpu_time();
  solution_4_count = 0ul;
  solution_4_best.n_moves = final_position + 100;
  solution_4_non_recursion(final_position);
  solution_4_elapsed_time = cpu_time() - solution_4_elapsed_time;
}

static void solve_5(int final_position)
{
  if (final_position < 1 || final_position > _max_road_size_)
  {
    fprintf(stderr, "solve_5: bad final_position\n");
    exit(1);
  }
  solution_5_elapsed_time = cpu_time();
  solution_5_count = 0ul;
  solution_5_best.n_moves = final_position + 100;
  solution_5.n_moves = solution_5_listas_guardadas.n_moves;
  for (int i = 0; i <= final_position; i++)
  {
    solution_5.positions[i] = solution_5_listas_guardadas.positions[i];
  }
  guardar_listas = 0;
  listas_guardadas = 0;
  solution_5_dynamic(solution_5_listas_guardadas.n_moves, last_position, last_speed, final_position);
  solution_5_elapsed_time = cpu_time() - solution_5_elapsed_time;
}

//
// main program
//

int main(int argc, char *argv[argc + 1])
{
#define _time_limit_ 3600.0
  int n_mec, final_position, print_this_one;
  char file_name[64];

  // initialization
  n_mec = (argc < 2) ? 0xAED2022 : atoi(argv[1]);
  srandom((unsigned int)n_mec);
  init_road_speeds();
  // run all solution methods for all interesting sizes of the problem
  final_position = 1;
  solution_1_elapsed_time = 0.0;
  printf("    + --- ---------------- --------- +\n");
  printf("    |                plain recursion |\n");
  printf("--- + --- ---------------- --------- +\n");
  printf("  n | sol            count  cpu time |\n");
  printf("--- + --- ---------------- --------- +\n");
  while (final_position <= _max_road_size_ /* && final_position <= 20 */)
  {
    print_this_one = (final_position == 10 || final_position == 20 || final_position == 50 || final_position == 100 || final_position == 200 || final_position == 400 || final_position == 800) ? 1 : 0;
    printf("%3d |", final_position);

    // first solution method
#if defined solution1
    if (solution_1_elapsed_time < _time_limit_)
    {
      solve_1(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_1.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_1_best.n_moves, &solution_1_best.positions[0], solution_1_elapsed_time, solution_1_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_1_best.n_moves, solution_1_count, solution_1_elapsed_time);
    }
    else
    {
      solution_1_best.n_moves = -1;
      printf("                                |");
    }
#endif

    // second solution method
#if defined solution2
    if (solution_2_elapsed_time < _time_limit_)
    {
      solve_2(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_2.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_2_best.n_moves, &solution_2_best.positions[0], solution_2_elapsed_time, solution_2_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_2_best.n_moves, solution_2_count, solution_2_elapsed_time);
    }
    else
    {
      solution_2_best.n_moves = -1;
      printf("                                |");
    }
#endif

    // third solution method
#if defined solution3
    if (solution_3_elapsed_time < _time_limit_)
    {
      solve_3(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_3.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_3_best.n_moves, &solution_3_best.positions[0], solution_3_elapsed_time, solution_3_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_3_best.n_moves, solution_3_count, solution_3_elapsed_time);
    }
    else
    {
      solution_3_best.n_moves = -1;
      printf("                                |");
    }
#endif

    // fourth solution method
#if defined solution4
    if (solution_4_elapsed_time < _time_limit_)
    {
      solve_4(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_4.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_4_best.n_moves, &solution_4_best.positions[0], solution_4_elapsed_time, solution_4_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_4_best.n_moves, solution_4_count, solution_4_elapsed_time);
    }
    else
    {
      solution_4_best.n_moves = -1;
      printf("                                |");
    }
#endif

    // fifth solution method
#if defined solution5
    if (solution_5_elapsed_time < _time_limit_)
    {
      solve_5(final_position);
      if (print_this_one != 0)
      {
        sprintf(file_name, "%03d_5.pdf", final_position);
        make_custom_pdf_file(file_name, final_position, &max_road_speed[0], solution_5_best.n_moves, &solution_5_best.positions[0], solution_5_elapsed_time, solution_5_count, "Plain recursion");
      }
      printf(" %3d %16lu %9.3e |", solution_5_best.n_moves, solution_5_count, solution_5_elapsed_time);
    }
    else
    {
      solution_5_best.n_moves = -1;
      printf("                                |");
    }
#endif

    // done
    printf("\n");
    fflush(stdout);
    // new final_position
    if (final_position < 50)
      final_position += 1;
    else if (final_position < 100)
      final_position += 5;
    else if (final_position < 200)
      final_position += 10;
    else
      final_position += 20;
  }
  printf("--- + --- ---------------- --------- +\n");
  return 0;
#undef _time_limit_
}