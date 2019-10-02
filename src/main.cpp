#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include "../headers/util.hpp"

#include "../headers/device_picker.hpp"
#include "../headers/reduce.hpp"
#include "../headers/tests.hpp"
//#define TEST2

int main(int argc,char* argv[])
{


	try
	{//************************************************************************************************
		/*	0 - GPU			1 - CPU	*/
		cl_uint devIndex = 0;

		vector<cl::Device> devList;
		unsigned numDevices = getDeviceList(devList);
		cl::Device dev 		= devList[devIndex];
		parseArguments(argc, argv, &devIndex);

		string devName;
		getDeviceName(dev,devName);

		cout<<"Device: "<<devName<<"\n";

		vector<cl::Device> currentDevice;
		currentDevice.push_back(dev);

		cl::Context context(currentDevice);
		cl::CommandQueue queue(context,dev,CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE|CL_QUEUE_PROFILING_ENABLE);
		cl::Program program(context, util::loadProgram("/home/thilina/system_studio/workspace/Test/src/reduce.cl"), true);

	//**********************************************************************************************

		test1(&context, &queue, &program);
	#ifdef TEST1
			int LOCALSIZE = 128;
			int GLOBALSIZE;

			int passed = 0;
			int failed = 0;

			FILE* dataPoints;
			dataPoints = fopen("dataPoints.csv","wt");

			for(int NWORKGROUPS = 1; NWORKGROUPS <= 1000; NWORKGROUPS ++)
			{
				GLOBALSIZE = LOCALSIZE * NWORKGROUPS;
				LENGTH = GLOBALSIZE * 5 ;
				vector<int> data(LENGTH);

				for(int j = 0; j < LENGTH; j++)
				{
					data[j] = rand() % 256;
				}

				devReduce(context,queue,program,data,&devResults, &devTime, GLOBALSIZE,LOCALSIZE);
				hostReduce(data,&hostResults,&hostTime);

				if(devResults == hostResults)
				{
					passed += 1;
					fprintf(dataPoints,"%d,%f,%f\n", NWORKGROUPS, LENGTH/devTime, LENGTH/hostTime);
				}
				else
				{
					failed += 1;
				}

/*cout << "GPU Result: "<< devResults <<"\t"<< "GPU Time: "<< devTime << " ms\t";
				cout << "CPU Result: "<< hostResults <<"\t"<< "CPU Time: "<< hostTime << " ms\n";*/


			}
			cout << "Passed "<< passed <<"/"<<passed+failed <<"\n";
			cout << "Failed "<< failed <<"/"<<passed+failed;
			fclose(dataPoints);
	#endif

	#ifdef TEST2
	vector<int> LENGTH;
	LENGTH = {1048576};

	int passed;
	int failed;

	float threadLoad;
	int GLOBALSIZE;
	float iterations = 0;

	FILE* dataPoints;
	FILE* errorLog;

	dataPoints = fopen("test2/dataPoints.csv","wt");
	fprintf(dataPoints,"LocalSize,Load per Thread,GPU Time(ms),CPU Time(ms)\n");
	errorLog   = fopen("test2/errorLog.csv","wt");

	for(int i = 0; i < LENGTH.size(); i++)
	{
		vector<int> data(LENGTH[i]);
		passed = 0;
		failed = 0;

		for(int j = 0; j < LENGTH[i]; j++)
		{
			data[j] = rand() % 256;
		}

		for(int LOCALSIZE = 256; LOCALSIZE <= 256; LOCALSIZE *= 2)
		{
			for(int NWORKGROUPS = 2; NWORKGROUPS < 2048; NWORKGROUPS += 4)
			{
				threadLoad = LENGTH[i] / (float)(2 * LOCALSIZE * NWORKGROUPS);
				GLOBALSIZE = LOCALSIZE * NWORKGROUPS;

				devReduce(context,queue,program,data,&devResults, &devTime, GLOBALSIZE,LOCALSIZE);
				hostReduce(data,&hostResults,&hostTime);

				if(devResults == hostResults)
				{
					passed += 1;
					fprintf(dataPoints,"%d,%f,%f,%f\n",LOCALSIZE,threadLoad,devTime,hostTime);
				}
				else
				{
					failed += 1;
					fprintf(errorLog,"%d,%d,%d\n",LENGTH[i],LOCALSIZE,NWORKGROUPS);
				}
				iterations += 1;
			}

			cout << "Completed: "<< (iterations/((512-1)*1))*100<< " %\n";
		}
	}
	cout << "Passed: "<< passed << "\t";
	cout << "failed: "<< failed;

#endif

	}
	catch(cl::Error err)
	{
		 std::cout << "Exception\n";
		 std::cerr<< "ERROR: "<< err.what()<< "("<< err_code(err.err())<< ")"<< std::endl;
		 exit(1);
	}


	return 0;


}
