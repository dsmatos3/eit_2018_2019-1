#include "netxpto_20180418.h"
#include <stdio.h>

#include "binary_source_20180523.h"
#include "sink_20180118.h"
#include "fork_20180112.h"

#include "huffman_encoder_20180621.h"
#include "huffman_decoder_20180621.h"
#include "source_code_efficiency_20180621.h"
#include "entropy_estimator_20180621.h"

int main()
{
	// #####################################################################################################
	// ################################# System Input Parameters #########################################
	// #####################################################################################################
	int sourceOrderX2 = 2;
	int sourceOrderX3 = 3;
	int sourceOrderX4 = 4;
	double probabilityOfZero = 0.05;

	// #####################################################################################################
	// ################################# Simulation Parameters #########################################
	// #####################################################################################################


	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################
	Binary S0   { "S0.sgn" };
	Binary S0_0 { "S0_0.sgn" };
	Binary S0_1 { "S0_1.sgn" };
	Binary S0_2 { "S0_2.sgn" };
	Binary S0_3 { "S0_3.sgn" };
	Binary S0_4 { "S0_4.sgn" };
	Binary S0_5 { "S0_5.sgn" };

	Binary S1   { "S1.sgn" };
	Binary S1_1 { "S1_1.sgn" };
	Binary S1_2 { "S1_2.sgn" };

	Binary S2   { "S2.sgn" };

	Binary S3   { "S3.sgn" };
	Binary S3_1 { "S3_1.sgn" };
	Binary S3_2 { "S3_2.sgn" };

	Binary S4   { "S4.sgn" };

	Binary S5   { "S5.sgn" };
	Binary S5_1 { "S5_1.sgn" };
	Binary S5_2 { "S5_2.sgn" };

	Binary S6   { "S6.sgn" };

	TimeContinuousAmplitudeContinuousReal X2 { "X2.sgn" };
	TimeContinuousAmplitudeContinuousReal X3 { "X3.sgn" };
	TimeContinuousAmplitudeContinuousReal X4 { "X4.sgn" };

	TimeContinuousAmplitudeContinuousReal ENT { "ENT.sgn" };

	S0.setSaveSignal(true);
	S0_0.setSaveSignal(false);
	S0_1.setSaveSignal(false);
	S0_2.setSaveSignal(false);
	S0_3.setSaveSignal(false);
	S0_4.setSaveSignal(false);
	S0_5.setSaveSignal(false);

	S1.setSaveSignal(true);
	S1_1.setSaveSignal(false);
	S1_2.setSaveSignal(false);

	S2.setSaveSignal(true);

	S3.setSaveSignal(true);
	S3_1.setSaveSignal(false);
	S3_2.setSaveSignal(false);

	S4.setSaveSignal(true);

	S5.setSaveSignal(true);
	S5_1.setSaveSignal(false);
	S5_2.setSaveSignal(false);

	S6.setSaveSignal(true);

	X2.setSaveSignal(true);
	X3.setSaveSignal(true);
	X4.setSaveSignal(true);

	ENT.setSaveSignal(true);

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ##################################
	// #####################################################################################################

	BinarySource B0{ vector <Signal*> {}, vector <Signal*> {&S0} };
	B0.setBitPeriod(1);
	B0.setNumberOfBits(480000); 
	B0.setProbabilityOfZero(probabilityOfZero);
	B0.setMode(Random);
	//B0.setMode(DeterministicCyclic);
	//B0.setBitStream("001001010001"); 
	
	Fork F0{ vector <Signal*> {&S0}, vector <Signal*> {&S0_0, &S0_1}};
	Fork F1{ vector <Signal*> {&S0_0}, vector <Signal*> {&S0_2, &S0_3}};
	Fork F1_1{ vector <Signal*> {&S0_3}, vector <Signal*> {&S0_4, &S0_5}};

	EntropyEstimator E1{ vector <Signal*> {&S0_4}, vector <Signal*> {&ENT}};

	Sink B0_0(vector<Signal*>{&ENT}, vector <Signal*>{});
	B0_0.setDisplayNumberOfSamples(false);

	// Source Order 2
	HuffmanEncoder H1{ vector <Signal*> {&S0_1}, vector <Signal*> {&S1} };
	H1.setProbabilityOfZero(probabilityOfZero);
	H1.setSourceOrder(sourceOrderX2);

	Fork F2{ vector <Signal*> {&S1}, vector <Signal*> {&S1_1, &S1_2} };

	SourceCodeEfficiency H2{ vector <Signal*> {&S1_1}, vector <Signal*> {&X2} };
	H2.setProbabilityOfZero(probabilityOfZero);
	H2.setSourceOrder(sourceOrderX2);

	HuffmanDecoder H3{ vector <Signal*> {&S1_2}, vector <Signal*> {&S2} };
	H3.setProbabilityOfZero(probabilityOfZero);
	H3.setSourceOrder(sourceOrderX2);

	Sink B1(vector<Signal*>{&X2}, vector <Signal*>{});
	B1.setDisplayNumberOfSamples(false);
	Sink B2(vector<Signal*>{&S2}, vector <Signal*>{});
	B2.setDisplayNumberOfSamples(false);

	// Source Order 3 
	HuffmanEncoder H4{ vector <Signal*> {&S0_2}, vector <Signal*> {&S3} };
	H4.setProbabilityOfZero(probabilityOfZero);
	H4.setSourceOrder(sourceOrderX3);

	Fork F3{ vector <Signal*> {&S3}, vector <Signal*> {&S3_1, &S3_2} };

	SourceCodeEfficiency H5{ vector <Signal*> {&S3_1}, vector <Signal*> {&X3} };
	H5.setProbabilityOfZero(probabilityOfZero);
	H5.setSourceOrder(sourceOrderX3);

	HuffmanDecoder H6{ vector <Signal*> {&S3_2}, vector <Signal*> {&S4} };
	H6.setProbabilityOfZero(probabilityOfZero);
	H6.setSourceOrder(sourceOrderX3);

	Sink B3(vector<Signal*>{&X3}, vector <Signal*>{});
	B3.setDisplayNumberOfSamples(false);
	Sink B4(vector<Signal*>{&S4}, vector <Signal*>{});
	B4.setDisplayNumberOfSamples(false);

	///Source Order 4
	HuffmanEncoder H7{ vector <Signal*> {&S0_5}, vector <Signal*> {&S5} };
	H7.setProbabilityOfZero(probabilityOfZero);
	H7.setSourceOrder(sourceOrderX4);

	Fork F4{ vector <Signal*> {&S5}, vector <Signal*> {&S5_1, &S5_2} };

	SourceCodeEfficiency H8{ vector <Signal*> {&S5_1}, vector <Signal*> {&X4} };
	H8.setProbabilityOfZero(probabilityOfZero);
	H8.setSourceOrder(sourceOrderX4);

	HuffmanDecoder H9{ vector <Signal*> {&S5_2}, vector <Signal*> {&S6} };
	H9.setProbabilityOfZero(probabilityOfZero);
	H9.setSourceOrder(sourceOrderX4);

	Sink B5(vector<Signal*>{&X4}, vector <Signal*>{});
	B5.setDisplayNumberOfSamples(false);
	Sink B6(vector<Signal*>{&S6}, vector <Signal*>{});
	B6.setDisplayNumberOfSamples(false);
	
	
	// ####################################################################################################
	// ########################### System Declaration and Inicialization ##################################
	// #####################################################################################################

	System MainSystem{ vector <Block*> {&B0, &F0, &F1, &F1_1, &E1, &B0_0, &H1, &F2, &H2, &H3, &B1, &B2, &H4, &F3, &H5, &H6, &B3, &B4, &H7, &F4, &H8, &H9, &B5, &B6} };

	// ####################################################################################################
	// ########################### Run ##################################
	// #####################################################################################################

	MainSystem.run();

	return 0;
}