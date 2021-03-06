#ifndef FEATUREMATCH_H
#define FEATUREMATCH_H
#include "itrbase.h"
#include "../itrvision.h"

using itr_vision::VectorFeaturePoint;

namespace itr_vision
{
class FeatureMatch
{
public:
    FeatureMatch();
    virtual ~FeatureMatch();
    /**
      * \brief 特征向量匹配函数
      * \param vectorfeaturepoint1[] 特征点1
      * \param vectorfeaturepoint2[] 特征点2
      * \param num1 特征点长度1
      * \param num2 特征点长度2
      * \param matchnum 匹配的ID
      * \param pos1[] 坐标1
      * \param pos2[] 坐标2
      */
    void FeatureMatchDo( std::vector<VectorFeaturePoint>& vectorfeaturepoint1, std::vector<VectorFeaturePoint>&vectorfeaturepoint2,S32 &matchnum);
    virtual void Max(F32* SourceA, S32 Length, F32& ResultM, F32& ResultC,S32 &Order) const;
protected:
private:
};
}
#endif // FEATUREMATCH_H
