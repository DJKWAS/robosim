#include "potentialfieldalgorithdynamic.h"
#include "routingalgorithmregistry.h"
#include "robot.h"

#include <math.h>
#include <QPointF>
#include <QDebug>


static const StaticAlgorithmRegistration registrar( "PotentialFieldDynamic",
                                                    [](){
    return std::unique_ptr< RoutingAlgorithm >( new PotentialFieldAlgorithmDynamic() );
} );

PotentialFieldAlgorithmDynamic::PotentialFieldAlgorithmDynamic()
{
    PotFieldPlot = new Plot();
    PotFieldPlot->show();
    lastPosition = QPointF(-100,-100);

    PotFieldPlot->registerSignalToPlot("Angle");

}

PotentialFieldAlgorithmDynamic::~PotentialFieldAlgorithmDynamic()
{
}

void PotentialFieldAlgorithmDynamic::initialize( const Robot& robot )
{

}

float PotentialFieldAlgorithmDynamic::run( const Robot& robot, const float elapsed )
{
    float ksi = 1;
    static float time = -elapsed;
    static float lastTime = -0.5;
    static float lastTimeExtraForce = -0.5;
    static bool anglePunishFlag = false;
    static float anglePunish = 0;

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
            if (robot.obstacle_map().at(x,y) == ObstacleType::Wall)
            {
              QPointF obstPos(x + 0.5f,y + 0.5f);
              dq = robPos -  obstPos;
              Frepul += dq / pow( sqrt( pow(dq.x(),2) + pow(dq.y(),2) ), 3);
            }
            if (robot.obstacle_map().at(x,y) == ObstacleType::Robot &&
                    x != robot.x() && y != robot.y())
            {
              QPointF obstPos(x + 0.5f,y + 0.5f);
              dq = robPos -  obstPos;
              Frepul += 15 * dq / pow( sqrt( pow(dq.x(),2) + pow(dq.y(),2) ), 3);
            }
        }
    }

   ksi = 1 + time/10000;

    QPointF Fres = 1.1 * Frepul + ksi * Fpull;

    const float angle = atan2f( Fres.y(), Fres.x() );

    if ( time - lastTimeExtraForce > 5)
    {
      QPointF tmp = robPos - lastPosition;

      if (sqrt( pow(tmp.x(),2) + pow(tmp.y(),2) ) < 0.3)
      {
          anglePunish = angle - M_PI / 2.1;
          anglePunishFlag = true;
      }
      else
      {
          anglePunish = 0;
          anglePunishFlag = false;
      }

      lastPosition = robPos;
      lastTimeExtraForce = time;
    }

    time += elapsed;
    if (time - lastTime > 0.05)
    {
        if (anglePunishFlag)
            PotFieldPlot->addData("Angle",QPointF(time,anglePunish));
        else
            PotFieldPlot->addData("Angle",QPointF(time,angle));
        lastTime = time;
    }

    if (anglePunishFlag)
    {
        return anglePunish;
    }
    else
    {
        return angle;
    }

}
