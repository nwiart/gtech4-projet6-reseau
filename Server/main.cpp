#include "Networking.h"


int main(int argc, const char** argv)
{
	network::initializeWinsock();



	network::cleanupWinsock();

	return 0;
}
