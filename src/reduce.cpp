#include "../headers/reduce.hpp"




void devReduce(cl::Context *context, cl::CommandQueue *queue,cl::Program *program,vector<int> &data,int* devResults, double *devTime,int globalSize,int localSize)
{
	try
	{
		cl::Kernel reduce;
		reduce = cl::Kernel(*program,"reduce");

		cl::Event event;
		cl_ulong startTime;
		cl_ulong endTime;
		*devTime = 0;

		int n = data.size();
		vector<int> gpuOut(1);
		int kernelInDataSize1;
		int kernelInDataSize2;

		bool lev1 = true;
		bool lev2 = false;
		bool lev3 = false;

		int global = globalSize;
		int local  = localSize;
		/* **************************LEVEL 1****************************************** */

		//Check whether data size is a multiple of 2 * globalSize
		kernelInDataSize1 = (n % (2 * globalSize))== 0 ? n : ((n / (2 * globalSize))+ 1) * (2 * globalSize);
		data.resize(kernelInDataSize1);

		int partialLength =  (globalSize / localSize) == 1 ? 1 : pow(2, ceil(log2(globalSize / localSize)));
		vector<int> partial(partialLength);

		cl::Buffer dataBuffer  = cl::Buffer(*context,CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,sizeof(int) * kernelInDataSize1 , &data[0]);
		cl::Buffer partialSums = cl::Buffer(*context,CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * partialLength, &partial[0]);

		reduce.setArg(0,dataBuffer);
		reduce.setArg(1,partialSums);
		reduce.setArg(3,cl::Local(sizeof(int) * localSize));
		reduce.setArg(2,kernelInDataSize1);

		queue->enqueueNDRangeKernel(reduce,cl::NullRange,cl::NDRange(global),cl::NDRange(local),NULL,&event);
		event.wait();
		event.getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_START,&startTime);
		event.getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_END,&endTime);
		*devTime += (double)(endTime-startTime);
		/* *************************LEVEL2*********************************************** */
		//turn on or off the level 2 kernels
		lev2 = ((globalSize / localSize)!= 1);
		if(lev2)
		{
			global = (globalSize / localSize) > 2048 ? 1024 : pow(2, ceil(log2(globalSize / localSize))) / 2;
			local  = global;
			kernelInDataSize2 = partialLength;

			reduce.setArg(1,dataBuffer);
			reduce.setArg(0,partialSums);
			reduce.setArg(3,cl::Local(sizeof(int) * local));
			reduce.setArg(2,kernelInDataSize2);

			queue->enqueueNDRangeKernel(reduce,cl::NullRange,cl::NDRange(global),cl::NDRange(local),NULL,&event);
			event.wait();
			event.getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_START,&startTime);
			event.getProfilingInfo<cl_ulong>(CL_PROFILING_COMMAND_END,&endTime);
			*devTime += (double)(endTime-startTime);
		}


		*devTime = *devTime / 1000000;

		if(lev2)
		{
			queue->enqueueReadBuffer(dataBuffer,CL_TRUE,0,sizeof(cl_int) ,(int*)&gpuOut[0],NULL,NULL);
		}
		else
		{
			queue->enqueueReadBuffer(partialSums,CL_TRUE,0,sizeof(cl_int) ,(int*)&gpuOut[0],NULL,NULL);
		}


		#ifdef 	DEV_REDUCE_DEBUG
		cout << "GPU output: "
		for(int i = 0; i < 4; i++)
		{
			cout<<gpuOut[i]<<" ";
		}
		cout << "\n";
		#endif

		*devResults = gpuOut[0];
		data.resize(n);
	}


	catch(cl::Error err)
	{
			 std::cout << "Exception\n";
			            std::cerr
			            << "ERROR: "
			            << err.what()
			            << "("
			            << err_code(err.err())
			            << ") "
			            << std::endl;
			  exit(1);
	}
}

void hostReduce(vector<int> &data, int* hostResults, double *hostTime)
{
	auto start  = chrono::high_resolution_clock::now();
	*hostResults = 0;
	for(int i = 0; i < data.size(); i++)
	{
		*hostResults += data[i];
	}
	auto end	= chrono::high_resolution_clock::now();

	*hostTime = chrono::duration<double,milli>(end - start).count();
}

