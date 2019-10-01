#include "../headers/tests.hpp"

void test1(cl::Context *context, cl::CommandQueue *queue, cl::Program *program)
{
	int devResults;
	int hostResults;

	double hostTime;
	double devTime;

	vector<int> LENGTH;
	LENGTH = {1048576,2097152,4194304};

	int passed;
	int failed;

	float threadLoad;
	int GLOBALSIZE;
	float iterations = 0;
	int maxWorkGroupSize;
	int stepSize;

	ofstream dataPoints;
	ofstream errorLog;


	for(int i = 0; i < LENGTH.size(); i++)
	{
		vector<int> data(LENGTH[i]);
		passed = 0;
		failed = 0;

		string fileNum 		  = to_string(i);
		string fileNameCommon = "tests/test1/dataSize";
		fileNameCommon.append(fileNum);
		fileNameCommon.append(".csv");
		dataPoints.open(fileNameCommon, ios::out | ios::trunc );
		dataPoints << "Load per Thread,GPU Time(ms)\n";

		string fileNameError = "tests/test1/errorLog";
		fileNameError.append(fileNum);
		fileNameError.append(".csv");
		errorLog.open(fileNameError, ios::out | ios:: trunc);

		//cout <<"yes\n";
		for(int j = 0; j < LENGTH[i]; j++)
		{
			data[j] = rand() % 256;
		}

		for(int LOCALSIZE = 2; LOCALSIZE <= 1024; LOCALSIZE *= 2)
		{
			cout << "Running: "<<"DataSize: "<< i<<"\t"<<"LocalSize: "<< LOCALSIZE<< "\n";

			maxWorkGroupSize = LENGTH[i]/(2 * LOCALSIZE);
			stepSize = maxWorkGroupSize / 250;

			for(int NWORKGROUPS = 2; NWORKGROUPS < maxWorkGroupSize; NWORKGROUPS += stepSize)
			{
				threadLoad = LENGTH[i] / (float)(2 * LOCALSIZE * NWORKGROUPS);
				GLOBALSIZE = LOCALSIZE * NWORKGROUPS;

				devReduce(context,queue,program,data,&devResults, &devTime, GLOBALSIZE,LOCALSIZE);
				hostReduce(data,&hostResults,&hostTime);

				if(devResults == hostResults)
				{
					passed += 1;
					dataPoints << threadLoad << "," << devTime << "\n";
				}
				else
				{
					failed += 1;
					errorLog <<LENGTH[i] << "," << LOCALSIZE << "," << NWORKGROUPS << "\n";
				}
				iterations += 1;
			}
			dataPoints <<"\n";
		}
		dataPoints.close();
		errorLog.close();
	}
	cout << "Passed: "<< passed << "\t";
	cout << "failed: "<< failed;
}
