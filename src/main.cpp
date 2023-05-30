

#include "grapher.hpp"

#include <cstdlib>
#include <iostream> 
#include <stdexcept>


int main() {
	neu::Logger log(std::cout, "bin/log.out");
	try {
		neu::Grapher gr(log);
		gr.run();
	} catch (vk::SystemError e) {
		log << neu::LogLevel::error << "vk: "<< e.what() << "\n";
	} catch (std::runtime_error e) {
		log << neu::LogLevel::error << e.what() << "\n";
	} catch (std::exception e) {
		log << neu::LogLevel::error << "exc: "<< e.what() << "\n";
	}
  
	return EXIT_SUCCESS;
}
