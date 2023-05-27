

#include "logger.hpp"

#include <cstdlib>
#include <iostream> 
#include <stdexcept>


int main() {
  neu::Logger log(std::cout, "bin/log.out");
  
  
  return EXIT_SUCCESS;
}
