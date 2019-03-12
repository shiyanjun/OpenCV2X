#include "artery/lte/Mode4Mobility.h"
#include <cmath>

namespace artery
{

Define_Module(Mode4Mobility)

void Mode4Mobility::initialize(int stage)
{
    if (stage == 0) {
        WATCH(mVehicleId);
        WATCH(mPosition);
        WATCH(mDirection);
        WATCH(mSpeed);
    } else if (stage == 1) {
        mPosition.z = move.getStartPos().z;
        move.setStart(mPosition);
        move.setSpeed(mSpeed);
        move.setDirectionByVector(mDirection);
    }
    BaseMobility::initialize(stage);
}

void Mode4Mobility::initialize(const Position& pos, Angle heading, double speed)
{
    using boost::units::si::meter;
    mPosition.x = pos.x / meter;
    mPosition.y = pos.y / meter;
    move.setStart(mPosition);

    mSpeed = speed;
    move.setSpeed(mSpeed);

    mDirection = Coord { cos(heading.radian()), -sin(heading.radian()) };
    move.setDirectionByVector(mDirection);
}

void Mode4Mobility::update(const Position& pos, Angle heading, double speed)
{
    initialize(pos, heading, speed);

    BaseMobility::updatePosition(); // emits update signal for Mode4
    // assert there is no identical signal emitted twice
    ASSERT(BaseMobility::mobilityStateChangedSignal != MobilityBase::stateChangedSignal);
    emit(MobilityBase::stateChangedSignal, this);
}

} // namespace artery
