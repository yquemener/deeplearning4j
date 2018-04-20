//
// @author Yurii Shyrma (iuriish@yahoo.com), created on 05.02.2018
//

#include <ops/declarable/CustomOperations.h>
#include<ops/declarable/helpers/meshgrid.h>
#include <numeric>

namespace nd4j {
namespace ops  {

CUSTOM_OP_IMPL(meshgrid, -1, -1, false, 0, 0) {    
    
    int rank = block.width();

    if(rank == 1) {
        OUTPUT_VARIABLE(0)->assign(INPUT_VARIABLE(0));
        return Status::OK();
    }

    bool swapFirst2Dims = block.getIArguments()->size() > 0 ? (bool)INT_ARG(0) : true;

    std::vector<NDArray<T>*> inArrs(rank);
    std::vector<NDArray<T>*> outArrs(rank);

    for(int i = 0; i < rank; ++i) {        
        inArrs[i]  = INPUT_VARIABLE(i);
        outArrs[i] = OUTPUT_VARIABLE(i);
    }

    helpers::meshgrid<T>(inArrs, outArrs, swapFirst2Dims);

    return Status::OK();
}



DECLARE_SHAPE_FN(meshgrid) {

    bool swapFirst2Dims = block.getIArguments()->size() > 0 ? (bool)INT_ARG(0) : true;
    
    int rank = block.width();
    int* outShapeInfo = nullptr;
    ALLOCATE(outShapeInfo, block.getWorkspace(), shape::shapeInfoLength(rank), int);
    outShapeInfo[0] = rank;    
    for(int i = 1; i <= rank; ++i)
        outShapeInfo[i] = (int)shape::length(inputShape->at(i - 1));
    
    if(swapFirst2Dims && rank > 1)
        math::nd4j_swap<int>(outShapeInfo[1], outShapeInfo[2]);
    
    shape::updateStrides(outShapeInfo, shape::order(inputShape->at(0)));

    auto shapes = SHAPELIST();
    shapes->push_back(outShapeInfo);
    
    int* tempShapeInfo = nullptr;
    for(int i = 2; i <= rank; ++i) {
        COPY_SHAPE(outShapeInfo, tempShapeInfo);
        shapes->push_back(tempShapeInfo);
    }
    
    return shapes;
}




}
}