#ifndef POTENTIALFIELDALGORITHM_H
#define POTENTIALFIELDALGORITHM_H

#include "routingalgorithm.h"

class PotentialFieldAlgorithm : public RoutingAlgorithm
{
    public:
        explicit PotentialFieldAlgorithm();
        virtual ~PotentialFieldAlgorithm();

        virtual void initialize( const Robot& robot ) override;
        virtual float run( const Robot& robot, const float elapsed ) override;
};

#endif // DUMMYALGORITHM_H
