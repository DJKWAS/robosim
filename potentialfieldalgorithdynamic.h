#ifndef POTENTIALFIELDALGORITHMDYNAMIC
#define POTENTIALFIELDALGORITHMDYNAMIC_H

#include "routingalgorithm.h"
#include "plot.h"

class PotentialFieldAlgorithmDynamic : public RoutingAlgorithm
{
    public:
        explicit PotentialFieldAlgorithmDynamic();
        virtual ~PotentialFieldAlgorithmDynamic();

        virtual void initialize( const Robot& robot ) override;
        virtual float run( const Robot& robot, const float elapsed ) override;
    private:
        Plot *PotFieldPlot;
        QPointF lastPosition;
};

#endif // DUMMYALGORITHM_H
