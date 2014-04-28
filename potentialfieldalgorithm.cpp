#include "potentialfieldalgorithm.h"
#include "routingalgorithmregistry.h"
#include "robot.h"

#include <math.h>
#include <QPointF>


static const StaticAlgorithmRegistration registrar( "PotentialField", [](){
    return std::unique_ptr< RoutingAlgorithm >( new PotentialFieldAlgorithm() );
} );

PotentialFieldAlgorithm::PotentialFieldAlgorithm()
{
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

    QPointF robPos(robot.x() + robot.frac_x(), robot.y() + robot.frac_y());

    QPointF goalPos(robot.goal_x() + 0.5f, robot.goal_y() + 0.5f);

    QPointF Fpull;

    QPointF dq = robPos - goalPos;
    Fpull = - (dq) / sqrt( pow(dq.x(),2) + pow(dq.y(),2) );


    QPointF Frepul(0,0);
    for(int y=0; y < robot.obstacle_map().height(); y++)
    {
        for(int x=0; x < robot.obstacle_map().width(); x++)
        {
            if (robot.obstacle_map().at(x,y) == ObstacleType::Wall )
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

    return angle;


//    const float x = robot.x() + robot.frac_x();
//    const float y = robot.y() + robot.frac_y();

//    const float goal_x = robot.goal_x() + 0.5f;
//    const float goal_y = robot.goal_y() + 0.5f;

//    const float dx = goal_x - x;
//    const float dy = goal_y - y;

//const float angle = atan2f( dy, dx );



//    return angle;
//   return -1;
}
