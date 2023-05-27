

#include "grapher.hpp"

#include <cstdlib>
#include <iostream> 
#include <stdexcept>


int main() {
	neu::Logger log(std::cout, "bin/log.out");
	try {
		neu::Grapher gr(log);
	} catch (vk::SystemError e) {
		log << neu::LogLevel::error << "vk: "<< e.what() << "\n";
	} catch (std::exception e) {
		log << neu::LogLevel::error << e.what() << "\n";
	}
  
	return EXIT_SUCCESS;
}
