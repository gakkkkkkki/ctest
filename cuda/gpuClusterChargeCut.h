#ifndef RecoLocalTracker_SiPixelClusterizer_plugins_gpuClusterChargeCut_h
#define RecoLocalTracker_SiPixelClusterizer_plugins_gpuClusterChargeCut_h

#include <cassert>
#include <cstdint>
#include <cstdio>

#include "gpuClusteringConstants.h"

#include "prefixScan.h"

namespace gpuClustering {

  __global__ void  clusterChargeCut(
                           uint16_t * __restrict__ id,             // module id of each pixel (modified if bad cluster)
                           uint16_t const * __restrict__ adc,              //  charge of each pixel
                           uint32_t const * __restrict__ moduleStart,    // index of the first pixel of each module
                           uint32_t * __restrict__ nClustersInModule,    // modified: number of clusters found in each module
                           uint32_t const * __restrict__ moduleId,             // module id of each module
                           int32_t * __restrict__  clusterId,            // modified: cluster id of each pixel
                           int numElements)
  {

    if (blockIdx.x >= moduleStart[0])
      return;

    auto firstPixel = moduleStart[1 + blockIdx.x];
    auto thisModuleId = id[firstPixel];
    assert(thisModuleId < MaxNumModules);
    assert(thisModuleId==moduleId[blockIdx.x]);

    auto nclus = nClustersInModule[thisModuleId];
    if (nclus==0) return;

    assert(nclus<=MaxNumClustersPerModules);

#ifdef GPU_DEBUG
    if (thisModuleId % 100 == 1)
      if (threadIdx.x == 0)
        printf("start clusterizer for module %d in block %d\n", thisModuleId, blockIdx.x);
#endif

    auto first = firstPixel + threadIdx.x;

    // find the index of the first pixel not belonging to this module (or invalid)
    __shared__ int msize;
    msize = numElements;
    __syncthreads();

    // skip threads not associated to an existing pixel
      for (int i = first; i < numElements; i += blockDim.x) {
        if (id[i] == InvId)                 // skip invalid pixels
          continue;
        if (id[i] != thisModuleId) {        // find the first pixel in a different module
          atomicMin(&msize, i);
          break;
        }
      }

//    __syncthreads();

    __shared__ int32_t charge[MaxNumClustersPerModules];
    for (int i=threadIdx.x; i<nclus; i += blockDim.x) {
      charge[i]=0;
    }
    __syncthreads();

    assert(msize<=numElements);

    for (int i = first; i < msize; i += blockDim.x) {
      if (id[i] == InvId) continue;     // not valid
      atomicAdd(&charge[clusterId[i]], adc[i]);
    }
    __syncthreads();

    auto chargeCut = thisModuleId<96 ? 2000 : 4000; // move in constants (calib?)
    __shared__ uint8_t ok[MaxNumClustersPerModules];
    __shared__ uint16_t newclusId[MaxNumClustersPerModules];
    for (int i=threadIdx.x; i<nclus; i += blockDim.x) {
       newclusId[i] = ok[i] =  charge[i]>chargeCut ? 1 : 0;
    }

    __syncthreads();

    // renumber
    __shared__ uint16_t ws[32];
    blockPrefixScan(newclusId, nclus, ws);

    // assert(nclus>=newclusId[nclus-1]);
    // __syncthreads();

    // if(nclus==newclusId[nclus-1]) return;
    // __syncthreads();

    nClustersInModule[thisModuleId] = newclusId[nclus-1];
    __syncthreads();

    // mark bad cluster again
    for (int i=threadIdx.x; i<nclus; i += blockDim.x) {
      if (0==ok[i]) newclusId[i]=InvId+1;
    }
    __syncthreads();

    // reassign id
    for (int i = first; i < msize; i += blockDim.x) {
      if (id[i] == InvId) continue;     // not valid
      clusterId[i] = newclusId[clusterId[i]]-1;
      if(clusterId[i]==InvId) id[i] = InvId;
    }

    //done
  }


} // namespace
#endif
