__kernel void reduce(__global int* data,
					 __global int* partialSums,
					 int n,
					 __local int* localData)
{
	int gid 	= get_global_id(0);
	int lid 	= get_local_id(0);
	int groupid = get_group_id(0);
	
	int localSize	= get_local_size(0);
	int globalSize	= get_global_size(0);
	
	int nWorkGroups    = globalSize / localSize;
	int nSets 		   = n / (globalSize);
	int nworkGroupData = n / (nWorkGroups);
	
	localData[lid] = 0;
	
	for(int i = 0; i < nSets; i += 2 )
	{
		localData[lid] += (data[lid + i * localSize + groupid * nworkGroupData  ] + data[lid + (i+1) * localSize + groupid * nworkGroupData ]);
		barrier(CLK_LOCAL_MEM_FENCE);	
	}	
	
	
	for(int offset = localSize/2; offset > 0; offset /= 2)
	{	
		if(lid < offset)
		{
			localData[lid] += localData[lid+offset];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	if(lid == 0)
	{
		partialSums[groupid] = localData[lid];
	}
		
		
}