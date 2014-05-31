#include "potentialfieldalgorithm.h"
#include "routingalgorithmregistry.h"
#include "robot.h"

#include <math.h>
#include <QPointF>
#include <QDebug>


static const StaticAlgorithmRegistration registrar( "PotentialField", [](){
    return std::unique_ptr< RoutingAlgorithm >( new PotentialFieldAlgorithm() );
} );

PotentialFieldAlgorithm::PotentialFieldAlgorithm()
{
    PotFieldPlot = new Plot();
    PotFieldPlot->show();

    PotFieldPlot->registerSignalToPlot("Angle");

}

PotentialFieldAlgorithm::~PotentialFieldAlgorithm()
{
}

void PotentialFieldAlgorithm::initialize( const Robot& robot )
{

}

float PotentialFieldAlgorithm::run( const Robot& robot, const float elapsed )
{
    const float ksi = 1;
    static float time = -elapsed;
    static float lastTime = -0.5;

    QPointF robPos(robot.x() + robot.frac_x(), robot.y() + robot.frac_y());

    QPointF goalPos(robot.goal_x() + 0.5f, robot.goal_y() + 0.5f);

    QPointF Fpull;

    QPointF dq = robPos - goalPos;
    Fpull = - (dq) / sqrt( pow(dq.x(),2) + pow(dq.y(),2) );


    QPointF Frepul(0,0);
    for(unsigned int y=0; y < robot.obstacle_map().height(); y++)
    {
        for(unsigned int x=0; x < robot.obstacle_map().width(); x++)
        {
            if (robot.obstacle_map().at(x,y) != ObstacleType::None )
                    // ||robot.obstacle_map().at(x,y) == ObstacleType::Robot)
            {
              QPointF obstPos(x + 0.5f,y + 0.5f);
              dq = robPos -  obstPos;
              Frepul += dq / pow( sqrt( pow(dq.x(),2) + pow(dq.y(),2) ), 3);
            }
        }
    }

    QPointF Fres = 1.1 * Frepul + ksi * Fpull;

    const float angle = atan2f( Fres.y(), Fres.x() );

    time += elapsed;
    if (time - lastTime > 0.05)
    {
        PotFieldPlot->addData("Angle",QPointF(time,angle));
        lastTime = time;
    }


    return angle;

}
