#include "netxpto_20180418.h"
#include <stdio.h>

#include "binary_source_20180523.h"
#include "add_20180620.h"
#include "sink_20180118.h"
#include "bit_error_rate_20180424.h"
#include "fork_20180112.h"
#include "hamming_encoder_20180608.h"
#include "hamming_decoder_20180608.h"

/* Hamming encoder decoder*/
int main(void) {
	// #####################################################################################################
	// ################################# System Input Parameters ###########################################
	// #####################################################################################################

	double probalilityOfZero_ErrorVector		= 0.99;
	/* Variable used to implement errors to simulate a channel. The probability of error is p1 = 1 - p0. We 
	   define the probability of p0. */

	double probalilityOfZero_Source				= 0.5;
	/* Defines the relation between ones and zeros in the creation of data Signal */

	int hammingCode_nBits						= 7;
	int hammingCode_kBits						= 4;
	/* This variable defines the type of hamming code to use in the encoding and decoding blocks. 
		Valid values for this input:
		parity bits -> 2     Total bits:   3	Data bits:   1	Hamming Code (  3,   1)		Rate: 1  /  3
		parity bits -> 3     Total bits:   7	Data bits:   4	Hamming Code (  7,   4)		Rate: 4  /  7
		parity bits -> 4     Total bits:  15	Data bits:  11	Hamming Code ( 15,  11)		Rate: 11 / 15
		parity bits -> 5     Total bits:  31	Data bits:  26	Hamming Code ( 31,  26)		Rate: 26 / 31
		parity bits -> 6     Total bits:  63	Data bits:  57	Hamming Code ( 63,  57)		Rate: 57 / 63
		parity bits -> 7     Total bits: 127	Data bits: 120	Hamming Code (127, 120)		Rate: 120/127
		parity bits -> 8     Total bits: 255	Data bits: 247	Hamming Code (255, 247)		Rate: 247/255 */



	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary S0{ "S0.sgn" };				/* Source Signal  */
	Binary S0_1{ "S0_1.sgn" };			/* Source Signal - Copy 1 */
	Binary S0_2{ "S0_2.sgn" };			/* Source Signal - Copy 2 */
	Binary S1{ "S1.sgn" };				/* Encoded Signal */
	Binary S1_1{ "S1_1.sgn" };			/* Encoded Signal - Copy 1 */
	Binary S1_2{ "S1_2.sgn" };			/* Encoded Signal - Copy 2 */
	Binary S2{ "S2.sgn" };				/* Noise Signal */
	Binary S3{ "S3.sgn" };				/* Noise + Encoded Signal */
	Binary S3_1{ "S3_1.sgn" };			/* Encoded Signal - Copy 1 */
	Binary S3_2{ "S3_2.sgn" };			/* Encoded Signal - Copy 2 */
	Binary S4{ "S4.sgn" };				/* Decoded Signal */
	Binary S5{ "S5.sgn" };				/* BER Signal Channel*/
	Binary S6{ "S6.sgn" };				/* BER Signal */

	/* Save Signal */
	S0.setSaveSignal(true);
	S0_1.setSaveSignal(false);
	S0_2.setSaveSignal(false);
	S1.setSaveSignal(true);
	S1_1.setSaveSignal(false);
	S1_2.setSaveSignal(false);
	S2.setSaveSignal(true);
	S3.setSaveSignal(true);
	S3_1.setSaveSignal(false);
	S3_2.setSaveSignal(false);
	S4.setSaveSignal(true);
	S5.setSaveSignal(true);



	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	/* Source Signal */
	BinarySource B1(vector <Signal*> {}, vector <Signal*> {&S0});
	B1.setBitPeriod(1);
	B1.setMode(Random);
	B1.setProbabilityOfZero(probalilityOfZero_Source);
	B1.setNumberOfBits(10000);
	/* Use a predefined sequence that repeats cyclically */
	//B1.setMode(DeterministicCyclic);
	//B1.setBitStream("1010");

	/* Create a copy of S0 Signal */
	Fork F1(vector <Signal*> {&S0}, vector <Signal*> {&S0_1, &S0_2});

	/* Hamming Encoder */
	HammingCoder H1(vector <Signal*> {&S0_1}, vector <Signal*> {&S1});
	H1.setNBits(hammingCode_nBits);
	H1.setKBits(hammingCode_kBits);

	Fork F2(vector <Signal*> {&S1}, vector <Signal*> {&S1_1, &S1_2});

	/* Noise Signal - Introduces errors with percentage of P1 = 1 - P0 */
	BinarySource B2(vector <Signal*> {}, vector <Signal*> {&S2});
	B2.setMode(Random);
	B2.setProbabilityOfZero(probalilityOfZero_ErrorVector);
	B2.setBitPeriod(1);
	/* Use a predefined sequence that repeats cyclically */
	//B2.setMode(DeterministicCyclic);
	//B2.setBitStream("0000000000000000001000000000000000010000");

	/* Add Noise Signal to the Encoded Signal */
	Add A1(vector <Signal*> {&S1_1, &S2}, vector <Signal*> {&S3});
	
	Fork F3(vector <Signal*> {&S3}, vector <Signal*> {&S3_1, &S3_2});

	/* BER */
	//BitErrorRate B3(vector <Signal*> {&S1_2, &S3_1}, vector <Signal*> {&S5});

	/* Hamming Decoder */
	HammingDeCoder H2(vector <Signal*> {&S3_2}, vector <Signal*> {&S4});
	H2.setNBits(hammingCode_nBits);
	H2.setKBits(hammingCode_kBits);
	
	/* BER */
	BitErrorRate B4(vector <Signal*> {&S0_2, &S4}, vector <Signal*> {&S6});

	/* Flush the BER Channel signal */
	Sink B5(vector <Signal*> {&S1_2}, vector <Signal*> {});
	B5.setDisplayNumberOfSamples(true);
	/* Flush the output signal */
	Sink B6(vector <Signal*> {&S3_1}, vector <Signal*> {});
	B6.setDisplayNumberOfSamples(true);
	/* Flush the BER Channel signal */
	Sink B7(vector <Signal*> {&S6}, vector <Signal*> {});
	B7.setDisplayNumberOfSamples(true);


	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector <Block*> {&B1, &F1, &H1, &F2, &B2, &A1, &F3, /*&B3,*/ &H2, &B4, &B5, &B6, &B7}};



	// #####################################################################################################
	// ########################### Run #####################################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;
}
