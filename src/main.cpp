#include "../include/window.h"

int main(int argc, char** argv)
{
  Window window("Superficies", argc, argv);
  window.render();
  return (0);
}
