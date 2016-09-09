#include "ExtDigitalOutput.h"
#include "Constants.hpp"

void TestChannelsExtChannels(ExtDigitalOutput output) {

	output.write(E1, 1);
	delay(500);
	output.write(E1, 0);

	output.write(E2, 1);
	delay(500);
	output.write(E2, 0);

	output.write(E3, 1);
	delay(500);
	output.write(E3, 0);

	output.write(E4, 1);
	delay(500);
	output.write(E4, 0);

	output.write(E5, 1);
	delay(500);
	output.write(E5, 0);

	output.write(E6, 1);
	delay(500);
	output.write(E6, 0);

	output.write(E7, 1);
	delay(500);
	output.write(E7, 0);

	output.write(E8, 1);
	delay(500);
	output.write(E8, 0);

	output.write(E9, 1);
	delay(500);
	output.write(E9, 0);

	output.write(E10, 1);
	delay(500);
	output.write(E10, 0);

	output.write(E11, 1);
	delay(500);
	output.write(E11, 0);

	output.write(E12, 1);
	delay(500);
	output.write(E12, 0);

	output.write(E13, 1);
	delay(500);
	output.write(E13, 0);

	// output.write(E14, 1);
	// delay(500);
	// output.write(E14, 0);
}
