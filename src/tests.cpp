#include "../headers/tests.hpp"

void test1(cl::Context *context, cl::CommandQueue *queue, cl::Program *program)
{
	int devResults;
	int hostResults;

	double hostTime;
	double devTime;

	vector<int> LENGTH;
	
	LENGTH  = {1000000, 2000000,4000000,8000000,100000, 200000, 400000, 800000};
	
	vector<double> mean(6);

	int passed = 0;
	int failed = 0;

	float threadLoad;
	int GLOBALSIZE;
	float iterations = 0;
	int maxWorkGroupSize;
	int stepSize;

	ofstream dataPoints;
	ofstream errorLog;


	for(int i = 4; i < LENGTH.size(); i++)
	{
		vector<int> data(LENGTH[i]);

		string fileNum 		  = to_string(i);
		string fileNameCommon = "tests/test1/dataSizes/dataSize";
		fileNameCommon.append(fileNum);
		fileNameCommon.append(".csv");
		dataPoints.open(fileNameCommon, ios::out | ios::trunc );
		dataPoints << "Load per Thread,GPU Time(ms)\n";

		string fileNameError = "tests/test1/errorLogs/errorLog";
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
			stepSize = maxWorkGroupSize / 45;

			for(int NWORKGROUPS = 2; NWORKGROUPS < maxWorkGroupSize; NWORKGROUPS += stepSize)
			{
				threadLoad = LENGTH[i] / (float)(2 * LOCALSIZE * NWORKGROUPS);
				GLOBALSIZE = LOCALSIZE * NWORKGROUPS;

				for(int count = 0; count < 6; count ++)
				{
					devReduce(context,queue,program,data,&devResults, &devTime, GLOBALSIZE,LOCALSIZE);
					mean[count] = devTime;
				}
				sort(mean.begin(), mean.end());
				devTime = (mean[2] + mean[3]) / 2.0;

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
