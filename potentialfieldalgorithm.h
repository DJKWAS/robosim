#ifndef POTENTIALFIELDALGORITHM_H
#define POTENTIALFIELDALGORITHM_H

#include "routingalgorithm.h"
#include "plot.h"

class PotentialFieldAlgorithm : public RoutingAlgorithm
{
    public:
        explicit PotentialFieldAlgorithm();
        virtual ~PotentialFieldAlgorithm();

        virtual void initialize( const Robot& robot ) override;
        virtual float run( const Robot& robot, const float elapsed ) override;
    private:
        Plot *PotFieldPlot;
};

#endif // DUMMYALGORITHM_H
